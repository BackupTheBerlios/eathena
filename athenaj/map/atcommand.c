//atcommand.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "socket.h"
#include "timer.h"

#include "clif.h"
#include "chrif.h"
#include "intif.h"
#include "itemdb.h"
#include "map.h"
#include "pc.h"
#include "skill.h"
#include "mob.h"
#include "pet.h"
#include "battle.h"
#include "party.h"
#include "guild.h"
#include "atcommand.h"

static char msg_table[200][1024];	/* Server message */

struct Atcommand_Config atcommand_config;

int atcommand(int fd,struct map_session_data *sd,char *message)
{
	int gm_level = pc_isGM(sd);
	int flag;
	char s_flag=0;
	//���R�}���hGM��p��
//	if (battle_config.atc_gmonly && !gm_level ) return 0;

	message += strlen(sd->status.name);
	while(*message && (isspace(*message) || (s_flag==0 && *message==':'))) {
		if(*message==':') s_flag=1;
		message++;
	}

	if (message && message[0] == '@') {	//���b�Z�[�W���� @ �����ꂽ��
		char command[100];		//@rura �Ȃǂ̃V�X�e���R�}���h�����񂪓���
		char temp0[100];			//����1��
		char temp1[100];			//����2��
//      char temp2[100];         //����3��
		char moji[200];			//����������
		int x = 0, y = 0, z = 0;	//���W�Ƃ�
		int i1 = 0, i2 = 0, i3 = 0, i;
		struct map_session_data *pl_sd;

		sscanf(message, "%s",command);

//�o�V���[��
//�u@rura+ ���O �}�b�v�t�@�C���� �����W y���W�v�Ɠ���
		if (strcmpi(command, "@rura+") == 0 && gm_level >= atcommand_config.rurap) {
			sscanf(message, "%s%s%s%d%d", command, temp1, temp0, &x, &y);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					if (x >= 0 && x < 400 && y >= 0 && y < 400) {
						if(pc_setpos(pl_sd, temp0, x, y, 3)==0){
							clif_displaymessage(pl_sd->fd,msg_table[0]);
						}else{
							clif_displaymessage(fd,msg_table[1]);
						}
					}else{
						clif_displaymessage(fd,msg_table[2]);
					}
				}
			}else{
				clif_displaymessage(fd,msg_table[3]);
			}
			return 1;
		}
//���[��
//�u@rura �}�b�v�t�@�C���� �����W y���W�v�Ɠ���
		if (strcmpi(command, "@rura") == 0 && gm_level >= atcommand_config.rura) {
			sscanf(message, "%s%s%d%d", command, temp0, &x, &y);
			if (x >= 0 && x < 400 && y >= 0 && y < 400) {
				if(pc_setpos(sd, temp0, x, y, 3)==0){
					clif_displaymessage(fd,msg_table[0]);
				}else{
					clif_displaymessage(fd,msg_table[1]);
				}
			}else{
				clif_displaymessage(fd,msg_table[2]);
			}
			return 1;
		}
//�C�ӂ̃L�����̋��ꏊ�𒲂ׂ�
//�u@where �L�������v�Ɠ���
		if(strcmpi(command, "@where") == 0 && gm_level >= atcommand_config.where){
			sscanf(message, "%s%s", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				sprintf(moji, "%s %s %d %d", temp1, pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y);
				clif_displaymessage(fd,moji);
			}else{
//				clif_displaymessage(fd,msg_table[4]);
				sprintf(moji, "%s %d %d", sd->mapname, sd->bl.x, sd->bl.y);
				clif_displaymessage(fd,moji);
			}
			return 1;
		}
//�C�ӂ̃L�����̏��ֈړ�����
//�u@jumpto �L�������v�Ɠ���
		if(strcmpi(command, "@jumpto") == 0 && gm_level >= atcommand_config.jumpto){
			sscanf(message, "%s %[^\n]", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				pc_setpos(sd, pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y, 3);
				sprintf(moji,msg_table[4],temp1);
				clif_displaymessage(fd,moji);
			}else{
				clif_displaymessage(fd,msg_table[3]);
			}
			return 1;
		}
//���}�b�v���̔C�Ӎ��W�ɒ���
//�u@jump x���W y���W�v�Ɠ���
		if(strcmpi(command, "@jump") == 0 && gm_level >= atcommand_config.jump){
			sscanf(message, "%s%d%d", command, &x, &y);
			if (x >= 0 && x < 400 && y >= 0 && y < 400) {
				pc_setpos(sd, sd->mapname, x, y, 3);
				sprintf(moji, msg_table[5], x, y);
				clif_displaymessage(fd,moji);
			}else{
				clif_displaymessage(fd,msg_table[2]);
			}
			return 1;
		}
//���݃��O�C�����Ă���S�ẴL��������񋓂���
//�u@who�v�Ɠ���
		if(strcmpi(command, "@who") == 0 && gm_level >= atcommand_config.who){
			for(i=0;i<fd_max;i++){	//�l�������[�v
				if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
					clif_displaymessage(fd,pl_sd->status.name);
				}
			}
			return 1;
		}
//���݈ʒu���܂߁A�`���̋L�^���Z�[�u����
//�u@save�v�Ɠ���
		if (strcmpi(command, "@save") == 0 && gm_level >= atcommand_config.save) {
			pc_setsavepoint(sd,sd->mapname,sd->bl.x,sd->bl.y);
			if(sd->status.pet_id > 0 && sd->pd)
				intif_save_petdata(sd->status.account_id,&sd->pet);
			pc_makesavestatus(sd);
			chrif_save(sd);
			storage_storage_save(sd);
			clif_displaymessage(fd,msg_table[6]);
			return 1;
		}
//�Z�[�u�n�_�Ƀ��[�v����
//�u@load�v�Ɠ���
		if (strcmpi(command, "@load") == 0 && gm_level >= atcommand_config.load) {
			pc_setpos(sd,sd->status.save_point.map , sd->status.save_point.x , sd->status.save_point.y, 0);
			clif_displaymessage(fd,msg_table[7]);
			return 1;
		}
//���s�X�s�[�h�ύX
//�u@speed �X�s�[�h�l[1�`1000]�v�Ɠ���,�������قǑ����A�߂��ዓ���s�R��
		if (strcmpi(command, "@speed") == 0 && gm_level >= atcommand_config.speed) {
			sscanf(message, "%s%d", command, &x);
			if (x > 0 && x < 1000) {
				sd->speed = x;
				//sd->walktimer = x;
				//���̕���ǉ� by �ꂠ
				clif_updatestatus(sd,SP_SPEED);
				clif_displaymessage(fd,msg_table[8]);
			}
			return 1;
		}
//�q�ɗ��p
//�u@storage�v�Ɠ���
		if(strcmpi(command, "@storage") == 0 && gm_level >= atcommand_config.storage) {
			storage_storageopen(sd);
			return 1;
		}
//��ԕύX
//�u@option 1 20 15�v�̂悤�ɒl���L�q
/*
	R 0119 <ID>.l <param1>.w <param2>.w <param3>.w ?.B
	�����ڕύX
	param1=01 �Ή��H(�ł܂�)
	param1=02 �t���X�g�_�C�o�œ���Ђ�?
	param1=03 �҂��
	param1=04 ����
	param1=06 �È�(������)
	
	param2=01 ��
	param2=02 �w���
	param2=04 ���ُ��
	param2=16 ���͂��Â�����
	
	param3=01 �T�C�g�������b�`?
	param3=02 �n�C�f�B���O���?
	param3=04 �N���[�L���O���?
	param3=08 �J�[�g�t��
	param3=16 ��t��
	param3=32 �y�R�y�R���
	param3=64 ������
*/
		if (strcmpi(command, "@option") == 0 && gm_level >= atcommand_config.option) {
			sscanf(message, "%s%d%d%d", command, &x, &y, &z);
			/*
			   p->option �͎��̂悤��bit�ɂ����Đ��藧���Ă���H
			   pram1        0000 0000 0000 0000
			   pram2        0000 0000 0000 0000
			   pram3        0000 0000 0000 0000 

			 */
			sd->opt1=x;
			sd->opt2=y;
			if(!(sd->status.option & CART_MASK) && z & CART_MASK) {
				clif_cart_itemlist(sd);
				clif_cart_equiplist(sd);
				clif_updatestatus(sd,SP_CARTINFO);
			}
			sd->status.option=z;
			clif_changeoption(&sd->bl);
			clif_displaymessage(fd,msg_table[9]);
			return 1;
		}
//������
//�u@hide�v�Ɠ���
		if (strcmpi(command, "@hide") == 0 && gm_level >= atcommand_config.hide) {
			if(sd->status.option&0x40){
				sd->status.option&=~0x40;
				clif_displaymessage(fd,msg_table[10]);
			}else{
				sd->status.option|=0x40;
				clif_displaymessage(fd,msg_table[11]);
			}
			clif_changeoption(&sd->bl);
			return 1;
		}
//���̏�œ]�E
//�u@jobchange �E��ID[0�`21]�v�̂悤�ɐE�Ƃ��Ƃɒl���L�q
/*
0	Novice
1	Swordman
2	Mage
3	Archer
4	Acolyte
5	Merchant
6	Thief
7	Knight
8	Priest
9	Wizard
10	Blacksmith
11	Hunter
12	Assassin
13	Knight2
14	Crusader
15	Monk
16	Sage
17	Rogue
18	Alchem
19	Bard
20	Dancer
21	Crusader2
*/
		if (strcmpi(command, "@jobchange") == 0 && gm_level >= atcommand_config.jobchange) {
			sscanf(message, "%s%d", command, &x);
			if ((x >= 0 && x < MAX_PC_CLASS)) {
				pc_jobchange(sd,x);
				clif_displaymessage(fd,msg_table[12]);
			}
			return 1;
		}
//���E
//�u@die�v�Ɠ��͂���B
		if (strcmpi(command, "@die") == 0 && gm_level >= atcommand_config.die) {
			pc_damage(NULL,sd,sd->status.hp+1);
			clif_displaymessage(fd,msg_table[13]);
			return 1;
		}
//���E
//Modified by Syrus22 - September 17, 2003
//@kill <char name>
//Description: Kills target character. Including yourself.
		if (strcmpi(command, "@kill") == 0 && gm_level >= atcommand_config.kill) {
			sscanf(message, "%s %[^\n]", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					pc_damage(NULL,pl_sd,pl_sd->status.hp+1);
					clif_displaymessage(fd,msg_table[14]);
				}
			}else{
				clif_displaymessage(fd,msg_table[15]);
			}
			return 1;
		}
//�����Ԃ�
//[@alive]�Ɠ��͂���
		if (strcmpi(command, "@alive") == 0 && gm_level >= atcommand_config.alive) {
			sd->status.hp=sd->status.max_hp;
			pc_setstand(sd);
			if(battle_config.pc_invincible_time > 0)
				pc_setinvincibletimer(sd,battle_config.pc_invincible_time);
			clif_updatestatus(sd,SP_HP);
			clif_resurrection(&sd->bl,1);
			clif_displaymessage(fd,msg_table[16]);
			return 1;
		}
//�V�̐�
//�u@kami �䎌�v�Ɠ���
		if ((strcmpi(command, "@kami") == 0 || strcmpi(command, "@kamib") == 0) && gm_level >= atcommand_config.kami) {
			sscanf(message, "%s %[^\n]", command, moji);
			intif_GMmessage(moji,strlen(moji)+1,(command[5]=='b')?0x10:0);
			return 1;
		}
//HP,SP �X�V
//�u@heal ���l(HP) ���l(SP)�v�Ɠ���
//�ȗ����āu@heal�v�Ƃ����łĂ΁A�����S��
		if (strcmpi(command, "@heal") == 0 && gm_level >= atcommand_config.heal) {
			sscanf(message, "%s%d%d", command, &x, &y);
			if (x <= 0 && y <= 0) {	//�ȗ��L�q�őS��
				x=sd->status.max_hp-sd->status.hp;
				y=sd->status.max_sp-sd->status.sp;
			} else {
				if (sd->status.hp + x > sd->status.max_hp) {
					x = sd->status.max_hp - sd->status.hp;
				}
				if (sd->status.sp + y > sd->status.max_sp) {
					y = sd->status.max_sp - sd->status.sp;
				}
			}
			clif_heal(fd,SP_HP,(x > 0x7fff)? 0x7fff:x);
			clif_heal(fd,SP_SP,(y > 0x7fff)? 0x7fff:y);
			pc_heal(sd,x,y);
			clif_displaymessage(fd,msg_table[17]);
			return 1;
		}
//�A�C�e���Q�b�g
//�u@item �A�C�e���̖��OorID ���v�Ɠ���
		if (strcmpi(command, "@item") == 0 && gm_level >= atcommand_config.item) {
			struct item item_tmp;
			struct item_data *item_data;
			sscanf(message, "%s%s%d", command, temp0, &y);
			if(y==0) y=1;	//���w�肪����������P�ɂ���by�ꂠ
			
			if( (x=atoi(temp0))>0 ){
				if(battle_config.item_check){
					x=(( (item_data=itemdb_exists(x)) && itemdb_available(x))?x:0);
				}else
					item_data=itemdb_search(x);
			}else if( (item_data=itemdb_searchname(temp0))!=NULL )
				x=(!battle_config.item_check ||
				 itemdb_available(item_data->nameid))?item_data->nameid:0;
			
			if( x>0 ) {
				int a1=1,a2=y,i;
				if(item_data->type==4 || item_data->type==5 || item_data->type==7){
					a1=y;
					a2=1;
				}
				for(i=0;i<a1;i++){
					memset(&item_tmp,0,sizeof(item_tmp));
					item_tmp.nameid=x;
					item_tmp.identify=1;
					if((flag = pc_additem(sd,&item_tmp,a2)))
						clif_additem(sd,0,0,flag);
				}
				clif_displaymessage(fd,msg_table[18]);
			} else {
				clif_displaymessage(fd,msg_table[19]);
			}
			return 1;
		}
//�����A�C�e�����Z�b�g
//�u@itemreset�v�Ɠ��́@�p�����[�^�����i����)
		if (strcmpi(command, "@itemreset") == 0 && gm_level >= atcommand_config.itemreset) {
			for(i=0;i<MAX_INVENTORY;i++) {
				if (sd->status.inventory[i].amount && sd->status.inventory[i].equip ==0)
					pc_delitem(sd,i,sd->status.inventory[i].amount,0);
			}
			clif_displaymessage(fd,msg_table[20]);
			return 1;
		}
//�����A�C�e���̃`�F�b�N
		if (strcmpi(command, "@itemcheck") == 0 && gm_level >= atcommand_config.itemcheck) {
			pc_checkitem(sd);
		}
//Lv�A�b�v�R�}���h
//�u@Lvup �����l�v�Ɠ���
		if (strcmpi(command, "@lvup") == 0 && gm_level >= atcommand_config.lvup) {
			sscanf(message, "%s%d", command, &x);
			if(x >= 1){
				for(i=sd->status.base_level+1;i<=sd->status.base_level+x;i++){
					if(i>=99)
						sd->status.status_point += 22;
					else
						sd->status.status_point += (i+14) / 5 ;
				}
				sd->status.base_level+=x;
				clif_updatestatus(sd,SP_BASELEVEL);
				clif_updatestatus(sd,SP_NEXTBASEEXP);
				clif_updatestatus(sd,SP_STATUSPOINT);
				pc_calcstatus(sd,0);
				pc_heal(sd,sd->status.max_hp,sd->status.max_sp);
				clif_misceffect(&sd->bl,0);
				clif_displaymessage(fd,msg_table[21]);
			}else if(x<0 && sd->status.base_level+x>0){
				sd->status.base_level+=x;
				clif_updatestatus(sd,SP_BASELEVEL);
				clif_updatestatus(sd,SP_NEXTBASEEXP);
				pc_calcstatus(sd,0);
				clif_displaymessage(fd,msg_table[22]);
			}
			return 1;
		}
//JobLv�A�b�v�R�}���h
//�u@jobLvup �����l�v�Ɠ���
		if(strcmpi(command, "@joblvup") == 0 && gm_level >= atcommand_config.joblvup){
			y = 50;
			sscanf(message, "%s%d", command, &x);
			if(sd->status.class == 0)
				y -= 40;
			if(sd->status.job_level == y){
				clif_displaymessage(fd,msg_table[23]);
			}else if(x >= 1){
				if(sd->status.job_level + x > y)
					x = y - sd->status.job_level;
				sd->status.job_level+=x;
				clif_updatestatus(sd,SP_JOBLEVEL);
				clif_updatestatus(sd,SP_NEXTJOBEXP);
				sd->status.skill_point+=x;
				clif_updatestatus(sd,SP_SKILLPOINT);
				pc_calcstatus(sd,0);
				clif_misceffect(&sd->bl,1);
				clif_displaymessage(fd,msg_table[24]);
			}else if(x<0 && sd->status.job_level+x>0 ){
				sd->status.job_level+=x;
				clif_updatestatus(sd,SP_JOBLEVEL);
				clif_updatestatus(sd,SP_NEXTJOBEXP);
				pc_calcstatus(sd,0);
				clif_displaymessage(fd,msg_table[25]);
			}
			return 1;
		}
//�w���v�R�}���h�@���R�}���h�̑S�e��������Ă����B
//�u@help�v�Ɠ���
		if ((strcmpi(command, "@h") == 0 || strcmpi(command, "@help") == 0) && gm_level >= atcommand_config.help) {
			char moji[400];
			FILE *file;
			if(	(file = fopen(help_txt, "r"))!=NULL){
				clif_displaymessage(fd,msg_table[26]);
				while (fgets(moji, 380, file) != NULL) {
					{
						int i;
						for (i = 0; moji[i] != '\0'; i++) {
							if (moji[i] == '\r' || moji[i] == '\n') {
								moji[i] = '\0';
							}
						}
					}
					clif_displaymessage(fd,moji);
				}
				fclose(file);
			}else
				clif_displaymessage(fd,msg_table[27]);
			return 1;
		}
//GM�ɂȂ�I ���C�̑S�L������PT���甲���A�q�ɂ͋�ɂ��ĉ������B
//�u@GM�v�Ɠ���
		if (strcmpi(command, "@GM") == 0 && gm_level >= atcommand_config.gm) {
			moji[0]=0;
			sscanf(message, "%s %[^\n]", command,moji);
			if(sd->status.party_id)
				clif_displaymessage(fd,msg_table[28]);
			else if(sd->status.guild_id)
				clif_displaymessage(fd,msg_table[29]);
			else{
				if(sd->status.pet_id > 0 && sd->pd)
					intif_save_petdata(sd->status.account_id,&sd->pet);
				pc_makesavestatus(sd);
				chrif_save(sd);
				storage_storage_save(sd);
				clif_displaymessage(fd,msg_table[30]);
				chrif_changegm(sd->status.account_id,moji,strlen(moji)+1);
			}
			return 1;
		}

//PVP�����@�@�����A�}�b�v�ړ����Ȃ��Ɖ�������܂���B
//�u@pvpoff�v�Ɠ���
		if (strcmpi(command, "@pvpoff") == 0 && gm_level >= atcommand_config.pvpoff) {
			if(map[sd->bl.m].flag.pvp) {
				map[sd->bl.m].flag.pvp = 0;
				clif_send0199(sd->bl.m,0);
				for(i=0;i<fd_max;i++){	//�l�������[�v
					if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
						if(sd->bl.m == pl_sd->bl.m) {
							clif_pvpset(pl_sd,0,0,2);
							if(pl_sd->pvp_timer != -1) {
								delete_timer(pl_sd->pvp_timer,pc_calc_pvprank_timer);
								pl_sd->pvp_timer = -1;
							}
						}
					}
				}
				clif_displaymessage(fd,msg_table[31]);
			}
			return 1;
		}

//PVP�����i���j
//�u@pvp�v�Ɠ���
		if (strcmpi(command, "@pvpon") == 0 && gm_level >= atcommand_config.pvpon) {
			if(!map[sd->bl.m].flag.pvp) {
				map[sd->bl.m].flag.pvp = 1;
				clif_send0199(sd->bl.m,1);
				for(i=0;i<fd_max;i++){	//�l�������[�v
					if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
						if(sd->bl.m == pl_sd->bl.m && pl_sd->pvp_timer == -1) {
							pl_sd->pvp_timer=add_timer(gettick()+200,pc_calc_pvprank_timer,pl_sd->bl.id,0);
							pl_sd->pvp_rank=0;
							pl_sd->pvp_lastusers=0;
							pl_sd->pvp_point=5;
						}
					}
				}
				clif_displaymessage(fd,msg_table[32]);
			}
			return 1;
		}

//@gvgoff
		if (strcmpi(command, "@gvgoff") == 0 && gm_level >= atcommand_config.gvgoff) {
			if(map[sd->bl.m].flag.gvg) {
				map[sd->bl.m].flag.gvg = 0;
				clif_send0199(sd->bl.m,0);
				clif_displaymessage(fd,msg_table[33]);
			}
			return 1;
		}

//@gvgon
		if (strcmpi(command, "@gvgon") == 0 && gm_level >= atcommand_config.gvgon) {
			if(!map[sd->bl.m].flag.gvg) {
				map[sd->bl.m].flag.gvg = 1;
				clif_send0199(sd->bl.m,3);
				clif_displaymessage(fd,msg_table[34]);
			}
			return 1;
		}

//���^�A���̐F�A���̐F�A�ύX
//�u@model x y z�v�̂悤�ɒl����́A������������ύX������Ԃ�2HQ���g���ƃG���[���E�E�E(�L�D�M)�@�𖾂����������܂��B
/*	��F@model 15 4 0

x [0�`19]���^
y [0�`8]���̐F
z [0�`4]���̐F

*/
		if (strcmpi(command, "@model") == 0 && gm_level >= atcommand_config.model) {
			sscanf(message, "%s%d%d%d", command, &x, &y, &z);
			if (x >= 0 && x < 20 && y >= 0 && y < 9 && z >= 0 && z <= 4) {
				//���̐F�ύX
				if ((sd->status.class == 12 || sd->status.class == 14 || sd->status.class == 15 || sd->status.class == 16 || sd->status.class == 17 || sd->status.class == 21) && z != 0) {
					//���̐F�������E�̔���
					clif_displaymessage(fd,msg_table[35]);
				} else {
					pc_changelook(sd,LOOK_HAIR,x);
					pc_changelook(sd,LOOK_HAIR_COLOR,y);
					pc_changelook(sd,LOOK_CLOTHES_COLOR,z);
					clif_displaymessage(fd,msg_table[36]);
				}
			} else {
				clif_displaymessage(fd,msg_table[37]);
			}
			return 1;
		}

// ADDED
// ���[���ȈՔ�
//�u@go �����v�Ɠ���
		if (strcmpi(command, "@go") == 0 && gm_level >= atcommand_config.go) {
			struct { char map[16]; int x,y; } data[] = {
				{	"prontera.gat",	156, 191	},	//	0=�v�����e��
				{	"morocc.gat",	156,  93	},	//	1=�����N
				{	"geffen.gat",	119,  59	},	//	2=�Q�t�F��
				{	"payon.gat",	 89, 122	},	//	3=�t�F�C����
				{	"alberta.gat",	192, 147	},	//	4=�A���x���^
				{	"izlude.gat",	128, 114	},	//	5=�C�Y���[�h
				{	"aldebaran.gat",140, 131	},	//	6=�A���f�o����
				{	"xmas.gat",		147, 134	},	//	7=���e�B�G
				{	"comodo.gat",	209, 143	},	//	8=�R���h
				{	"yuno.gat",		157,  51	},	//	9=�W���m�[
				{	"amatsu.gat",	198,  84	},	//	10=�A�}�c
				{	"gonryun.gat",	160, 120	},	//	11=�S��������

			};
			sscanf(message, "%s%d", command, &x);
			if (x >= 0 && x<sizeof(data)/sizeof(data[0])) {
				pc_setpos(sd, data[x].map, data[x].x, data[x].y, 3);
			} else {
				clif_displaymessage(fd,msg_table[38]);
			}
			return 1;
		}
//�����X�^�[����
//�u@monster �\���� �����X�^�[IDor���O �����X�^�[�� x���W y���W�v�Ɠ���
// x���W y���W �͏ȗ��i�����_���j
		if (strcmpi(command, "@monster") == 0 && gm_level >= atcommand_config.monster) {
			i1 = i2 = x = y = 0;
			if(sscanf(message, "%s %s %s %d%d%d", command, temp1, temp0, &i2, &x, &y) >= 3) {
				int count=0;
				if( (i1=atoi(temp0))==0 )
					i1=mobdb_searchname(temp0);
				if( i2 <= 0 ) i2 = 1;
				if(battle_config.etc_log)
					printf("%s name=%s id=%d count=%d (%d,%d)\n", command, temp1, i1, i2, x, y);

				for(i=0;i<i2;i++) {
					int mx,my;
					if(x<=0) mx = sd->bl.x + (rand()%10 - 5);
					else mx = x;
					if(y<=0) my=sd->bl.y + (rand()%10 - 5);
					else my = y;
					count+=(mob_once_spawn(sd,"this",mx,my,temp1,i1,1,"")!=0)? 1:0;
				}
				if(count != 0){
					clif_displaymessage(fd,msg_table[39]);
				}else{
					clif_displaymessage(fd,msg_table[40]);
				}
			}
			return 1;
		}
// ���B @refine �����ꏊID +���l
// �E��=2 ����=32 ����=34 ��=256/257/768/769 ��=16 ��=4 ��=64
		if(strcmpi(command,"@refine")==0 && gm_level >= atcommand_config.refine){
			i1=i2=0;
			if(sscanf(message,"%s%d%d",command,&i1,&i2) >= 3) {
				for(i=0;i< MAX_INVENTORY;i++){
					if( sd->status.inventory[i].nameid &&	// �Y�����̑����𐸘B����
					    (sd->status.inventory[i].equip&i1 || (sd->status.inventory[i].equip && !i1) ) ) {
						if(i2<=0 || i2>10 ) i2=1;
						if( sd->status.inventory[i].refine<10 ){
							sd->status.inventory[i].refine+=i2;
							if(sd->status.inventory[i].refine>10)
								sd->status.inventory[i].refine=10;
							x = sd->status.inventory[i].equip;
							pc_unequipitem(sd,i,0);	// ��������
							clif_refine(fd,sd,0,i,sd->status.inventory[i].refine);	// ���B�G�t�F�N�g �Ƃ肠�����K������
							clif_delitem(sd,i,1);	// ���B�O�A�C�e������
							clif_additem(sd,i,1,0);	// ���B��A�C�e������
							pc_equipitem(sd,i,x);	// �đ����i�܃X�e�[�^�X�v�Z�j
							clif_misceffect(&sd->bl,3); // ���l�ɂ�������ʒm
						}
					}
				}
			}
			return 1;
		}
// ���퐻�� @produce �A�C�e��IDor���O ���� ���̂�����̐�
// ���� 0=���� 1=�� 2=�n 3=�� 4=��
// �ʂɑ����{���̂�����R�ł������܂��B
		if(strcmpi(command,"@produce")==0 && gm_level >= atcommand_config.produce){
			i1=i2=i3=0;
			if(sscanf(message,"%s%s%d%d",command,temp0,&i2,&i3) >= 2) {
				if( (i1=atoi(temp0))==0){
					struct item_data *item_data=itemdb_searchname(temp0);
					if(item_data)
						i1=item_data->nameid;
				}
				if( itemdb_exists(i1) &&
					(i1<=500 || i1>1099) && (i1<4001 || i1>4148) &&
					(i1<7001 || i1>10019) && itemdb_isequip(i1)){
					struct item tmp_item;
					if(i2<0 || i2>4) i2=0;	// �����͈�
					if(i3<0 || i3>3 ) i3=0;	// ���̂������
					memset(&tmp_item,0,sizeof(tmp_item));
					tmp_item.nameid=i1;
					tmp_item.amount=1;
					tmp_item.identify=1;
					tmp_item.card[0]=0x00ff; // ��������t���O
					tmp_item.card[1]=((i3*5)<<8)+i2;	// �����Ƃ悳
					*((unsigned long *)(&tmp_item.card[2]))=sd->char_id;	// �L����ID
					clif_produceeffect(sd,0,i1);// �����G�t�F�N�g�p�P�b�g
					clif_misceffect(&sd->bl,3); // ���l�ɂ�������ʒm�i���B�����G�t�F�N�g�Ɠ����ł����́H�j
					if((flag = pc_additem(sd,&tmp_item,1)))
						clif_additem(sd,0,0,flag);
				}
				else {
					if(battle_config.error_log)
						printf("@produce NOT WEAPON [%d]\n",i1);
				}
			}
			return 1;
		}
// �C�ӈʒu���� @memo �ԍ� �ԍ���0~2
		if(strcmpi(command,"@memo")==0 && gm_level >= atcommand_config.memo){
			i1=0;
			sscanf(message,"%s%d",command,&i1);
			if(i1<0 || i1>2) i1=0;
			pc_memo(sd,i1);
			return 1;
		}

// �f�o�O�p�i����gat�𒲂ׂ�j
		if(strcmpi(command,"@gat")==0 && gm_level >= atcommand_config.gat){
			for(y=2;y>=-2;y--){
				sprintf(moji,"%s (x= %d, y= %d) %02X %02X %02X %02X %02X",map[sd->bl.m].name,sd->bl.x-2,sd->bl.y+y,
					map_getcell(sd->bl.m,sd->bl.x-2,sd->bl.y+y),map_getcell(sd->bl.m,sd->bl.x-1,sd->bl.y+y),
					map_getcell(sd->bl.m,sd->bl.x,sd->bl.y+y),map_getcell(sd->bl.m,sd->bl.x+1,sd->bl.y+y),
					map_getcell(sd->bl.m,sd->bl.x+2,sd->bl.y+y));
				clif_displaymessage(fd,moji);
			}
			return 1;
		}

// �f�o�O�p�i�p�P�b�g�F�X�j
		if(strcmpi(command,"@packet")==0 && gm_level >= atcommand_config.packet){
			sscanf(message,"%s %d %d",command,&x,&y);
			clif_status_change(&sd->bl,x,y);
			return 1;
		}

// �X�e�[�^�X�|�C���g����
		if(strcmpi(command,"@stpoint")==0 && gm_level >= atcommand_config.stpoint) {
			sscanf(message,"%s%d",command,&i1);
			if(i1>0 || sd->status.status_point+i1>=0){
				sd->status.status_point += i1;
				clif_updatestatus(sd,SP_STATUSPOINT);
			}else
				clif_displaymessage(fd,msg_table[41]);
			return 1;
		}

// �X�L���|�C���g����
		if(strcmpi(command,"@skpoint")==0 && gm_level >= atcommand_config.skpoint) {
			sscanf(message,"%s%d",command,&i1);
			if(i1>0 || sd->status.skill_point+i1>=0){
				sd->status.skill_point += i1;
				clif_updatestatus(sd,SP_SKILLPOINT);
			}else
				clif_displaymessage(fd,msg_table[41]);
			return 1;
		}

// �[�j�[�A�b�v����
		if(strcmpi(command,"@zeny")==0 && gm_level >= atcommand_config.zeny) {
			sscanf(message,"%s%d",command,&i1);
			if(i1>0 || sd->status.zeny+i1>=0){
				sd->status.zeny += i1;
				clif_updatestatus(sd,SP_ZENY);
			}else
				clif_displaymessage(fd,msg_table[41]);
			return 1;
		}

// ��{�p�����[�^����
		if(	( (i2=0,strcmpi(command,"@str")==0) ||
			(i2=1,strcmpi(command,"@agi")==0) ||
			(i2=2,strcmpi(command,"@vit")==0) ||
			(i2=3,strcmpi(command,"@int")==0) ||
			(i2=4,strcmpi(command,"@dex")==0) ||
			(i2=5,strcmpi(command,"@luk")==0) ) && gm_level >= atcommand_config.param){
			short *p[]={
				&sd->status.str,&sd->status.agi,&sd->status.vit,
				&sd->status.int_,&sd->status.dex,&sd->status.luk
			};

			sscanf(message,"%s%d",command,&i1);
			
			i=(int)(*p[i2])+i1;
			if(i< 1) i1=1-*p[i2];
			if(i>battle_config.max_parameter) i1=battle_config.max_parameter-*p[i2];
			*p[i2]+=i1;
			clif_updatestatus(sd,SP_STR+i2);
			clif_updatestatus(sd,SP_USTR+i2);
			pc_calcstatus(sd,0);
			clif_displaymessage(fd,msg_table[42]);
			return 1;
		}

// �M���hLv����
		if(strcmpi(command,"@guildlvup")==0 && gm_level >= atcommand_config.guildlvup) {
			struct guild *g;
			sscanf(message,"%s%d",command,&i1);
			if( sd->status.guild_id<=0 || (g=guild_search(sd->status.guild_id))==NULL){
				clif_displaymessage(fd,msg_table[43]);
				return 1;
			}
			if( strcmp(sd->status.name,g->master)!=0 ){
				clif_displaymessage(fd,msg_table[44]);
				return 1;
			}
			
			if( g->guild_lv+i1>=1 && g->guild_lv+i1<=50){
				intif_guild_change_basicinfo(g->guild_id,GBI_GUILDLV,&i1,2);
			}else
				clif_displaymessage(fd,msg_table[45]);
			return 1;
		}

// �y�b�g�쐬
		if(strcmpi(command,"@makepet")==0 && gm_level >= atcommand_config.makepet) {
			sscanf(message,"%s%d",command,&i1);
			x = search_petDB_index(i1,PET_CLASS);
			if(x < 0)
				x = search_petDB_index(i1,PET_EGG);
			if(x >= 0) {
				sd->catch_target_class = pet_db[x].class;
				intif_create_pet(sd->status.account_id,sd->status.char_id,pet_db[x].class,mob_db[pet_db[x].class].lv,
					pet_db[x].EggID,0,pet_db[x].intimate,100,0,1,pet_db[x].jname);
			}
			return 1;
		}

// �y�b�g�e���x�ύX
		if(strcmpi(command,"@petfriendly")==0 && gm_level >= atcommand_config.petfriendly) {
			sscanf(message,"%s%d",command,&i1);
			if(sd->status.pet_id > 0 && sd->pd) {
				if(i1 >= 0 && i1 <= 1000) {
					int t = sd->pet.intimate;
					sd->pet.intimate = i1;
					clif_send_petstatus(sd);
					if(battle_config.pet_status_support) {
						if((sd->pet.intimate > 0 && t <= 0) || (sd->pet.intimate <= 0 && t > 0) ) {
							if(sd->bl.prev != NULL)
								pc_calcstatus(sd,0);
							else
								pc_calcstatus(sd,2);
						}
					}
				}
			}
			return 1;
		}

// �y�b�g�����x�ύX
		if(strcmpi(command,"@pethungry")==0 && gm_level >= atcommand_config.pethungry) {
			sscanf(message,"%s%d",command,&i1);
			if(sd->status.pet_id > 0 && sd->pd) {
				if(i1 >= 0 && i1 <= 100) {
					sd->pet.hungry = i1;
					clif_send_petstatus(sd);
				}
			}
			return 1;
		}

// �y�b�g���O�ύX
		if(strcmpi(command,"@petrename")==0 && gm_level >= atcommand_config.petrename) {
			if(sd->status.pet_id > 0 && sd->pd) {
				sd->pet.rename_flag = 0;
				intif_save_petdata(sd->status.account_id,&sd->pet);
				clif_send_petstatus(sd);
			}
			return 1;
		}

//-Begin- Commands Added by Syrus22
//September 17, 2003
//@recall <char name>
//Description: Warps target character to you.
		if(strcmpi(command, "@recall") == 0 && gm_level >= atcommand_config.recall){
			sscanf(message, "%s %[^\n]", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					pc_setpos(pl_sd, sd->mapname, sd->bl.x, sd->bl.y, 2);
					sprintf(moji,msg_table[46],temp1);
					clif_displaymessage(fd,moji);
				}
			}else{
				clif_displaymessage(fd,msg_table[47]);
			}
			return 1;
		}
//September 17, 2003
//@charjob <job ID> <char name>
//Description: Changes target characters job.
		if(strcmpi(command, "@charjob") == 0 && gm_level >= atcommand_config.charjob){
			sscanf(message, "%s%d %[^\n]", command, &x, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					if ((x >= 0 && x < MAX_PC_CLASS)) {
						pc_jobchange(pl_sd,x);
						clif_displaymessage(fd,msg_table[48]);
					}else{
						clif_displaymessage(fd,msg_table[49]);
					}
				}
			}else{
				clif_displaymessage(fd,msg_table[50]);
			}
		return 1;
		}
//September 17, 2003
//@revive <char name>
//Description: Revives target character.
		if (strcmpi(command, "@revive") == 0 && gm_level >= atcommand_config.revive) {
			sscanf(message, "%s %[^\n]", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				pl_sd->status.hp=pl_sd->status.max_hp;
				pc_setstand(pl_sd);
				if(battle_config.pc_invincible_time > 0)
					pc_setinvincibletimer(sd,battle_config.pc_invincible_time);
				clif_updatestatus(pl_sd,SP_HP);
				clif_resurrection(&pl_sd->bl,1);
				clif_displaymessage(fd,msg_table[51]);
			}else{
				clif_displaymessage(fd,msg_table[52]);
			}
			return 1;
		}
//September 18, 2003
//@charstats <charname>
//Description: Displays a characters stats.
		if (strcmpi(command, "@charstats") == 0 && gm_level >= atcommand_config.charstats) {
			sscanf(message, "%s %[^\n]", command, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				sprintf(moji,msg_table[53], pl_sd->status.name);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Base Level - %d", pl_sd->status.base_level);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Job Level - %d", pl_sd->status.job_level);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Hp - %d", pl_sd->status.hp);
				clif_displaymessage(fd,moji);
				sprintf(moji,"MaxHp - %d", pl_sd->status.max_hp);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Sp - %d", pl_sd->status.sp);
				clif_displaymessage(fd,moji);
				sprintf(moji,"MaxSp - %d", pl_sd->status.max_sp);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Str - %d", pl_sd->status.str);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Agi - %d", pl_sd->status.agi);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Vit - %d", pl_sd->status.vit);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Int - %d", pl_sd->status.int_);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Dex - %d", pl_sd->status.dex);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Luk - %d", pl_sd->status.luk);
				clif_displaymessage(fd,moji);
				sprintf(moji,"Zeny - %d", pl_sd->status.zeny);
				clif_displaymessage(fd,moji);
			}else{
				clif_displaymessage(fd,msg_table[54]);
			}
			return 1;
		}
//September 18, 2003
//@charoption <param1> <param2> <param3> <charname>
//Description: Does the same as the @option command only to target character.
		if (strcmpi(command, "@charoption") == 0 && gm_level >= atcommand_config.charoption) {
			sscanf(message, "%s%d%d%d %[^\n]", command, &x, &y, &z, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
				/*
			   	p->option �͎��̂悤��bit�ɂ����Đ��藧���Ă���H
			   	pram1        0000 0000 0000 0000
			   	pram2        0000 0000 0000 0000
			   	pram3        0000 0000 0000 0000 

			 	*/
					pl_sd->opt1=x;
					pl_sd->opt2=y;
					pl_sd->status.option=z;
					clif_changeoption(&pl_sd->bl);
					clif_displaymessage(fd,msg_table[55]);
				}
			}else{
				clif_displaymessage(fd,msg_table[56]);
			}
			return 1;
		}
//September 20, 2003
//@charsave <map> <x> <y> <charname>
//Description: Changes the target players respawn point.
		if (strcmpi(command, "@charsave") == 0 && gm_level >= atcommand_config.charsave) {
			sscanf(message, "%s%s%d%d %[^\n]", command, temp0, &x, &y, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					pc_setsavepoint(pl_sd,temp0,x,y);
					clif_displaymessage(fd,msg_table[57]);
				}
			}else{
				clif_displaymessage(fd,msg_table[58]);
			}
			return 1;
		}
//September 20, 2003
//@night
//Description: Uses @option 00 16 00 on all characters.
		if(strcmpi(command, "@night") == 0 && gm_level >= atcommand_config.night){
			sscanf(message, "%s", command);
			for(i=0;i<fd_max;i++){
				if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
					pl_sd->opt2 |= 0x10;
					clif_changeoption(&pl_sd->bl);
					clif_displaymessage(pl_sd->fd,msg_table[59]);
				}
			}
			return 1;
		}
//September 20, 2003
//@day
//Description: Uses @option 00 00 00 on all characters.
			if(strcmpi(command, "@day") == 0 && gm_level >= atcommand_config.day){
				sscanf(message, "%s", command);
			for(i=0;i<fd_max;i++){
				if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
					pl_sd->opt2 &= !0x10;
					clif_changeoption(&pl_sd->bl);
					clif_displaymessage(pl_sd->fd,msg_table[60]);
				}
			}
			return 1;
		}
//September 20, 2003
//Edited September 24, 2003
//@doom
//Description: Kills all NON GM chars on the server.
	if(strcmpi(command, "@doom") == 0 && gm_level >= atcommand_config.doom){
		sscanf(message, "%s", command);
		for(i=0;i<fd_max;i++){
			if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
				if(gm_level > pc_isGM(pl_sd)) {
					pc_damage(NULL,pl_sd,pl_sd->status.hp+1);
					clif_displaymessage(pl_sd->fd,msg_table[61]);
				}
			}
		}
		clif_displaymessage(fd,msg_table[62]);
		return 1;
	}
//October 6, 2003
//@doommap
//Description: Kills all non GM characters on the map.
	if(strcmpi(command, "@doommap") == 0 && gm_level >= atcommand_config.doommap){
		sscanf(message, "%s", command);				
		for(i=0;i<fd_max;i++){
			if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
				if(sd->bl.m == pl_sd->bl.m){
					if(gm_level > pc_isGM(pl_sd)) {
						pc_damage(NULL,pl_sd,pl_sd->status.hp+1);
						clif_displaymessage(pl_sd->fd,msg_table[61]);
					}
				}
			}
		}
		clif_displaymessage(fd,msg_table[62]);
		return 1;
	}
//September 21, 2003
//Edited September 24, 2003
//@raise
//Description: Resurrects all characters on the server.
	if(strcmpi(command, "@raise") == 0 && gm_level >= atcommand_config.raise){
		sscanf(message, "%s", command);
		for(i=0;i<fd_max;i++){
			if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth
				 && pc_isdead(pl_sd)){
				pl_sd->status.hp=pl_sd->status.max_hp;
				pc_setstand(pl_sd);
				clif_updatestatus(pl_sd,SP_HP);
				clif_resurrection(&pl_sd->bl,1);
				clif_displaymessage(pl_sd->fd,msg_table[63]);
			}
		}
		clif_displaymessage(fd,msg_table[64]);
		return 1;
	}
//October 6, 2003
//@raisemap
//Description: Resurrects all characters on the map.
	if(strcmpi(command, "@raisemap") == 0 && gm_level >= atcommand_config.raisemap){
		sscanf(message, "%s", command);
		for(i=0;i<fd_max;i++){
			if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
				if(sd->bl.m == pl_sd->bl.m && pc_isdead(pl_sd)){
					pl_sd->status.hp=pl_sd->status.max_hp;
					pc_setstand(pl_sd);
					clif_updatestatus(pl_sd,SP_HP);
					clif_resurrection(&pl_sd->bl,1);
					clif_displaymessage(pl_sd->fd,msg_table[63]);
				}
			}
		}
		clif_displaymessage(fd,msg_table[64]);
		return 1;
	}
//@charbaselvl <#> <nickname>
//Description: Change a characters base level.
	if (strcmpi(command, "@charbaselvl") == 0 && gm_level >= atcommand_config.charbaselvl) {
		sscanf(message, "%s%d %[^\n]", command, &x, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					if(x >= 1){
						for(i=pl_sd->status.base_level+1;i<=pl_sd->status.base_level+x;i++){
							if(i>=99)
								pl_sd->status.status_point += 22;
							else
								pl_sd->status.status_point += (i+14) / 5 ;
						}
						pl_sd->status.base_level+=x;
						clif_updatestatus(pl_sd,SP_BASELEVEL);
						clif_updatestatus(pl_sd,SP_NEXTBASEEXP);
						clif_updatestatus(pl_sd,SP_STATUSPOINT);
						pc_calcstatus(pl_sd,0);
						pc_heal(pl_sd,pl_sd->status.max_hp,pl_sd->status.max_sp);
						clif_misceffect(&pl_sd->bl,0);
						clif_displaymessage(fd,msg_table[65]);
					}else if(x<0 && pl_sd->status.base_level+x>0){
						pl_sd->status.base_level+=x;
						clif_updatestatus(pl_sd,SP_BASELEVEL);
						clif_updatestatus(pl_sd,SP_NEXTBASEEXP);
						pc_calcstatus(pl_sd,0);
						clif_displaymessage(fd,msg_table[66]);
					}
				}
			}
			return 1;
		}
//@charjlvl
		y = 50;
	if(strcmpi(command, "@charjlvl") == 0 && gm_level >= atcommand_config.charjlvl){
		sscanf(message, "%s%d %[^\n]", command, &x, temp1);
			if ((pl_sd=map_nick2sd(temp1))!=NULL) {
				if(gm_level > pc_isGM(pl_sd)) {
					if(pl_sd->status.class == 0)
						y -= 40;
					if(pl_sd->status.job_level == y){
						clif_displaymessage(fd,msg_table[67]);
					}else if(x >= 1){
						if(pl_sd->status.job_level + x > y)
							x = y - pl_sd->status.job_level;
						pl_sd->status.job_level+=x;
						clif_updatestatus(pl_sd,SP_JOBLEVEL);
						clif_updatestatus(pl_sd,SP_NEXTJOBEXP);
						pl_sd->status.skill_point+=x;
						clif_updatestatus(pl_sd,SP_SKILLPOINT);
						pc_calcstatus(pl_sd,0);
						clif_misceffect(&pl_sd->bl,1);
						clif_displaymessage(fd,msg_table[68]);
					}else if(x<0 && sd->status.job_level+x>0 ){
						pl_sd->status.job_level+=x;
						clif_updatestatus(pl_sd,SP_JOBLEVEL);
						clif_updatestatus(pl_sd,SP_NEXTJOBEXP);
						pc_calcstatus(pl_sd,0);
						clif_displaymessage(fd,msg_table[69]);
					}
				}
			}
			return 1;
		}
//-End- Commands added by Syrus22

		if(strcmpi(command, "@kick") == 0 && gm_level >= atcommand_config.kick){
			sscanf(message, "%s %[^\n]", command, temp1);
			if((pl_sd=map_nick2sd(temp1))!=NULL && gm_level > pc_isGM(pl_sd))
				clif_GM_kick(sd,pl_sd,1);
			else
				clif_GM_kickack(sd,0);
			return 1;
		}

		if(strcmpi(command, "@kickall") == 0 && gm_level >= atcommand_config.kickall){
			for(i=0;i<fd_max;i++){
				if(session[i] && (pl_sd=session[i]->session_data) && pl_sd->state.auth){
					if(sd->status.account_id != pl_sd->status.account_id)
						clif_GM_kick(sd,pl_sd,0);
				}
			}
			clif_GM_kick(sd,sd,0);
			return 1;
		}

		if(strcmpi(command, "@allskill") == 0 && gm_level >= atcommand_config.allskill){
			pc_allskillup(sd);
			return 1;
		}

		if(strcmpi(command, "@questskill") == 0 && gm_level >= atcommand_config.questskill){
			sscanf(message, "%s %d", command, &x);
			if(skill_get_inf2(x)&0x01){
				pc_skill(sd,x,1,0);
				clif_displaymessage(fd,msg_table[70]);
			}
			return 1;
		}

		if(strcmpi(command, "@lostskill") == 0 && gm_level >= atcommand_config.lostskill){
			sscanf(message, "%s %d", command, &x);
			if(x>0 && x<MAX_SKILL && pc_checkskill(sd,x)>0){
				sd->status.skill[x].lv=0;
				sd->status.skill[x].flag=0;
				clif_skillinfoblock(sd);
				clif_displaymessage(fd,msg_table[71]);
			}
			return 1;
		}
		if(strcmpi(command, "@spiritball") == 0 && gm_level >= atcommand_config.spiritball){
			sscanf(message, "%s %d", command, &x);
			if(x>=0 && x<=0x7FFF){
				if(sd->spiritball > 0)
					pc_delspiritball(sd,sd->spiritball,1);
				sd->spiritball = x;
				clif_spiritball(sd);
			}
			return 1;
		}

		if(strcmpi(command, "@party") == 0 && gm_level >= atcommand_config.party){
			if(sscanf(message, "%s %[^\n]", command, moji) >= 2) {
				party_create(sd,moji);
			}
			return 1;
		}

		if(strcmpi(command, "@guild") == 0 && gm_level >= atcommand_config.guild){
			if(sscanf(message, "%s %[^\n]", command, moji) >= 2) {
				int temp = battle_config.guild_emperium_check;
				battle_config.guild_emperium_check = 0;
				guild_create(sd,moji);
				battle_config.guild_emperium_check = temp;
			}
			return 1;
		}
		if(strcmpi(command, "@agitstart") == 0 && gm_level >= atcommand_config.agitstart){
			if(sscanf(message, "%s", command)) {
				if(agit_flag==1){
					clif_displaymessage(fd,msg_table[73]);
					return 1;
				}
				agit_flag=1;
				guild_agit_start();
				clif_displaymessage(fd,msg_table[72]);
			}
			return 1;
		}
		if(strcmpi(command, "@agitend") == 0 && gm_level >= atcommand_config.agitend){
			if(sscanf(message, "%s", command)) {
				if(agit_flag==0){
					clif_displaymessage(fd,msg_table[75]);
					return 1;
				}
				agit_flag=0;
				guild_agit_end();
				clif_displaymessage(fd,msg_table[74]);
			}
			return 1;
		}
	}

	return 0;
}

/* Read Message Data */
int msg_config_read(const char *cfgName)
{
	int i,msg_number;
	char line[1024],msg[1024];
	FILE *fp;

	fp=fopen(cfgName,"r");
	if(fp==NULL){
		printf("file not found: %s\n",cfgName);
		return 1;
	}
	while(fgets(line,1020,fp)){
		if(line[0] == '/' && line[1] == '/')
			continue;
		i=sscanf(line,"%d: %[^\r\n]",&msg_number,msg);
		if(i!=2)
			continue;
		if(msg_number>=0&&msg_number<=200)
			strcpy(msg_table[msg_number],msg);
		//printf("%d:%s\n",msg_number,msg);
	}	
	fclose(fp);
	return 0;
}

int atcommand_config_read(const char *cfgName)
{
	int i;
	char line[1024],w1[1024],w2[1024];
	FILE *fp;

	memset(&atcommand_config,0,sizeof(atcommand_config));
	atcommand_config.kickall = 99;

	if(battle_config.atc_gmonly > 0) {
		fp=fopen(cfgName,"r");
		if(fp==NULL){
			printf("file not found: %s\n",cfgName);
			return 1;
		}
		while(fgets(line,1020,fp)){
			const struct {
				char str[32];
				int *val;
			} data[] ={
				{ "broadcast",&atcommand_config.broadcast },
				{ "local_broadcast",&atcommand_config.local_broadcast },
				{ "mapmove",&atcommand_config.mapmove },
				{ "resetstate",&atcommand_config.resetstate },
				{ "rura+",&atcommand_config.rurap },
				{ "rura",&atcommand_config.rura },
				{ "where",&atcommand_config.where },
				{ "jumpto",&atcommand_config.jumpto },
				{ "jump",&atcommand_config.jump },
				{ "who",&atcommand_config.who },
				{ "save",&atcommand_config.save },
				{ "load",&atcommand_config.load },
				{ "speed",&atcommand_config.speed },
				{ "storage",&atcommand_config.storage },
				{ "option",&atcommand_config.option },
				{ "hide",&atcommand_config.hide },
				{ "jobchange",&atcommand_config.jobchange },
				{ "die",&atcommand_config.die },
				{ "kill",&atcommand_config.kill },
				{ "alive",&atcommand_config.alive },
				{ "kami",&atcommand_config.kami },
				{ "heal",&atcommand_config.heal },
				{ "item",&atcommand_config.item },
				{ "itemcheck",&atcommand_config.itemcheck },
				{ "itemreset",&atcommand_config.itemreset },
				{ "lvup",&atcommand_config.lvup },
				{ "joblvup",&atcommand_config.joblvup },
				{ "help",&atcommand_config.help },
				{ "GM",&atcommand_config.gm },
				{ "pvpoff",&atcommand_config.pvpoff },
				{ "pvpon",&atcommand_config.pvpon },
				{ "gvgoff",&atcommand_config.gvgoff },
				{ "gvgon",&atcommand_config.gvgon },
				{ "model",&atcommand_config.model },
				{ "go",&atcommand_config.go },
				{ "monster",&atcommand_config.monster },
				{ "refine",&atcommand_config.refine },
				{ "produce",&atcommand_config.produce },
				{ "memo",&atcommand_config.memo },
				{ "gat",&atcommand_config.gat },
				{ "packet",&atcommand_config.packet },
				{ "stpoint",&atcommand_config.stpoint },
				{ "skpoint",&atcommand_config.skpoint },
				{ "zeny",&atcommand_config.zeny },
				{ "param",&atcommand_config.param },
				{ "guildlvup",&atcommand_config.guildlvup },
				{ "makepet",&atcommand_config.makepet },
				{ "petfriendly",&atcommand_config.petfriendly },
				{ "pethungry",&atcommand_config.pethungry },
				{ "petrename",&atcommand_config.petrename },
				{ "recall",&atcommand_config.recall },
				{ "charjob",&atcommand_config.charjob },
				{ "revive",&atcommand_config.revive },
				{ "charstats",&atcommand_config.charstats },
				{ "charoption",&atcommand_config.charoption },
				{ "charsave",&atcommand_config.charsave },
				{ "charload",&atcommand_config.charload },
				{ "night",&atcommand_config.night },
				{ "day",&atcommand_config.day },
				{ "doom",&atcommand_config.doom },
				{ "doommap",&atcommand_config.doommap },
				{ "raise",&atcommand_config.raise },
				{ "raisemap",&atcommand_config.raisemap },
				{ "charbaselvl",&atcommand_config.charbaselvl },
				{ "charjlvl",&atcommand_config.charjlvl },
				{ "kick",&atcommand_config.kick },
				{ "kickall",&atcommand_config.kickall },
				{ "allskill",&atcommand_config.allskill },
				{ "questskill",&atcommand_config.questskill },
				{ "lostskill",&atcommand_config.lostskill },
				{ "spiritball",&atcommand_config.spiritball },
				{ "party",&atcommand_config.party },
				{ "guild",&atcommand_config.guild },
				{ "agitstart",&atcommand_config.agitstart },
				{ "agitend",&atcommand_config.agitend },
			};
		
			if(line[0] == '/' && line[1] == '/')
				continue;
			i=sscanf(line,"%[^:]:%s",w1,w2);
			if(i!=2)
				continue;
			for(i=0;i<sizeof(data)/(sizeof(data[0]));i++)
				if(strcmpi(w1,data[i].str)==0)
					*data[i].val=atoi(w2);
		}
		fclose(fp);
	}

	return 0;
}
