// $Id: login.c,v 1.16 2004/03/12 01:07:00 sara-chan Exp $
// original : login2.c 2003/01/28 02:29:17 Rev.1.1.1.1

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "core.h"
#include "socket.h"
#include "login.h"
#include "mmo.h"
#include "version.h"
#include "db.h"


#ifdef PASSWORDENC
#include "md5calc.h"
#endif

#ifdef MEMWATCH
#include "memwatch.h"
#endif

int account_id_count = START_ACCOUNT_NUM;
int server_num;
int new_account_flag = 0;
int login_port = 6900;
char lan_char_ip[128]; 
int subneti[4]; 
int subnetmaski[4]; 

char account_filename[1024] = "account.txt";
char GM_account_filename[1024] = "conf/GM_account.txt";
char login_log_filename[1024] = "login.log";

struct mmo_char_server server[MAX_SERVERS];
int server_fd[MAX_SERVERS];

enum {
	ACO_DENY_ALLOW=0,
	ACO_ALLOW_DENY,
	ACO_MUTUAL_FAILTURE,
	ACO_STRSIZE=128,
};

int access_order=ACO_DENY_ALLOW;
int access_allownum=0;
int access_denynum=0;
char *access_allow=NULL;
char *access_deny=NULL;

struct login_session_data {
	int md5keylen;
	char md5key[20];
};

#define AUTH_FIFO_SIZE 256
struct {
  int account_id,login_id1,login_id2;
  int sex,delflag;
} auth_fifo[AUTH_FIFO_SIZE];
int auth_fifo_pos=0;

struct {
	int account_id,sex;
	char userid[24],pass[24],lastlogin[24];
	int logincount;
	int state;
	int account_reg_num;
	struct global_reg account_reg[ACCOUNT_REG_NUM];
} *auth_dat;

int auth_num=0,auth_max=0;

char admin_pass[64]="";
char gm_pass[64]="";
const int gm_start=704554,gm_last=704583;


static struct dbt *gm_account_db;

int login_log(char *fmt,...)
{
	FILE *logfp;
	va_list ap;
	va_start(ap,fmt);
	
	logfp=fopen(login_log_filename,"a");
	if(logfp){
		vfprintf(logfp,fmt,ap);
		fclose(logfp);
	}
	
	va_end(ap);
	return 0;
}

int isGM(int account_id)
{
	struct gm_account *p;
	p = numdb_search(gm_account_db,account_id);
	if( p == NULL)
		return 0;
	return p->level;
}

int read_gm_account()
{
	char line[8192];
	struct gm_account *p;
	FILE *fp;
	int c=0;

	gm_account_db = numdb_init();

	if( (fp=fopen(GM_account_filename,"r"))==NULL )
		return 1;
	while(fgets(line,sizeof(line),fp)){
		if(line[0] == '/' && line[1] == '/')
			continue;
		p=malloc(sizeof(struct gm_account));
		if(p==NULL){
			printf("gm_account: out of memory!\n");
			exit(0);
		}
		if(sscanf(line,"%d %d",&p->account_id,&p->level) != 2 || p->level <= 0) {
			printf("gm_account: broken data [%s] line %d\n",GM_account_filename,c);
		}
		else {
			if(p->level > 99)
				p->level = 99;
			numdb_insert(gm_account_db,p->account_id,p);
		}
		c++;
	}
	fclose(fp);
//	printf("gm_account: %s read done (%d gm account ID)\n",gm_account_txt,c);
	return 0;
}

// IPマスクチェック
int check_ipmask(unsigned int ip,const unsigned char *str)
{
	unsigned int mask=0,i,m,ip2, a0,a1,a2,a3;
	unsigned char *p=(unsigned char *)&ip2, *p2=(unsigned char *)&mask;
	if( sscanf(str,"%d.%d.%d.%d/%n",&a0,&a1,&a2,&a3,&i)!=4 )
		return 0;
	p[0]=a0; p[1]=a1; p[2]=a2; p[3]=a3;

	if( sscanf(str+i,"%d.%d.%d.%d",&a0,&a1,&a2,&a3)==4 ){
		p2[0]=a0; p2[1]=a1; p2[2]=a2; p2[3]=a3;
		mask=ntohl(mask);
	}else if( sscanf(str+i,"%d",&m)==1 ){
		for(i=0;i<m;i++)
			mask=(mask>>1)|0x80000000;
	}else
		return 0;
	
//	printf("%08x %08x %08x\n",
//		(unsigned int)ntohl(ip),(unsigned int)ntohl(ip2),(unsigned int)mask);
	return ( (ntohl(ip)&mask) == (ntohl(ip2)&mask) );
}

// アクセスコントロール
int check_ip(unsigned int ip)
{
	int i;
	unsigned char *p=(unsigned char *)&ip;
	char buf[128];
	enum { ACF_DEF, ACF_ALLOW, ACF_DENY } flag = ACF_DEF;

	if( access_allownum==0 && access_denynum==0 )
		return 1;		// 制限を使わない場合は常に許可

	// + 012.345. 形式の前方一致と all（全てのIPにマッチ）および、
	//   012.345.678.901/24 形式、012.345.678.901/234.567.890.123 形式の
	//   ネットマスク付き表記に対応。
	// + .ne.jpなどのDNS後方一致はホスト名逆引きのコストを考えると
	//   対応しないほうがいいと思われる。(短い時間でDNSが引ける保障はないし、
	//   実際にタイムアウトまで1分近く待たされるケースがあることを確認している)
	//   対応させるなら非同期にDNSを引くか、極短い時間でタイムアウトをとるべき.
	sprintf(buf,"%d.%d.%d.%d",p[0],p[1],p[2],p[3]);
	
	for(i=0;i<access_allownum;i++){
		const char *p=access_allow+i*ACO_STRSIZE;
		if( memcmp(p,buf,strlen(p))==0 || check_ipmask(ip,p)){
			flag=ACF_ALLOW;
			if( access_order==ACO_DENY_ALLOW )
				return 1;	// deny,allow ならallowにあった時点で許可
			break;
		}
	}
	for(i=0;i<access_denynum;i++){
		const char *p=access_deny+i*ACO_STRSIZE;
		if( memcmp(p,buf,strlen(p))==0 || check_ipmask(ip,p)){
			flag=ACF_DENY;
			return 0;		// denyにあると不許可
			break;
		}
	}
	return (flag==ACF_ALLOW || access_order==ACO_ALLOW_DENY)? 1:0;
		// allow,denyのときは無条件で（deny以外しかここまでこない）は許可
		// mutual-failtureのときは、記述なしなら不許可
}


// アカウントデータベースの読み込み
int mmo_auth_init(void)
{
	FILE *fp;
	int i,account_id,logincount,state,n,j,v;
	char line[1024],*p,userid[24],pass[24],lastlogin[24],sex;
	char str[64];
	fp=fopen(account_filename,"r");
	auth_dat=malloc(sizeof(auth_dat[0])*256);
	auth_max=256;
	if(fp==NULL)
		return 0;
	while(fgets(line,1023,fp)!=NULL){
		p=line;
		i=sscanf(line,"%d\t%[^\t]\t%[^\t]\t%[^\t]\t%c\t%d\t%d\t%n",
			&account_id,userid,pass,lastlogin,&sex,&logincount,&state,&n);
		if(i>=5){
			if(auth_num>=auth_max){
				auth_max+=256;
				auth_dat=realloc(auth_dat,sizeof(auth_dat[0])*auth_max);
			}
			auth_dat[auth_num].account_id=account_id;
			strncpy(auth_dat[auth_num].userid,userid,24);
			strncpy(auth_dat[auth_num].pass,pass,24);
			strncpy(auth_dat[auth_num].lastlogin,lastlogin,24);
			auth_dat[auth_num].sex = sex == 'S' ? 2 : sex=='M';

			//データが足りないときの補完
			if(i>=6)
				auth_dat[auth_num].logincount=logincount;
			else
				auth_dat[auth_num].logincount=1;
			if(i>=7)
				auth_dat[auth_num].state=state;
			else
				auth_dat[auth_num].state=0;

			
			for(j=0;j<ACCOUNT_REG_NUM;j++){
				p+=n;
				if(sscanf(p,"%[^\t,],%d %n",str,&v,&n)!=2)
					break;
				strcpy(auth_dat[auth_num].account_reg[j].str,str);
				auth_dat[auth_num].account_reg[j].value=v;
			}
			auth_dat[auth_num].account_reg_num=j;

			auth_num++;
			if(account_id>=account_id_count)
			account_id_count=account_id+1;
		}
	}
	fclose(fp);

	return 0;
}

// アカウントデータベースの書き込み
void mmo_auth_sync(void)
{
	FILE *fp;
	int i,j;
	fp=fopen(account_filename,"w");
	if(fp==NULL)
		return;
	for(i=0;i<auth_num;i++){
		if(auth_dat[i].account_id<0)
			continue;
		
		fprintf(fp,"%d\t%s\t%s\t%s\t%c\t%d\t%d\t",auth_dat[i].account_id,
			auth_dat[i].userid,auth_dat[i].pass,auth_dat[i].lastlogin,
			auth_dat[i].sex==2 ? 'S' : (auth_dat[i].sex ? 'M' : 'F'),
			auth_dat[i].logincount,auth_dat[i].state);
		
		for(j=0;j<auth_dat[i].account_reg_num;j++){
			fprintf(fp,"%s,%d ",
				auth_dat[i].account_reg[j].str,
				auth_dat[i].account_reg[j].value);
		}
		fprintf(fp,RETCODE);
	}
	fclose(fp);
}

// アカウント作成
int mmo_auth_new( struct mmo_account* account,const char *tmpstr,char sex )
{
	int i=auth_num;
	login_log("auth new %s %s %s" RETCODE,
		tmpstr,account->userid,account->passwd);
	
	if(auth_num>=auth_max){
		auth_max+=256;
		auth_dat=realloc(auth_dat,sizeof(auth_dat[0])*auth_max);
	}
	while(isGM(account_id_count) > 0)
		account_id_count++;
	auth_dat[i].account_id=account_id_count++;
	strncpy(auth_dat[i].userid,account->userid,24);
	strncpy(auth_dat[i].pass,account->passwd,24);
	memcpy(auth_dat[i].lastlogin,"-",2);
	auth_dat[i].sex= sex=='M';
	auth_dat[i].logincount=0;
	auth_dat[i].state = 0;
	auth_dat[i].account_reg_num = 0;
	auth_num++;
	return 0;
}

// 認証
int mmo_auth( struct mmo_account* account, int fd )
{
	int i;
	struct timeval tv;
	char tmpstr[256];
	int len,newaccount=0;
	char md5str[64],md5bin[32];

	len=strlen(account->userid)-2;
	// Re-added _M _F account creation - Sara
	if(account->passwdenc==0 && account->userid[len]=='_' && (strncmp(account->userid,"\n",1) != 0) && (account->userid[len+1]=='F' || account->userid[len+1]=='M') && new_account_flag == 1 && account_id_count < END_ACCOUNT_NUM){
		if(new_account_flag == 1){
	  		newaccount=1;
		} else {
			newaccount=0;
		}
		account->userid[len]=0;
	}
    char *adm_pass=strchr(account->passwd,'@');
	if(adm_pass==NULL)
		adm_pass="";
	else
		adm_pass++;
	
	if(strcmp(adm_pass,admin_pass)==0){
		if(adm_pass[0])
			*(adm_pass-1)=0;
/*		
		newaccount=1;
	    account->userid[len]=0;
	}else
		account->userid[0]=0;
*/
	}
	gettimeofday(&tv,NULL);
	strftime(tmpstr,24,"%Y-%m-%d %H:%M:%S",localtime(&(tv.tv_sec)));
	sprintf(tmpstr+19,".%03d",(int)tv.tv_usec/1000);

	for(i=0;i<auth_num;i++){
		if(strcmp(account->userid,auth_dat[i].userid)==0)
			break;
	}
	if(i!=auth_num){
		int encpasswdok=0;
		struct login_session_data *ld=session[fd]->session_data;
#ifdef PASSWORDENC
		if(account->passwdenc>0){
			int j=account->passwdenc;
			if(!ld){
				login_log("md5key not created %s %s" RETCODE,
					tmpstr,account->userid);
				return 1;
			}
			if(j>2)j=1;
			do{
				if(j==1){
					strcpy(md5str,ld->md5key);
					strcat(md5str,auth_dat[i].pass);
				}else if(j==2){
					strcpy(md5str,auth_dat[i].pass);
					strcat(md5str,ld->md5key);
				}else
					md5str[0]=0;
				MD5_String2binary(md5str,md5bin);
				encpasswdok = ( memcmp( account->passwd, md5bin, 16) == 0);
			}while(j<2 && !encpasswdok && (j++)!=account->passwdenc);
//			printf("key[%s] md5 [%s] ",md5key,md5);
//			printf("client [%s] accountpass [%s]\n",account->passwd,auth_dat[i].pass);
		}
#endif
		if( (strcmp(account->passwd,auth_dat[i].pass) && !encpasswdok) || newaccount){
		  	if(account->passwdenc==0)
				login_log("auth failed pass error %s %s %s %d" RETCODE,tmpstr,account->userid,account->passwd,newaccount);
#ifdef PASSWORDENC
			else{
				char logbuf[1024],*p=logbuf;
				int j;
				p+=sprintf(p,"auth failed pass error %s %s recv-md5[",tmpstr,account->userid);
				for(j=0;j<16;j++)
					p+=sprintf(p,"%02x",((unsigned char *)account->passwd)[j]);
				p+=sprintf(p,"] calc-md5[");
				for(j=0;j<16;j++)
					p+=sprintf(p,"%02x",((unsigned char *)md5bin)[j]);
				p+=sprintf(p,"] md5key[");			
				for(j=0;j<ld->md5keylen;j++)
					p+=sprintf(p,"%02x",((unsigned char *)ld->md5key)[j]);
				p+=sprintf(p,"] %d" RETCODE,newaccount);
				login_log(logbuf);
			}
#endif
			return 1;
		}

		login_log("auth ok %s %s new=%d" RETCODE,
					tmpstr,account->userid,newaccount);
	} else {
		if(newaccount==0){
			login_log("auth failed no account %s %s %s %d" RETCODE,
				tmpstr,account->userid,account->passwd,newaccount);
			return 0;
		}

/*		login_log("auth new %s %s %s %d" RETCODE,
			tmpstr,account->userid,account->passwd,newaccount);
		
		if(auth_num>=auth_max){
			auth_max+=256;
			auth_dat=realloc(auth_dat,sizeof(auth_dat[0])*auth_max);
		}
		auth_dat[i].account_id=account_id_count++;
		strncpy(auth_dat[i].userid,account->userid,24);
		strncpy(auth_dat[i].pass,account->passwd,24);
		auth_dat[i].sex=account->userid[len+1]=='M';
		auth_dat[i].logincount=0;
		auth_num++;*/
		mmo_auth_new(account,tmpstr,account->userid[len+1]);
	}

	if(auth_dat[i].state){
		login_log("auth banned account %s %s %s %d\n" RETCODE,
			tmpstr,account->userid,account->passwd,auth_dat[i].state);
		switch(auth_dat[i].state) {
			case 1:
				return 2;
				break;
			case 2:
				return 3;
				break;
			case 3:
				return 4;
				break;
		}
	}

	account->account_id = auth_dat[i].account_id;
	account->login_id1 = rand();
	account->login_id2 = rand();
	memcpy(account->lastlogin,auth_dat[i].lastlogin,24);
	memcpy(auth_dat[i].lastlogin,tmpstr,24);
	account->sex = auth_dat[i].sex;
	auth_dat[i].logincount++;

	mmo_auth_sync();
	return 100;
}

// 自分以外の全てのcharサーバーにデータ送信（送信したmap鯖の数を返す）
int charif_sendallwos(int sfd,unsigned char *buf,unsigned int len)
{
	int i,c;
	for(i=0,c=0;i<MAX_SERVERS;i++){
		int fd;
		if((fd=server_fd[i])>0 && fd!=sfd){
			memcpy(WFIFOP(fd,0),buf,len);
			WFIFOSET(fd,len);
			c++;
		}
	}
	return c;
}

int parse_fromchar(int fd)
{
  int i,id;

  for(id=0;id<MAX_SERVERS;id++)
    if(server_fd[id]==fd)
      break;
  if(id==MAX_SERVERS)
    session[fd]->eof=1;
  if(session[fd]->eof){
    for(i=0;i<MAX_SERVERS;i++)
      if(server_fd[i]==fd)
	server_fd[i]=-1;
    close(fd);
    delete_session(fd);
    return 0;
  }
  while(RFIFOREST(fd)>=2){
    switch(RFIFOW(fd,0)){
    case 0x2712:
      if(RFIFOREST(fd)<15)
	return 0;
      for(i=0;i<AUTH_FIFO_SIZE;i++){
	if(auth_fifo[i].account_id==RFIFOL(fd,2) &&
	   auth_fifo[i].login_id1==RFIFOL(fd,6) &&
	   /*auth_fifo[i].login_id2==RFIFOL(fd,10) &&*/
	   auth_fifo[i].sex==RFIFOB(fd,14) &&
	   !auth_fifo[i].delflag){
	  auth_fifo[i].delflag=1;
//	  printf("%d\n",i);
	  break;
	}
      }
	  
	  if(i!=AUTH_FIFO_SIZE){	// account_reg送信
		int p,j,i;
		for(i=0;i<auth_num;i++){
			if(auth_dat[i].account_id==RFIFOL(fd,2))
				break;
		}
		if(i<auth_num){
			WFIFOW(fd,0)=0x2729;
			WFIFOL(fd,4)=RFIFOL(fd,2);
			for(p=8,j=0;j<auth_dat[i].account_reg_num;p+=36,j++){
				memcpy(WFIFOP(fd,p),auth_dat[i].account_reg[j].str,32);
				WFIFOL(fd,p+32)=auth_dat[i].account_reg[j].value;
			}
			WFIFOW(fd,2)=p;
			WFIFOSET(fd,p);
//			printf("account_reg send : login->char (auth fifo)\n");
		}
	  }
	  
      WFIFOW(fd,0)=0x2713;
      WFIFOL(fd,2)=RFIFOL(fd,2);
      if(i!=AUTH_FIFO_SIZE){
	WFIFOB(fd,6)=0;
      } else {
	WFIFOB(fd,6)=1;
      }
      WFIFOSET(fd,7);
      RFIFOSKIP(fd,15);
      break;
    case 0x2714:
      //printf("set users %s : %d\n",server[id].name,RFIFOL(fd,2));
      server[id].users=RFIFOL(fd,2);
      RFIFOSKIP(fd,6);
      break;
	
	case 0x2720:	// GM
	  {
	  	int newacc=0,oldacc,i=0,j;
		if(RFIFOREST(fd)<4)
			return 0;
		if(RFIFOREST(fd)<RFIFOW(fd,2))
			return 0;
		oldacc=RFIFOL(fd,4);
		printf("gm search %d\n",oldacc);
		if(strcmp(RFIFOP(fd,8),gm_pass)==0 &&
			(oldacc<gm_start || oldacc>gm_last)){
			for(j=gm_start,i=0;j<gm_last && i<auth_num;j++){
				for(i=0;i<auth_num;i++){
					if(auth_dat[i].account_id==j)
						break;
				}
			}
			if(i==auth_num){
				newacc=j-1;
				for(i=0;i<auth_num;i++){
					if(auth_dat[i].account_id==oldacc){
						auth_dat[i].account_id=newacc;
					}
				}
				printf("change GM! %d=>%d\n",oldacc,newacc);
			}else
				printf("change GM error %d %s\n",oldacc,RFIFOP(fd,8));
		}else
				printf("change GM error %d %s\n",oldacc,RFIFOP(fd,8));

		RFIFOSKIP(fd,RFIFOW(fd,2));
		WFIFOW(fd,0)=0x2721;
		WFIFOL(fd,2)=oldacc;
		WFIFOL(fd,6)=newacc;
		WFIFOSET(fd,10);
	  }
	  return 0;

	case 0x2722:	// changesex
	  {
	  	int acc,sex,i=0,j=0;
		if(RFIFOREST(fd)<4)
			return 0;
		if(RFIFOREST(fd)<RFIFOW(fd,2))
			return 0;
		acc=RFIFOL(fd,4);
		sex=RFIFOB(fd,8);
		for(i=0;i<auth_num;i++){
//			printf("%d,",auth_dat[i].account_id);
			if(auth_dat[i].account_id==acc){
				auth_dat[i].sex=sex;
				j=1;
			}
		}
		RFIFOSKIP(fd,RFIFOW(fd,2));
		WFIFOW(fd,0)=0x2723;
		WFIFOL(fd,2)=acc;
		WFIFOB(fd,6)=sex;
		WFIFOSET(fd,7);
	  }
	  return 0;

	case 0x2728: {	// save account_reg
			int acc,p,i,j;
			if(RFIFOREST(fd)<4)
				return 0;
			if(RFIFOREST(fd)<RFIFOW(fd,2))
				return 0;
			acc=RFIFOL(fd,4);
			for(i=0;i<auth_num;i++){
				if(auth_dat[i].account_id==acc)
					break;
			}
			if(i<auth_num){
				unsigned char buf[4096];
				for(p=8,j=0;p<RFIFOW(fd,2) && j<ACCOUNT_REG_NUM;p+=36,j++){
					memcpy(auth_dat[i].account_reg[j].str,RFIFOP(fd,p),32);
					auth_dat[i].account_reg[j].value=RFIFOL(fd,p+32);
				}
				auth_dat[i].account_reg_num=j;
				// 他のサーバーへポスト（同垢ログインがなければ送らなくていい）
				memcpy(WBUFP(buf,0),RFIFOP(fd,0),RFIFOW(fd,2));
				WBUFW(buf,0)=0x2729;
				charif_sendallwos(fd,buf,WBUFW(buf,2));
				// 保存
				mmo_auth_sync();
			}
			RFIFOSKIP(fd,RFIFOW(fd,2));
//			printf("login: save account_reg (from char)\n");
		}break;
		
    default:
	  printf("login: unknown packet %x! (from char).\n",RFIFOW(fd,0));
      close(fd);
      session[fd]->eof=1;
      return 0;
    }
  }
  return 0;
}

int parse_admin(int fd)
{
	int i;
	if(session[fd]->eof){
		for(i=0;i<MAX_SERVERS;i++)
			if(server_fd[i]==fd)
				server_fd[i]=-1;
		close(fd);
		delete_session(fd);
		return 0;
	}
	while(RFIFOREST(fd)>=2){
		switch(RFIFOW(fd,0)){
		case 0x7920:	{	// アカウントリスト
				int st,ed,len;
				if(RFIFOREST(fd)<11)
					return 0;
				st=RFIFOL(fd,2);
				ed=RFIFOL(fd,6);
				RFIFOSKIP(fd,11);
				WFIFOW(fd,0)=0x7921;
				if(st<0)st=0;
				if(ed>END_ACCOUNT_NUM || ed<st || ed<=0)ed=END_ACCOUNT_NUM;
				for(i=0,len=4;i<auth_num && len<30000;i++){
					int account_id=auth_dat[i].account_id;
					if( account_id>=st && account_id<=ed ){
						WFIFOL(fd,len   )=auth_dat[i].account_id;
						memcpy(WFIFOP(fd,len+4),auth_dat[i].userid,24);
						WFIFOB(fd,len+28)=auth_dat[i].sex;
						WFIFOL(fd,len+53)=auth_dat[i].logincount;
						WFIFOL(fd,len+57)=auth_dat[i].state;
						len+=61;
					}
				}
				WFIFOW(fd,2)=len;
				WFIFOSET(fd,len);
			}break;
		case 0x7930: {	// アカウント作成
				struct mmo_account ma;
				if(RFIFOREST(fd)<4 || RFIFOREST(fd)<RFIFOW(fd,2))
					return 0;
				ma.userid=RFIFOP(fd, 4);
				ma.passwd=RFIFOP(fd,28);
				memcpy(ma.lastlogin,"-",2);
				ma.sex=RFIFOB(fd,52);
				WFIFOW(fd,0)=0x7931;
				WFIFOW(fd,2)=0;
				memcpy(WFIFOP(fd,4),RFIFOP(fd,4),24);
				for(i=0;i<auth_num;i++){
					if( strncmp(auth_dat[i].userid,ma.userid,24)==0 ){
						WFIFOW(fd,2)=1;
						break;
					}
				}
				if(i==auth_num)
					mmo_auth_new(&ma,"( ladmin )",ma.sex);
				WFIFOSET(fd,28);
				RFIFOSKIP(fd,RFIFOW(fd,2));
			}break;
		case 0x7932:	// アカウント削除
			WFIFOW(fd,0)=0x7933;
			WFIFOW(fd,2)=1;
			memcpy(WFIFOP(fd,4),RFIFOP(fd,4),24);
			for(i=0;i<auth_num;i++){
				if( strncmp(auth_dat[i].userid,RFIFOP(fd,4),24)==0 ){
					auth_dat[i].userid[0]=0;
					auth_dat[i].account_id=-1;
					WFIFOW(fd,2)=0;
					break;
				}
			}
			WFIFOSET(fd,28);
			RFIFOSKIP(fd,RFIFOW(fd,2));
			break;
		case 0x7934:	// パスワード変更
			WFIFOW(fd,0)=0x7935;
			WFIFOW(fd,2)=1;
			memcpy(WFIFOP(fd,4),RFIFOP(fd,4),24);
			for(i=0;i<auth_num;i++){
				if( strncmp(auth_dat[i].userid,RFIFOP(fd,4),24)==0 ){
					memcpy(auth_dat[i].pass,RFIFOP(fd,28),24);
					WFIFOW(fd,2)=0;
					break;
				}
			}
			WFIFOSET(fd,28);
			RFIFOSKIP(fd,RFIFOW(fd,2));
			break;
		case 0x7936:	// バン状態変更
			WFIFOW(fd,0)=0x7937;
			WFIFOW(fd,2)=1;
			memcpy(WFIFOP(fd,4),RFIFOP(fd,4),24);
			for(i=0;i<auth_num;i++){
				if( strncmp(auth_dat[i].userid,RFIFOP(fd,4),24)==0 ){
					auth_dat[i].state=RFIFOL(fd,28);
					WFIFOW(fd,2)=0;
					break;
				}
			}
			WFIFOL(fd,28)=auth_dat[i].state;
			WFIFOSET(fd,32);
			RFIFOSKIP(fd,RFIFOW(fd,2));
			break;			
		default:
			close(fd);
			session[fd]->eof=1;
			return 0;
		}
		//WFIFOW(fd,0)=0x791f;
		//WFIFOSET(fd,2);
	}
	return 0;
}

//-----------------------------------------------------
//Check IP Comes Form Lan or Wan.
//-----------------------------------------------------
int lan_ip_check(unsigned char *p){
	int i;
	int lancheck=0;
	//printf("SubnetMask : %d.%d.%d.%d\n",subnetmaski[0],subnetmaski[1],subnetmaski[2],subnetmaski[3]);
	//printf("Subnet : %d.%d.%d.%d\n",subneti[0],subneti[1],subneti[2],subneti[3]);
	for(i=0;i<4;i++){
		if(subnetmaski[i]==255){
			//printf("Sub Comp : %d , %d\n",subneti[i],p[i]);
			if(subneti[i]== p[i]){
				lancheck=1;
			}
			else{
				lancheck=0;
				break;
			}
		}
	}
	printf("Lan Check Value : %d\n",lancheck);
	return lancheck;
}

int parse_login(int fd)
{
  struct mmo_account account;
  int result,i;
  unsigned char *p=(unsigned char *) &session[fd]->client_addr.sin_addr;

  if(session[fd]->eof){
    for(i=0;i<MAX_SERVERS;i++)
      if(server_fd[i]==fd)
				server_fd[i]=-1;
    close(fd);
    delete_session(fd);
    return 0;
  }
  if(RFIFOW(fd,0)<30000) {
  	if(RFIFOW(fd,0) == 0x64 || RFIFOW(fd,0) == 0x01dd)
	  printf("parse_login : %d %d %d %s\n",fd,RFIFOREST(fd),RFIFOW(fd,0),RFIFOP(fd,6));
		else
		  printf("parse_login : %d %d %d\n",fd,RFIFOREST(fd),RFIFOW(fd,0));
	}
  while(RFIFOREST(fd)>=2){
	switch(RFIFOW(fd,0)){
	case 0x64:		// クライアントログイン要求
	case 0x01dd:	// 暗号化ログイン要求
		if(RFIFOREST(fd)< ((RFIFOW(fd,0)==0x64)?55:47))
			return 0;
		{
			unsigned char *p=(unsigned char *)&session[fd]->client_addr.sin_addr;
			login_log("client connection request %s from %d.%d.%d.%d" RETCODE,
				RFIFOP(fd,6),p[0],p[1],p[2],p[3]);
		}

		if( !check_ip(session[fd]->client_addr.sin_addr.s_addr) ){
		  struct timeval tv;
			char tmpstr[256];
			gettimeofday(&tv,NULL);
			strftime(tmpstr,24,"%Y-%m-%d %H:%M:%S",localtime(&(tv.tv_sec)));
			sprintf(tmpstr+19,".%03d",(int)tv.tv_usec/1000);
			login_log("access denied %s" RETCODE, tmpstr);
			WFIFOW(fd,0)=0x6a;
			WFIFOB(fd,2)=0x03;
			WFIFOSET(fd,3);
		}
		
		account.userid = RFIFOP(fd,6);
		account.passwd = RFIFOP(fd,30);
#ifdef PASSWORDENC
		account.passwdenc= (RFIFOW(fd,0)==0x64)?0:PASSWORDENC;
#else
		account.passwdenc=0;
#endif
		result=mmo_auth(&account,fd);
		if(result==100){
			server_num=0;
			for(i=0;i<MAX_SERVERS;i++){
				if(server_fd[i]>=0){
					if(lan_ip_check(p)) WFIFOL(fd,47+server_num*32) = inet_addr(lan_char_ip); 
					else WFIFOL(fd,47+server_num*32) = server[i].ip; 
					WFIFOW(fd,47+server_num*32+4) = server[i].port;
					memcpy(WFIFOP(fd,47+server_num*32+6), server[i].name, 20);
					WFIFOW(fd,47+server_num*32+26) = server[i].users;
					WFIFOW(fd,47+server_num*32+28) = server[i].maintenance;
					WFIFOW(fd,47+server_num*32+30) = server[i].new;
					server_num++;
				}
			}
			WFIFOW(fd,0)=0x69;
			WFIFOW(fd,2)=47+32*server_num;
			WFIFOL(fd,4)=account.login_id1;
			WFIFOL(fd,8)=account.account_id;
			WFIFOL(fd,12)=account.login_id2;
			WFIFOL(fd,16)=0;
			memcpy(WFIFOP(fd,20),account.lastlogin,24);
			WFIFOB(fd,46)=account.sex;
			WFIFOSET(fd,47+32*server_num);
			if(auth_fifo_pos>=AUTH_FIFO_SIZE){
				auth_fifo_pos=0;
			}
			auth_fifo[auth_fifo_pos].account_id=account.account_id;
			auth_fifo[auth_fifo_pos].login_id1=account.login_id1;
			auth_fifo[auth_fifo_pos].login_id2=account.login_id2;
			auth_fifo[auth_fifo_pos].sex=account.sex;
			auth_fifo[auth_fifo_pos].delflag=0;
			auth_fifo_pos++;
		} else {
			memset(WFIFOP(fd,0),0,23);
			WFIFOW(fd,0)=0x6a;
			WFIFOB(fd,2)=result;
			WFIFOSET(fd,23);
		}
		RFIFOSKIP(fd,(RFIFOW(fd,0)==0x64)?55:47);
		break;
	
	case 0x01db:	// 暗号化Key送信要求
	case 0x791a:	// 管理パケットで暗号化key要求
		{
			struct login_session_data *ld;
			if(session[fd]->session_data){
				printf("login: illeagal md5key request.");
				close(fd);
				session[fd]->eof=1;
				return 0;
			}
			ld=session[fd]->session_data=malloc(sizeof(*ld));
			if(!ld){
				printf("login: md5key request: out of memory !\n");
				close(fd);
				session[fd]->eof=1;
				return 0;
			}
			// 暗号化キー生成
			memset(ld->md5key,0,sizeof(ld->md5key));
			ld->md5keylen=rand()%4+12;
			for(i=0;i<ld->md5keylen;i++)
			ld->md5key[i]=rand()%255+1;
	
		RFIFOSKIP(fd,2);
		WFIFOW(fd,0)=0x01dc;
			WFIFOW(fd,2)=4+ld->md5keylen;
			memcpy(WFIFOP(fd,4),ld->md5key,ld->md5keylen);
		WFIFOSET(fd,WFIFOW(fd,2));
		}
		break;
		
	case 0x2710:	// Charサーバー接続要求
		if(RFIFOREST(fd)<76)
			return 0;
		{
			FILE *logfp=fopen(login_log_filename,"a");
			if(logfp){
				unsigned char *p=(unsigned char *)&session[fd]->client_addr.sin_addr;
				fprintf(logfp,"server connection request %s @ %d.%d.%d.%d:%d (%d.%d.%d.%d)" RETCODE,
				RFIFOP(fd,60),RFIFOB(fd,54),RFIFOB(fd,55),RFIFOB(fd,56),RFIFOB(fd,57),RFIFOW(fd,58),
				p[0],p[1],p[2],p[3]);
				fclose(logfp);
			}
		}
		account.userid = RFIFOP(fd,2);
		account.passwd = RFIFOP(fd,26);
		account.passwdenc = 0;
		result = mmo_auth(&account,fd);
		if(result == 100 && account.sex==2 && account.account_id<MAX_SERVERS && server_fd[account.account_id]<0){
			server[account.account_id].ip=RFIFOL(fd,54);
			server[account.account_id].port=RFIFOW(fd,58);
			memcpy(server[account.account_id].name,RFIFOP(fd,60),20);
			server[account.account_id].users=0;
			server[account.account_id].maintenance=RFIFOW(fd,82);
			server[account.account_id].new=RFIFOW(fd,84);
			server_fd[account.account_id]=fd;
			WFIFOW(fd,0)=0x2711;
			WFIFOB(fd,2)=0;
			WFIFOSET(fd,3);
			session[fd]->func_parse=parse_fromchar;
			realloc_fifo(fd,FIFOSIZE_SERVERLINK,FIFOSIZE_SERVERLINK);	
		} else {
			WFIFOW(fd,0)=0x2711;
			WFIFOB(fd,2)=3;
			WFIFOSET(fd,3);
		}
		RFIFOSKIP(fd,86);
		return 0;

	case 0x7530:	// Athena情報所得
		WFIFOW(fd,0)=0x7531;
		WFIFOB(fd,2)=ATHENA_MAJOR_VERSION;
		WFIFOB(fd,3)=ATHENA_MINOR_VERSION;
		WFIFOB(fd,4)=ATHENA_REVISION;
		WFIFOB(fd,5)=ATHENA_RELEASE_FLAG;
		WFIFOB(fd,6)=ATHENA_OFFICIAL_FLAG;
		WFIFOB(fd,7)=ATHENA_SERVER_LOGIN;
		WFIFOW(fd,8)=ATHENA_MOD_VERSION;
		WFIFOSET(fd,10);
		RFIFOSKIP(fd,2);
		break;
	case 0x7532:	// 接続の切断(defaultと処理は一緒だが明示的にするため)
		close(fd);
		session[fd]->eof=1;
		return 0;
	
	case 0x7918:	// 管理モードログイン
		{
			struct login_session_data *ld=session[fd]->session_data;
		if(RFIFOREST(fd)<4 || RFIFOREST(fd)<RFIFOW(fd,2))
			return 0;

		WFIFOW(fd,0)=0x7919;
		WFIFOB(fd,2)=1;
			
			if(RFIFOW(fd,4)==0){	// プレーン
		if(strcmp(RFIFOP(fd,6),admin_pass)==0){
			WFIFOB(fd,2)=0;
			session[fd]->func_parse=parse_admin;
		}
			}else{					// 暗号化
				if(!ld){
					printf("login: md5key not created for admin login\n");
				}else{
					char md5str[64]="",md5bin[32];
					if(RFIFOW(fd,4)==1){
						strcpy(md5str,ld->md5key);
						strcat(md5str,admin_pass);
					}else if(RFIFOW(fd,4)==2){
						strcpy(md5str,admin_pass);
						strcat(md5str,ld->md5key);
					};
					MD5_String2binary(md5str,md5bin);
					if(memcmp(md5bin,RFIFOP(fd,6),16)==0){
						WFIFOB(fd,2)=0;
						session[fd]->func_parse=parse_admin;
					}
				}
			}
		WFIFOSET(fd,3);
		RFIFOSKIP(fd,RFIFOW(fd,2));
		}
		break;

	default:
		close(fd);
		session[fd]->eof=1;
		return 0;
	}
  }
  return 0;
}


//-----------------------------------------------------
// reading Lan Support configuration
//-----------------------------------------------------
int login_lan_config_read(const char *lancfgName){
	int i;
	int j;
	char subnet[128];
	char subnetmask[128];
	struct hostent * subnets=NULL;
	struct hostent * subnetmasks=NULL;
	char line[1024], w1[1024], w2[1024];
	FILE *fp;
 
	fp=fopen(lancfgName,"r");
 
	if(fp==NULL){
		printf("file not found: %s\n", lancfgName);
		return 1;
	}
	printf ("start reading Lan Support configuration...\n");
	while(fgets(line, 1020, fp)){
		if(line[0] == '/' && line[1] == '/')
			continue;
 
		i=sscanf(line,"%[^:]: %[^\r\n]",w1,w2);
		if(i!=2)
			continue;
 
		else if(strcmpi(w1,"lan_char_ip")==0){ //Read Char Server Lan IP Address 
			strcpy(lan_char_ip, w2);
			printf ("set Lan_Char_IP : %s\n",w2);
 
		}
		else if(strcmpi(w1,"subnet")==0){//Read Subnet 
			strcpy(subnet, w2);
			subnets = gethostbyname (subnet);
			printf ("set Subet : %s\n",w2);
			for(j=0;j<4;j++){
				subneti[j]=(unsigned char)subnets->h_addr[j];
			}
 
		}
		else if(strcmpi(w1,"subnetmask")==0){//Read SubnetMask 
			strcpy(subnetmask, w2);
			subnetmasks = gethostbyname (subnetmask);
			printf ("set SubnetMask : %s\n",w2);
			for(j=0;j<4;j++){
				subnetmaski[j]=(unsigned char)subnetmasks->h_addr[j];
			}
		}
	}
	fclose(fp);
 
	printf ("End reading Lan Support configuration...\n");
	return 0;
}

int login_config_read(const char *cfgName)
{
	int i;
	char line[1024],w1[1024],w2[1024];
	FILE *fp;

	fp=fopen(cfgName,"r");
	if(fp==NULL){
		printf("file not found: %s\n",cfgName);
		return 1;
	}
	while(fgets(line,1020,fp)){
		if(line[0] == '/' && line[1] == '/')
			continue;

		i=sscanf(line,"%[^:]: %[^\r\n]",w1,w2);
		if(i!=2)
			continue;

		if(strcmpi(w1,"admin_pass")==0){
			strcpy(admin_pass,w2);
		}
		else if(strcmpi(w1,"gm_pass")==0){
			strcpy(gm_pass,w2);
		}
		else if(strcmpi(w1,"new_account")==0){
			new_account_flag = atoi(w2);
		}
		else if(strcmpi(w1,"login_port")==0){
			login_port=atoi(w2);
		}
		else if(strcmpi(w1,"account_filename")==0){
			strcpy(account_filename,w2);
		}
		else if(strcmpi(w1,"gm_account_filename")==0){
			strcpy(GM_account_filename,w2);
		}
		
		else if(strcmpi(w1,"order")==0){
			access_order=atoi(w2);
			if(strcmpi(w2,"deny,allow")==0) access_order=ACO_DENY_ALLOW;
			if(strcmpi(w2,"allow,deny")==0) access_order=ACO_ALLOW_DENY;
			if(strcmpi(w2,"mutual-failture")==0) access_order=ACO_MUTUAL_FAILTURE;
		}
		else if(strcmpi(w1,"allow")==0){
			if(access_allow)
				access_allow=realloc( access_allow, (access_allownum+1)*ACO_STRSIZE);
			else
				access_allow=malloc( ACO_STRSIZE );
			if(strcmpi(w2,"all")==0)
				access_allow[(access_allownum++)*ACO_STRSIZE]=0;
			else if(w2[0])
				strcpy( access_allow+(access_allownum++)*ACO_STRSIZE,w2 );
		}
		else if(strcmpi(w1,"deny")==0){
			if(access_deny)
				access_deny=realloc( access_deny,(access_denynum+1)*ACO_STRSIZE);
			else
				access_deny=malloc( ACO_STRSIZE );
			if(strcmpi(w2,"all")==0)
				access_deny[(access_denynum++)*ACO_STRSIZE]=0;
			else if(w2[0])
				strcpy( access_deny+(access_denynum++)*ACO_STRSIZE,w2 );
		}
		else if(strcmpi(w1,"login_log_filename")==0){
			strcpy(login_log_filename,w2);
		}
	}
	fclose(fp);

	return 0;
}

int do_init(int argc,char **argv)
{
  int i;

  // read login configue  
  login_config_read( (argc>1)?argv[1]:LOGIN_CONF_NAME );
  login_lan_config_read( (argc>1)?argv[1]:LOGIN_LAN_CONF_NAME ); 
  srand(time(NULL));

  for(i=0;i<AUTH_FIFO_SIZE;i++){
    auth_fifo[i].delflag=1;
  }
  for(i=0;i<MAX_SERVERS;i++){
    server_fd[i]=-1;
  }
  make_listen_port(login_port);
  mmo_auth_init();
	read_gm_account();
  set_termfunc(mmo_auth_sync);
  set_defaultparse(parse_login);

  return 0;
}
