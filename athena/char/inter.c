#include "mmo.h"
#include "char.h"
#include "socket.h"
#include "timer.h"
#include "db.h"
#include <string.h>
#include <stdlib.h>

#include "inter.h"
#include "int_party.h"
#include "int_guild.h"
#include "int_storage.h"
#include "int_pet.h"

#define WISDATA_TTL		(60*1000)	// Wisデータの生存時間(60秒)
#define WISDELLIST_MAX	128			// Wisデータ削除リストの要素数

char inter_log_filename[1024]="inter.log";

int party_share_level = 10;


// 送信パケット長リスト
int inter_send_packet_length[]={
	-1,-1,27, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	-1, 7, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	35,-1,11,15, 34,29, 7,-1,  0, 0, 0, 0,  0, 0,  0, 0,
	10,-1,15, 0, 79,19, 7,-1,  0,-1,-1,-1, 14,67,186,-1,
	 9, 9, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	11,-1, 7, 3,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
};
// 受信パケット長リスト
int inter_recv_packet_length[]={
	-1,-1, 7, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 6,-1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	72, 6,52,14, 10,29, 6,-1, 34, 0, 0, 0,  0, 0,  0, 0,
	-1, 6,-1, 0, 55,19, 6,-1, 14,-1,-1,-1, 14,19,186,-1,
	 5, 9, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
	48,14,-1, 6,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0,  0, 0,
};


struct WisData {
	int id,fd,count,len;
	unsigned long tick;
	unsigned char src[24],dst[24],msg[512];
};
static struct dbt * wis_db = NULL;
static int wis_dellist[WISDELLIST_MAX], wis_delnum;


// WISデータの生存チェック
int check_ttl_wisdata_sub(void *key,void *data,va_list ap)
{
	unsigned long tick;
	struct WisData *wd=(struct WisData *)data;
	tick=va_arg(ap,unsigned long);
	
	if( DIFF_TICK(tick,wd->tick)>WISDATA_TTL && wis_delnum< WISDELLIST_MAX ){
		wis_dellist[wis_delnum++]=wd->id;
	}
	return 0;
}
int check_ttl_wisdata()
{
	unsigned long tick=gettick();
	int i;
	
	do{
		wis_delnum=0;
		numdb_foreach( wis_db, check_ttl_wisdata_sub, tick );
		for(i=0;i<wis_delnum;i++){
			struct WisData *wd=numdb_search(wis_db,wis_dellist[i]);
			printf("inter: wis data id=%d time out : from %s to %s\n",
				wd->id,wd->src,wd->dst);
			numdb_erase(wis_db,wd->id);
			free(wd);
	}
	}while(wis_delnum>=WISDELLIST_MAX);
	return 0;
}

//--------------------------------------------------------

/*==========================================
 * 設定ファイルを読み込む
 *------------------------------------------
 */
int inter_config_read(const char *cfgName)
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
		i=sscanf(line,"%[^:]: %[^\r\n]",w1,w2);
		if(i!=2)
			continue;
		if(strcmpi(w1,"storage_txt")==0){
			strncpy(storage_txt,w2,sizeof(storage_txt));
		}
		else if(strcmpi(w1,"party_txt")==0){
			strncpy(party_txt,w2,sizeof(party_txt));
		}
		else if(strcmpi(w1,"guild_txt")==0){
			strncpy(guild_txt,w2,sizeof(guild_txt));
		}
		else if(strcmpi(w1,"pet_txt")==0){
			strncpy(pet_txt,w2,sizeof(pet_txt));
		}
		else if(strcmpi(w1,"castle_txt")==0){
			strncpy(castle_txt,w2,sizeof(castle_txt));
		}
		else if(strcmpi(w1,"party_share_level")==0){
			party_share_level=atoi(w2);
			if(party_share_level < 0) party_share_level = 0;
		}
		else if(strcmpi(w1,"inter_log_filename")==0){
			strcpy(inter_log_filename,w2);
		}
	}
	fclose(fp);

	return 0;
}

// ログ書き出し
int inter_log(char *fmt,...)
{
	FILE *logfp;
	va_list ap;
	va_start(ap,fmt);
	logfp=fopen(inter_log_filename,"a");
	if(logfp){
		vfprintf(logfp,fmt,ap);
		fclose(logfp);
	}
	va_end(ap);
	return 0;
}

// セーブ
int inter_save()
{
	inter_storage_save();
	inter_party_save();
	inter_guild_save();
	inter_pet_save();

	return 0;
}

// 初期化
int inter_init(const char *file)
{
	inter_config_read(file);

	wis_db = numdb_init();

	inter_storage_init();
	inter_party_init();
	inter_guild_init();
	inter_pet_init();

	return 0;
}

//--------------------------------------------------------

// GMメッセージ送信
int mapif_GMmessage(unsigned char *mes,int len)
{
	unsigned char buf[len];
	WBUFW(buf,0)=0x3800;
	WBUFW(buf,2)=len;
	memcpy(WBUFP(buf,4),mes,len-4);
	mapif_sendall(buf,len);
//	printf("inter server: GM:%d %s\n",len,mes);
	return 0;
}

// Wis送信
int mapif_wis_message(struct WisData *wd)
{
	unsigned char buf[1024];
	WBUFW(buf, 0)=0x3801;
	WBUFW(buf, 2)=8 + 48 +wd->len;
	WBUFL(buf, 4)=wd->id;
	memcpy(WBUFP(buf, 8),wd->src,24);
	memcpy(WBUFP(buf,32),wd->dst,24);
	memcpy(WBUFP(buf,56),wd->msg,wd->len);
	wd->count = mapif_sendall(buf,WBUFW(buf,2));
	
	return 0;
}
// Wis送信結果
int mapif_wis_end(struct WisData *wd,int flag)
{
	unsigned char buf[32];
	
	WBUFW(buf, 0)=0x3802;
	memcpy(WBUFP(buf, 2),wd->src,24);
	WBUFB(buf,26)=flag;
	mapif_send(wd->fd,buf,27);
//	printf("inter server wis_end %d\n",flag);
	return 0;
}

//--------------------------------------------------------

// GMメッセージ送信
int mapif_parse_GMmessage(int fd)
{
	mapif_GMmessage(RFIFOP(fd,4),RFIFOW(fd,2));
	return 0;
}


// Wis送信要求
int mapif_parse_WisRequest(int fd)
{
	struct WisData* wd;
	static int wisid=0;
	
	if( RFIFOW(fd,2)-52 >= sizeof(wd->msg) ){
		printf("inter: Wis message size too long.\n");
		return 0;
	}
	
	wd = (struct WisData *)malloc(sizeof(struct WisData));
	if(wd==NULL){
		// Wis送信失敗（パケットを送る必要ありかも）
		printf("inter: WisRequest: out of memory !\n");
		return 0;
	}
	
	check_ttl_wisdata();
	
	wd->id = ++wisid;
	wd->fd = fd;
	wd->len= RFIFOW(fd,2)-52;
	memcpy(wd->src, RFIFOP(fd, 4), 24);
	memcpy(wd->dst, RFIFOP(fd,28), 24);
	memcpy(wd->msg, RFIFOP(fd,52), wd->len);
	wd->tick = gettick();
	numdb_insert(wis_db, wd->id, wd);
	mapif_wis_message(wd);
	
	return 0;
}

// Wis送信結果
int mapif_parse_WisReply(int fd)
{
	int id=RFIFOL(fd,2),flag=RFIFOB(fd,6);
	
	struct WisData *wd = numdb_search(wis_db, id);
	
	if(wd==NULL)
		return 0;	// タイムアウトしたかIDが存在しない
	
	if( (--wd->count)==0 || flag!=1){
		mapif_wis_end(wd,flag);
		numdb_erase(wis_db, id);
		free(wd);
	}
	return 0;
}
//--------------------------------------------------------

// map server からの通信（１パケットのみ解析すること）
// エラーなら0(false)、処理できたなら1、
// パケット長が足りなければ2をかえさなければならない
int inter_parse_frommap(int fd)
{
	int cmd=RFIFOW(fd,0);
	int len=0;

	// inter鯖管轄かを調べる
	if(cmd<0x3000 || cmd>=0x3000+( sizeof(inter_recv_packet_length)/
		sizeof(inter_recv_packet_length[0]) ) )
		return 0;

	// パケット長を調べる
	if(	(len=inter_check_length(fd,inter_recv_packet_length[cmd-0x3000]))==0 )
		return 2;
	
	switch(cmd){
	case 0x3000: mapif_parse_GMmessage(fd); break;
	case 0x3001: mapif_parse_WisRequest(fd); break;
	case 0x3002: mapif_parse_WisReply(fd); break;
	default:
		if( inter_party_parse_frommap(fd) )
			break;
		if( inter_guild_parse_frommap(fd) )
			break;
		if( inter_storage_parse_frommap(fd) )
			break;
		if( inter_pet_parse_frommap(fd) )
			break;
		return 0;
	}
	RFIFOSKIP(fd, len );
	return 1;
}

// RFIFOのパケット長確認
// 必要パケット長があればパケット長、まだ足りなければ0
int inter_check_length(int fd,int length)
{
	if(length==-1){	// 可変パケット長
		if(RFIFOREST(fd)<4)	// パケット長が未着
			return 0;
		length = RFIFOW(fd,2);
	}
	
	if(RFIFOREST(fd)<length)	// パケットが未着
		return 0;
	
	return length;
}

