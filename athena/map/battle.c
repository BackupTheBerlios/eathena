#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "battle.h"

#include "timer.h"

#include "map.h"
#include "pc.h"
#include "skill.h"
#include "mob.h"
#include "itemdb.h"
#include "clif.h"


int attr_fix_table[4][10][10];

struct Battle_Config battle_config;

// �e��p�����[�^����

int battle_get_class(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return ((struct mob_data *)bl)->class;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->status.class;
	else if(bl->type==BL_PET)
		return ((struct pet_data *)bl)->class;
	else
		return 0;
}
int battle_get_lv(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].lv;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->status.base_level;
	else if(bl->type==BL_PET)
		return ((struct pet_data *)bl)->msd->pet.level;
	else
		return 0;
}
int battle_get_hp(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return ((struct mob_data *)bl)->hp;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->status.hp;
	else
		return 1;
}
int battle_get_max_hp(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].max_hp;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->status.max_hp;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].max_hp;
	else
		return 1;
}
int battle_get_str(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].str;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->paramc[0];
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].str;
	else
		return 0;
}
int battle_get_agi(struct block_list *bl)
{
	int agi=0;
	struct status_change *sc_data=battle_get_sc_data(bl);
	
	if(bl->type==BL_MOB){
		agi=mob_db[((struct mob_data *)bl)->class].agi;

		if( sc_data[SC_INCREASEAGI].timer!=-1)	// ���x����(PC��pc.c��)
			agi += 2+sc_data[SC_INCREASEAGI].val1;

	}
	else if(bl->type==BL_PC)
		agi=((struct map_session_data *)bl)->paramc[1];
	else if(bl->type==BL_PET)
		agi=mob_db[((struct pet_data *)bl)->class].agi;
	else
		return 0;

	if(sc_data) {
		if(sc_data[SC_DECREASEAGI].timer!=-1)	// ���x����
			agi -= 2+sc_data[SC_DECREASEAGI].val1;

		if(sc_data[SC_QUAGMIRE].timer!=-1 )	// �N�@�O�}�C�A
			agi/=2;
	}

	return agi;
}
int battle_get_vit(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].vit;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->paramc[2];
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].vit;
	else
		return 0;
}
int battle_get_int(struct block_list *bl)
{
	if(bl->type==BL_MOB){
		struct status_change *sc_data=battle_get_sc_data(bl);
		int int_=mob_db[((struct mob_data *)bl)->class].int_;

		if( sc_data[SC_BLESSING].timer!=-1){	// �u���b�V���O
			int race=battle_get_race(bl);
			if( race==1 || race==6 )	int_/=2;	// �� ��/�s��
			else int_ += sc_data[SC_BLESSING].val1;	// ���̑�
		}
		return int_;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->paramc[3];
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].int_;
	else
		return 0;
}
int battle_get_dex(struct block_list *bl)
{
	int dex=0;
	struct status_change *sc_data=battle_get_sc_data(bl);

	if(bl->type==BL_MOB){
		dex= mob_db[((struct mob_data *)bl)->class].dex;

		if(sc_data[SC_BLESSING].timer!=-1){	// �u���b�V���O
			int race=battle_get_race(bl);
			if( race==1 || race==6 )	dex/=2;		// �� ��/�s��
			else dex += sc_data[SC_BLESSING].val1;	// ���̑�
		}
	}
	else if(bl->type==BL_PC)
		dex= ((struct map_session_data *)bl)->paramc[4];
	else if(bl->type==BL_PET)
		dex = mob_db[((struct pet_data *)bl)->class].dex;
	else
		return 0;

	if(sc_data && sc_data[SC_QUAGMIRE].timer!=-1 )	// �N�@�O�}�C�A
		dex/=2;

	return dex;
}
int battle_get_luk(struct block_list *bl)
{
	int luk=0;
	struct status_change *sc_data=battle_get_sc_data(bl);
	if(bl->type==BL_MOB){
		luk=mob_db[((struct mob_data*)bl)->class].luk;
		// MOB�̂ݒǉ��v�Z�iPC��pc_calcstatus�Ōv�Z����j
		if(sc_data[SC_GLORIA].timer!=-1 )	// �O�����A(PC��pc.c��)
			luk+=30;
	}
	else if(bl->type==BL_PC)
		luk=((struct map_session_data*)bl)->paramc[5];
	else if(bl->type==BL_PET)
		luk=mob_db[((struct pet_data*)bl)->class].luk;
	else
		return 0;

	if(sc_data && sc_data[SC_CURSE].timer!=-1 )		// ��
		luk=0;

	return luk;
}

int battle_get_flee(struct block_list *bl)
{
	int flee;
	if(bl->type==BL_MOB){
		flee=battle_get_agi(bl) + battle_get_lv(bl);
		// MOB�̂ݒǉ��v�Z�iPC��pc_calcstatus�Ōv�Z����j
		if( battle_get_sc_data(bl)[SC_BLIND].timer!=-1 )	// �Í��ɂ��ቺ
			flee -= flee*25/100;
	}
	else if(bl->type==BL_PC)
		flee=((struct map_session_data *)bl)->flee;
	else if(bl->type==BL_PET)
		flee=battle_get_agi(bl) + battle_get_lv(bl);
	else
		return 0;
	
	return flee;
}
int battle_get_hit(struct block_list *bl)
{
	int hit;
	if(bl->type==BL_MOB){
		hit=battle_get_dex(bl) + battle_get_lv(bl);
		// MOB�̂ݒǉ��v�Z�iPC��pc_calcstatus�Ōv�Z����j
		if( battle_get_sc_data(bl)[SC_BLIND].timer!=-1 )	// �Í��ɂ��ቺ
			hit -= hit*25/100;
	}
	else if(bl->type==BL_PC)
		hit=((struct map_session_data *)bl)->hit;
	else if(bl->type==BL_PET)
		hit=battle_get_dex(bl) + battle_get_lv(bl);
	else
		return 0;

	return hit;
}
int battle_get_flee2(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return battle_get_luk(bl)/10+1;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->flee2;
	else if(bl->type==BL_PET)
		return battle_get_luk(bl)/10+1;
	else
		return 0;
	
}
int battle_get_critical(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return battle_get_luk(bl)*3+1;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->critical*10;
	else if(bl->type==BL_PET)
		return battle_get_luk(bl)*3+1;
	else
		return 0;
}

int battle_get_atk(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data*)bl)->class].atk1;
	else if(bl->type==BL_PC){
		int atk=((struct map_session_data*)bl)->watk;

		// PC�̂ݒǉ��v�Z�iatk1,atk2�̒�`��mob�ƈقȂ�Bmob��atk2�Ōv�Z�j
		if( battle_get_sc_data(bl)[SC_CURSE].timer!=-1 )		// ��
			atk-= atk*25/100;
		return atk;
	}
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data*)bl)->class].atk1;
	else
		return 0;
}
int battle_get_atk_(struct block_list *bl)
{
	if(bl->type==BL_PC){
		int atk=((struct map_session_data*)bl)->watk_;

		// PC�̂ݒǉ��v�Z�iatk1,atk2�̒�`��mob�ƈقȂ�Bmob��atk2�Ōv�Z�j
		if( battle_get_sc_data(bl)[SC_CURSE].timer!=-1 )		// ��
			atk-= atk*25/100;
		return atk;
	}else
		return 0;
}
int battle_get_atk2(struct block_list *bl)
{
	if(bl->type==BL_MOB){
		struct status_change *sc_data=battle_get_sc_data(bl);
		int atk2=mob_db[((struct mob_data*)bl)->class].atk2;
		
		// MOB�̂ݒǉ��v�Z�iPC��pc_calcstatus�Ōv�Z����j
		if( sc_data[SC_IMPOSITIO].timer!=-1)	// �C���|�V�e�B�I�}�k�X
			atk2 += sc_data[SC_IMPOSITIO].val1*5;
		if( sc_data[SC_PROVOKE].timer!=-1 )		// �v���{�b�N
			atk2 = atk2*(100+2*sc_data[SC_PROVOKE].val1)/100;
		if( sc_data[SC_CURSE].timer!=-1 )		// ��
			atk2 -= atk2*25/100;
		return atk2;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data*)bl)->watk2;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data*)bl)->class].atk2;
	else
		return 0;
}
int battle_get_atk_2(struct block_list *bl)
{
	if(bl->type==BL_PC)
		return ((struct map_session_data*)bl)->watk_2;
	else
		return 0;
}
int battle_get_matk1(struct block_list *bl)
{
	if(bl->type==BL_MOB){
		int matk,int_=battle_get_int(bl);
		matk = int_+(int_/5)*(int_/5);
		return matk;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->matk1;
	else if(bl->type==BL_PET){
		int matk,int_=battle_get_int(bl);
		matk = int_+(int_/5)*(int_/5);
		return matk;
	}
	else
		return 0;
}
int battle_get_matk2(struct block_list *bl)
{
	if(bl->type==BL_MOB){
		int matk,int_=battle_get_int(bl);
		matk = int_+(int_/7)*(int_/7);
		return matk;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->matk2;
	else if(bl->type==BL_PET){
		int matk,int_=battle_get_int(bl);
		matk = int_+(int_/7)*(int_/7);
		return matk;
	}
	else
		return 0;
}
int battle_get_def(struct block_list *bl)
{
	if(bl->type==BL_MOB) {
		struct status_change *sc_data=battle_get_sc_data(bl);
		int def= mob_db[((struct mob_data *)bl)->class].def;

		if( sc_data[SC_PROVOKE].timer!=-1 )		// �v���{�b�N
			def= (def*(100-6*sc_data[SC_PROVOKE].val1)+50)/100;

		return def;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->def;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].def;
	else
		return 0;
}
int battle_get_mdef(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].mdef;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->mdef;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].mdef;
	else
		return 0;
}
int battle_get_def2(struct block_list *bl)
{
	if(bl->type==BL_MOB){
		struct status_change *sc_data=battle_get_sc_data(bl);
		int def2= mob_db[((struct mob_data *)bl)->class].vit;

		// MOB�̂ݒǉ��v�Z�iPC��pc_calcstatus�Ōv�Z����j
		if( sc_data[SC_ANGELUS].timer!=-1)	// �G���W�F���X
			def2 = def2*(110+5*sc_data[SC_ANGELUS].val1)/100;

		if( sc_data[SC_PROVOKE].timer!=-1 )		// �v���{�b�N
			def2= def2*(100-6*sc_data[SC_PROVOKE].val1)/100;

		return def2;
	}
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->def2;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].vit;
	else
		return 0;
}
int battle_get_mdef2(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].int_ + (mob_db[((struct mob_data *)bl)->class].vit>>1);
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->mdef2 + (((struct map_session_data *)bl)->paramc[2]>>1);
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].int_ + (mob_db[((struct pet_data *)bl)->class].vit>>1);
	else
		return 0;
}
int battle_get_amotion(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].amotion;
	else if(bl->type==BL_PC)
		return ((struct map_session_data *)bl)->amotion;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].amotion;
	else
		return 200;
}
int battle_get_dmotion(struct block_list *bl)
{
	int ret;
	struct status_change *sc_data=battle_get_sc_data(bl);
	if(bl->type==BL_MOB) {
		ret=mob_db[((struct mob_data *)bl)->class].dmotion;
		if(battle_config.monster_damage_delay_rate != 100)
			ret = ret*battle_config.monster_damage_delay_rate/100;
	}
	else if(bl->type==BL_PC) {
		ret=((struct map_session_data *)bl)->dmotion;
		if(battle_config.pc_damage_delay_rate != 100)
			ret = ret*battle_config.pc_damage_delay_rate/100;
	}
	else if(bl->type==BL_PET)
		ret=mob_db[((struct pet_data *)bl)->class].dmotion;
	else
		return 200;

	if(sc_data && sc_data[SC_ENDURE].timer!=-1 )
		ret=0;

	return ret;
}
int battle_get_element(struct block_list *bl)
{
	int ret;
	struct status_change *sc_data=battle_get_sc_data(bl);

	if(bl->type==BL_MOB)	// 10�̈ʁ�Lv*2�A�P�̈ʁ�����
		ret=((struct mob_data *)bl)->def_ele;
	else if(bl->type==BL_PC)
		ret=20+((struct map_session_data *)bl)->def_ele;	// �h�䑮��Lv1
	else if(bl->type==BL_PET)
		ret = mob_db[((struct pet_data *)bl)->class].element;
	else
		return 20;

	if(sc_data) {
		if( sc_data[SC_BENEDICTIO].timer!=-1 )	// ���̍~��
			ret=26;
		if( sc_data[SC_FREEZE].timer!=-1 )	// ����
			ret=21;
	}

	return ret;
}
int battle_get_attack_element(struct block_list *bl)
{
	int ret;
	struct status_change *sc_data=battle_get_sc_data(bl);

	if(bl->type==BL_MOB)
		ret=0;
	else if(bl->type==BL_PC)
		ret=((struct map_session_data *)bl)->atk_ele;
	else if(bl->type==BL_PET)
		ret=0;
	else
		return 0;

	if(sc_data) {
		if( sc_data[SC_FROSTWEAPON].timer!=-1)	// �t���X�g�E�F�|��
			ret=1;
		if( sc_data[SC_SEISMICWEAPON].timer!=-1)	// �T�C�Y�~�b�N�E�F�|��
			ret=2;
		if( sc_data[SC_FLAMELAUNCHER].timer!=-1)	// �t���[�������`���[
			ret=3;
		if( sc_data[SC_LIGHTNINGLOADER].timer!=-1)	// ���C�g�j���O���[�_�[
			ret=4;
		if( sc_data[SC_ENCPOISON].timer!=-1)	// �G���`�����g�|�C�Y��
			ret=5;
		if( sc_data[SC_ASPERSIO].timer!=-1)		// �A�X�y���V�I
			ret=6;
	}

	return ret;
}
int battle_get_attack_element2(struct block_list *bl)
{
	int ret;
	struct status_change *sc_data=battle_get_sc_data(bl);

	if(bl->type==BL_MOB)
		ret=0;
	else if(bl->type==BL_PC)
		ret=((struct map_session_data *)bl)->atk_ele_;
	else if(bl->type==BL_PET)
		ret=0;
	else
		return 0;

	if(sc_data) {
		if( sc_data[SC_FROSTWEAPON].timer!=-1)	// �t���X�g�E�F�|��
			ret=1;
		if( sc_data[SC_SEISMICWEAPON].timer!=-1)	// �T�C�Y�~�b�N�E�F�|��
			ret=2;
		if( sc_data[SC_FLAMELAUNCHER].timer!=-1)	// �t���[�������`���[
			ret=3;
		if( sc_data[SC_LIGHTNINGLOADER].timer!=-1)	// ���C�g�j���O���[�_�[
			ret=4;
		if( sc_data[SC_ENCPOISON].timer!=-1)	// �G���`�����g�|�C�Y��
			ret=5;
		if( sc_data[SC_ASPERSIO].timer!=-1)		// �A�X�y���V�I
			ret=6;
	}

	return ret;
}
int battle_get_party_id(struct block_list *bl)
{
	if( bl->type == BL_PC )
		return ((struct map_session_data *)bl)->status.party_id;
	else if( bl->type==BL_MOB ){
		struct mob_data *md=(struct mob_data *)bl;
		if( md->master_id>0 )
			return md->master_id;
		return md->bl.id;
	}
	else if( bl->type==BL_SKILL )
		return ((struct skill_unit *)bl)->group->party_id;
	else
		return -1;
}
int battle_get_guild_id(struct block_list *bl)
{
	if( bl->type == BL_PC )
		return ((struct map_session_data *)bl)->status.guild_id;
	else if( bl->type==BL_MOB )
		return ((struct mob_data *)bl)->class;
	else if( bl->type==BL_SKILL )
		return ((struct skill_unit *)bl)->group->guild_id;
	else
		return -1;
}
int battle_get_race(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].race;
	else if(bl->type==BL_PC)
		return 7;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].race;
	else
		return 0;
}
int battle_get_size(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].size;
	else if(bl->type==BL_PC)
		return 1;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].size;
	else
		return 0;
}
int battle_get_mode(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].mode;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].mode;
	else
		return 0x01;	// �Ƃ肠���������Ƃ������Ƃ�1
}

int battle_get_mexp(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_db[((struct mob_data *)bl)->class].mexp;
	else if(bl->type==BL_PET)
		return mob_db[((struct pet_data *)bl)->class].mexp;
	else
		return 0;
}

// StatusChange�n�̏���
struct status_change *battle_get_sc_data(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return ((struct mob_data*)bl)->sc_data;
	else if(bl->type==BL_PC)
		return ((struct map_session_data*)bl)->sc_data;
	return NULL;
}
short *battle_get_sc_count(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return &((struct mob_data*)bl)->sc_count;
	else if(bl->type==BL_PC)
		return &((struct map_session_data*)bl)->sc_count;
	return NULL;
}
short *battle_get_opt1(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return &((struct mob_data*)bl)->opt1;
	else if(bl->type==BL_PC)
		return &((struct map_session_data*)bl)->opt1;
	return NULL;
}
short *battle_get_opt2(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return &((struct mob_data*)bl)->opt2;
	else if(bl->type==BL_PC)
		return &((struct map_session_data*)bl)->opt2;
	return NULL;
}
short *battle_get_option(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return &((struct mob_data*)bl)->option;
	else if(bl->type==BL_PC)
		return &((struct map_session_data*)bl)->status.option;
	return NULL;
}

//-------------------------------------------------------------------

// �_���[�W�̒x��
struct battle_delay_damage_ {
	struct block_list *src,*target;
	int damage;
};
int battle_delay_damage_sub(int tid,unsigned int tick,int id,int data)
{
	struct battle_delay_damage_ *dat=(struct battle_delay_damage_ *)data;
	if( map_id2bl(id)==dat->src && dat->target->prev!=NULL)
		battle_damage(dat->src,dat->target,dat->damage);
	free(dat);
	return 0;
}
int battle_delay_damage(unsigned int tick,struct block_list *src,struct block_list *target,int damage)
{
	struct battle_delay_damage_ *dat=malloc(sizeof(struct battle_delay_damage_));
	dat->src=src;
	dat->target=target;
	dat->damage=damage;
	add_timer(tick,battle_delay_damage_sub,src->id,(int)dat);
	return 0;
}

// ���ۂ�HP�𑀍�
int battle_damage(struct block_list *bl,struct block_list *target,int damage)
{
	struct map_session_data *sd=NULL;
	struct status_change *sc_data=battle_get_sc_data(target);
	short *sc_count;

	if(damage==0 || target->type == BL_PET)
		return 0;

	if(target->prev == NULL)
		return 0;

	if(bl) {
		if(bl->prev==NULL)
			return 0;

		if(bl->type==BL_PC)
			sd=(struct map_session_data *)bl;
	}
		
	if(damage<0)
		return battle_heal(bl,target,-damage,0);
	
	if( (sc_count=battle_get_sc_count(target))!=NULL && *sc_count>0){
		// �����A�Ή��A����������
		if(sc_data[SC_FREEZE].timer!=-1)
			skill_status_change_end(target,SC_FREEZE,-1);
		if(sc_data[SC_STONE].timer!=-1)
			skill_status_change_end(target,SC_STONE,-1);
		if(sc_data[SC_SLEEP].timer!=-1)
			skill_status_change_end(target,SC_SLEEP,-1);
		if(sc_data[SC_SPELLBREAKER].timer!=-1)		// AppleGirl
			skill_castcancel(target);
	}


	if(target->type==BL_MOB){	// MOB

		struct mob_data *md=(struct mob_data *)target;
		if(md->skilltimer!=-1 && md->state.skillcastcancel)	// �r���W�Q
			skill_castcancel(target);
		return mob_damage(bl,md,damage);

	}else if(target->type==BL_PC){	// PC

		struct map_session_data *tsd=(struct map_session_data *)target;
		if(tsd->skilltimer!=-1){	// �r���W�Q
				// �t�F���J�[�h��W�Q����Ȃ��X�L�����̌���
			if( (!tsd->special_state.no_castcancel || map[bl->m].flag.gvg) && tsd->state.skillcastcancel &&
				!tsd->special_state.no_castcancel2)
				skill_castcancel(target);
		}
		return pc_damage(bl,tsd,damage);

	}else if(target->type==BL_SKILL)
		return skill_unit_ondamaged((struct skill_unit *)target,bl,damage,gettick());
	return 0;
}
int battle_heal(struct block_list *bl,struct block_list *target,int hp,int sp)
{
	if(hp==0 || target->type == BL_PET)
		return 0;

	if(hp<0)
		return battle_damage(bl,target,-hp);

	if(target->type==BL_MOB)
		return mob_heal((struct mob_data *)target,hp);
	else if(target->type==BL_PC)
		return pc_heal((struct map_session_data *)target,hp,sp);
	return 0;
}

// �U����~
int battle_stopattack(struct block_list *bl)
{
	if(bl->type==BL_MOB)
		return mob_stopattack((struct mob_data*)bl);
	else if(bl->type==BL_PC)
		return pc_stopattack((struct map_session_data*)bl);
	return 0;
}
// �ړ���~
int battle_stopwalking(struct block_list *bl,int type)
{
	if(bl->type==BL_MOB)
		return mob_stop_walking((struct mob_data*)bl,type);
	else if(bl->type==BL_PC)
		return pc_stop_walking((struct map_session_data*)bl,type);
	return 0;
}


/*==========================================
 * �_���[�W�̑����C��
 *------------------------------------------
 */
int battle_attr_fix(int damage,int atk_elem,int def_elem)
{
	int def_type= def_elem%10, def_lv=def_elem/10/2;

	if(	atk_elem<0 || atk_elem>9 || def_type<0 || def_type>9 ||
		def_lv<1 || def_lv>4){	// �� ���l�����������̂łƂ肠�������̂܂ܕԂ�
		printf("battle_attr_fix: unknown attr type: atk=%d def_type=%d def_lv=%d\n",atk_elem,def_type,def_lv);
		return damage;
	}

	return damage*attr_fix_table[def_lv-1][atk_elem][def_type]/100;
}


/*==========================================
 * �_���[�W�ŏI�v�Z
 *------------------------------------------
 */
int battle_calc_damage(struct block_list *bl,int damage,int flag)
{
	struct map_session_data *sd=NULL;
	struct mob_data *md=NULL;
	struct status_change *sc_data,*sc;
	short *sc_count;
	int i;

	if(bl->type==BL_MOB) md=(struct mob_data *)bl;
	else				 sd=(struct map_session_data *)bl;
	
	sc_data=battle_get_sc_data(bl);
	sc_count=battle_get_sc_count(bl);

	if(sc_count!=NULL && *sc_count>0){
	
		if(sc_data[SC_SAFETYWALL].timer!=-1 && damage>0 &&
			flag&BF_WEAPON && flag&BF_SHORT ){
			// �Z�[�t�e�B�E�H�[��
			struct skill_unit *unit=(struct skill_unit*)sc_data[SC_SAFETYWALL].val2;
			if( unit->alive && (--unit->group->val2)<=0 )
				skill_delunit(unit);
			skill_unit_move(bl,gettick(),1);	// �d�ˊ|���`�F�b�N
			damage=0;
		}
		if(sc_data[SC_PNEUMA].timer!=-1 && damage>0 &&
			flag&BF_WEAPON && flag&BF_LONG ){
			// �j���[�}
			damage=0;
		}

		if(sc_data[SC_ROKISWEIL].timer!=-1 && damage>0 &&
			flag&BF_MAGIC ){
			// �j���[�}
			damage=0;
		}

		if(sc_data[SC_LANDPROTECTOR].timer!=-1 && damage>0 &&
			flag&BF_MAGIC ){
			// �j���[�}
			damage=0;
		}
		
		if(sc_data[SC_DEFENDER].timer!=-1 && damage>0 &&	// AppleGirl
			flag&BF_WEAPON && flag&BF_LONG ){
			// �j���[�}
			damage-=damage*sc_data[SC_DEFENDER].val2/100;
		}
		
		if(sc_data[SC_AETERNA].timer!=-1 && damage>0){	// ���b�N�X�G�[�e���i
			damage<<=1;
			skill_status_change_end( bl,SC_AETERNA,-1 );
		}

		if(sc_data[SC_ENERGYCOAT].timer!=-1 && damage>0){	// �G�i�W�[�R�[�g
			if(sd){
				if(sd->status.sp>0 && flag&BF_WEAPON){
					int per = sd->status.sp * 5 / (sd->status.max_sp + 1);
					sd->status.sp -= damage * (per * 5 + 10) / 1000;
					if( sd->status.sp <0 ) sd->status.sp=0;
					damage -= damage * (per * 6) / 100;
					clif_updatestatus(sd,SP_SP);
				}
				if(sd->status.sp<=0)
					skill_status_change_end( bl,SC_ENERGYCOAT,-1 );
			}
		}

		if(sc_data[SC_KYRIE].timer!=-1){	// �L���G�G���C�\��
			sc=&sc_data[SC_KYRIE];
			sc->val2-=damage;
			if(flag&BF_WEAPON){
				if(sc->val2>=0)	damage=0;
				else			damage=-sc->val2;
			}
			if((--sc->val1)<=0 || (sc->val2<=0))
				skill_status_change_end(bl, SC_KYRIE, -1);
		}
	}

	if(	damage>0 && sc_data!=NULL && (
		sc_data[i=SC_STONE].timer!=-1	||	// �Ή�
		sc_data[i=SC_FREEZE].timer!=-1	||	// ����
		sc_data[i=SC_SLEEP].timer!=-1	)){	// ����
		skill_status_change_end( bl, i, -1 );	// �_���[�W�󂯂��������
	}

	if( md!=NULL && md->hp>0 && damage > 0 )	// �����Ȃǂ�MOB�X�L������
		mobskill_event(md,flag);

	return damage;
}

/*==========================================
 * �C���_���[�W
 *------------------------------------------
 */
int battle_addmastery(struct map_session_data *sd,struct block_list *target,int dmg,int type)
{
	int damage,skill;
	int race=battle_get_race(target);
	int weapon;
	damage = 0;

	// �f�[�����x�C��(+3 �` +30) vs �s�� or ���� (���l�͊܂߂Ȃ��H)
	if((skill = pc_checkskill(sd,AL_DEMONBANE)) > 0 && (race==1 || race==6) ) 
		damage += (skill * 3);

	// �r�[�X�g�x�C��(+4 �` +40) vs ���� or ����
	if((skill = pc_checkskill(sd,HT_BEASTBANE)) > 0 && (race==2 || race==4) ) 
		damage += (skill * 4);

	if(type == 0)
		weapon = sd->weapontype1;
	else
		weapon = sd->weapontype2;
	switch(weapon)
	{
		case 0x01:	// �Z�� (Updated By AppleGirl)
		case 0x02:	// 1HS
		{
			// ���C��(+4 �` +40) �Ў茕 �Z���܂�
			if((skill = pc_checkskill(sd,SM_SWORD)) > 0) {
				damage += (skill * 4);
			}
			break;
		}
		case 0x03:	// 2HS
		{
			// ���茕�C��(+4 �` +40) ���茕
			if((skill = pc_checkskill(sd,SM_TWOHAND)) > 0) {
				damage += (skill * 4);
			}
			break;
		}
		case 0x04:	// 1HL
		{
			// ���C��(+4 �` +40,+5 �` +50) ��
			if((skill = pc_checkskill(sd,KN_SPEARMASTERY)) > 0) {
				if(!pc_isriding(sd))
					damage += (skill * 4);	// �y�R�ɏ���ĂȂ�
				else
					damage += (skill * 5);	// �y�R�ɏ���Ă�
			}
			break;
		}
		case 0x05:	// 2HL
		{
			// ���C��(+4 �` +40,+5 �` +50) ��
			if((skill = pc_checkskill(sd,KN_SPEARMASTERY)) > 0) {
				if(!pc_isriding(sd))
					damage += (skill * 4);	// �y�R�ɏ���ĂȂ�
				else
					damage += (skill * 5);	// �y�R�ɏ���Ă�
			}
			break;
		}
		case 0x06:	// �Ў蕀
		{
			if((skill = pc_checkskill(sd,AM_AXEMASTERY)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x07: // Axe by Tato
		{
			if((skill = pc_checkskill(sd,AM_AXEMASTERY)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x08:	// ���C�X
		{
			// ���C�X�C��(+3 �` +30) ���C�X
			if((skill = pc_checkskill(sd,PR_MACEMASTERY)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x09:	// �Ȃ�?
			break;
		case 0x0a:	// ��
			break;
		case 0x0b:	// �|
			break;
		case 0x00:	// �f��
		case 0x0c:	// Knuckles
		{
			// �S��(+3 �` +30) �f��,�i�b�N��
			if((skill = pc_checkskill(sd,MO_IRONHAND)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x0d:	// Musical Instrument
		{
			// �y��̗��K(+3 �` +30) �y��
			if((skill = pc_checkskill(sd,BA_MUSICALLESSON)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x0e:	// Dance Mastery
		{
			// Dance Lesson Skill Effect(+3 damage for every lvl = +30) ��
			if((skill = pc_checkskill(sd,DC_DANCINGLESSON)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x0f:	// Book
		{
			// Advance Book Skill Effect(+3 damage for every lvl = +30) {
			if((skill = pc_checkskill(sd,SA_ADVANCEDBOOK)) > 0) {
				damage += (skill * 3);
			}
			break;
		}
		case 0x10:	// Katars
		{
			// �J�^�[���C��(+3 �` +30) �J�^�[��
			if((skill = pc_checkskill(sd,AS_KATAR)) > 0) {
				//�\�j�b�N�u���[���͕ʏ����i1���ɕt��1/8�K��)
				damage += (skill * 3);
			}
			break;
		}
	}
	damage = dmg + damage;
	return (damage);
}

static struct Damage battle_calc_pet_weapon_attack(
	struct block_list *src,struct block_list *target,int skill_num,int skill_lv,int wflag)
{
	struct pet_data *pd = (struct pet_data *)src;
	struct mob_data *tmd=NULL;
	int hitrate,cri = 0,atkmin,atkmax;
	int str,luk;
	int def1 = battle_get_def(target);
	int def2 = battle_get_def2(target);
	struct Damage wd;
	int damage,type,div_,blewcount=0;
	int flag;
	int t_mode=0,t_race=0,t_size=1,s_race=0,s_ele=0;
	struct status_change *t_sc_data;

	s_race=battle_get_race(src);
	s_ele=battle_get_attack_element(src);

	// �^�[�Q�b�g
	if(target->type == BL_MOB)
		tmd=(struct mob_data *)target;
	else {
		memset(&wd,0,sizeof(wd));
		return wd;
	}
	t_race=battle_get_race( target );
	t_size=battle_get_size( target );
	t_mode=battle_get_mode( target );
	t_sc_data=battle_get_sc_data( target );

	flag=BF_SHORT|BF_WEAPON|BF_NORMAL;	// �U���̎�ނ̐ݒ�
	
	// ��𗦌v�Z�A��𔻒�͌��
	hitrate=battle_get_hit(src) - battle_get_flee(target) + 80;

	type=0;	// normal
	div_ = 1; // single attack

	str=battle_get_str(src);
	luk=battle_get_luk(src);

	damage = (str/10)*(str/10)+str;
	atkmin = battle_get_atk(src);
	atkmax = battle_get_atk2(src);
	if( mob_db[pd->class].range>2 )
		flag=(flag&~BF_RANGEMASK)|BF_LONG;

	if(atkmin > atkmax) atkmin = atkmax;

	cri = 1 + luk*3;
	cri -= battle_get_luk(target) * 3;
	if(battle_config.enemy_critical_rate != 100) {
		cri = cri*battle_config.enemy_critical_rate/100;
		if(cri < 1)
			cri = 1;
	}
	if(t_sc_data != NULL && t_sc_data[SC_SLEEP].timer!=-1 )
		cri <<=1;

	if(skill_num==0 && battle_config.enemy_critical && (rand() % 1000) < cri)
	{
		damage += atkmax;
		type = 0x0a;
	}
	else {
		int vitbonusmax, t_vit=0;
	
		if(atkmax > atkmin)
			damage += atkmin + rand() % (atkmax-atkmin + 1);
		else
			damage += atkmin ;
		// �X�L���C���P�i�U���͔{���n�j
		// �I�[�o�[�g���X�g(+5% �` +25%),���U���n�X�L���̏ꍇ�����ŕ␳
		// �o�b�V��,�}�O�i���u���C�N,
		// �{�[�����O�o�b�V��,�X�s�A�u�[������,�u�����f�B�b�V���X�s�A,�X�s�A�X�^�b�u,
		// ���}�[�i�C�g,�J�[�g���{�����[�V����
		// �_�u���X�g���C�t�B���O,�A���[�V�����[,�`���[�W�A���[,
		// �\�j�b�N�u���[
		if(skill_num>0){
			int i;
			if( (i=skill_get_pl(skill_num))>0 )
				s_ele=i;

			flag=(flag&~BF_SKILLMASK)|BF_SKILL;
			switch( skill_num ){
			case SM_BASH:		// �o�b�V��
				damage = damage*(100+ 30*skill_lv)/100;
				hitrate = (hitrate*(100+5*skill_lv))/100;
				break;
			case SM_MAGNUM:		// �}�O�i���u���C�N
				damage = damage*(5*skill_lv +(wflag)?65:115 )/100;
				blewcount=2;
				break;
			case MC_MAMMONITE:	// ���}�[�i�C�g
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case AC_DOUBLE:	// �_�u���X�g���C�t�B���O
				damage = damage*(180+ 20*skill_lv)/100;
				div_=2;
				break;
			case AC_SHOWER:	// �A���[�V�����[
				damage = damage*(75 + 5*skill_lv)/100;
				blewcount=2;
				break;
			case KN_PIERCE:	// �s�A�[�X
				damage = damage*(100+ 10*skill_lv)/100;
				hitrate = hitrate*(100+5*skill_lv)/100;
				div_=t_size+1;
				damage*=div_;
				break;
			case KN_SPEARSTAB:	// �X�s�A�X�^�u
				damage = damage*(100+ 15*skill_lv)/100;
				blewcount=6;
				break;
			case KN_SPEARBOOMERANG:	// �X�s�A�u�[������
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case KN_BRANDISHSPEAR:
				damage = damage*(100+ 20*skill_lv)/100;
				break;
			case KN_BOWLINGBASH:	// �{�E�����O�o�b�V��
				damage = damage*(100+ 50*skill_lv)/100;
				//blewcount=4;skill.c�Ő�����΂�����Ă݂�
				break;
			case AS_SONICBLOW:	// �\�j�b�N�u���E
				damage = damage*(300+ 50*skill_lv)/100;
				div_=8;
				break;
			case AC_CHARGEARROW:	// �`���[�W�A���[
				damage = damage*150/100;
				blewcount=6;
				break;
			case TF_SPRINKLESAND:	// ���܂�
				damage = damage*125/100;
				break;
			case MC_CARTREVOLUTION:	// �J�[�g���{�����[�V����
				damage = (damage*150)/100;
				blewcount=2;
				break;
			// �ȉ�MOB
			case NPC_COMBOATTACK:	// ���i�U��
				div_=skill_get_num(171,skill_lv);
				damage *= div_;
				break;
			case NPC_RANDOMATTACK:	// �����_��ATK�U��
				damage = damage*(50+rand()%150)/100;
				break;
			// �����U���i�K���j
			case NPC_WATERATTACK:
			case NPC_GROUNDATTACK:
			case NPC_FIREATTACK:
			case NPC_WINDATTACK:
			case NPC_POISONATTACK:
			case NPC_HOLYATTACK:
			case NPC_DARKNESSATTACK:
			case NPC_TELEKINESISATTACK:
				damage = damage*(100+25*skill_lv)/100;
				break;
			case NPC_GUIDEDATTACK:
				hitrate = 1000000;
				break;
			case RG_BACKSTAP:	// �o�b�N�X�^�u
				damage = damage*(300+ 40*skill_lv)/100;
				hitrate = 1000000;
				break;
			case RG_RAID:	// �T�v���C�Y�A�^�b�N
				damage = damage*(100+ 40*skill_lv)/100;
				break;
			case RG_INTIMIDATE:	// �C���e�B�~�f�C�g
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_SHIELDCHARGE:	// �V�[���h�`���[�W
				damage = damage*(100+ 20*skill_lv)/100;
				blewcount=4+skill_lv;
				break;
			case CR_SHIELDBOOMERANG:	// �V�[���h�u�[������
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_HOLYCROSS:	// �z�[���[�N���X
				damage = damage*(100+ 35*skill_lv)/100;
				div_=2;
				break;
			case CR_GRANDCROSS:
				hitrate= 1000000;
				break;
			case MO_FINGEROFFENSIVE:	//�w�e
				damage = damage * (100 + 50 * skill_lv) / 100;
				div_ = 1;
				break;
			case MO_INVESTIGATE:	// �� ��
				damage = damage*(100+ 75*skill_lv)/100 * (def1 + def2)/100;
				hitrate = 1000000;
				s_ele = 0;
				break;
			case MO_EXTREMITYFIST:	// ���C���e�P��
				damage = damage * 8 + 250 + (skill_lv * 150);
				hitrate = 1000000;
				s_ele = 0;
				break;
			case MO_CHAINCOMBO:	// �A�ŏ�
				damage = damage*(150+ 50*skill_lv)/100;
				div_=4;
				break;
			case MO_COMBOFINISH:	// �җ���
				blewcount=3;
				damage = damage*(240+ 60*skill_lv)/100;
				break;
			case BA_MUSICALSTRIKE:	// �~���[�W�J���X�g���C�N
				damage = damage*(100+ 50 * skill_lv)/100;
				break;
			case BA_DISSONANCE:	// �s���a��
				damage = skill_lv*20+30;
				break;
			case DC_THROWARROW:	// ���
				damage = damage*(100+ 50 * skill_lv)/100;
				break;
			}
		}

		if( skill_num!=NPC_CRITICALSLASH ){
			// �� �ۂ̖h��͂ɂ��_���[�W�̌���
			// �f�B�o�C���v���e�N�V�����i�����ł����̂��ȁH�j
			if ( skill_num != MO_INVESTIGATE && skill_num != MO_EXTREMITYFIST) {	//DEF, VIT����
				t_vit = def2*8/10;

				vitbonusmax = (t_vit/20)*(t_vit/20)-1;
				damage = damage * (100 - def1) /100
					- t_vit - ((vitbonusmax < 1)?0: rand()%(vitbonusmax+1) );
			}
		}
	}

	// 0�����������ꍇ1�ɕ␳
	if(damage<1) damage=1;

	// ����C��
	if(hitrate < 1000000)
		hitrate = ( (hitrate>95)?95: ((hitrate<5)?5:hitrate) );
	if(	hitrate < 1000000 &&			// �K���U��
		(t_sc_data != NULL && (t_sc_data[SC_SLEEP].timer!=-1 ||	// �����͕K��
		t_sc_data[SC_STAN].timer!=-1 ||		// �X�^���͕K��
		t_sc_data[SC_FREEZE].timer!=-1 ) ) )	// �����͕K��
		hitrate = 100;
	if(type == 0 && rand()%100 >= hitrate)
		damage = 0;

	// �� ���̓K�p
	damage=battle_attr_fix(damage, s_ele, battle_get_element(target) );

	// �C���x�i���C��
	if(skill_num==TF_POISON){
		damage = battle_attr_fix(damage + 15*skill_lv, s_ele, battle_get_element(target) );
	}
	if(skill_num==MC_CARTREVOLUTION){
		damage = battle_attr_fix(damage, 0, battle_get_element(target) );
	}

	// ���S����̔���
	if(battle_config.enemy_perfect_flee) {
		if( skill_num==0 && tmd!=NULL && rand()%1000<battle_get_luk(target)+1 ){
			damage=0;
			type=0x0b;
		}
	}

	if(def1 >= 10000) {
		if(damage > 0)
			damage = 1;
	}

	if(battle_config.skill_min_damage) {
		if(div_ < 255) {
			if(damage > 0 && damage < div_)
				damage = div_;
		}
		else if(damage > 0 && damage < 3)
			damage = 3;
	}

	if(skill_num != CR_GRANDCROSS)
		damage=battle_calc_damage(target,damage,flag);

	wd.damage=damage;
	wd.damage2=0;
	wd.type=type;
	wd.div_=div_;
	wd.amotion=battle_get_amotion(src);
	wd.dmotion=battle_get_dmotion(target);
	wd.blewcount=blewcount;

	return wd;
}

static struct Damage battle_calc_mob_weapon_attack(
	struct block_list *src,struct block_list *target,int skill_num,int skill_lv,int wflag)
{
	struct map_session_data *tsd=NULL;
	struct mob_data* md=(struct mob_data *)src,*tmd=NULL;
	int hitrate,cri = 0,atkmin,atkmax;
	int str,luk;
	int def1 = battle_get_def(target);
	int def2 = battle_get_def2(target);
	struct Damage wd;
	int damage,type,div_,blewcount=0;
	int flag,skill;
	int t_mode=0,t_race=0,t_size=1,s_race=0,s_ele=0;
	struct status_change *sc_data,*t_sc_data;
	short *sc_count;
	short *option, *opt1, *opt2;

	s_race=battle_get_race(src);
	s_ele=battle_get_attack_element(src);
	sc_data=battle_get_sc_data(src);
	sc_count=battle_get_sc_count(src);
	option=battle_get_option(src);
	opt1=battle_get_opt1(src);
	opt2=battle_get_opt2(src);
	
	// �^�[�Q�b�g
	if(target->type==BL_PC)
		tsd=(struct map_session_data *)target;
	else
		tmd=(struct mob_data *)target;
	t_race=battle_get_race( target );
	t_size=battle_get_size( target );
	t_mode=battle_get_mode( target );
	t_sc_data=battle_get_sc_data( target );

	flag=BF_SHORT|BF_WEAPON|BF_NORMAL;	// �U���̎�ނ̐ݒ�
	
	// ��𗦌v�Z�A��𔻒�͌��
	hitrate=battle_get_hit(src) - battle_get_flee(target) + 80;

	type=0;	// normal
	div_ = 1; // single attack

	str=battle_get_str(src);
	luk=battle_get_luk(src);

	damage = (str/10)*(str/10)+str;
	atkmin = battle_get_atk(src);
	atkmax = battle_get_atk2(src);
	if( mob_db[md->class].range>2 )
		flag=(flag&~BF_RANGEMASK)|BF_LONG;

	if(atkmin > atkmax) atkmin = atkmax;

	if(sc_data != NULL && sc_data[SC_MAXIMIZEPOWER].timer!=-1 ){	// �}�L�V�}�C�Y�p���[
		atkmin=atkmax;
	}

	cri = 1 + luk*3;
	cri -= battle_get_luk(target) * 3;
	if(battle_config.enemy_critical_rate != 100) {
		cri = cri*battle_config.enemy_critical_rate/100;
		if(cri < 1)
			cri = 1;
	}
	if(t_sc_data != NULL && t_sc_data[SC_SLEEP].timer!=-1 )	// �������̓N���e�B�J�����{��
		cri <<=1;

	if(tsd && tsd->critical_def)
		cri = cri * (100 - tsd->critical_def);

	if(skill_num==0 && battle_config.enemy_critical && (rand() % 1000) < cri) 	// ����i�X�L���̏ꍇ�͖����j
			// �G�̔���
	{
		damage += atkmax;
		type = 0x0a;
	}
	else {
		int vitbonusmax, t_vit=0;
	
		if(atkmax > atkmin)
			damage += atkmin + rand() % (atkmax-atkmin + 1);
		else
			damage += atkmin ;
		// �X�L���C���P�i�U���͔{���n�j
		// �I�[�o�[�g���X�g(+5% �` +25%),���U���n�X�L���̏ꍇ�����ŕ␳
		// �o�b�V��,�}�O�i���u���C�N,
		// �{�[�����O�o�b�V��,�X�s�A�u�[������,�u�����f�B�b�V���X�s�A,�X�s�A�X�^�b�u,
		// ���}�[�i�C�g,�J�[�g���{�����[�V����
		// �_�u���X�g���C�t�B���O,�A���[�V�����[,�`���[�W�A���[,
		// �\�j�b�N�u���[
		if( sc_data!=NULL && sc_data[SC_OVERTHRUST].timer!=-1)	// �I�[�o�[�g���X�g
			damage += damage*(5*sc_data[SC_OVERTHRUST].val1)/100;
		if(skill_num>0){
			int i;
			if( (i=skill_get_pl(skill_num))>0 )
				s_ele=i;

			flag=(flag&~BF_SKILLMASK)|BF_SKILL;
			switch( skill_num ){
			case SM_BASH:		// �o�b�V��
				damage = damage*(100+ 30*skill_lv)/100;
				hitrate = (hitrate*(100+5*skill_lv))/100;
				break;
			case SM_MAGNUM:		// �}�O�i���u���C�N
				damage = damage*(5*skill_lv +(wflag)?65:115 )/100;
				blewcount=2;
				break;
			case MC_MAMMONITE:	// ���}�[�i�C�g
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case AC_DOUBLE:	// �_�u���X�g���C�t�B���O
				damage = damage*(180+ 20*skill_lv)/100;
				div_=2;
				break;
			case AC_SHOWER:	// �A���[�V�����[
				damage = damage*(75 + 5*skill_lv)/100;
				blewcount=2;
				break;
			case KN_PIERCE:	// �s�A�[�X
				damage = damage*(100+ 10*skill_lv)/100;
				hitrate=hitrate*(100+5*skill_lv)/100;
				div_=t_size+1;
				damage*=div_;
				break;
			case KN_SPEARSTAB:	// �X�s�A�X�^�u
				damage = damage*(100+ 15*skill_lv)/100;
				blewcount=6;
				break;
			case KN_SPEARBOOMERANG:	// �X�s�A�u�[������
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case KN_BRANDISHSPEAR:
				damage = damage*(100+ 20*skill_lv)/100;
				break;
			case KN_BOWLINGBASH:	// �{�E�����O�o�b�V��
				damage = damage*(100+ 50*skill_lv)/100;
				//blewcount=4;skill.c�Ő�����΂�����Ă݂�
				break;
			case AS_SPLASHER:		// AppleGirl
				damage = damage*(200+ 20*skill_lv)/100;
				div_=6;
				break;
			case AS_SONICBLOW:	// �\�j�b�N�u���E
				damage = damage*(300+ 50*skill_lv)/100;
				div_=8;
				break;
			case AC_CHARGEARROW:	// �`���[�W�A���[
				damage = damage*150/100;
				blewcount=6;
				break;
			case TF_SPRINKLESAND:	// ���܂�
				damage = damage*125/100;
				break;
			case MC_CARTREVOLUTION:	// �J�[�g���{�����[�V����
				damage = (damage*150)/100;
				blewcount=2;
				break;
			// �ȉ�MOB
			case NPC_COMBOATTACK:	// ���i�U��
				div_=skill_get_num(171,skill_lv);
				damage *= div_;
				break;
			case NPC_RANDOMATTACK:	// �����_��ATK�U��
				damage = damage*(50+rand()%150)/100;
				break;
			// �����U���i�K���j
			case NPC_WATERATTACK:
			case NPC_GROUNDATTACK:
			case NPC_FIREATTACK:
			case NPC_WINDATTACK:
			case NPC_POISONATTACK:
			case NPC_HOLYATTACK:
			case NPC_DARKNESSATTACK:
			case NPC_TELEKINESISATTACK:
				damage = damage*(100+25*skill_lv)/100;
				break;
			case NPC_GUIDEDATTACK:
				hitrate = 1000000;
				break;
			case RG_BACKSTAP:	// �o�b�N�X�^�u
				damage = damage*(300+ 40*skill_lv)/100;
				hitrate = 1000000;
				break;
			case RG_RAID:	// �T�v���C�Y�A�^�b�N
				damage = damage*(100+ 40*skill_lv)/100;
				break;
			case RG_INTIMIDATE:	// �C���e�B�~�f�C�g
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_SHIELDCHARGE:	// �V�[���h�`���[�W
				damage = damage*(100+ 20*skill_lv)/100;
				blewcount=4+skill_lv;
				break;
			case CR_SHIELDBOOMERANG:	// �V�[���h�u�[������
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_HOLYCROSS:	// �z�[���[�N���X
				damage = damage*(100+ 35*skill_lv)/100;
				div_=2;
				break;
			case CR_GRANDCROSS:
				hitrate= 1000000;
				break;
			case MO_FINGEROFFENSIVE:	//�w�e
				damage = damage * (100 + 50 * skill_lv) / 100;
				div_ = 1;
				break;
			case MO_INVESTIGATE:	// �� ��
				damage = damage*(100+ 75*skill_lv)/100 * (def1 + def2)/100;
				hitrate = 1000000;
				s_ele = 0;
				break;
			case MO_EXTREMITYFIST:	// ���C���e�P��
				damage = damage * 8 + 250 + (skill_lv * 150);
				hitrate = 1000000;
				s_ele = 0;
				break;
			case MO_CHAINCOMBO:	// �A�ŏ�
				damage = damage*(150+ 50*skill_lv)/100;
				div_=4;
				break;
			case MO_COMBOFINISH:	// �җ���
				blewcount=3;
				damage = damage*(240+ 60*skill_lv)/100;
				break;
			case BA_MUSICALSTRIKE:	// �~���[�W�J���X�g���C�N
				damage = damage*(100+ 50 * skill_lv)/100;
				break;
			case BA_DISSONANCE:	// �s���a��
				damage = skill_lv*20+30;
				break;
			case DC_THROWARROW:	// ���
				damage = damage*(100+ 50 * skill_lv)/100;
				break;
			}
		}

		if( skill_num!=NPC_CRITICALSLASH ){
			// �� �ۂ̖h��͂ɂ��_���[�W�̌���
			// �f�B�o�C���v���e�N�V�����i�����ł����̂��ȁH�j
			if ( skill_num != MO_INVESTIGATE && skill_num != MO_EXTREMITYFIST) {	//DEF, VIT����
				t_vit = def2*8/10;
				if(s_race==1 || s_race==6)
					if(target->type==BL_PC && (skill=pc_checkskill(tsd,AL_DP)) > 0 )
						t_vit+=skill*3;

				vitbonusmax = (t_vit/20)*(t_vit/20)-1;
				damage = damage * (100 - def1) /100
					- t_vit - ((vitbonusmax < 1)?0: rand()%(vitbonusmax+1) );
			}
		}
	}

	// 0�����������ꍇ1�ɕ␳
	if(damage<1) damage=1;

	// ����C��
	if(hitrate < 1000000)
		hitrate = ( (hitrate>95)?95: ((hitrate<5)?5:hitrate) );
	if(	hitrate < 1000000 &&			// �K���U��
		(t_sc_data != NULL && (t_sc_data[SC_SLEEP].timer!=-1 ||	// �����͕K��
		t_sc_data[SC_STAN].timer!=-1 ||		// �X�^���͕K��
		t_sc_data[SC_FREEZE].timer!=-1 ) ) )	// �����͕K��
		hitrate = 100;
	if(type == 0 && rand()%100 >= hitrate)
		damage = 0;

	if( target->type==BL_PC ){
		int cardfix=100,i;
		cardfix=cardfix*(100-tsd->subrace[s_race])/100;	// �푰�ɂ��_���[�W�ϐ�
		cardfix=cardfix*(100-tsd->subele[s_ele])/100;	// �� ���ɂ��_���[�W�ϐ�
		if(flag&BF_LONG)
			cardfix=cardfix*(100-tsd->long_attack_def_rate)/100;
		if(flag&BF_SHORT)
			cardfix=cardfix*(100-tsd->near_attack_def_rate)/100;
		if(mob_db[md->class].mexp > 0)
			cardfix=cardfix*(100-tsd->subrace[10])/100;
		else
			cardfix=cardfix*(100-tsd->subrace[11])/100;
		for(i=0;i<tsd->add_def_class_count;i++) {
			if(tsd->add_def_classid[i] == md->class) {
				cardfix=cardfix*(100-tsd->add_def_classrate[i])/100;
				break;
			}
		}
		damage=damage*cardfix/100;
	}
	if(damage < 0) damage =0;

	// �� ���̓K�p
	damage=battle_attr_fix(damage, s_ele, battle_get_element(target) );

	// �C���x�i���C��
	if(skill_num==TF_POISON){
		damage = battle_attr_fix(damage + 15*skill_lv, s_ele, battle_get_element(target) );
	}
	if(skill_num==MC_CARTREVOLUTION){
		damage = battle_attr_fix(damage, 0, battle_get_element(target) );
	}

	// ���S����̔���
	if( skill_num==0 && tsd!=NULL && rand()%100 < tsd->flee2){
		damage=0;
		type=0x0b;
	}

	if(battle_config.enemy_perfect_flee) {
		if( skill_num==0 && tmd!=NULL && rand()%1000<battle_get_luk(target)+1 ){
			damage=0;
			type=0x0b;
		}
	}

	if(def1 >= 10000) {
		if(damage > 0)
			damage = 1;
	}

	if(battle_config.skill_min_damage) {
		if(div_ < 255) {
			if(damage > 0 && damage < div_)
				damage = div_;
		}
		else if(damage > 0 && damage < 3)
			damage = 3;
	}

	if( tsd && tsd->special_state.no_weapon_damage)
		damage = 0;

	if(skill_num != CR_GRANDCROSS)
		damage=battle_calc_damage(target,damage,flag);

	wd.damage=damage;
	wd.damage2=0;
	wd.type=type;
	wd.div_=div_;
	wd.amotion=battle_get_amotion(src);
	wd.dmotion=battle_get_dmotion(target);
	wd.blewcount=blewcount;

	return wd;
}

static struct Damage battle_calc_pc_weapon_attack(
	struct block_list *src,struct block_list *target,int skill_num,int skill_lv,int wflag)
{
	struct map_session_data *sd=(struct map_session_data *)src,*tsd=NULL;
	struct mob_data *tmd=NULL;
	int hitrate,cri = 0,atkmin,atkmax;
	int str,dex,luk;
	int def1 = battle_get_def(target);
	int def2 = battle_get_def2(target);
	struct Damage wd;
	int damage,damage2,type,div_,blewcount=0;
	int flag,skill;
	int t_mode=0,t_race=0,t_size=1,s_race=7,s_ele=0;
	struct status_change *sc_data,*t_sc_data;
	short *sc_count;
	short *option, *opt1, *opt2;
	int atkmax_=0, atkmin_=0, s_ele_;	//�񓁗��p
	int watk,watk_,cardfix,t_ele;
	int da=0,i,t_class;
	int idef_flag=0,idef_flag_=0;

	// �A�^�b�J�[
	s_race=battle_get_race(src);
	s_ele=battle_get_attack_element(src);
	s_ele_=battle_get_attack_element2(src);
	sc_data=battle_get_sc_data(src);
	sc_count=battle_get_sc_count(src);
	option=battle_get_option(src);
	opt1=battle_get_opt1(src);
	opt2=battle_get_opt2(src);

	if(skill_num != CR_GRANDCROSS)
		sd->state.attack_type = BF_WEAPON;

	// �^�[�Q�b�g
	if(target->type==BL_PC)
		tsd=(struct map_session_data *)target;
	else
		tmd=(struct mob_data *)target;
	t_ele=battle_get_elem_type(target);
	t_race=battle_get_race( target );
	t_size=battle_get_size( target );
	t_mode=battle_get_mode( target );
	t_sc_data=battle_get_sc_data( target );

	flag=BF_SHORT|BF_WEAPON|BF_NORMAL;	// �U���̎�ނ̐ݒ�
	
	// ��𗦌v�Z�A��𔻒�͌��
	hitrate=battle_get_hit(src) - battle_get_flee(target) + 80;

	type=0;	// normal
	div_ = 1; // single attack

	str=battle_get_str(src);
	dex=battle_get_dex(src);
	luk=battle_get_luk(src);
	watk = battle_get_atk(src);
	watk_ = battle_get_atk_(src);

	damage = damage2 = sd->base_atk;
	atkmin = atkmin_ = dex;
	sd->state.arrow_atk = 0;
	if(sd->weapontype1 == 11) {
		atkmin = watk * ((dex<watk)? dex:watk);
		flag=(flag&~BF_RANGEMASK)|BF_LONG;
		s_ele = sd->arrow_ele;
		sd->state.arrow_atk = 1;
	}
	if(sd->equip_index[9] >= 0 && sd->inventory_data[sd->equip_index[9]])
		atkmin = atkmin*(80 + sd->inventory_data[sd->equip_index[9]]->wlv*20)/100;
	if(sd->equip_index[8] >= 0 && sd->inventory_data[sd->equip_index[8]])
		atkmin_ = atkmin_*(80 + sd->inventory_data[sd->equip_index[8]]->wlv*20)/100;
	if(sd->state.arrow_atk)
		atkmin/=100;

		// �T�C�Y�C��
		// �y�R�R�悵�Ă��āA���ōU�������ꍇ�͒��^�̃T�C�Y�C����100�ɂ���
		// �E�F�|���p�[�t�F�N�V����,�h���C�NC
	if(sd->special_state.no_sizefix){	// �h���C�N�J�[�h
		atkmax = watk;
		atkmax_ = watk_;
	}
	else if( target->type==BL_MOB ){
		if(pc_isriding(sd) && (sd->weapontype1==4 || sd->weapontype1==5) && t_size==1) {	//�y�R�R�悵�Ă��āA���Œ��^���U��
			atkmax = watk;
			atkmax_ = watk_;
		}
		else {
			atkmax = (watk * sd->atkmods[ t_size ]) / 100;
			atkmax_ = (watk_ * sd->atkmods_[ t_size ]) / 100;
		}
	}
	else {
		atkmax = watk;
		atkmax_ = watk_;
	}

	if(sc_data != NULL && sc_data[SC_WEAPONPERFECTION].timer!=-1 ) {	// �E�F�|���p�[�t�F�N�V����
		atkmax = watk;
		atkmax_ = watk_;
	}

	if(atkmin > atkmax) atkmin = atkmax;
	if(atkmin_ > atkmax_) atkmin_ = atkmax_;

	if(sc_data != NULL && sc_data[SC_MAXIMIZEPOWER].timer!=-1 ){	// �}�L�V�}�C�Y�p���[
		atkmin=atkmax;
		atkmin_=atkmax_;
	}

	//�_�u���A�^�b�N����
	if(sd->weapontype1 == 0x01) {
		if( skill_num==0 && (skill = pc_checkskill(sd,TF_DOUBLE)) > 0)
			da = (rand()%100 < (skill*5)) ? 1:0;
	}

	//�O�i��
	if(skill_num==0 && (skill = pc_checkskill(sd,MO_TRIPLEATTACK)) > 0 && sd->status.weapon <= 16 && !sd->state.arrow_atk) {
			da = (rand()%100 < (30 - skill)) ? 2:0;
	}

	if(sd->double_rate > 0 && da == 0 && skill_num == 0)
		da = (rand()%100 < sd->double_rate) ? 1:0;

 	// �ߏ萸�B�{�[�i�X
	if(sd->overrefine>0 )
		damage+=(rand()%sd->overrefine)+1;
	if(sd->overrefine_>0 )
		damage2+=(rand()%sd->overrefine_)+1;

	if(da == 0){ //�_�u���A�^�b�N���������Ă��Ȃ�
		// �N���e�B�J���v�Z
		cri = 1 + luk*3;

		// �J�[�h���̒ǉ����ʂ̍Čv�Z(�S������番���ɒ���)
		cri += 10*(sd->critical - ((sd->paramc[3]*3/10)+1));

		if(sd->state.arrow_atk)
			cri += sd->arrow_cri*10;
		if(sd->weapontype1 == 16)
				// �J�^�[���̏ꍇ�A�N���e�B�J����{��
			cri <<=1;
		cri -= battle_get_luk(target) * 3;
		if(t_sc_data != NULL && t_sc_data[SC_SLEEP].timer!=-1 )	// �������̓N���e�B�J�����{��
			cri <<=1;
	}

	if(tsd && tsd->critical_def)
		cri = cri * (100-tsd->critical_def);

	if(da == 0 && skill_num==0 && //�_�u���A�^�b�N���������Ă��Ȃ�
		(rand() % 1000) < cri) 	// ����i�X�L���̏ꍇ�͖����j
	{
		damage += atkmax;
		damage2 += atkmax_;
		if(sd->atk_rate != 100) {
			damage = (damage * sd->atk_rate)/100;
			damage2 = (damage2 * sd->atk_rate)/100;
		}
		if(sd->state.arrow_atk)
			damage += sd->arrow_atk;
		type = 0x0a;
	}
	else {
		int vitbonusmax, t_vit=0;

		if(atkmax > atkmin)
			damage += atkmin + rand() % (atkmax-atkmin + 1);
		else
			damage += atkmin ;
		if(atkmax_ > atkmin_)
			damage2 += atkmin_ + rand() % (atkmax_-atkmin_ + 1);
		else
			damage2 += atkmin_ ;
		if(sd->atk_rate != 100) {
			damage = (damage * sd->atk_rate)/100;
			damage2 = (damage2 * sd->atk_rate)/100;
		}

		if(sd->state.arrow_atk && sd->arrow_atk > 0)
			damage += rand()%(sd->arrow_atk+1);

		if(skill_num != MO_INVESTIGATE) {
			if(sd->def_ratio_atk_ele & (1<<t_ele) || sd->def_ratio_atk_race & (1<<t_race)) {
				damage = (damage * (def1 + def2))/100;
				idef_flag = 1;
			}
			if(sd->def_ratio_atk_ele_ & (1<<t_ele) || sd->def_ratio_atk_race_ & (1<<t_race)) {
				damage2 = (damage2 * (def1 + def2))/100;
				idef_flag_ = 1;
			}
			if(tmd) {
				if(mob_db[tmd->class].mexp > 0) {
					if(!idef_flag && sd->def_ratio_atk_race & (1<<10)) {
						damage = (damage * (def1 + def2))/100;
						idef_flag = 1;
					}
					if(!idef_flag_ && sd->def_ratio_atk_race_ & (1<<10)) {
						damage2 = (damage2 * (def1 + def2))/100;
						idef_flag_ = 1;
					}
				}
				else {
					if(!idef_flag && sd->def_ratio_atk_race & (1<<11)) {
						damage = (damage * (def1 + def2))/100;
						idef_flag = 1;
					}
					if(!idef_flag_ && sd->def_ratio_atk_race_ & (1<<11)) {
						damage2 = (damage2 * (def1 + def2))/100;
						idef_flag_ = 1;
					}
				}
			}
		}

		// �X�L���C���P�i�U���͔{���n�j
		// �I�[�o�[�g���X�g(+5% �` +25%),���U���n�X�L���̏ꍇ�����ŕ␳
		// �o�b�V��,�}�O�i���u���C�N,
		// �{�[�����O�o�b�V��,�X�s�A�u�[������,�u�����f�B�b�V���X�s�A,�X�s�A�X�^�b�u,
		// ���}�[�i�C�g,�J�[�g���{�����[�V����
		// �_�u���X�g���C�t�B���O,�A���[�V�����[,�`���[�W�A���[,
		// �\�j�b�N�u���[
		if( sc_data!=NULL && sc_data[SC_OVERTHRUST].timer!=-1)	// �I�[�o�[�g���X�g
			damage += damage*(5*sc_data[SC_OVERTHRUST].val1)/100;
		if(skill_num>0){
			int i;
			if( (i=skill_get_pl(skill_num))>0 )
				s_ele=s_ele_=i;

			flag=(flag&~BF_SKILLMASK)|BF_SKILL;
			switch( skill_num ){
			case SM_BASH:		// �o�b�V��
				damage = damage*(100+ 30*skill_lv)/100;
				hitrate = (hitrate*(100+5*skill_lv))/100;
				break;
			case SM_MAGNUM:		// �}�O�i���u���C�N
				damage = damage*(5*skill_lv +(wflag)?65:115 )/100;
				blewcount=2;
				break;
			case MC_MAMMONITE:	// ���}�[�i�C�g
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case AC_DOUBLE:	// �_�u���X�g���C�t�B���O
				if(!sd->state.arrow_atk && sd->arrow_atk > 0)
					damage += rand()%(sd->arrow_atk+1);
				damage = damage*(180+ 20*skill_lv)/100;
				div_=2;
				s_ele = sd->arrow_ele;
				sd->state.arrow_atk = 1;
				break;
			case AC_SHOWER:	// �A���[�V�����[
				damage = damage*(75 + 5*skill_lv)/100;
				blewcount=2;
				s_ele = sd->arrow_ele;
				sd->state.arrow_atk = 1;
				break;
			case KN_PIERCE:	// �s�A�[�X
				damage = damage*(100+ 10*skill_lv)/100;
				hitrate=hitrate*(100+5*skill_lv)/100;
				div_=t_size+1;
				damage*=div_;
				break;
			case KN_SPEARSTAB:	// �X�s�A�X�^�u
				damage = damage*(100+ 15*skill_lv)/100;
				blewcount=6;
				break;
			case KN_SPEARBOOMERANG:	// �X�s�A�u�[������
				damage = damage*(100+ 50*skill_lv)/100;
				break;
			case KN_BRANDISHSPEAR:
				damage = damage*(100+ 20*skill_lv)/100;
				break;
			case KN_BOWLINGBASH:	// �{�E�����O�o�b�V��
				damage = damage*(100+ 50*skill_lv)/100;
				//blewcount=4;skill.c�Ő�����΂�����Ă݂�
				break;
			case AS_SONICBLOW:	// �\�j�b�N�u���E
				damage = damage*(300+ 50*skill_lv)/100;
				div_=8;
				break;
			case AS_SPLASHER:		// AppleGirl
				damage = damage*(200+ 20*skill_lv)/100;
				div_=6;
				break;
			case AC_CHARGEARROW:	// �`���[�W�A���[
				if(!sd->state.arrow_atk && sd->arrow_atk > 0)
					damage += rand()%(sd->arrow_atk+1);
				damage = damage*150/100;
				blewcount=6;
				s_ele = sd->arrow_ele;
				sd->state.arrow_atk = 1;
				break;
			case TF_SPRINKLESAND:	// ���܂�
				damage = damage*125/100;
				break;
			case MC_CARTREVOLUTION:	// �J�[�g���{�����[�V����
				if(sd->cart_max_weight > 0 && sd->cart_weight > 0)
						damage = (damage*(150 + pc_checkskill(sd,BS_WEAPONRESEARCH) + (sd->cart_weight*100/sd->cart_max_weight) ) )/100;
					else
						damage = (damage*150)/100;
				blewcount=2;
				break;
			// �ȉ�MOB
			case NPC_COMBOATTACK:	// ���i�U��
				div_=skill_get_num(171,skill_lv);
				damage *= div_;
				break;
			case NPC_RANDOMATTACK:	// �����_��ATK�U��
				damage = damage*(50+rand()%150)/100;
				break;
			// �����U���i�K���j
			case NPC_WATERATTACK:
			case NPC_GROUNDATTACK:
			case NPC_FIREATTACK:
			case NPC_WINDATTACK:
			case NPC_POISONATTACK:
			case NPC_HOLYATTACK:
			case NPC_DARKNESSATTACK:
			case NPC_TELEKINESISATTACK:
				damage = damage*(100+25*skill_lv)/100;
				break;
			case NPC_GUIDEDATTACK:
				hitrate = 1000000;
				break;
			case RG_BACKSTAP:	// �o�b�N�X�^�u
				damage = damage*(300+ 40*skill_lv)/100;
				hitrate = 1000000;
				break;
			case RG_RAID:	// �T�v���C�Y�A�^�b�N
				damage = damage*(100+ 40*skill_lv)/100;
				break;
			case RG_INTIMIDATE:	// �C���e�B�~�f�C�g
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_SHIELDCHARGE:	// �V�[���h�`���[�W
				damage = damage*(100+ 20*skill_lv)/100;
				blewcount=4+skill_lv;
				break;
			case CR_SHIELDBOOMERANG:	// �V�[���h�u�[������
				damage = damage*(100+ 30*skill_lv)/100;
				break;
			case CR_HOLYCROSS:	// �z�[���[�N���X
				damage = damage*(100+ 35*skill_lv)/100;
				div_=2;
				break;
			case CR_GRANDCROSS:
				hitrate= 1000000;
				break;
			case MO_FINGEROFFENSIVE:	//�w�e
				if(battle_config.finger_offensive_type == 0) {
					damage = damage * (100 + 50 * skill_lv) / 100 * sd->spiritball_old;
					div_ = sd->spiritball_old;
				}
				else {
					damage = damage * (100 + 50 * skill_lv) / 100;
					div_ = 1;
				}
				break;
			case MO_INVESTIGATE:	// �� ��
				damage = damage*(100+ 75*skill_lv)/100 * (def1 + def2)/100;
				hitrate = 1000000;
				s_ele = 0;
				s_ele_ = 0;
				break;
			case MO_EXTREMITYFIST:	// ���C���e�P��
				damage = damage * (8 + ((sd->status.sp)/10)) + 250 + (skill_lv * 150);
				sd->status.sp = 0;
				clif_updatestatus(sd,SP_SP);
				hitrate = 1000000;
				s_ele = 0;
				s_ele_ = 0;
				break;
			case MO_CHAINCOMBO:	// �A�ŏ�
				damage = damage*(150+ 50*skill_lv)/100;
				div_=4;
				break;
			case MO_COMBOFINISH:	// �җ���
				blewcount=3;
				damage = damage*(240+ 60*skill_lv)/100;
				break;
			case BA_MUSICALSTRIKE:	// �~���[�W�J���X�g���C�N
				if(!sd->state.arrow_atk && sd->arrow_atk > 0)
					damage += rand()%(sd->arrow_atk+1);
				damage = damage*(100+ 50 * skill_lv)/100;
				s_ele = sd->arrow_ele;
				sd->state.arrow_atk = 1;
				break;
			case BA_DISSONANCE:	// �s���a��
				damage = skill_lv*20+30;
				break;
			case DC_THROWARROW:	// ���
				if(!sd->state.arrow_atk && sd->arrow_atk > 0)
					damage += rand()%(sd->arrow_atk+1);
				damage = damage*(100+ 50 * skill_lv)/100;
				s_ele = sd->arrow_ele;
				sd->state.arrow_atk = 1;
				break;
			}
		}
		if(da == 2) { //�O�i�����������Ă��邩
			type = 0x08;
			div_ = 255;	//�O�i���p�Ɂc
			damage = damage * (100 + 20 * pc_checkskill(sd, MO_TRIPLEATTACK)) / 100;
		}

		if( skill_num!=NPC_CRITICALSLASH ){
			// �� �ۂ̖h��͂ɂ��_���[�W�̌���
			// �f�B�o�C���v���e�N�V�����i�����ł����̂��ȁH�j
			if ( skill_num != MO_INVESTIGATE && skill_num != MO_EXTREMITYFIST) {	//DEF, VIT����
				t_vit = def2*8/10;
				vitbonusmax = (t_vit/20)*(t_vit/20)-1;
				if(sd->ignore_def_ele & (1<<t_ele) || sd->ignore_def_race & (1<<t_race))
					idef_flag = 1;
				if(sd->ignore_def_ele_ & (1<<t_ele) || sd->ignore_def_race_ & (1<<t_race))
					idef_flag_ = 1;
				if(tmd) {
					if(mob_db[tmd->class].mexp > 0) {
						if(sd->ignore_def_race & (1<<10))
							idef_flag = 1;
						if(sd->ignore_def_race_ & (1<<10))
							idef_flag_ = 1;
					}
					else {
						if(sd->ignore_def_race & (1<<11))
							idef_flag = 1;
						if(sd->ignore_def_race_ & (1<<11))
							idef_flag_ = 1;
					}
				}

				if(!idef_flag)
					damage = damage * (100 - def1) /100
						- t_vit - ((vitbonusmax < 1)?0: rand()%(vitbonusmax+1) );
				if(!idef_flag_)
					damage2 = damage2 * (100 - def1) /100
						- t_vit - ((vitbonusmax < 1)?0: rand()%(vitbonusmax+1) );
			}
		}
	}
	// ���B�_���[�W�̒ǉ�
	if( skill_num != MO_INVESTIGATE && skill_num != MO_EXTREMITYFIST) {			//DEF, VIT����
		damage += battle_get_atk2(src);
		damage2 += battle_get_atk_2(src);
	}

	// 0�����������ꍇ1�ɕ␳
	if(damage<1) damage=1;
	if(damage2<1) damage2=1;

	// �X�L���C���Q�i�C���n�j
	// �C���_���[�W(�E��̂�) �\�j�b�N�u���[���͕ʏ����i1���ɕt��1/8�K��)
	if( skill_num != MO_INVESTIGATE && skill_num != MO_EXTREMITYFIST && skill_num != CR_GRANDCROSS) {			//�C���_���[�W����
		damage = battle_addmastery(sd,target,damage,0);
		damage2 = battle_addmastery(sd,target,damage2,1);
	}

	if(sd->perfect_hit > 0) {
		if(rand()%100 < sd->perfect_hit)
			hitrate = 1000000;
	}

	// ����C��
	hitrate = (hitrate<5)?5:hitrate;
	if(	hitrate < 1000000 && // �K���U��
		(t_sc_data != NULL && (t_sc_data[SC_SLEEP].timer!=-1 ||	// �����͕K��
		t_sc_data[SC_STAN].timer!=-1 ||		// �X�^���͕K��
		t_sc_data[SC_FREEZE].timer!=-1 ) ) )	// �����͕K��
		hitrate = 100;
	if(type == 0 && rand()%100 >= hitrate)
		damage = damage2 = 0;

	// �X�L���C���R�i���팤���j
	if( (skill=pc_checkskill(sd,BS_WEAPONRESEARCH)) > 0) {
		damage+= skill*2;
		damage2+= skill*2;
	}
	// �J�[�h�E�_���[�WUP�C�� ���莝���̏ꍇ����̃J�[�h���E��ɓK�p,����̓J�[�h�ɂ��␳�͂Ȃ�
	cardfix=100;
	if(!sd->state.arrow_atk) {
		cardfix=cardfix*(100+sd->addrace[t_race])/100;	// �푰�ɂ��_���[�W�C��
		cardfix=cardfix*(100+sd->addele[t_ele])/100;	// �� ���ɂ��_���[�W�C��
		cardfix=cardfix*(100+sd->addsize[t_size])/100;	// �T�C�Y�ɂ��_���[�W�C��
	}
	else {
		cardfix=cardfix*(100+sd->addrace[t_race]+sd->arrow_addrace[t_race])/100;	// �푰�ɂ��_���[�W�C��
		cardfix=cardfix*(100+sd->addele[t_ele]+sd->arrow_addele[t_ele])/100;	// �� ���ɂ��_���[�W�C��
		cardfix=cardfix*(100+sd->addsize[t_size]+sd->arrow_addsize[t_size])/100;	// �T�C�Y�ɂ��_���[�W�C��
	}
	if(tmd) {
		if(mob_db[tmd->class].mexp > 0)
			cardfix=cardfix*(100+sd->addrace[10])/100;
		else
			cardfix=cardfix*(100+sd->addrace[11])/100;
	}
	t_class = battle_get_class(target);
	for(i=0;i<sd->add_damage_class_count;i++) {
		if(sd->add_damage_classid[i] == t_class) {
			cardfix=cardfix*(100+sd->add_damage_classrate[i])/100;
			break;
		}
	}
	damage=damage*cardfix/100;

	cardfix=100;
	cardfix=cardfix*(100+sd->addrace_[t_race])/100;	// �푰�ɂ��_���[�W�C��
	cardfix=cardfix*(100+sd->addele_[t_ele])/100;	// �� ���ɂ��_���[�W�C��
	cardfix=cardfix*(100+sd->addsize_[t_size])/100;	// �T�C�Y�ɂ��_���[�W�C��
	if(tmd) {
		if(mob_db[tmd->class].mexp > 0)
			cardfix=cardfix*(100+sd->addrace_[10])/100;
		else
			cardfix=cardfix*(100+sd->addrace_[11])/100;
	}
	for(i=0;i<sd->add_damage_class_count_;i++) {
		if(sd->add_damage_classid_[i] == t_class) {
			cardfix=cardfix*(100+sd->add_damage_classrate_[i])/100;
			break;
		}
	}
	damage2=damage2*cardfix/100;

	if( target->type==BL_PC ){
		cardfix=100;
		cardfix=cardfix*(100-tsd->subrace[s_race])/100;	// �푰�ɂ��_���[�W�ϐ�
		cardfix=cardfix*(100-tsd->subele[s_ele])/100;	// �� ���ɂ��_���[�W�ϐ�
		if(flag&BF_LONG)
			cardfix=cardfix*(100-tsd->long_attack_def_rate)/100;
		if(flag&BF_SHORT)
			cardfix=cardfix*(100-tsd->near_attack_def_rate)/100;
		for(i=0;i<tsd->add_def_class_count;i++) {
			if(tsd->add_def_classid[i] == sd->status.class) {
				cardfix=cardfix*(100-tsd->add_def_classrate[i])/100;
				break;
			}
		}
		damage=damage*cardfix/100;
		damage2=damage2*cardfix/100;
	}
	if(damage < 0) damage = 0;
	if(damage2 < 0) damage2 = 0;

	// �� ���̓K�p
	damage=battle_attr_fix(damage,s_ele, battle_get_element(target) );
	damage2=battle_attr_fix(damage2,s_ele_, battle_get_element(target) );

	// ���̂�����̓K�p
	damage += sd->star;
	damage2 += sd->star_;
	damage += sd->spiritball*3;
	damage2 += sd->spiritball*3;

	// >�񓁗��̍��E�_���[�W�v�Z�N������Ă��ꂥ�������������I
	// >map_session_data �ɍ���_���[�W(atk,atk2)�ǉ�����
	// >pc_calcstatus()�ł��ׂ����ȁH
	// map_session_data �ɍ��蕐��(atk,atk2,ele,star,atkmods)�ǉ�����
	// pc_calcstatus()�Ńf�[�^����͂��Ă��܂�

	if(sd->weapontype1 == 0 && sd->weapontype2 > 0) {
		damage = damage2;
		damage2 = 0;
	}
	// �E��A����C���̓K�p
	if(sd->status.weapon > 16 && pc_checkskill(sd,AS_LEFT) > 0) {// �񓁗���?
		int dmg = damage, dmg2 = damage2;
		// �E��C��(60% �` 100%) �E��S��
		skill = pc_checkskill(sd,AS_RIGHT);
		damage = damage * (50 + (skill * 10))/100;
		if(dmg > 0 && damage < 1) damage = 1;
		// ����C��(40% �` 80%) ����S��
		skill = pc_checkskill(sd,AS_LEFT);
		damage2 = damage2 * (30 + (skill * 10))/100;
		if(dmg2 > 0 && damage2 < 1) damage2 = 1;
	}
	else{
		damage2 = 0;
	}
		// �E��,�Z���̂�
	if(da == 1) { //�_�u���A�^�b�N���������Ă��邩
		div_ = 2;
		damage += damage;
		type = 0x08;
	}

	if(sd->weapontype1 == 16) {
		// �J�^�[���ǌ��_���[�W
		skill = pc_checkskill(sd,TF_DOUBLE);
		damage2 = damage * (1 + (skill * 2))/100;
		if(damage > 0 && damage2 < 1) damage2 = 1;
	}

	// �C���x�i���C��
	if(skill_num==TF_POISON){
		damage = battle_attr_fix(damage + 15*skill_lv, s_ele, battle_get_element(target) );
	}
	if(skill_num==MC_CARTREVOLUTION){
		damage = battle_attr_fix(damage, 0, battle_get_element(target) );
	}

	// ���S����̔���
	if( skill_num==0 && tsd!=NULL && rand()%100 < tsd->flee2){
		damage=damage2=0;
		type=0x0b;
	}

	if(battle_config.enemy_perfect_flee) {
		if( skill_num==0 && tmd!=NULL && rand()%1000<battle_get_luk(target)+1 ) {
			damage=damage2=0;
			type=0x0b;
		}
	}

	if(def1 >= 10000) {
		if(damage > 0)
			damage = 1;
		if(damage2 > 0)
			damage2 = 1;
	}

	if(battle_config.skill_min_damage) {
		if(div_ < 255) {
			if(damage > 0 && damage < div_)
				damage = div_;
		}
		else if(damage > 0 && damage < 3)
			damage = 3;
	}

	if( tsd && tsd->special_state.no_weapon_damage)
		damage = damage2 = 0;

	if(skill_num != CR_GRANDCROSS) {
		if(damage2<1)		// �_���[�W�ŏI�C��
			damage=battle_calc_damage(target,damage,flag);
		else if(damage<1)	// �E�肪�~�X�H
			damage2=battle_calc_damage(target,damage2,flag);
		else {	// �� ��/�J�^�[���̏ꍇ�͂�����ƌv�Z��₱����
			int d1=damage+damage2,d2=damage2;
			damage=battle_calc_damage(target,damage+damage2,flag);
			damage2=(d2*100/d1)*damage/100;
			if(damage2<1) damage2=1;
			damage-=damage2;
		}
	}

	wd.damage=damage;
	wd.damage2=damage2;
	wd.type=type;
	wd.div_=div_;
	wd.amotion=battle_get_amotion(src);
	wd.dmotion=battle_get_dmotion(target);
	wd.blewcount=blewcount;

	return wd;
}

/*==========================================
 * ����_���[�W�v�Z
 *------------------------------------------
 */
struct Damage battle_calc_weapon_attack(
	struct block_list *src,struct block_list *target,int skill_num,int skill_lv,int wflag)
{
	struct Damage wd;

	if(target->type == BL_PET)
		memset(&wd,0,sizeof(wd));
	else if(src->type == BL_PC)
		wd = battle_calc_pc_weapon_attack(src,target,skill_num,skill_lv,wflag);
	else if(src->type == BL_MOB)
		wd = battle_calc_mob_weapon_attack(src,target,skill_num,skill_lv,wflag);
	else if(src->type == BL_PET)
		wd = battle_calc_pet_weapon_attack(src,target,skill_num,skill_lv,wflag);
	else
		memset(&wd,0,sizeof(wd));

	return wd;
}

/*==========================================
 * ���@�_���[�W�v�Z
 *------------------------------------------
 */
struct Damage battle_calc_magic_attack(
	struct block_list *bl,struct block_list *target,int skill_num,int skill_lv,int flag)
{
	int mdef1=battle_get_mdef(target);
	int mdef2=battle_get_mdef2(target);
	int matk1,matk2,damage=0,div_=1,blewcount=0;
	struct Damage md;
	int aflag;
	int normalmagic_flag=1;
	int ele=0,race=7,t_ele=0,t_race=7,cardfix,t_class,i;
	struct map_session_data *sd=NULL,*tsd=NULL;
	struct mob_data *tmd = NULL;

	if(target->type == BL_PET) {
		memset(&md,0,sizeof(md));
		return md;
	}

	matk1=battle_get_matk1(bl);
	matk2=battle_get_matk2(bl);
	ele = skill_get_pl(skill_num);
	race = battle_get_race(bl);
	t_ele = battle_get_elem_type(target);
	t_race = battle_get_race( target );

#define MATK_FIX( a,b ) { matk1=matk1*(a)/(b); matk2=matk2*(a)/(b); }
	
	if( bl->type==BL_PC ){
		sd=(struct map_session_data *)bl;
		sd->state.attack_type = BF_MAGIC;
		if(sd->matk_rate != 100)
			MATK_FIX(sd->matk_rate,100);
	}
	if( target->type==BL_PC )
		tsd=(struct map_session_data *)target;
	else if( target->type==BL_MOB )
		tmd=(struct mob_data *)target;

	aflag=BF_MAGIC|BF_LONG|BF_SKILL;
	
	if(skill_num > 0){
		switch(skill_num){	// ��{�_���[�W�v�Z(�X�L�����Ƃɏ���)
					// �q�[��or����
		case AL_HEAL:
		case PR_BENEDICTIO:
			damage = skill_calc_heal(bl,skill_lv)/2;
			normalmagic_flag=0;
			break;
		case PR_SANCTUARY:	// �T���N�`���A��
			damage = (skill_lv>6)?388:skill_lv*50;
			normalmagic_flag=0;
			blewcount=3|0x10000;
			break;
		case ALL_RESURRECTION:
		case PR_TURNUNDEAD:	// �U�����U���N�V�����ƃ^�[���A���f�b�h
			if( battle_get_elem_type(target)==9){
				int hp = 0, mhp = 0, thres = 0;
				hp = battle_get_hp(target);
				mhp = battle_get_max_hp(target);
				thres = (skill_lv * 20) + battle_get_luk(bl)+
						battle_get_int(bl) + battle_get_lv(bl)+
						((200 - hp * 200 / mhp));
//				printf("�^�[���A���f�b�h�I �m��%d ��(�番��)\n", thres);
				if(rand()%1000 < thres && !(battle_get_mexp(target)))	// ����
					damage = hp;
				else					// ���s
					damage = battle_get_lv(bl) + battle_get_int(bl) + skill_lv * 10;
			}
			normalmagic_flag=0;
			break;

		case MG_NAPALMBEAT:	// �i�p�[���r�[�g�i���U�v�Z���݁j
			MATK_FIX(70+ skill_lv*10,100);
			if(flag>0){
				MATK_FIX(1,flag);
			}else
				printf("battle_calc_magic_attack(): napam enemy count=0 !\n");
			break;
		case MG_FIREBALL:	// �t�@�C���[�{�[��
			{
				const int drate[]={100,90,70};
				if(flag>2)
					matk1=matk2=0;
				else
					MATK_FIX( (95+skill_lv*5)*drate[flag] ,10000 );
			}
			break;
		case MG_FIREWALL:	// �t�@�C���[�E�H�[��
			{
				int ele=battle_get_elem_type(target);
				if( ele!=3 && ele!=9 )
					blewcount=2|0x10000;
				MATK_FIX( 1,2 );
			}
			break;
		case MG_THUNDERSTORM:	// �T���_�[�X�g�[��
			MATK_FIX( 80,100 );
			break;
		case MG_FROSTDIVER:	// �t���X�g�_�C�o
			MATK_FIX( 100+skill_lv*10, 100);
			break;
		case WZ_FIREPILLAR:	// �t�@�C���[�s���[
			mdef1=mdef2=0;	// MDEF����
			MATK_FIX( 1,5 );
			matk1+=50;
			matk2+=50;
			break;
		case WZ_METEOR:
//			MATK_FIX( skill_lv*30+80, 100 );
			break;
		case WZ_JUPITEL:	// ���s�e���T���_�[
			blewcount=(skill_lv>6)?6:skill_lv;
			break;
		case WZ_VERMILION:	// ���[�h�I�u�o�[�~���I��
			MATK_FIX( skill_lv*20+80, 100 );
			break;
		case WZ_WATERBALL:	// �E�H�[�^�[�{�[��
			matk1+= skill_lv*30;
			matk2+= skill_lv*30;
			break;
		case WZ_STORMGUST:	// �X�g�[���K�X�g
			MATK_FIX( skill_lv*40+100 ,100 );
			blewcount=2|0x10000;
			break;
		case AL_HOLYLIGHT:	// �z�[���[���C�g
			MATK_FIX( 125,100 );
			break;
		case AL_RUWACH:
			MATK_FIX( 145,100 );
			break;
		}
	}

	if(normalmagic_flag){	// ��ʖ��@�_���[�W�v�Z
		int imdef_flag=0;
		if(matk1>matk2)
			damage= matk2+rand()%(matk1-matk2+1);
		else
			damage= matk2;
		if(sd) {
			if(sd->ignore_mdef_ele & (1<<t_ele) || sd->ignore_mdef_race & (1<<t_race))
				imdef_flag = 1;
			if(tmd) {
				if(mob_db[tmd->class].mexp > 0) {
					if(sd->ignore_mdef_race & (1<<10))
						imdef_flag = 1;
				}
				else {
					if(sd->ignore_mdef_race & (1<<11))
						imdef_flag = 1;
				}
			}
		}
		if(!imdef_flag)
			damage = (damage*(100-mdef1))/100 - mdef2;

		if(damage<1)
			damage=1;
	}

	damage=battle_attr_fix(damage, ele, battle_get_element(target) );		// �� ���C��

	if(sd) {
		cardfix=100;
		cardfix=cardfix*(100+sd->magic_addrace[t_race])/100;
		cardfix=cardfix*(100+sd->magic_addele[t_ele])/100;
		if(tmd) {
			if(mob_db[tmd->class].mexp > 0)
				cardfix=cardfix*(100+sd->magic_addrace[10])/100;
			else
				cardfix=cardfix*(100+sd->magic_addrace[11])/100;
		}
		t_class = battle_get_class(target);
		for(i=0;i<sd->add_magic_damage_class_count;i++) {
			if(sd->add_magic_damage_classid[i] == t_class) {
				cardfix=cardfix*(100+sd->add_magic_damage_classrate[i])/100;
				break;
			}
		}
		damage=damage*cardfix/100;
	}

	if( target->type==BL_PC ){
		int s_class = battle_get_class(bl);
		cardfix=100;
		cardfix=cardfix*(100-tsd->subele[ele])/100;	// �� ���ɂ��_���[�W�ϐ�
		cardfix=cardfix*(100-tsd->magic_subrace[race])/100;
		cardfix=cardfix*(100-tsd->magic_def_rate)/100;
		damage=damage*cardfix/100;
		for(i=0;i<tsd->add_mdef_class_count;i++) {
			if(tsd->add_mdef_classid[i] == s_class) {
				cardfix=cardfix*(100-tsd->add_mdef_classrate[i])/100;
				break;
			}
		}
	}

	if(skill_num == CR_GRANDCROSS) {	// �O�����h�N���X
		struct Damage wd;
		wd=battle_calc_weapon_attack(bl,target,skill_num,skill_lv,flag);
		damage = (damage + wd.damage) * (100 + 40*skill_lv)/100;
	}

	div_=skill_get_num( skill_num,skill_lv );
	
	if(div_>1 && skill_num != WZ_VERMILION)
		damage*=div_;

	if(mdef1 >= 10000 && damage > 0) {
		damage = 1;
	}
	if(battle_config.skill_min_damage) {
		if(damage > 0 && damage < div_)
			damage = div_;
	}

	if( target->type==BL_PC && ((struct map_session_data *)target)->special_state.no_magic_damage)
		damage=0;	// �� ��峃J�[�h�i���@�_���[�W�O�j

	damage=battle_calc_damage(target,damage,aflag);	// �ŏI�C��

	md.damage=damage;
	md.div_=div_;
	md.amotion=battle_get_amotion(bl);
	md.dmotion=battle_get_dmotion(target);
	md.damage2=0;
	md.type=0;
	md.blewcount=blewcount;

	return md;
}

/*==========================================
 * ���̑��_���[�W�v�Z
 *------------------------------------------
 */
struct Damage  battle_calc_misc_attack(
	struct block_list *bl,struct block_list *target,int skill_num,int skill_lv,int flag)
{
	int int_=battle_get_int(bl);
//	int luk=battle_get_luk(bl);
	int dex=battle_get_dex(bl);
	int skill,ele,cardfix;
	struct map_session_data *sd=NULL,*tsd=NULL;
	int damage=0,div_=1,blewcount=0;
	struct Damage md;
	int damagefix=1;

	int aflag=BF_MISC|BF_LONG|BF_SKILL;

	if(bl->type == BL_PC)
		((struct map_session_data *)bl)->state.attack_type = BF_MISC;

	if(target->type == BL_PET) {
		memset(&md,0,sizeof(md));
		return md;
	}

	if( bl->type==BL_PC )
		sd=(struct map_session_data *)bl;

	if( target->type==BL_PC )
		tsd=(struct map_session_data *)target;

	switch(skill_num){

	case HT_LANDMINE:	// �����h�}�C��
		damage=skill_lv*(dex+75)*(100+int_)/100;
		break;

	case HT_BLASTMINE:	// �u���X�g�}�C��
		damage=skill_lv*(dex/2+50)*(100+int_)/100;
		break;
	
	case HT_CLAYMORETRAP:	// �N���C���A�[�g���b�v
		damage=skill_lv*(dex/2+75)*(100+int_)/100;
		break;

	case HT_BLITZBEAT:	// �u���b�c�r�[�g
		if( sd==NULL || (skill = pc_checkskill(sd,HT_STEELCROW)) <= 0)
			skill=0;
		damage=(dex/10+int_/2+skill*3+40)*2;
		if(flag > 1)
			damage /= flag;
		break;

	case TF_THROWSTONE:	// �Γ���
		damage=30;
		damagefix=0;
		break;

	case NPC_SELFDESTRUCTION:	// ����
		damage=battle_get_hp(bl)-((bl==target)?1:0);
		damagefix=0;
		break;

	case NPC_SMOKING:	// �^�o�R���z��
		damage=3;
		damagefix=0;
		break;
	}

	ele = skill_get_pl(skill_num);
	if(damagefix){
		if(damage<1)
			damage=1;

		damage=battle_attr_fix(damage, ele, battle_get_element(target) );		// �� ���C��
		if( target->type==BL_PC ){
			cardfix=100;
			cardfix=cardfix*(100-tsd->subele[ele])/100;	// �� ���ɂ��_���[�W�ϐ�
			cardfix=cardfix*(100-tsd->misc_def_rate)/100;
			damage=damage*cardfix/100;
		}
	}

	div_=skill_get_num( skill_num,skill_lv );
	if(div_>1)
		damage*=div_;

	if(damage > 0 && (damage < div_ || (battle_get_def(target) >= 10000 && battle_get_mdef(target) >= 10000) ) ) {
		damage = div_;
	}

	damage=battle_calc_damage(target,damage,aflag);	// �ŏI�C��

	if (target->type == BL_MOB) {
		struct mob_data* tmd=(struct mob_data *)target;
		if (tmd->class == 1288)
			damage = 0;
	}

	md.damage=damage;
	md.div_=div_;
	md.amotion=battle_get_amotion(bl);
	md.dmotion=battle_get_dmotion(target);
	md.damage2=0;
	md.type=0;
	md.blewcount=blewcount;
	return md;

}
/*==========================================
 * �_���[�W�v�Z�ꊇ�����p
 *------------------------------------------
 */
struct Damage battle_calc_attack(	int attack_type,
	struct block_list *bl,struct block_list *target,int skill_num,int skill_lv,int flag)
{
	struct Damage d;
	switch(attack_type){
	case BF_WEAPON:
		return battle_calc_weapon_attack(bl,target,skill_num,skill_lv,flag);
	case BF_MAGIC:
		return battle_calc_magic_attack(bl,target,skill_num,skill_lv,flag);
	case BF_MISC:
		return battle_calc_misc_attack(bl,target,skill_num,skill_lv,flag);
	default:
		printf("battle_calc_attack: unknwon attack type ! %d\n",attack_type);
		break;
	}
	return d;
}
/*==========================================
 * �ʏ�U�������܂Ƃ�
 *------------------------------------------
 */
int battle_weapon_attack( struct block_list *src,struct block_list *target,
	 unsigned int tick,int flag)
{
	struct map_session_data *sd=NULL;
	short *opt1;
	sd = (struct map_session_data *)src;

	if(src->prev == NULL || target->prev == NULL)
		return 0;
	if(src->type == BL_PC && pc_isdead((struct map_session_data *)src))
		return 0;
	if(target->type == BL_PC && pc_isdead((struct map_session_data *)target))
		return 0;

	opt1=battle_get_opt1(src);
	if(opt1 && *opt1 > 0) {
		battle_stopattack(src);
		return 0;
	}
	if(battle_check_target(src,target,BCT_ENEMY) > 0 &&
		battle_check_range(src,target->x,target->y,0)){
		// �U���ΏۂƂȂ肤��̂ōU��
		struct Damage wd;
		wd=battle_calc_weapon_attack(src,target,0,0,0);
		if(src->type == BL_PC && sd->weapontype1 == 11) {
			if(sd->equip_index[10] >= 0) {
				if(battle_config.arrow_decrement)
					pc_delitem(sd,sd->equip_index[10],1,0);
			}
			else {
				clif_arrow_fail(sd,0);
				pc_stopattack(sd);			// Addeded by RoVeRT
				return 0;
			}
		}
		if (wd.div_ == 255 && src->type == BL_PC)	{ //�O�i��
			sd->skill_old = 0;
			sd->triple_delay = 1000 - 4 * battle_get_agi(src) - 2 *  battle_get_dex(src);
			if (sd->triple_delay < sd->aspd*2) sd->triple_delay = sd->aspd*2;
			if (pc_checkskill(sd, MO_COMBOFINISH)) sd->triple_delay += 300;
			clif_status_change(src, 0x59, 1);
			clif_combo_delay(src, sd->triple_delay);
			clif_skill_damage(src , target , tick , wd.amotion , wd.dmotion , 
				wd.damage , 3 , MO_TRIPLEATTACK, pc_checkskill(sd,MO_TRIPLEATTACK) , wd.type );
		}
		else
			clif_damage(src,target,tick, wd.amotion, wd.dmotion, 
				wd.damage, wd.div_ , wd.type, wd.damage2);
		//�񓁗�����ƃJ�^�[���ǌ��̃~�X�\��(�������`)
		if(wd.damage2==-1){wd.damage2=0;clif_damage(src,target,tick+200, wd.amotion, wd.dmotion,0, 1, 0, 0);}
		map_freeblock_lock();

		battle_damage(src,target,(wd.damage+wd.damage2));
		if((wd.damage+wd.damage2)>0)
			skill_additional_effect(src,target,0,0,BF_WEAPON,tick);

		map_freeblock_unlock();
	}
	return 0;
}

/*==========================================
 * �G��������(1=�m��,0=�ے�,-1=�G���[)
 * flag&0xf0000 = 0x00000:�G����Ȃ�������iret:1���G�ł͂Ȃ��j
 *				= 0x10000:�p�[�e�B�[����iret:1=�p�[�e�B�[�����o)
 *				= 0x20000:�S��(ret:1=�G��������)
 *				= 0x40000:�G������(ret:1=�G)
 *				= 0x50000:�p�[�e�B�[����Ȃ�������(ret:1=�p�[�e�B�łȂ�)
 *------------------------------------------
 */
int battle_check_target( struct block_list *src, struct block_list *target,int flag)
{
	int s_p,s_g,t_p,t_g;
	struct block_list *ss=src;

	if( flag&0x40000 ){	// ���]�t���O
		int ret=battle_check_target(src,target,flag&0x30000);
		if(ret!=-1)
			return !ret;
		return -1;
	}

	if( flag&0x20000 ){
		if( target->type==BL_MOB || target->type==BL_PC )
			return 1;
		else
			return -1;
	}
	
//	if( target->type==BL_SKILL )	// �� �ۂ��X�L�����j�b�g�Ȃ疳�����m��
//		return 0;

	if(target->type == BL_PET)
		return -1;

				// �X�L�����j�b�g�̏ꍇ�A�e�����߂�
	if( src->type==BL_SKILL)
		if( (ss=map_id2bl( ((struct skill_unit *)src)->group->src_id))==NULL )
			return -1;

	if( src==target || ss==target )	// �����Ȃ�m��
		return 1;

	if( src->prev==NULL ||	// ����ł�Ȃ�G���[
		(src->type==BL_PC && pc_isdead((struct map_session_data *)src) ) )
		return -1;

	if( (ss->type == BL_PC && target->type==BL_MOB) ||
		(ss->type == BL_MOB && target->type==BL_PC) )
		return 0;	// PCvsMOB�Ȃ�ے�

	if(ss->type == BL_PET && target->type==BL_MOB)
		return 0;

	s_p=battle_get_party_id(src);
	s_g=battle_get_guild_id(src);

	t_p=battle_get_party_id(target);
	t_g=battle_get_guild_id(target);

	if( s_p<0 || t_p<0 )	// PC��MOB����Ȃ�
		return -1;

	if(flag&0x10000 && s_p>0 && s_p == t_p )	// �����p�[�e�B�Ȃ�m��i�����j
		return 1;

	if(flag&0x10000 && s_p>0 && s_p != t_p )		// �p�[�e�B�����Ȃ瓯���p�[�e�B����Ȃ����_�Ŕے�
		return 0;

	if(ss->type == BL_MOB && s_g>0 && s_g == t_g )	// �����M���h/mob�N���X�Ȃ�m��i�����j
		return 1;

	if( ss->type==BL_PC && target->type==BL_PC) { // �� ��PVP���[�h�Ȃ�ے�i�G�j
		if(map[src->m].flag.pvp) {
			if(map[src->m].flag.pvp_noparty && s_p > 0 && s_p == t_p)
				return 1;
			else if(map[src->m].flag.pvp_noguild && s_g > 0 && s_g == t_g)
				return 1;
			return 0;
		}
		if(map[src->m].flag.gvg) {
			if(s_g > 0 && s_g == t_g)
				return 1;
			else if(map[src->m].flag.gvg_noparty && s_p > 0 && s_p == t_p)
				return 1;
			return 0;
		}
	}

	return 1;	// �Y�����Ȃ��̂Ŗ��֌W�l���i�܂��G����Ȃ��̂Ŗ����j
}
/*==========================================
 * �˒�����
 *------------------------------------------
 */
int battle_check_range(struct block_list *src,int x,int y,int range)
{
	int dx=abs(x-src->x),dy=abs(y-src->y);
	struct walkpath_data wpd;
	if( range>0 && range < ((dx>dy)?dx:dy) )	// ��������
		return 0;

	if( src->x==x && src->y==y )	// �����}�X
		return 1;

	// ��Q������
	wpd.path_len=0;
	wpd.path_pos=0;
	wpd.path_half=0;
	return (path_search(&wpd,src->m,src->x,src->y,x,y,0x10001)!=-1)?1:0;
}

/*==========================================
 * �ݒ�t�@�C���ǂݍ��ݗp�i�t���O�j
 *------------------------------------------
 */
int battle_config_switch(const char *str)
{
	if(strcmpi(str,"on")==0 || strcmpi(str,"yes")==0)
		return 1;
	if(strcmpi(str,"off")==0 || strcmpi(str,"no")==0)
		return 0;
	return atoi(str);
}
/*==========================================
 * �ݒ�t�@�C����ǂݍ���
 *------------------------------------------
 */
int battle_config_read(const char *cfgName)
{
	int i;
	char line[1024],w1[1024],w2[1024];
	FILE *fp;

	battle_config.warp_point_debug=0;
	battle_config.enemy_critical=1;
	battle_config.enemy_critical_rate=100;
	battle_config.enemy_perfect_flee=0;
	battle_config.cast_rate=100;
	battle_config.delay_rate=100;
	battle_config.delay_dependon_dex=0;
	battle_config.sdelay_attack_enable=0;
	battle_config.pc_skill_add_range=0;
	battle_config.skill_out_range_consume=1;
	battle_config.mob_skill_add_range=0;
	battle_config.pc_damage_delay=1;
	battle_config.pc_damage_delay_rate=100;
	battle_config.defnotenemy=1;
	battle_config.random_monster_checklv=1;
	battle_config.attr_recover=1;
	battle_config.flooritem_lifetime=LIFETIME_FLOORITEM*1000;
	battle_config.item_rate_common=100;	// Modified by RoVeRT
	battle_config.item_rate_equip=100;	// Added by RoVeRT
	battle_config.item_rate_card=100;	// Added by RoVeRT
	battle_config.drop_rate0item=0;
	battle_config.base_exp_rate=100;
	battle_config.job_exp_rate=100;
	battle_config.death_penalty_type=0;
	battle_config.death_penalty_base=0;
	battle_config.death_penalty_job=0;
	battle_config.restart_hp_rate=0;
	battle_config.restart_sp_rate=0;
	battle_config.mvp_item_rate=100;
	battle_config.mvp_exp_rate=100;
	battle_config.mvp_hp_rate=100;
	battle_config.atc_gmonly=0;
	battle_config.gm_allskill=0;
	battle_config.wp_rate=100;
	battle_config.monster_active_enable=1;
	battle_config.monster_damage_delay_rate=100;
	battle_config.monster_loot_type=0;
	battle_config.mob_skill_use=1;
	battle_config.mob_count_rate=100;
	battle_config.quest_skill_learn=0;
	battle_config.quest_skill_reset=1;
	battle_config.basic_skill_check=1;
	battle_config.guild_emperium_check=1;
	battle_config.ghost_time=5000;
	battle_config.pet_catch_rate=100;
	battle_config.pet_rename=0;
	battle_config.pet_friendly_rate=100;
	battle_config.pet_hungry_delay_rate=100;
	battle_config.pet_support=0;
	battle_config.pet_support_rate=100;
	battle_config.pet_attack_exp_to_master=0;
	battle_config.skill_min_damage=0;
	battle_config.sanctuary_type=0;
	battle_config.finger_offensive_type=0;
	battle_config.heal_exp=0;
	battle_config.shop_exp=0;
	battle_config.asuradelay=300;
	battle_config.item_check=1;
	battle_config.wedding_modifydisplay=0;
	battle_config.natural_healhp_interval=4000;
	battle_config.natural_healsp_interval=8000;
	battle_config.natural_heal_skill_interval=10000;
	battle_config.natural_heal_weight_rate=50;
	battle_config.item_name_override_grffile=1;
	battle_config.arrow_decrement=1;
	battle_config.max_aspd = 199;
	battle_config.max_hp = 32500;
	battle_config.max_sp = 32500;
	battle_config.max_cart_weight = 8000;
	battle_config.custom_db = 0;		// Added by RoVeRT
	battle_config.prevent_logout = 1;	// Added by RoVeRT
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
			{ "warp_point_debug",		&battle_config.warp_point_debug		},
			{ "enemy_critical",			&battle_config.enemy_critical		},
			{ "enemy_critical_rate",			&battle_config.enemy_critical_rate		},
			{ "enemy_perfect_flee", &battle_config.enemy_perfect_flee },
			{ "casting_rate",			&battle_config.cast_rate			},
			{ "delay_rate",				&battle_config.delay_rate			},
			{ "delay_dependon_dex",		&battle_config.delay_dependon_dex	},
			{ "skill_delay_attack_enable", &battle_config.sdelay_attack_enable },
			{ "player_skill_add_range", &battle_config.pc_skill_add_range },
			{ "skill_out_range_consume", &battle_config.skill_out_range_consume },
			{ "monster_skill_add_range", &battle_config.mob_skill_add_range },
			{ "player_damage_delay",	&battle_config.pc_damage_delay		},
			{ "player_damage_delay_rate",	&battle_config.pc_damage_delay_rate		},
			{ "defunit_not_enemy",		&battle_config.defnotenemy			},
			{ "random_monster_checklv",		&battle_config.random_monster_checklv	},
			{ "attribute_recover",		&battle_config.attr_recover			},
			{ "flooritem_lifetime",		&battle_config.flooritem_lifetime	},
			{ "item_rate_common",				&battle_config.item_rate_common			},	// Modified by RoVeRT
			{ "drop_rate0item",				&battle_config.drop_rate0item			},
			{ "base_exp_rate",			&battle_config.base_exp_rate		},
			{ "job_exp_rate",			&battle_config.job_exp_rate			},
			{ "death_penalty_type",		&battle_config.death_penalty_type	},
			{ "death_penalty_base",		&battle_config.death_penalty_base	},
			{ "death_penalty_job",		&battle_config.death_penalty_job	},
			{ "restart_hp_rate",		&battle_config.restart_hp_rate		},
			{ "restart_sp_rate",		&battle_config.restart_sp_rate		},
			{ "mvp_hp_rate",			&battle_config.mvp_hp_rate			},
			{ "mvp_item_rate",			&battle_config.mvp_item_rate		},
			{ "mvp_exp_rate",			&battle_config.mvp_exp_rate			},
			{ "atcommand_gm_only",		&battle_config.atc_gmonly			},
			{ "gm_all_skill",			&battle_config.gm_allskill			},
			{ "weapon_produce_rate",	&battle_config.wp_rate				},
			{ "monster_active_enable",	&battle_config.monster_active_enable},
			{ "monster_damage_delay_rate",	&battle_config.monster_damage_delay_rate		},
			{ "monster_loot_type",		&battle_config.monster_loot_type	},
			{ "mob_skill_use",			&battle_config.mob_skill_use		},
			{ "mob_count_rate",			&battle_config.mob_count_rate		},
			{ "quest_skill_learn",		&battle_config.quest_skill_learn	},
			{ "quest_skill_reset",		&battle_config.quest_skill_reset	},
			{ "basic_skill_check",		&battle_config.basic_skill_check	},
			{ "guild_emperium_check",	&battle_config.guild_emperium_check	},
			{ "ghost_time",				&battle_config.ghost_time			},
			{ "pet_catch_rate",			&battle_config.pet_catch_rate		},
			{ "pet_rename",				&battle_config.pet_rename		},
			{ "pet_friendly_rate",		&battle_config.pet_friendly_rate	},
			{ "pet_hungry_delay_rate",	&battle_config.pet_hungry_delay_rate	},
			{ "pet_support",	&battle_config.pet_support },
			{ "pet_support_rate",	&battle_config.pet_support_rate },
			{ "pet_attack_exp_to_master",	&battle_config.pet_attack_exp_to_master },
			{ "skill_min_damage",		&battle_config.skill_min_damage		},
			{ "sanctuary_type",			&battle_config.sanctuary_type		},
			{ "finger_offensive_type",	&battle_config.finger_offensive_type},
			{ "heal_exp",				&battle_config.heal_exp				},
			{ "shop_exp",				&battle_config.shop_exp				},
			{ "asuradelay",				&battle_config.asuradelay			},
			{ "item_check",				&battle_config.item_check			},
			{ "wedding_modifydisplay",				&battle_config.wedding_modifydisplay			},
			{ "natural_healhp_interval", &battle_config.natural_healhp_interval },
			{ "natural_healsp_interval", &battle_config.natural_healsp_interval },
			{ "natural_heal_skill_interval", &battle_config.natural_heal_skill_interval },
			{ "natural_heal_weight_rate", &battle_config.natural_heal_weight_rate },
			{ "item_name_override_grffile", &battle_config.item_name_override_grffile },
			{ "arrow_decrement", &battle_config.arrow_decrement },
			{ "max_aspd", &battle_config.max_aspd },
			{ "max_hp", &battle_config.max_hp },
			{ "max_sp", &battle_config.max_sp },
			{ "max_cart_weight", &battle_config.max_cart_weight },
		{ "item_rate_equip",	&battle_config.item_rate_equip },		// Added by RoVeRT
		{ "item_rate_card",	&battle_config.item_rate_card },
		{ "custom_db",		&battle_config.custom_db },
		{ "prevent_logout", 	&battle_config.prevent_logout },		/// End Addition

		};
		
		if(line[0] == '/' && line[1] == '/')
			continue;
		i=sscanf(line,"%[^:]:%s",w1,w2);
		if(i!=2)
			continue;
		for(i=0;i<sizeof(data)/(sizeof(data[0]));i++)
			if(strcmpi(w1,data[i].str)==0)
				*data[i].val=battle_config_switch(w2);
	}
	fclose(fp);

	if(battle_config.flooritem_lifetime < 1000)
		battle_config.flooritem_lifetime = LIFETIME_FLOORITEM*1000;
	if(battle_config.restart_hp_rate < 0)
		battle_config.restart_hp_rate = 0;
	else if(battle_config.restart_hp_rate > 100)
		battle_config.restart_hp_rate = 100;
	if(battle_config.restart_sp_rate < 0)
		battle_config.restart_sp_rate = 0;
	else if(battle_config.restart_sp_rate > 100)
		battle_config.restart_sp_rate = 100;
	if(battle_config.natural_healhp_interval < NATURAL_HEAL_INTERVAL)
		battle_config.natural_healhp_interval=NATURAL_HEAL_INTERVAL;
	if(battle_config.natural_healsp_interval < NATURAL_HEAL_INTERVAL)
		battle_config.natural_healsp_interval=NATURAL_HEAL_INTERVAL;
	if(battle_config.natural_heal_skill_interval < NATURAL_HEAL_INTERVAL)
		battle_config.natural_heal_skill_interval=NATURAL_HEAL_INTERVAL;
	if(battle_config.natural_heal_weight_rate < 50)
		battle_config.natural_heal_weight_rate = 50;
	if(battle_config.natural_heal_weight_rate > 101)
		battle_config.natural_heal_weight_rate = 101;
	battle_config.max_aspd = 2000 - battle_config.max_aspd*10;
	if(battle_config.max_aspd < 10)
		battle_config.max_aspd = 10;
	if(battle_config.max_aspd > 1000)
		battle_config.max_aspd = 1000;
	if(battle_config.max_hp > 1000000)
		battle_config.max_hp = 1000000;
	if(battle_config.max_hp < 100)
		battle_config.max_hp = 100;
	if(battle_config.max_sp > 1000000)
		battle_config.max_sp = 1000000;
	if(battle_config.max_sp < 100)
		battle_config.max_sp = 100;
	if(battle_config.max_cart_weight > 1000000)
		battle_config.max_cart_weight = 1000000;
	if(battle_config.max_cart_weight < 100)
		battle_config.max_cart_weight = 100;
	battle_config.max_cart_weight *= 10;
	if(battle_config.item_rate_common < 100)		// Added by RoVeRT
		battle_config.item_rate_common = 100;
	if(battle_config.item_rate_equip < 100)
		battle_config.item_rate_equip = 100;
	if(battle_config.item_rate_card < 100)
		battle_config.item_rate_card= 100;		// End Addition

	add_timer_func_list(battle_delay_damage_sub,"battle_delay_damage_sub");

	return 0;
}