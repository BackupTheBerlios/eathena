/* ƒXƒLƒ‹ŠÖŒW */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"

#include "skill.h"
#include "map.h"
#include "clif.h"
#include "pc.h"
#include "mob.h"
#include "battle.h"
#include "party.h"
#include "itemdb.h"

#define SKILLUNITTIMER_INVERVAL	100


/* ƒXƒLƒ‹”Ô†„ƒXƒe
[ƒ^ƒXˆÙí”Ô†•ÏŠ·ƒe[ƒuƒ‹ */
int SkillStatusChangeTable[]={	/* skill.h‚Ìenum‚ÌSC_***‚Æ‚ ‚í‚¹‚é‚±‚Æ */
/* 0- */
	-1,-1,-1,-1,-1,-1,
	SC_PROVOKE,			/* ƒvƒƒ{ƒbƒN */
	-1, 1,-1,
/* 10- */
	SC_SIGHT,			/* ƒTƒCƒg */
	-1,-1,-1,-1,
	SC_FREEZE,			/* ƒtƒƒXƒgƒ_ƒCƒo[ */
	SC_STONE,			/* ƒXƒg[ƒ“ƒJ[ƒX */
	-1,-1,-1,
/* 20- */
	-1,-1,-1,-1,
	SC_RUWACH,			/* ƒ‹ƒAƒt */
	-1,-1,-1,-1,
	SC_INCREASEAGI,		/* ‘¬“x‘‰Á */
/* 30- */
	SC_DECREASEAGI,		/* ‘¬“xŒ¸­ */
	-1,
	SC_SIGNUMCRUCIS,	/* ƒVƒOƒiƒ€ƒNƒ‹ƒVƒX */
	SC_ANGELUS,			/* ƒGƒ“ƒWƒFƒ‰ƒX */
	SC_BLESSING,		/* ƒuƒŒƒbƒVƒ“ƒO */
	-1,-1,-1,-1,-1,
/* 40- */
	-1,-1,-1,-1,-1,
	SC_CONCENTRATE,		/* W’†—ÍŒüã */
	-1,-1,-1,-1,
/* 50- */
	-1,
	SC_HIDDING,			/* ƒnƒCƒfƒBƒ“ƒO */
	-1,-1,-1,-1,-1,-1,-1,-1,
/* 60- */
	SC_TWOHANDQUICKEN,	/* 2HQ */
	SC_AUTOCOUNTER,
	-1,-1,-1,-1,
	SC_IMPOSITIO,		/* ƒCƒ“ƒ|ƒVƒeƒBƒIƒ}ƒkƒX */
	SC_SUFFRAGIUM,		/* ƒTƒtƒ‰ƒMƒEƒ€ */
	SC_ASPERSIO,		/* ƒAƒXƒyƒ‹ƒVƒI */
	SC_BENEDICTIO,		/* ¹‘Ì~•Ÿ */
/* 70- */
	-1,-1,-1,
	SC_KYRIE,			/* ƒLƒŠƒGƒGƒŒƒCƒ\ƒ“ */
	SC_MAGNIFICAT,		/* ƒ}ƒOƒjƒtƒBƒJ[ƒg */
	SC_GLORIA,			/* ƒOƒƒŠƒA */
	SC_DIVINA,			/* ƒŒƒbƒNƒXƒfƒBƒr[ƒi */
	-1,
	SC_AETERNA,			/* ƒŒƒbƒNƒXƒG[ƒeƒ‹ƒi */
	-1,
/* 80- */
	-1,
	SC_SIGHTTRASHER,
	-1,
	SC_METEOSTORM,
	-1,-1,-1,-1,-1,-1,
/* 90- */
	-1,-1,
	SC_QUAGMIRE,		/* ƒNƒ@ƒOƒ}ƒCƒA */
	-1,-1,-1,-1,-1,-1,-1,
/* 100- */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
/* 110- */
	-1,
	SC_ADRENALINE,		/* ƒAƒhƒŒƒiƒŠƒ“ƒ‰ƒbƒVƒ… */
	SC_WEAPONPERFECTION,/* ƒEƒFƒ|ƒ“ƒp[ƒtƒFƒNƒVƒ‡ƒ“ */
	SC_OVERTHRUST,		/* ƒI[ƒo[ƒgƒ‰ƒXƒg */
	SC_MAXIMIZEPOWER,	/* ƒ}ƒLƒVƒ}ƒCƒYƒpƒ[ */
	-1,-1,-1,-1,-1,
/* 120- */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
/* 130- */
	-1,-1,-1,-1,-1,
	SC_CLOAKING,		/* ƒNƒ[ƒLƒ“ƒO */
	SC_STAN,			/* ƒ\ƒjƒbƒNƒuƒ[ */
	-1,
	SC_ENCPOISON,		/* ƒGƒ“ƒ`ƒƒƒ“ƒgƒ|ƒCƒYƒ“ */
	SC_POISONREACT,		/* ƒ|ƒCƒYƒ“ƒŠƒAƒNƒg */
/* 140- */
	SC_POISON,			/* ƒxƒmƒ€ƒ_ƒXƒg */
	-1,-1,
	SC_TRICKDEAD,		/* €‚ñ‚¾‚Ó‚è */
	-1,-1,-1,-1,-1,-1,
/* 150- */
	-1,-1,-1,-1,-1,
	SC_LOUD,			/* ƒ‰ƒEƒhƒ{ƒCƒX */
	-1,
	SC_ENERGYCOAT,		/* ƒGƒiƒW[ƒR[ƒg */
	-1,-1,
/* 160- */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
/* 210- */
	-1,-1,-1,-1,-1,
	SC_STRIPWEAPON,
	SC_STRIPSHIELD,
	SC_STRIPARMOR,
	SC_STRIPHELM,
	-1,
/* 220- */
	-1,-1,-1,
	SC_GANGSTER,
	-1,-1,-1,-1,-1,-1,
/* 230- */
	-1,-1,
	SC_CANNIBALIZE,
	SC_SPHEREMINE,
	SC_CP_WEAPON,
	SC_CP_SHIELD,
	SC_CP_ARMOR,
	SC_CP_HELM,
	-1,-1,
/* 240- */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,
	SC_AUTOGUARD,
/* 250- */
	-1,-1,
	SC_REFLECTSHIELD,
	-1,-1,
	SC_DEVOTION,
	SC_PROVIDENCE,
	SC_DEFENDER,
	SC_SPEARSQUICKEN,
	-1,
/* 260- */
	-1,-1,-1,-1,-1,-1,-1,-1,
	SC_STEELBODY,
	SC_BLADESTOP,
/* 270- */
	SC_EXPLOSIONSPIRITS,
	-1,-1,-1,-1,
	SC_CASTCANCEL,
	-1,
	SC_SPELLBREAKER,
	SC_FREECAST,
	SC_AUTOSPELL,
/* 280- */
	SC_FLAMELAUNCHER,
	SC_FROSTWEAPON,
	SC_LIGHTNINGLOADER,
	SC_SEISMICWEAPON,
	-1,
	SC_VOLCANO,
	SC_DELUGE,
	SC_VIOLENTGALE,
	SC_LANDPROTECTOR,
	-1,
/* 290- */
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
/* 300- */
	-1,-1,-1,-1,
	SC_ADAPTATION,
	-1,
	SC_LULLABY,
	SC_RICHMANKIM,
	SC_ETERNALCHAOS,
	SC_DRUMBATTLE,
/* 310- */
	SC_NIBELUNGEN,
	SC_ROKISWEIL,
	SC_INTOABYSS,
	SC_SIEGFRIED,
	-1,-1,-1,
	SC_DISSONANCE,
	SC_FROSTJOKE,
	SC_WHISTLE,
/* 320- */
	SC_ASSNCROS,
	SC_POEMBRAGI,
	SC_APPLEIDUN,
	-1,-1,
	SC_UGLYDANCE,
	SC_SCREAM,
	SC_HUMMING,
	SC_DONTFORGETME,
	SC_FORTUNE,
/* 330- */
	SC_SERVICE4U,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

/* ƒXƒLƒ‹ƒf[ƒ^ƒx[ƒX */
struct skill_db skill_db[MAX_SKILL_DB];

/* ƒAƒCƒeƒ€ì¬ƒf[ƒ^ƒx[ƒX */
struct skill_produce_db skill_produce_db[MAX_SKILL_PRODUCE_DB];

/* –îì¬ƒXƒLƒ‹ƒf[ƒ^ƒx[ƒX */
struct skill_arrow_db skill_arrow_db[MAX_SKILL_ARROW_DB];

int skill_get_range( int id ,int lv ){ return skill_db[id].range[lv-1]; }
int	skill_get_hit( int id ){ return skill_db[id].hit; }
int	skill_get_inf( int id ){ return skill_db[id].inf; }
int	skill_get_pl( int id ){ return skill_db[id].pl; }
int	skill_get_nk( int id ){ return skill_db[id].nk; }
int	skill_get_max( int id ){ return skill_db[id].max; }
int	skill_get_sp( int id ,int lv ){ return skill_db[id].sp[lv-1]; }
int	skill_get_num( int id ,int lv ){ return skill_db[id].num[lv-1]; }
int	skill_get_cast( int id ,int lv ){ return skill_db[id].cast[lv-1]; }
int	skill_get_delay( int id ,int lv ){ return skill_db[id].delay[lv-1]; }
int	skill_get_inf2( int id ){ return skill_db[id].inf2; }


/* ƒvƒƒgƒ^ƒCƒv */
struct skill_unit_group *skill_unitsetting( struct block_list *src, int skillid,int skilllv,int x,int y,int flag);

int skill_check_condition( struct map_session_data *sd );

int skill_castend_damage_id( struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag );

static int distance(int x0,int y0,int x1,int y1)
{
	int dx,dy;

	dx=abs(x0-x1);
	dy=abs(y0-y1);
	return dx>dy ? dx : dy;
}

/* ƒXƒLƒ‹ƒ†ƒjƒbƒgID‚ğ•Ô‚·i‚±‚ê‚àƒf[ƒ^ƒx[ƒX‚É“ü‚ê‚½‚¢‚Èj */
int skill_get_unit_id(int id,int flag)
{
	switch(id){
	case MG_SAFETYWALL:		return 0x7e;				/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
	case MG_FIREWALL:		return 0x7f;				/* ƒtƒ@ƒCƒA[ƒEƒH[ƒ‹ */
	case AL_WARP:			return (flag==0)?0x81:0x80;	/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
	case PR_BENEDICTIO:		return 0x82;				/* ¹‘Ì~•Ÿ */
	case PR_SANCTUARY:		return 0x83;				/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
	case PR_MAGNUS:			return 0x84;				/* ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€ */
	case AL_PNEUMA:			return 0x85;				/* ƒjƒ…[ƒ} */
	case WZ_SIGHTRASHER:	return 0x86;	/* ST */
	case WZ_METEOR:			return 0x86;				/* ƒƒeƒIƒXƒg[ƒ€ */
	case WZ_VERMILION:		return 0x86;				/* ƒ[ƒhƒIƒuƒ”ƒ@[ƒ~ƒŠƒIƒ“ */
	case WZ_STORMGUST:		return 0x86;				/* ƒXƒg[ƒ€ƒKƒXƒg(‚Æ‚è‚ ‚¦‚¸LoV‚Æ“¯‚¶‚Åˆ—) */
	case CR_GRANDCROSS:		return 0x86;				/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
	case WZ_FIREPILLAR:		return (flag==0)?0x87:0x88;	/* ƒtƒ@ƒCƒA[ƒsƒ‰[ */
	case HT_TALKIEBOX:		return (flag==0)?0x99:0x8c;	/* ƒg[ƒL[ƒ{ƒbƒNƒX */
	case WZ_ICEWALL:		return 0x8d;				/* ƒAƒCƒXƒEƒH[ƒ‹ */
	case WZ_QUAGMIRE:		return 0x8e;				/* ƒNƒ@ƒOƒ}ƒCƒA */
	case HT_BLASTMINE:		return 0x8f;				/* ƒuƒ‰ƒXƒgƒ}ƒCƒ“ */
	case HT_SKIDTRAP:		return 0x90;				/* ƒXƒLƒbƒhƒgƒ‰ƒbƒv */
	case HT_ANKLESNARE:		return 0x91;				/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
	case AS_VENOMDUST:		return 0x92;				/* ƒxƒmƒ€ƒ_ƒXƒg */
	case HT_LANDMINE:		return 0x93;				/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
	case HT_SHOCKWAVE:		return 0x94;				/* ƒVƒ‡ƒbƒNƒEƒF[ƒuƒgƒ‰ƒbƒv */
	case HT_SANDMAN:		return 0x95;				/* ƒTƒ“ƒhƒ}ƒ“ */
	case HT_FLASHER:		return 0x96;				/* ƒtƒ‰ƒbƒVƒƒ[ */
	case HT_FREEZINGTRAP:	return 0x97;				/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
	case HT_CLAYMORETRAP:	return 0x98;				/* ƒNƒŒƒCƒ‚ƒA[ƒgƒ‰ƒbƒv */
	case AM_DEMONSTRATION:	return 0xb3;				/* Demonstration */
//	case CR_HOLYCROSS:		return 0x86;				/* Test */
	case MO_EXTREMITYFIST:	return 0x86;				/* Practice */
	case SA_VOLCANO:		return 0x9a;				/* Volcano */
	case SA_DELUGE:			return 0x9b;				/* ƒfƒŠƒ…[ƒW */
	case SA_VIOLENTGALE:	return 0x9c;				/* Gale */
	case SA_LANDPROTECTOR:	return 0x9d;				/* LAND */
	case BD_LULLABY:		return 0x9e;				/* qç‰Ì */
	case BD_RICHMANKIM:		return 0x9f;				/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
	case BD_ETERNALCHAOS:	return 0xa0;				/* ‰i‰“‚Ì¬“× */
	case BD_DRUMBATTLEFIELD:return 0xa1;				/* í‘¾ŒÛ‚Ì‹¿‚« */
	case BD_RINGNIBELUNGEN:	return 0xa2;				/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case BD_ROKISWEIL:		return 0xa3;				/* ƒƒL‚Ì‹©‚Ñ */
	case BD_INTOABYSS:		return 0xa4;				/* [•£‚Ì’†‚É */
	case BD_SIEGFRIED:		return 0xa5;				/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
	case BA_DISSONANCE:		return 0xa6;				/* •s‹¦˜a‰¹ */
	case BA_WHISTLE:		return 0xa7;				/* Œû“J */
	case BA_ASSASSINCROSS:	return 0xa8;				/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
	case BA_POEMBRAGI:		return 0xa9;				/* ƒuƒ‰ƒM‚Ì */
	case BA_APPLEIDUN:		return 0xaa;				/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case DC_UGLYDANCE:		return 0xab;				/* ©•ªŸè‚Èƒ_ƒ“ƒX */
	case DC_HUMMING:		return 0xac;				/* ƒnƒ~ƒ“ƒO */
	case DC_DONTFORGETME:	return 0xad;				/* „‚ğ–Y‚ê‚È‚¢‚Åc */
	case DC_FORTUNEKISS:	return 0xae;				/* K‰^‚ÌƒLƒX */
	case DC_SERVICEFORYOU:	return 0xaf;				/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
	case RG_GRAFFITI:		return 0xb0;				/* ƒOƒ‰ƒtƒBƒeƒB */
	case 336:	return (flag==0)?0x81:0xb2;
	}
	return 0;
	/*
	0x89,0x8a,0x8b •\¦–³‚µ
	0x9a ‰Š‘®«‚Ì‰r¥‚İ‚½‚¢‚ÈƒGƒtƒFƒNƒg
	0x9b …‘®«‚Ì‰r¥‚İ‚½‚¢‚ÈƒGƒtƒFƒNƒg
	0x9c •—‘®«‚Ì‰r¥‚İ‚½‚¢‚ÈƒGƒtƒFƒNƒg
	0x9d ”’‚¢¬‚³‚ÈƒGƒtƒFƒNƒg
	0xb1 Alchemist Demonstration
	0xb2 = Pink Warp Portal
	*/
}

/*==========================================
 * ƒXƒLƒ‹’Ç‰ÁŒø‰Ê
 *------------------------------------------
 */
int skill_additional_effect( struct block_list* src, struct block_list *bl,int skillid,int skilllv,int attack_type,unsigned int tick)
{
	struct map_session_data *sd=NULL;
	struct map_session_data *sd2=NULL;
	int skill=0,skill2;
	int rate;
	
	if(src->type==BL_PC)
		sd=(struct map_session_data *)src;
	if(bl->type==BL_PC)
		sd2=(struct map_session_data *)bl;

	switch(skillid){
	case 0:					/* ’ÊíUŒ‚ */
		/* ©“®‘é */
		if( sd && pc_isfalcon(sd) && sd->status.weapon == 11 && (skill=pc_checkskill(sd,HT_BLITZBEAT))>0 &&
			rand()%1000 <= sd->paramc[5]*10/3+1 ) {
			int lv=(sd->status.job_level+9)/10;
			skill_castend_damage_id(src,bl,HT_BLITZBEAT,(skill<lv)?skill:lv,tick,0xf00000);
		}
		// ƒXƒiƒbƒ`ƒƒ[
		if(sd && sd->status.weapon != 11 && (skill=pc_checkskill(sd,RG_SNATCHER)) > 0)
			if((skill*15 + 55) + (skill2 = pc_checkskill(sd,TF_STEAL))*10 > rand()%1000)
				if(pc_steal_item(sd,bl))
					clif_skill_nodamage(src,bl,TF_STEAL,skill2,1);

		if (sd && sd->sc_data[SC_AUTOSPELL].timer != -1) {				// Added by RoVeRT
			if ((tick - sd->autospell_tick) >= 3000 && sd->sc_data[SC_AUTOSPELL].val1 < rand()%100) {
				int skilllv=1,max=3,limit=-1,sp;
				int lv = pc_checkskill(sd,SA_AUTOSPELL);

				int levels[]={1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3};
				struct { int skill,max; } skills[] = {
					{	MG_NAPALMBEAT,	3	},
					{	MG_COLDBOLT,	3	},
					{	MG_FIREBOLT,	3	},
					{	MG_LIGHTNINGBOLT,	3	},
					{	MG_SOULSTRIKE,	3	},
					{	MG_FIREBALL,	2	},
					{	MG_FROSTDIVER,	1	},
				};

				switch(lv) {
					case NV_BASIC:
						skill = 0;
						break;
					case SM_SWORD:
					case SM_TWOHAND:
					case SM_RECOVERY:
						skill = rand()%4;
						max = lv-1;
						limit = 0;
						break;
					case SM_BASH:
					case SM_PROVOKE:
					case SM_MAGNUM:
						skill = rand()%5;
						max = lv-4;
						limit = 3;
						break;		
					case SM_ENDURE:
					case MG_SRECOVERY:
						skill = rand()%6;
						max = lv-7;
						limit = 5;
						break;
					case MG_SIGHT:
						skill = rand()%7;
						max = 1;
						limit = 6;
					break;
				}

				do{ skilllv=levels[rand()%21]; } while(skilllv>skills[skill].max || (skill>limit && skilllv>max));

				sp=skill_get_sp(skills[skill].skill, skilllv) * 2/3;
				if(sd->dsprate!=100)
					sp=sp*sd->dsprate/100;

				if (sd->status.sp >= sp && pc_checkskill(sd,skills[skill].skill) >= skilllv) {
					sd->status.sp-=sp;
					clif_updatestatus(sd,SP_SP);

					sd->autospell_tick = tick;

					skill_castend_damage_id(src,bl,skills[skill].skill,skilllv,tick,0xf00000);
				}
			}
		}
		break;

//		case RG_SNATCHER:	//ƒXƒiƒbƒ`ƒƒ[
//			if( sd->status.weapon != 0 && sd->status.weapon != 1 && sd->status.weapon != 2 && sd->status.weapon != 6 ) {
//				clif_skill_fail(sd,sd->skillid,6,0);	//‘fèA’ZŒ•A•ĞèŒ•A•Ğè•€
//				return 0;
//			}
//			break;

	case SM_BASH:			/* ƒoƒbƒVƒ…i‹}ŠUŒ‚j */
		if( sd && (skill=pc_checkskill(sd,SM_FATALBLOW))>0 ){
			if( rand()%100 < 6*(skilllv-5) )
				skill_status_change_start(bl,SC_STAN,skilllv,5000);
		}
		break;

	case PR_STRECOVERY:		// Added by AppleGirl
		if( battle_get_race(bl)==1 && battle_get_race(bl)==6 )
			skill_status_change_start(bl,SC_BLIND,1,0);
		break;

	case TF_POISON:			/* ƒCƒ“ƒxƒiƒ€ */
		if(battle_get_elem_type(bl)!=9 && rand()%100< 2*skilllv+10 )
			skill_status_change_start(bl,SC_POISON,skilllv,0);
		break;

	case AS_SONICBLOW:		/* ƒ\ƒjƒbƒNƒuƒ[ */
		if( rand()%100 < 2*skilllv+10 )
			skill_status_change_start(bl,SC_STAN,skilllv,5000);
		break;

	case MG_FROSTDIVER:		/* ƒtƒƒXƒgƒ_ƒCƒo[ */
	case WZ_FROSTNOVA:		/* ƒtƒƒXƒgƒmƒ”ƒ@ */
	case WZ_STORMGUST:		/* ƒXƒg[ƒ€ƒKƒXƒg */
	case HT_FREEZINGTRAP:	/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
//	case BA_FROSTJOKE:		/* Š¦‚¢ƒWƒ‡[ƒN */
/*
		rate=battle_get_lv(src)/2 +battle_get_int(src)/3+ skilllv*2 +15 -battle_get_mdef(bl);
		if(rate>95)rate=95;
*/
		rate=skilllv*3+35;
		if( rand()%100 < rate )
			skill_status_change_start(bl,SC_FREEZE,skilllv,0);
		break;

	case HT_LANDMINE:		/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
		if( rand()%100 < 5*skilllv+30 ){
			skill_status_change_start(bl,SC_STAN,skilllv,skilllv*500+1000);
		}
		break;

	case HT_ANKLESNARE:		/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
		{
			int sec=(battle_get_mode(bl)&0x20)?1000:5000;
			battle_stopwalking(bl,1);
			skill_status_change_start(bl,SC_ANKLE,skilllv,skilllv*sec);
		}
		break;

	case HT_SANDMAN:		/* ƒTƒ“ƒhƒ}ƒ“ */
		if( rand()%100 < 5*skilllv+30 ){
			skill_status_change_start(bl,SC_SLEEP,skilllv,30000);
		}
		break;

	case TF_SPRINKLESAND:	/* »‚Ü‚« */
		if( rand()%100 < 15 )
			skill_status_change_start(bl,SC_BLIND,1,0);
		break;

	case TF_THROWSTONE:		/* Î“Š‚° */
		if( rand()%100 < 5 )
			skill_status_change_start(bl,SC_STAN,1,3000);
		break;

	case CR_AUTOGUARD:		/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */	// Added by AppleGirl
		if( rand()%100 < 3*skilllv ){
			skill_status_change_start(bl,SC_AUTOGUARD,skilllv,skilllv*500+1000);
		}
		break;

	case CR_REFLECTSHIELD:		/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */	// Added by AppleGirl
		if( rand()%100 < 3*skilllv ){
			skill_status_change_start(bl,SC_REFLECTSHIELD,skilllv,skilllv*500+1000);
		}
		break;

	case CR_HOLYCROSS:		/* ƒz[ƒŠ[ƒNƒƒX */
		if( rand()%100 < 3*skilllv )
			skill_status_change_start(bl,SC_BLIND,1,0);
		break;

	case CR_GRANDCROSS:		/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
		if( battle_get_elem_type(bl) == 9 || battle_get_race(bl) == 6)
			skill_status_change_start(bl,SC_BLIND,1,0);
		break;

	case RG_RAID:		/* ƒTƒvƒ‰ƒCƒYƒAƒ^ƒbƒN */
		if( rand()%100 < 10+3*skilllv )
			skill_status_change_start(bl,SC_STAN,1,3000);
		if( rand()%100 < 10+3*skilllv )
			skill_status_change_start(bl,SC_BLIND,1,0);
		break;

#if 0
	case BA_FROSTJOKE:		/* Š¦‚¢ƒWƒ‡[ƒN */
		if( rand()%100 < 15+5*skilllv )
			skill_status_change_start(bl,SC_FREEZE,1,0);
		break;
#endif

	case BD_LULLABY:	/* qç‰S */
		if( rand()%100 < 15 )
			skill_status_change_start(bl,SC_SLEEP,skilllv,30000);
		break;


	/* MOB‚Ì’Ç‰ÁŒø‰Ê•t‚«ƒXƒLƒ‹ */
	case NPC_POISON:
	case NPC_BLINDATTACK:
	case NPC_SILENCEATTACK:
	case NPC_STUNATTACK:
	case NPC_PETRIFYATTACK:
	case NPC_CURSEATTACK:
	case NPC_SLEEPATTACK:
		{
			const int sc[]={
				SC_POISON, SC_BLIND, SC_SILENCE, SC_STAN,
				SC_STONE, SC_CURSE, SC_SLEEP };
			const int sc2[]={
				6000, 6000, 6000, 1000,
				1000, 0, 6000 };
			skill_status_change_start(bl,
				sc[skillid-NPC_POISON],skilllv,sc2[skillid-NPC_POISON]*skilllv);
		}
		break;
	}

	if(sd && skillid != MC_CARTREVOLUTION && attack_type&BF_WEAPON){	/* ƒJ[ƒh‚É‚æ‚é’Ç‰ÁŒø‰Ê */
		int i;
		for(i=SC_STONE;i<=SC_BLIND;i++){
			if(!sd->state.arrow_atk) {
				if(rand()%100 < sd->addeff[i-SC_STONE] ){
					printf("PC %d skill_addeff: card‚É‚æ‚éˆÙí”­“® %d %d\n",sd->bl.id,i,sd->addeff[i-SC_STONE]);
					skill_status_change_start(bl,i,1,5);
				}
			}
			else {
				if(rand()%100 < sd->addeff[i-SC_STONE]+sd->arrow_addeff[i-SC_STONE] ){
					printf("PC %d skill_addeff: card‚É‚æ‚éˆÙí”­“® %d %d\n",sd->bl.id,i,sd->addeff[i-SC_STONE]);
					skill_status_change_start(bl,i,1,5);
				}
			}
		}
	}

	return 0;
}

/*=========================================================================
 ƒXƒLƒ‹UŒ‚‚«”ò‚Î‚µˆ—
-------------------------------------------------------------------------*/
int skill_blown( struct block_list *src, struct block_list *target,int count)
{
	static const int dirx[8]={0,-1,-1,-1,0,1,1,1};
	static const int diry[8]={1,1,0,-1,-1,-1,0,1};
	int dx=0,dy=0,nx,ny;
	int x=target->x,y=target->y;
	int ret;
	int moveblock;
	struct map_session_data *sd=NULL;
	struct mob_data *md=NULL;
	
	if (target->type==BL_PC)
		sd=(struct map_session_data *)target;
	else if (target->type==BL_MOB)
		md=(struct mob_data *)target;

	if(!(count&0x10000 && (sd||md))){	/* w’è‚È‚µ‚È‚çˆÊ’uŠÖŒW‚©‚ç•ûŒü‚ğ‹‚ß‚é */
		dx=target->x-src->x; dx=(dx>0)?1:((dx<0)?-1: 0);
		dy=target->y-src->y; dy=(dy>0)?1:((dy<0)?-1: 0);
	}
	if(dx==0 && dy==0){
		int dir=(sd)?sd->dir:md->dir;
		if(dir>=0 && dir<8){
			dx=-dirx[dir];
			dy=-diry[dir];
		}
	}

	ret=path_blownpos(target->m,x,y,dx,dy,count&0xffff);
	nx=ret>>16;
	ny=ret&0xffff;
	moveblock=( x/BLOCK_SIZE != nx/BLOCK_SIZE || y/BLOCK_SIZE != ny/BLOCK_SIZE);

	if(count&0x20000) {
		if(sd){
			sd->to_x=nx;
			sd->to_y=ny;
			if(sd->walktimer != -1)
				pc_stop_walking(sd,1);
			sd->walktimer = 1;
			clif_walkok(sd);
			clif_movechar(sd);
		}
		else if(md) {
			md->to_x=nx;
			md->to_y=ny;
			if(md->state.state == MS_WALK)
				mob_stop_walking(md,1);
			md->state.state = MS_WALK;
			clif_fixmobpos(md);
		}
	}

	dx = nx - x;
	dy = ny - y;

	if(sd)	/* ‰æ–ÊŠO‚Éo‚½‚Ì‚ÅÁ‹ */
		map_foreachinmovearea(clif_pcoutsight,
			target->m,x-AREA_SIZE,y-AREA_SIZE,
			x+AREA_SIZE,y+AREA_SIZE,dx,dy,0,sd);
	if(md)
		map_foreachinmovearea(clif_moboutsight,
			target->m,x-AREA_SIZE,y-AREA_SIZE,
			x+AREA_SIZE,y+AREA_SIZE,dx,dy,BL_PC,md);

	if(moveblock) map_delblock(target);
	target->x=nx;
	target->y=ny;
	if(moveblock) map_addblock(target);


	skill_unit_move(target,gettick(),(count&0xffff)+5);	/* ƒXƒLƒ‹ƒ†ƒjƒbƒg‚Ì”»’è */

	if(sd) {	/* ‰æ–Ê“à‚É“ü‚Á‚Ä‚«‚½‚Ì‚Å•\¦ */
		map_foreachinmovearea(clif_pcinsight,
			target->m,nx-AREA_SIZE,ny-AREA_SIZE,
			nx+AREA_SIZE,ny+AREA_SIZE,-dx,-dy,0,sd);
		if(count&0x20000)
			sd->walktimer = -1;
	}
	if(md) {
		map_foreachinmovearea(clif_mobinsight,
			target->m,nx-AREA_SIZE,ny-AREA_SIZE,
			nx+AREA_SIZE,ny+AREA_SIZE,-dx,-dy,BL_PC,md);
		if(count&0x20000)
			md->state.state = MS_IDLE;
	}

	return 0;
}


/*
 * =========================================================================
 * ƒXƒLƒ‹UŒ‚Œø‰Êˆ—‚Ü‚Æ‚ß
 * flag‚Ìà–¾B16i}
 * 	00XRTTff
 *  ff	= magic‚ÅŒvZ‚É“n‚³‚ê‚éj
 *	TT	= ƒpƒPƒbƒg‚Ìtype•”•ª(0‚ÅƒfƒtƒHƒ‹ƒgj
 *  X   = ƒpƒPƒbƒg‚ÌƒXƒLƒ‹Lv
 *  R	= —\–ñiskill_area_sub‚Åg—p‚·‚éj
 *-------------------------------------------------------------------------
 */
int skill_attack( int attack_type, struct block_list* src, struct block_list *dsrc,
	 struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag )
{
	struct Damage dmg;
	int type,lv,damage;

	if(src == NULL || dsrc == NULL || bl == NULL)
		return 0;
	if(src->prev == NULL || dsrc->prev == NULL || bl->prev == NULL)
		return 0;
	if(src->type == BL_PC && pc_isdead((struct map_session_data *)src))
		return 0;
	if(dsrc->type == BL_PC && pc_isdead((struct map_session_data *)dsrc))
		return 0;
	if(bl->type == BL_PC && pc_isdead((struct map_session_data *)bl))
		return 0;

	type=-1;
	lv=(flag>>20)&0xf;
	dmg=battle_calc_attack(attack_type, src,bl, skillid,skilllv, flag&0xff );

	damage = dmg.damage + dmg.damage2;

	if(lv==15)lv=-1;

	if( flag&0xff00 )
		type=(flag&0xff00)>>8;

	if(damage <= 0 || damage < dmg.div_)
		dmg.blewcount = 0;

	if(skillid == CR_GRANDCROSS && src == bl)
		dsrc = src;

	if( dmg.blewcount ){	/* ‚«”ò‚Î‚µˆ—‚Æ‚»‚ÌƒpƒPƒbƒg */
		clif_skill_damage2(dsrc,bl,tick,dmg.amotion,dmg.dmotion,
			damage, dmg.div_, skillid, (lv!=0)?lv:skilllv, type );
		skill_blown(dsrc,bl,dmg.blewcount);
	} else			/* ƒXƒLƒ‹‚Ìƒ_ƒ[ƒWƒpƒPƒbƒg */
		clif_skill_damage(dsrc,bl,tick,dmg.amotion,dmg.dmotion,
			damage, dmg.div_, skillid, (lv!=0)?lv:skilllv, type );

//FIX change Ugly Dance to sp from hp somewhere
	map_freeblock_lock();
	/* ÀÛ‚Éƒ_ƒ[ƒWˆ—‚ğs‚¤ */
	battle_damage(src,bl,damage);
	/* ƒ_ƒ[ƒW‚ª‚ ‚é‚È‚ç’Ç‰ÁŒø‰Ê”»’è */
	if(damage > 0)
		skill_additional_effect(src,bl,skillid,skilllv,attack_type,tick);

	if(bl->type==BL_MOB && src!=bl)	/* ƒXƒLƒ‹g—pğŒ‚ÌMOBƒXƒLƒ‹ */
		mobskill_use((struct mob_data *)bl,tick,MSC_SKILLUSED|(skillid<<16));

	map_freeblock_unlock();

	return (dmg.damage+dmg.damage2);	/* —^ƒ_ƒ‚ğ•Ô‚· */
}


/*==========================================
 * ƒXƒLƒ‹”ÍˆÍUŒ‚—p(map_foreachinarea‚©‚çŒÄ‚Î‚ê‚é)
 * flag‚É‚Â‚¢‚ÄF16i}‚ğŠm”F
 * MSB <- 00fTffff ->LSB
 *	T	=ƒ^[ƒQƒbƒg‘I‘ğ—p(BCT_*)
 *  ffff=©—R‚Ég—p‰Â”\
 *  0	=—\–ñB0‚ÉŒÅ’è
 *------------------------------------------
 */
static int skill_area_temp[8];	/* ˆê•Ï”B•K—v‚È‚çg‚¤B */
typedef int (*SkillFunc)(struct block_list *,struct block_list *,int,int,unsigned int,int);
int skill_area_sub( struct block_list *bl,va_list ap )
{
	struct block_list *src;
	int skill_id,skill_lv,flag;
	unsigned int tick;
	SkillFunc func;
	
	if(bl->type!=BL_PC && bl->type!=BL_MOB)
		return 0;

	src=va_arg(ap,struct block_list *);
	skill_id=va_arg(ap,int);
	skill_lv=va_arg(ap,int);
	tick=va_arg(ap,unsigned int);
	flag=va_arg(ap,int);
	func=va_arg(ap,SkillFunc);

	if(battle_check_target(src,bl,flag) > 0)
		func(src,bl,skill_id,skill_lv,tick,flag);
	return 0;
}
/*=========================================================================
 * ”ÍˆÍƒXƒLƒ‹g—pˆ—¬•ª‚¯‚±‚±‚©‚ç
 */
/* ‘ÎÛ‚Ì”‚ğƒJƒEƒ“ƒg‚·‚éBiskill_area_temp[0]‚ğ‰Šú‰»‚µ‚Ä‚¨‚­‚±‚Æj */
int skill_area_sub_count(struct block_list *src,struct block_list *target,int skillid,int skilllv,unsigned int tick,int flag)
{
	if(skill_area_temp[0] < 0xff)
		skill_area_temp[0]++;
	return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
static int skill_timerskill(int tid, unsigned int tick, int id,int data )
{
	struct map_session_data *sd = NULL;
	struct mob_data *md = NULL;
	struct block_list *src,*target;
	struct skill_timerskill *skl = NULL;
	
	src = map_id2bl(id);
	if(src == NULL || src->prev == NULL)
		return 0;

	if(src->type == BL_PC) {
		sd = (struct map_session_data *)src;
		skl = &sd->skilltimerskill[data];
	}
	else if(src->type == BL_MOB) {
		md = (struct mob_data *)src;
		skl = &md->skilltimerskill[data];
	}
	else
		return 0;

	skl->timer = -1;
	if(skl->target_id) {
		target = map_id2bl(skl->target_id);
		if(target == NULL || target->prev ==NULL)
			return 0;
		if(src->m != target->m)
			return 0;
		if(sd && pc_isdead(sd))
			return 0;
		if(target->type == BL_PC && pc_isdead((struct map_session_data *)target))
			return 0;

		switch(skl->skill_id) {
			case TF_BACKSLIDING:	
				clif_skill_nodamage(src,src,skl->skill_id,skl->skill_lv,1);
				break;
			default:
				skill_attack(skl->type,src,src,target,skl->skill_id,skl->skill_lv,tick,skl->flag);
				break;
		}
	}
	else {
		if(src->m != skl->map)
			return 0;
		switch(skl->skill_id) {
			case WZ_METEOR:
				if(skl->type >= 0) {
					skill_unitsetting(src,skl->skill_id,skl->skill_lv,skl->type>>16,skl->type&0xFFFF,0);
					clif_skill_poseffect(src,skl->skill_id,skl->skill_lv,skl->x,skl->y,tick);
				}
				else
					skill_unitsetting(src,skl->skill_id,skl->skill_lv,skl->x,skl->y,0);
				break;
		}
	}

	return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int skill_addtimerskill(struct block_list *src,unsigned int tick,int target,int x,int y,int skill_id,int skill_lv,int type,int flag)
{
	int i;

	if(src->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)src;
		for(i=0;i<MAX_SKILLTIMERSKILL;i++) {
			if(sd->skilltimerskill[i].timer == -1) {
				sd->skilltimerskill[i].timer = add_timer(tick, skill_timerskill, src->id, i);
				sd->skilltimerskill[i].src_id = src->id;
				sd->skilltimerskill[i].target_id = target;
				sd->skilltimerskill[i].skill_id = skill_id;
				sd->skilltimerskill[i].skill_lv = skill_lv;
				sd->skilltimerskill[i].map = src->m;
				sd->skilltimerskill[i].x = x;
				sd->skilltimerskill[i].y = y;
				sd->skilltimerskill[i].type = type;
				sd->skilltimerskill[i].flag = flag;

				return 0;
			}
		}
		return 1;
	}
	else if(src->type == BL_MOB) {
		struct mob_data *md = (struct mob_data *)src;
		for(i=0;i<MAX_SKILLTIMERSKILL/2;i++) {
			if(md->skilltimerskill[i].timer == -1) {
				md->skilltimerskill[i].timer = add_timer(tick, skill_timerskill, src->id, i);
				md->skilltimerskill[i].src_id = src->id;
				md->skilltimerskill[i].target_id = target;
				md->skilltimerskill[i].skill_id = skill_id;
				md->skilltimerskill[i].skill_lv = skill_lv;
				md->skilltimerskill[i].map = src->m;
				md->skilltimerskill[i].x = x;
				md->skilltimerskill[i].y = y;
				md->skilltimerskill[i].type = type;
				md->skilltimerskill[i].flag = flag;

				return 0;
			}
		}
		return 1;
	}

	return 1;
}

/*==========================================
 *
 *------------------------------------------
 */
int skill_cleartimerskill(struct block_list *src)
{
	int i;

	if(src->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)src;
		for(i=0;i<MAX_SKILLTIMERSKILL;i++) {
			if(sd->skilltimerskill[i].timer != -1) {
				delete_timer(sd->skilltimerskill[i].timer, skill_timerskill);
				sd->skilltimerskill[i].timer = -1;
			}
		}
	}
	else if(src->type == BL_MOB) {
		struct mob_data *md = (struct mob_data *)src;
		for(i=0;i<MAX_SKILLTIMERSKILL/2;i++) {
			if(md->skilltimerskill[i].timer != -1) {
				delete_timer(md->skilltimerskill[i].timer, skill_timerskill);
				md->skilltimerskill[i].timer = -1;
			}
		}
	}

	return 0;
}

/* ”ÍˆÍƒXƒLƒ‹g—pˆ—¬•ª‚¯‚±‚±‚Ü‚Å
 * -------------------------------------------------------------------------
 */


/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹AIDw’èUŒ‚Œnj
 * iƒXƒpƒQƒbƒeƒB‚ÉŒü‚¯‚Ä‚P•à‘OiI(ƒ_ƒƒ|)j
 *------------------------------------------
 */
int skill_castend_damage_id( struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag )
{
	struct map_session_data *sd=NULL;
	int i;

	if(src->type==BL_PC)
		sd=(struct map_session_data *)src;
	if(sd && pc_isdead(sd))
		return 0;

	if(bl == NULL || bl->prev == NULL)
		return 0;
	if(bl->type == BL_PC && pc_isdead((struct map_session_data *)bl))
		return 0;

	switch(skillid)
	{
	/* •ŠíUŒ‚ŒnƒXƒLƒ‹ */
	case SM_BASH:			/* ƒoƒbƒVƒ… */
	case MC_MAMMONITE:		/* ƒƒ}[ƒiƒCƒg */
	case AC_DOUBLE:			/* ƒ_ƒuƒ‹ƒXƒgƒŒƒCƒtƒBƒ“ƒO */
	case AS_SONICBLOW:		/* ƒ\ƒjƒbƒNƒuƒ[ */
	case KN_PIERCE:			/* ƒsƒA[ƒX */
//	case KN_BRANDISHSPEAR:
	case KN_SPEARBOOMERANG:	/* ƒXƒsƒAƒu[ƒƒ‰ƒ“ */
	case TF_POISON:			/* ƒCƒ“ƒxƒiƒ€ */
//	case AS_GRIMTOOTH:
	case TF_SPRINKLESAND:	/* »‚Ü‚« */
	case AC_CHARGEARROW:	/* ƒ`ƒƒ[ƒWƒAƒ[ */
	case KN_SPEARSTAB:		/* ƒXƒsƒAƒXƒ^ƒu */
//	case RG_RAID:		/* ƒTƒvƒ‰ƒCƒYƒAƒ^ƒbƒN */
	case RG_INTIMIDATE:		/* ƒCƒ“ƒeƒBƒ~ƒfƒCƒg */
	/* Shield Test */
	case CR_SHIELDCHARGE:
	case CR_SHIELDBOOMERANG:
	/* Testing */
	case BA_MUSICALSTRIKE:	/* ƒ~ƒ…[ƒWƒJƒ‹ƒXƒgƒ‰ƒCƒN */
	case DC_THROWARROW:		/* –îŒ‚‚¿ */
	case BA_DISSONANCE:		/* •s‹¦˜a‰¹ */
	case MO_INVESTIGATE:	/* ”­™¤ */
	case CR_HOLYCROSS:		/* ƒz[ƒŠ[ƒNƒƒX */
	/* ˆÈ‰ºMOBê—p */
	/* ’P‘ÌUŒ‚ASPŒ¸­UŒ‚A‰“‹——£UŒ‚A–hŒä–³‹UŒ‚A‘½’iUŒ‚ */
	case NPC_PIERCINGATT:
	case NPC_MENTALBREAKER:
	case NPC_RANGEATTACK:
	case NPC_CRITICALSLASH:
	case NPC_COMBOATTACK:
	/* •K’†UŒ‚A“ÅUŒ‚AˆÃ•UŒ‚A’¾–ÙUŒ‚AƒXƒ^ƒ“UŒ‚ */
	case NPC_GUIDEDATTACK:
	case NPC_POISON:
	case NPC_BLINDATTACK:
	case NPC_SILENCEATTACK:
	case NPC_STUNATTACK:
	/* Î‰»UŒ‚Aô‚¢UŒ‚A‡–°UŒ‚Aƒ‰ƒ“ƒ_ƒ€ATKUŒ‚ */
	case NPC_PETRIFYATTACK:
	case NPC_CURSEATTACK:
	case NPC_SLEEPATTACK:
	case NPC_RANDOMATTACK:
	/* …‘®«UŒ‚A’n‘®«UŒ‚A‰Î‘®«UŒ‚A•—‘®«UŒ‚ */
	case NPC_WATERATTACK:
	case NPC_GROUNDATTACK:
	case NPC_FIREATTACK:
	case NPC_WINDATTACK:
	/* “Å‘®«UŒ‚A¹‘®«UŒ‚AˆÅ‘®«UŒ‚A”O‘®«UŒ‚ASPŒ¸­UŒ‚ */
	case NPC_POISONATTACK:
	case NPC_HOLYATTACK:
	case NPC_DARKNESSATTACK:
	case NPC_TELEKINESISATTACK:
	case NPC_LICK:
		if(skillid==CR_SHIELDCHARGE && rand()%100 < 5*skilllv+15 )
			skill_status_change_start(bl,SC_STAN,1,3000);
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		break;
	case RG_BACKSTAP:		/* ƒoƒbƒNƒXƒ^ƒu */
		{
			struct status_change *sc_data = battle_get_sc_data(src);
			if(sc_data && sc_data[SC_HIDDING].timer != -1)
				skill_status_change_end(src, SC_HIDDING, -1);	// ƒnƒCƒfƒBƒ“ƒO‰ğœ
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		}
		break;
	case MO_FINGEROFFENSIVE:	/* w’e */
		if(!battle_config.finger_offensive_type)
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		else {
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			if(sd) {
				for(i=1;i<sd->spiritball_old;i++)
					skill_addtimerskill(src,tick+i*200,bl->id,0,0,skillid,skilllv,BF_WEAPON,flag);
				sd->canmove_tick = tick + (sd->spiritball_old-1)*200;
			}
		}
		break;
	case MO_CHAINCOMBO:		/* ˜A‘Å¶ */
		sd->combo_delay1 = 1000 - 4 * battle_get_agi(src) - 2 *  battle_get_dex(src);
		if (sd->combo_delay1 < sd->aspd*2)
			sd->combo_delay1 = sd->aspd*2;
		sd->combo_flag = 1;
		if (pc_checkskill(sd, MO_COMBOFINISH))
			sd->combo_delay1 += 300;
		clif_status_change(src, 0x59, 1);
		sd->canmove_tick = tick + sd->combo_delay1;
		clif_combo_delay(src, sd->combo_delay1);
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		sd->combo_delay2 = tick + sd->combo_delay1 - 300;
		break;
	case MO_COMBOFINISH:	/* –Ò—´Œ */
		sd->combo_delay2 = 700 - 4 * battle_get_agi(src) - 2 *  battle_get_dex(src);
		if (sd->combo_delay2 < sd->aspd*2)
			sd->combo_delay2 = sd->aspd*2;
		sd->combo_flag = 3;
		if (pc_checkskill(sd, MO_EXTREMITYFIST))
			sd->combo_delay2 += 300;
		clif_status_change(src, 0x59, 1);
		sd->canmove_tick = tick + sd->combo_delay2;
		clif_combo_delay(src, sd->combo_delay2);
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		sd->combo_delay3 = tick + sd->combo_delay2 - 300;
		break;
	case MO_EXTREMITYFIST:	/* ˆ¢C—…”e–PŒ */
	{
		int dx,dy;
		skill_status_change_end(src, SC_EXPLOSIONSPIRITS, -1);
		dx = ((sd->bl.x - bl->x)>0?-4:4);
		dy = ((sd->bl.y - bl->y)>0?-4:4);
		dx = ((sd->bl.x - bl->x)!=0?dx:0);
		dy = ((sd->bl.y - bl->y)!=0?dy:0);
		sd->bl.x = bl->x;
		sd->bl.y = bl->y;
		while(!pc_can_reach(sd, sd->bl.x + dx, sd->bl.y + dy)) {
			dx = (int)((float)dx / 4 * 3);
			dy = (int)((float)dy / 4 * 3);
			if(dx == 0 && dy == 0) break;
		}
		sd->to_x = sd->bl.x + dx;
		sd->to_y = sd->bl.y + dy;
		sd->canact_tick = tick + skill_get_delay(skillid, skilllv);
		sd->canmove_tick = tick + skill_get_delay(skillid, skilllv);
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		pc_walktoxy(sd, sd->to_x, sd->to_y);
		break;
	}
	/* •ŠíŒn”ÍˆÍUŒ‚ƒXƒLƒ‹ */
	case AC_SHOWER:			/* ƒAƒ[ƒVƒƒƒ[ */
	case SM_MAGNUM:			/* ƒ}ƒOƒiƒ€ƒuƒŒƒCƒN */
	case KN_BOWLINGBASH:	/* ƒ{ƒEƒŠƒ“ƒOƒoƒbƒVƒ… */
	case KN_BRANDISHSPEAR:	/*ƒuƒ‰ƒ“ƒfƒBƒbƒVƒ…ƒXƒsƒA*/
	case MC_CARTREVOLUTION:	/* ƒJ[ƒgƒŒƒ”ƒHƒŠƒ…[ƒVƒ‡ƒ“ */
	case AS_SPLASHER:
	case AS_GRIMTOOTH:		/* ƒOƒŠƒ€ƒgƒD[ƒX */
	case RG_RAID:
	case NPC_SPLASHATTACK:	/* ƒXƒvƒ‰ƒbƒVƒ…ƒAƒ^ƒbƒN */
		if(flag&3){
			/* ŒÂ•Ê‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é */
			if(bl->id!=skill_area_temp[1]){
				int dist=0;
				if(skillid==SM_MAGNUM || skillid==KN_BRANDISHSPEAR){	/* ƒ}ƒOƒiƒ€ƒuƒŒƒCƒN‚È‚ç’†S‚©‚ç‚Ì‹——£‚ğŒvZ */
					int dx=abs( bl->x - skill_area_temp[2] );
					int dy=abs( bl->y - skill_area_temp[3] );
					dist=((dx>dy)?dx:dy);
				}
				skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,
					0x0500|dist  );
			}
		}else{
			int ar=1;
			int x=bl->x,y=bl->y;
			if( skillid==SM_MAGNUM || skillid==KN_BRANDISHSPEAR){
				x=src->x;
				y=src->y;
			}else if(skillid==AC_SHOWER || skillid==AS_GRIMTOOTH || skillid==AS_SPLASHER)	/* ƒAƒ[ƒVƒƒƒ[”ÍˆÍ */
				ar=2;
			else if(skillid==NPC_SPLASHATTACK || skillid==RG_RAID)	/* ƒXƒvƒ‰ƒbƒVƒ…ƒAƒ^ƒbƒN‚Í”ÍˆÍ7*7 */
				ar=3;
			else if(skillid==KN_BOWLINGBASH){/*ƒ{ƒEƒŠƒ“ƒOƒoƒbƒVƒ…‚ğ‰¼À‘•‚µ‚Ä‚İ‚éi‚«”ò‚Î‚µ‚Í‚±‚±‚Å‚â‚éj */
				int i;	/* ‘¼l‚©‚ç•·‚¢‚½“®‚«‚È‚Ì‚ÅŠÔˆá‚Á‚Ä‚é‰Â”\«‘å•Œø—¦‚ªˆ«‚¢‚Á‚·„ƒ */
				for(i=0;i<4;i++){
					skill_blown(src,bl,1|0x20000);
					skill_area_temp[0]=0;
					map_foreachinarea(skill_area_sub,
						bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
						src,skillid,skilllv,tick, flag|BCT_ENEMY ,
						skill_area_sub_count);
					if(skill_area_temp[0]>1)break;
				}
			/*	if(i==4)break; */
				x=bl->x;y=bl->y;
			}
			skill_area_temp[1]=bl->id;
			skill_area_temp[2]=x;
			skill_area_temp[3]=y;
			/* ‚Ü‚¸ƒ^[ƒQƒbƒg‚ÉUŒ‚‚ğ‰Á‚¦‚é */
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0);
			/* ‚»‚ÌŒãƒ^[ƒQƒbƒgˆÈŠO‚Ì”ÍˆÍ“à‚Ì“G‘S‘Ì‚Éˆ—‚ğs‚¤ */
			map_foreachinarea(skill_area_sub,
				bl->m,x-ar,y-ar,x+ar,y+ar,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;

	/* –‚–@ŒnƒXƒLƒ‹ */
	case MG_SOULSTRIKE:			/* ƒ\ƒEƒ‹ƒXƒgƒ‰ƒCƒN */
	case MG_COLDBOLT:			/* ƒR[ƒ‹ƒhƒ{ƒ‹ƒg */
	case MG_FIREBOLT:			/* ƒtƒ@ƒCƒA[ƒ{ƒ‹ƒg */
	case MG_LIGHTNINGBOLT:		/* ƒ‰ƒCƒgƒjƒ“ƒOƒ{ƒ‹ƒg */
	case WZ_EARTHSPIKE:			/* ƒA[ƒXƒXƒpƒCƒN */
	case AL_HEAL:				/* ƒq[ƒ‹ */
	case AL_HOLYLIGHT:			/* ƒz[ƒŠ[ƒ‰ƒCƒg */
	case ALL_RESURRECTION:		/* ƒŠƒUƒŒƒNƒVƒ‡ƒ“ */
	case PR_TURNUNDEAD:			/* ƒ^[ƒ“ƒAƒ“ƒfƒbƒh */
	case MG_FROSTDIVER:			/* ƒtƒƒXƒgƒ_ƒCƒo[ */
	case WZ_JUPITEL:			/* ƒ†ƒsƒeƒ‹ƒTƒ“ƒ_[ */
	case NPC_MAGICALATTACK:		/* MOB:–‚–@‘ÅŒ‚UŒ‚ */
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		break;

	case WZ_WATERBALL:			/* ƒEƒH[ƒ^[ƒ{[ƒ‹ */
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		if(skilllv>1)
			skill_status_change_start(src,SC_WATERBALL,skilllv,bl->id);
		break;

	case PR_BENEDICTIO:			/* ¹‘Ì~•Ÿ */
		if(battle_get_race(bl)==1 || battle_get_race(bl)==6)
			skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		break;

	/* –‚–@Œn”ÍˆÍUŒ‚ƒXƒLƒ‹ */
	case MG_NAPALMBEAT:			/* ƒiƒp[ƒ€ƒr[ƒg */
	case MG_FIREBALL:			/* ƒtƒ@ƒCƒ„[ƒ{[ƒ‹ */
	case MG_THUNDERSTORM:		/* ƒTƒ“ƒ_[ƒXƒg[ƒ€(flag=2‚Ì‚İ) */
	case WZ_HEAVENDRIVE:		/* ƒwƒuƒ“ƒYƒhƒ‰ƒCƒu(flag=2‚Ì‚İ) */
		if(flag&3){
			/* ŒÂ•Ê‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é */
			if(bl->id!=skill_area_temp[1]){
				if(skillid==MG_FIREBALL){	/* ƒtƒ@ƒCƒ„[ƒ{[ƒ‹‚È‚ç’†S‚©‚ç‚Ì‹——£‚ğŒvZ */
					int dx=abs( bl->x - skill_area_temp[2] );
					int dy=abs( bl->y - skill_area_temp[3] );
					skill_area_temp[0]=((dx>dy)?dx:dy);
				}
				skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,
					skill_area_temp[0]| ((flag&2)?0:0x0500)  );
			}
		}else{
			int ar=(skillid==MG_NAPALMBEAT)?1:2;
			skill_area_temp[1]=bl->id;
			if(skillid==MG_NAPALMBEAT){	/* ƒiƒp[ƒ€‚Å‚Íæ‚É”‚¦‚é */
				skill_area_temp[0]=0;
				map_foreachinarea(skill_area_sub,
					bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
					src,skillid,skilllv,tick, flag|BCT_ENEMY ,
					skill_area_sub_count);
			}else{
				skill_area_temp[2]=bl->x;
				skill_area_temp[3]=bl->y;
			}
			/* ‚Ü‚¸ƒ^[ƒQƒbƒg‚ÉUŒ‚‚ğ‰Á‚¦‚é */
			skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,
					skill_area_temp[0] );
			/* ‚»‚ÌŒãƒ^[ƒQƒbƒgˆÈŠO‚Ì”ÍˆÍ“à‚Ì“G‘S‘Ì‚Éˆ—‚ğs‚¤ */
			map_foreachinarea(skill_area_sub,
				bl->m,bl->x-ar,bl->y-ar,bl->x+ar,bl->y+ar,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
			break;
		}
		break;

	case WZ_FROSTNOVA:			/* ƒtƒƒXƒgƒmƒ”ƒ@ */
		/* ŒÂ•Ê‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é */
		if(bl->id!=skill_area_temp[1] &&
			(bl->x!=skill_area_temp[2] || bl->y!=skill_area_temp[3]) )
			skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,0 );
		break;


	/* ‚»‚Ì‘¼ */
	case HT_BLITZBEAT:			/* ƒuƒŠƒbƒcƒr[ƒg */
		if(flag&1){
			/* ŒÂ•Ê‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é */
			if(bl->id!=skill_area_temp[1])
				skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,skill_area_temp[0]|(flag&0xf00000));
		}else{
			skill_area_temp[0]=0;
			skill_area_temp[1]=bl->id;
			if(flag&0xf00000)
				map_foreachinarea(skill_area_sub,bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
					src,skillid,skilllv,tick, flag|BCT_ENEMY ,skill_area_sub_count);
			/* ‚Ü‚¸ƒ^[ƒQƒbƒg‚ÉUŒ‚‚ğ‰Á‚¦‚é */
			skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,skill_area_temp[0]|(flag&0xf00000));
			/* ‚»‚ÌŒãƒ^[ƒQƒbƒgˆÈŠO‚Ì”ÍˆÍ“à‚Ì“G‘S‘Ì‚Éˆ—‚ğs‚¤ */
			map_foreachinarea(skill_area_sub,
				bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
			break;
		}
		break;

	case CR_GRANDCROSS:			/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
		/* ƒXƒLƒ‹ƒ†ƒjƒbƒg”z’u */
		skill_castend_pos2(src,bl->x,bl->y,skillid,skilllv,tick,0);
		if(sd)
			sd->canmove_tick = tick + 900;
		else if(src->type == BL_MOB)
			mob_changestate((struct mob_data *)src,MS_DELAY,900);

		break;

	case TF_THROWSTONE:			/* Î“Š‚° */
	case NPC_SMOKING:			/* ƒXƒ‚[ƒLƒ“ƒO */
		skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,0 );
		break;

	case NPC_SELFDESTRUCTION:	/* ©”š */
		if(flag&1){
			/* ŒÂ•Ê‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é */
			if(src->type==BL_MOB){
				struct mob_data *md=(struct mob_data *)src;
				md->hp=skill_area_temp[0];
				if(bl->id!=skill_area_temp[1])
					skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,flag );
				md->hp=1;
			}
		}else{
			skill_area_temp[0]=battle_get_hp(src);
			skill_area_temp[1]=bl->id;
			/* ‚Ü‚¸ƒ^[ƒQƒbƒg‚ÉUŒ‚‚ğ‰Á‚¦‚é */
			skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,flag );
			/* ‚»‚ÌŒãƒ^[ƒQƒbƒgˆÈŠO‚Ì”ÍˆÍ“à‚Ì“G‘S‘Ì‚Éˆ—‚ğs‚¤ */
			map_foreachinarea(skill_area_sub,
				bl->m,bl->x-5,bl->y-5,bl->x+5,bl->y+5,0,
				src,skillid,skilllv,tick, flag|BCT_ALL|1,
				skill_castend_damage_id);
			battle_damage(src,src,1);
			break;
		}
		break;

	case AC_MAKINGARROW:			// –îì¬ 
		if(sd!=NULL) {
			clif_skill_nodamage(src,bl,skillid,5,1);
			clif_arrow_create_list(sd);
		}
		break;

	/* HP‹zû/HP‹zû–‚–@ */
	case NPC_BLOODDRAIN:
	case NPC_ENERGYDRAIN:
		battle_heal(NULL,src,
			skill_attack( (skillid==NPC_BLOODDRAIN)?BF_WEAPON:BF_MAGIC,
				src,src,bl,skillid,skilllv,tick,flag), 0 );
		break;
	}

	return 0;
}



/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹AIDw’èx‰‡Œnj
 *------------------------------------------
 */
int skill_castend_nodamage_id( struct block_list *src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag )
{
	struct map_session_data *sd=NULL;
	struct map_session_data *dstsd=NULL;
	struct mob_data *md=NULL;
	int i;

	if(src->type==BL_PC)
		sd=(struct map_session_data *)src;
	else if(src->type==BL_MOB)
		md=(struct mob_data *)src;

	if(bl->type==BL_PC)
		dstsd=(struct map_session_data *)bl; 

	if(bl == NULL || bl->prev == NULL)
		return 0;
	if(sd && pc_isdead(sd))
		return 0;
	if(dstsd && pc_isdead(dstsd) && skillid != ALL_RESURRECTION)
		return 0;

	switch(skillid)
	{
#if 0
	case SM_RECOVERY:			/* SP‰ñ•œŒüã */
		{
        int a=0,i,bonus,c=0;
		bonus=100+(skilllv*10);
        if(skilllv==1){a=500 > 550;}
        else if(skilllv==2){a=500 > 551;}
        else if(skilllv==3){a=500 > 551;}
        else if(skilllv==4){a=500 > 551;}
		else if(skilllv==5){a=500 > 551;}
		else if(skilllv==6){a=500 > 551;}
		else if(skilllv==7){a=500 > 551;}
		else if(skilllv==8){a=500 > 551;}
		else if(skilllv==9){a=500 > 551;}
        else if(skilllv==10){a=500 > 551;}
        if(a>0){
           for(i=0;i<MAX_INVENTORY;i++){
          if(sd->status.inventory[i].nameid==a){
                 pc_delitem(sd,i,1,0);
                 c=1;
                 if(skilllv==1){a=rand()%15+30;}
                 else if(skilllv==2){a=rand()%20+70;}
                 else if(skilllv==3){a=rand()%60+175;}
                 else if(skilllv==4){a=rand()%80+350;}
                 else if(skilllv==5){a=rand()%80+350;}
				 else if(skilllv==6){a=rand()%80+350;}
				 else if(skilllv==7){a=rand()%80+350;}
				 else if(skilllv==8){a=rand()%80+350;}
				 else if(skilllv==9){a=rand()%80+350;}
				 else if(skilllv==10){a=rand()%80+350;}
                 if(a>1){
                    clif_skill_nodamage(src,bl,skillid,(bonus*a)/100,1);
                    battle_heal(NULL,bl,(bonus*a)/100,0);
                 }
          }
       }
       if(c==0){clif_displaymessage(sd->fd,"Potions.");}
#endif

	case AL_HEAL:				/* ƒq[ƒ‹ */
		{
			int heal=skill_calc_heal( src, skilllv );
			int heal_get_jobexp;
			if( bl->type==BL_PC && ((struct map_session_data *)bl)->special_state.no_magic_damage )
				heal=0;	/* ‰©‹àå³ƒJ[ƒhiƒq[ƒ‹—Ê‚Oj */
			clif_skill_nodamage(src,bl,skillid,heal,1);
			heal_get_jobexp = battle_heal(NULL,bl,heal,0);
			
			// JOBŒoŒ±’lŠl“¾
			if(src->type == BL_PC && bl->type==BL_PC && heal > 0 && src != bl){
				heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
				if(heal_get_jobexp <= 0)
					heal_get_jobexp = 1;
				pc_gainexp((struct map_session_data *)src,0,heal_get_jobexp);
			}
		}
		break;

	case ALL_RESURRECTION:		/* ƒŠƒUƒŒƒNƒVƒ‡ƒ“ */
		if(bl->type==BL_PC){
			int per=0;
			struct map_session_data *tsd=(struct map_session_data*)bl;

			if( (map[bl->m].flag.pvp) && tsd->pvp_point<=0 )
				break;			/* PVP‚Å•œŠˆ•s‰Â”\ó‘Ô */

			if(pc_isdead(tsd)){	/* €–S”»’è */
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				switch(skilllv){
				case 1: per=10; break;
				case 2: per=30; break;
				case 3: per=50; break;
				case 4: per=80; break;
				}
				tsd->status.hp=tsd->status.max_hp*per/100;
				if(tsd->status.hp<=0) tsd->status.hp=1;
				if(tsd->special_state.restart_full_recover ){	/* ƒIƒVƒŠƒXƒJ[ƒh */
					tsd->status.hp=tsd->status.max_hp;
					tsd->status.sp=tsd->status.max_sp;
				}
				pc_setstand(tsd);
				if(battle_config.ghost_time > 0)
					pc_setghosttimer(tsd,battle_config.ghost_time);
				clif_updatestatus(tsd,SP_HP);
				clif_resurrection(&tsd->bl,1);
			}
		}
		break;

	case AL_INCAGI:			/* ‘¬“x‘‰Á */
	case AL_DECAGI:			/* ‘¬“xŒ¸­ */
	case AL_BLESSING:		/* ƒuƒŒƒbƒVƒ“ƒO */
	case KN_TWOHANDQUICKEN:	/* ƒc[ƒnƒ“ƒhƒNƒCƒbƒPƒ“ */
	case KN_AUTOCOUNTER:
	case CR_SPEARQUICKEN:	/* ƒXƒsƒAƒNƒCƒbƒPƒ“ */
	case 350:
	case PR_IMPOSITIO:		/* ƒCƒ€ƒ|ƒVƒeƒBƒIƒ}ƒkƒX */
	case PR_ASPERSIO:		/* ƒAƒXƒyƒ‹ƒVƒI */
	case PR_KYRIE:			/* ƒLƒŠƒGƒGƒŒƒCƒ\ƒ“ */
	case PR_LEXDIVINA:		/* ƒŒƒbƒNƒXƒfƒBƒr[ƒi */
	case PR_LEXAETERNA:		/* ƒŒƒbƒNƒXƒG[ƒeƒ‹ƒi */
	case AS_ENCHANTPOISON:	/* ƒGƒ“ƒ`ƒƒƒ“ƒgƒ|ƒCƒYƒ“ */
	case AS_POISONREACT:	/* ƒ|ƒCƒYƒ“ƒŠƒAƒNƒg */
	case AC_CONCENTRATION:	/* W’†—ÍŒüã */
	case MC_LOUD:			/* ƒ‰ƒEƒhƒ{ƒCƒX */
	case MG_ENERGYCOAT:		/* ƒGƒiƒW[ƒR[ƒg */
	case SM_PROVOKE:		/* ƒvƒƒ{ƒbƒN */
	case SM_ENDURE:			/* ƒCƒ“ƒfƒ…ƒA */
	case PR_SUFFRAGIUM:		/* ƒTƒtƒ‰ƒMƒEƒ€ */
	case MG_SIGHT:			/* ƒTƒCƒg */
	case AL_RUWACH:			/* ƒ‹ƒAƒt */
	case PR_BENEDICTIO:		/* ¹‘Ì~•Ÿ */
	/* Alchemist Added by AppleGirl */
	case AM_CP_WEAPON:
	case AM_CP_SHIELD:
	case AM_CP_ARMOR:
	case AM_CP_HELM:
	case CR_PROVIDENCE:		/* ƒvƒƒ”ƒBƒfƒ“ƒX */
	case CR_DEFENDER:
	case MO_BLADESTOP:
	case SA_FLAMELAUNCHER:	/* ƒtƒŒƒCƒ€ƒ‰ƒ“ƒ`ƒƒ[ */
	case SA_FROSTWEAPON:	/* ƒtƒƒXƒgƒEƒFƒ|ƒ“ */
	case SA_LIGHTNINGLOADER:/* ƒ‰ƒCƒgƒjƒ“ƒOƒ[ƒ_[ */
	case SA_SEISMICWEAPON:	/* ƒTƒCƒYƒ~ƒbƒNƒEƒFƒ|ƒ“ */
	case SA_SPELLBREAKER:
	case MO_EXPLOSIONSPIRITS:	// ”š—ô”g“®
	case MO_STEELBODY:		// ‹à„
#if 0
	case CR_AUTOGUARD:		/* ƒI[ƒgƒK[ƒh */
	case CR_DEFENDER:		/* ƒfƒBƒtƒFƒ“ƒ_[ */
	case SA_CASTCANCEL:		/* ƒLƒƒƒXƒgƒLƒƒƒ“ƒZƒ‹ */
	case SA_VOLCANO:		/* ƒ{ƒ‹ƒP[ƒm */
	case SA_DELUGE:			/* ƒfƒŠƒ…[ƒW */
	case SA_VIOLENTGALE:	/* ƒoƒCƒIƒŒƒ“ƒgƒQƒCƒ‹ */
	case SA_LANDPROTECTOR:	/* ƒ‰ƒ“ƒhƒvƒƒeƒNƒ^[ */
	case BA_FROSTJOKE:		/* Š¦‚¢ƒWƒ‡[ƒN */
#endif
		clif_skill_nodamage( (skillid==73 || skillid==249)?bl:src,bl,skillid,skilllv,1);
		skill_status_change_start( bl,
			SkillStatusChangeTable[skillid], skilllv, 0 );
		if(skillid==SM_PROVOKE && bl->type==BL_MOB)
			mob_target((struct mob_data *)bl,src,skill_get_range(skillid, skilllv));
		break;
	case CR_AUTOGUARD:
	case CR_REFLECTSHIELD:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;
	case MO_CALLSPIRITS:	// ‹CŒ÷
		if(sd) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			pc_addspiritball(sd,60*10*1000,skilllv);
		}
		break;
	case MO_ABSORBSPIRITS:	// ‹C’D
		if(sd && dstsd) {
			if(sd == dstsd || map[sd->bl.m].flag.pvp || map[sd->bl.m].flag.gvg) {
				if(dstsd->spiritball > 0) {
					clif_skill_nodamage(src,bl,skillid,skilllv,1);
					i = dstsd->spiritball * 7;
					pc_delspiritball(dstsd,dstsd->spiritball,0);
					if(i > 0x7FFF)
						i = 0x7FFF;
					if(sd->status.sp + i > sd->status.max_sp) {
						i = sd->status.max_sp - sd->status.sp;
						sd->status.sp = sd->status.max_sp;
					}
					else
						sd->status.sp += i;
					clif_heal(sd->fd,SP_SP,i);
				}
				else
					clif_skill_nodamage(src,bl,skillid,skilllv,0);
			}
			else
				clif_skill_nodamage(src,bl,skillid,skilllv,0);
		}
		break;

/*	case AC_MAKINGARROW:			// –îì¬
		if(sd) {
			clif_arrow_create_list(sd);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;*/

	case AM_PHARMACY:			/* ƒ|[ƒVƒ‡ƒ“ì¬ */
		if(sd) {
			clif_skill_produce_mix_list(sd,32);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;


	case BS_HAMMERFALL:		/* ƒnƒ“ƒ}[ƒtƒH[ƒ‹ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if( rand()%100 < (20+ 10*skilllv) ) {
			skill_status_change_start(bl,SC_STAN,skilllv,10000);
		}
		break;

	case BA_FROSTJOKE:		/* ƒnƒ“ƒ}[ƒtƒH[ƒ‹ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if( rand()%100 < (20+ 10*skilllv) ) {
			skill_status_change_start(bl,SC_FREEZE,skilllv,10000);
		}
		break;

	case RG_RAID:			/* ƒTƒvƒ‰ƒCƒYƒAƒ^ƒbƒN */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		{
			int x=bl->x,y=bl->y;
			skill_area_temp[1]=bl->id;
			skill_area_temp[2]=x;
			skill_area_temp[3]=y;
			map_foreachinarea(skill_area_sub,
				bl->m,x-1,y-1,x+1,y+1,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		skill_status_change_end(src, SC_HIDDING, -1);	// ƒnƒCƒfƒBƒ“ƒO‰ğœ
		break;

	case RG_GANGSTER:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		{
			if(pc_checkskill(sd,RG_GANGSTER) < 0){
				return 0;
			}
			int x=bl->x,y=bl->y;
			skill_area_temp[1]=bl->id;
			skill_area_temp[2]=x;
			skill_area_temp[3]=y;
			map_foreachinarea(skill_area_sub,
				bl->m,x-1,y-1,x+1,y+1,0,
				src,skillid,skilllv,tick, flag|BCT_ALL,
				skill_castend_damage_id);
		}
		skill_status_change_start( bl, SC_GANGSTER , skilllv, 0 );
		break;

	/* ƒp[ƒeƒBƒXƒLƒ‹ */
	case AL_ANGELUS:		/* ƒGƒ“ƒWƒFƒ‰ƒX */
	case PR_MAGNIFICAT:		/* ƒ}ƒOƒjƒtƒBƒJ[ƒg */
	case PR_GLORIA:			/* ƒOƒƒŠƒA */
	case BS_ADRENALINE:		/* ƒAƒhƒŒƒiƒŠƒ“ƒ‰ƒbƒVƒ… */
	case BS_WEAPONPERFECT:	/* ƒEƒFƒ|ƒ“ƒp[ƒtƒFƒNƒVƒ‡ƒ“ */
	case BS_OVERTHRUST:		/* ƒI[ƒo[ƒgƒ‰ƒXƒg */
		if( sd==NULL || sd->status.party_id==0 || (flag&1) ){
			/* ŒÂ•Ê‚Ìˆ— */
			clif_skill_nodamage(bl,bl,skillid,skilllv,1);
			skill_status_change_start( bl,
				SkillStatusChangeTable[skillid], skilllv, 0 );
		}else{
			/* ƒp[ƒeƒB‘S‘Ì‚Ö‚Ìˆ— */
			party_foreachsamemap(skill_area_sub,
				sd,1,
				src,skillid,skilllv,tick, flag|BCT_PARTY|1,
				skill_castend_nodamage_id);
		}
		break;

	/*i•t‰Á‚Æ‰ğœ‚ª•K—vj */
	case BS_MAXIMIZE:		/* ƒ}ƒLƒVƒ}ƒCƒYƒpƒ[ */
	case NV_TRICKDEAD:		/* €‚ñ‚¾‚Ó‚è */
	case TF_HIDING:			/* ƒnƒCƒfƒBƒ“ƒO */
	case AS_CLOAKING:		/* ƒNƒ[ƒLƒ“ƒO */
		if(skillid != AS_CLOAKING)
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		{
			int sc=SkillStatusChangeTable[skillid];
			if( (battle_get_sc_data(bl))[sc].timer==-1 )
				/* •t‰Á‚·‚é */
				skill_status_change_start(bl, sc, skilllv, 0);
			else
				/* ‰ğœ‚·‚é */
				skill_status_change_end(bl, sc, -1);

			if(skillid==AS_CLOAKING)
				skill_check_cloaking(bl);
		}
		break;

	/* ‘Î’nƒXƒLƒ‹ */
	case BD_LULLABY:			/* qç‰S */
	case BD_RICHMANKIM:			/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
	case BD_ETERNALCHAOS:		/* ‰i‰“‚Ì¬“× */
	case BD_DRUMBATTLEFIELD:	/* í‘¾ŒÛ‚Ì‹¿‚« */
	case BD_RINGNIBELUNGEN:		/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case BD_ROKISWEIL:			/* ƒƒL‚Ì‹©‚Ñ */
	case BD_INTOABYSS:			/* [•£‚Ì’†‚É */
	case BD_SIEGFRIED:			/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
	case BA_DISSONANCE:			/* •s‹¦˜a‰¹ */
	case BA_POEMBRAGI:			/* ƒuƒ‰ƒM‚Ì */
	case BA_WHISTLE:			/* Œû“J */
	case BA_ASSASSINCROSS:		/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
	case BA_APPLEIDUN:			/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case DC_UGLYDANCE:			/* ©•ªŸè‚Èƒ_ƒ“ƒX */
	case DC_HUMMING:			/* ƒnƒ~ƒ“ƒO */
	case DC_DONTFORGETME:		/* „‚ğ–Y‚ê‚È‚¢‚Åc */
	case DC_FORTUNEKISS:		/* K‰^‚ÌƒLƒX */
	case DC_SERVICEFORYOU:		/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_unitsetting(src,skillid,skilllv,src->x,src->y,0);
		break;

	case BD_ADAPTATION:			/* ƒAƒhƒŠƒu */
		{
			struct skill_unit_group *group=skill_check_dancing(src);
			if(group){
				skill_delunitgroup(group);
				pc_calcstatus(sd,0);
			}
		} break;

	case TF_STEAL:			// ƒXƒeƒB[ƒ‹
		if(pc_steal_item(sd,bl)) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			mob_target((struct mob_data *)bl,src,skill_get_range(skillid, skilllv));
		}
		else
			clif_skill_nodamage(src,bl,skillid,skilllv,0);
		break;

	case RG_STEALCOIN:		// ƒXƒeƒB[ƒ‹ƒRƒCƒ“
		if(pc_steal_coin(sd,bl)) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			mob_target((struct mob_data *)bl,src,skill_get_range(skillid, skilllv));
		}
		else
			clif_skill_nodamage(src,bl,skillid,skilllv,0);
		break;

/*	case BD_INTOABYSS:
		if(skillid==BD_INTOABYSS){
			(void skill_check_condition( sd ));
		clif_skill_nodamage(src,bl,skillid,0,0);		
		skill_unitsetting(src,skillid,skilllv,src->x,src->y,0);
		}
		break;*/

	case MG_STONECURSE:			/* ƒXƒg[ƒ“ƒJ[ƒX */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if( rand()%100 < skilllv*4+20 )
			skill_status_change_start(bl,SC_STONE,skilllv,0);
		break;

	case NV_FIRSTAID:			/* ‰‹}è“– */
		clif_skill_nodamage(src,bl,skillid,5,1);
		battle_heal(NULL,bl,5,0);
		break;

	case AL_CURE:				/* ƒLƒ…ƒA[ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_end(bl, SC_SILENCE	, -1 );
		skill_status_change_end(bl, SC_BLIND	, -1 );
		skill_status_change_end(bl, SC_CONFUSION, -1 );
		break;

	case TF_DETOXIFY:			/* ‰ğ“Å */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_end(bl, SC_POISON	, -1 );
		break;

	case PR_SLOWPOISON:			/* ƒŠƒJƒoƒŠ[ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_end(bl, SC_POISON	, -1 );
		break;

	case PR_STRECOVERY:			/* ƒŠƒJƒoƒŠ[ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_end(bl, SC_FREEZE	, -1 );
		skill_status_change_end(bl, SC_STONE	, -1 );
		skill_status_change_end(bl, SC_SLEEP	, -1 );
		skill_status_change_end(bl, SC_STAN		, -1 );
		break;

	case WZ_ESTIMATION:			/* ƒ‚ƒ“ƒXƒ^[î•ñ */
		if(src->type==BL_PC){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			clif_skill_estimation((struct map_session_data *)src,bl);
		}
		break;

	case MC_IDENTIFY:			/* ƒAƒCƒeƒ€ŠÓ’è */
		if(sd!=NULL)
			clif_item_identify_list(sd);
		break;

	case MC_VENDING:			/* ˜I“XŠJİ */
		if(sd!=NULL)
			clif_openvendingreq(sd,2+sd->skilllv);
		break;

	case AL_TELEPORT:			/* ƒeƒŒƒ|[ƒg */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if( sd ){
			if(map[sd->bl.m].flag.noteleport)	/* ƒeƒŒƒ|‹Ö~ */
				break;
			if( sd->skilllv==1 )
				pc_randomwarp(sd,3);
			else{
				clif_skill_warppoint(sd,sd->skillid,"Random",
					sd->status.save_point.map,"","");
			}
		}else if( bl->type==BL_MOB )
			mob_warp((struct mob_data *)bl,-1,-1,3);
		break;

	case TF_PICKSTONE:			/* ƒAƒNƒAƒxƒlƒfƒBƒNƒ^ */
		if(sd) {
			int eflag;
			struct item item_tmp;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			memset(&item_tmp,0,sizeof(item_tmp));
			item_tmp.nameid=7049;	// ¹…
			item_tmp.identify=1;
			eflag = pc_additem(sd,&item_tmp,1);
			if(eflag) {
				clif_additem(sd,0,0,eflag);
				map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y);
			}
		}
		break;

	case AL_HOLYWATER:			/* ƒAƒNƒAƒxƒlƒfƒBƒNƒ^ */
		if(sd) {
			int eflag;
			struct item item_tmp;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			memset(&item_tmp,0,sizeof(item_tmp));
			item_tmp.nameid=523;	// ¹…
			item_tmp.identify=1;
			eflag = pc_additem(sd,&item_tmp,1);
			if(eflag) {
				clif_additem(sd,0,0,eflag);
				map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y);
			}
		}
		break;

	case WZ_FROSTNOVA:			/* ƒtƒƒXƒgƒmƒ”ƒ@ */
		skill_area_temp[1]=bl->id;
		skill_area_temp[2]=bl->x;
		skill_area_temp[3]=bl->y;
		/* ‚Ü‚¸ƒ^[ƒQƒbƒg‚ÉƒGƒtƒFƒNƒg‚ğo‚· */
/*		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		clif_skill_damage(src,bl,tick,battle_get_amotion(src),0,-1,1,skillid,skilllv,6);
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick, ); */
		/* ‚»‚ÌŒãƒ^[ƒQƒbƒgˆÈŠO‚Ì”ÍˆÍ“à‚Ì“G‘S‘Ì‚Éˆ—‚ğs‚¤ */
		map_foreachinarea(skill_area_sub,
			bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
		break;

	case RG_STRIPWEAPON:		/* ƒXƒgƒŠƒbƒvƒEƒFƒ|ƒ“ */
	case RG_STRIPSHIELD:		/* ƒXƒgƒŠƒbƒvƒV[ƒ‹ƒh */
	case RG_STRIPARMOR:			/* ƒXƒgƒŠƒbƒvƒA[ƒ}[ */
	case RG_STRIPHELM:			/* ƒXƒgƒŠƒbƒvƒwƒ‹ƒ€ */
		{
		int equip=0,item_pos;
		if(bl->type==BL_PC) {
			if ((item_pos = pc_checkequip(dstsd,skillid-215))) {
				equip=1;
				if (rand()%100 < (5+skilllv*2)) {
					equip=2;
					pc_unequipitem(dstsd,item_pos,0);
					skill_status_change_start( bl,SkillStatusChangeTable[skillid], skilllv, 0 );
				}
			}
			if(equip==0){clif_displaymessage(sd->fd,"Nothing Equiped.");}
			else if (equip==1){clif_skill_fail(sd,skillid,0,0);}
 			}
		}
		clif_skill_nodamage(src,bl,skillid,0,1);
		if(bl->type==BL_MOB && rand()%100 < (5+skilllv*2)) {
			skill_status_change_start( bl,SkillStatusChangeTable[skillid], skilllv, 0 );
		}
		break;

	case AM_ACIDTERROR:
		{
			int i,equip=0,item_id;
 		int EquipmentTable[]={16};
		if(bl->type==BL_PC) {
 			item_id = pc_checkequip(dstsd,EquipmentTable[skillid]);
			if (item_id) {
				equip=1;
				for(i=0;i<MAX_INVENTORY;i++)
					if ((dstsd->status.inventory[i].nameid == item_id)) {
						if (rand()%100 < (3+skilllv*2)) {
							equip=2;
							pc_unequipitem(dstsd,i,0);
							skill_status_change_start( bl, SkillStatusChangeTable[skillid], skilllv, 0 );
						}
 					}
 			}
			if(equip==0){clif_displaymessage(sd->fd,"Nothing Equiped.");}
			else if (equip==1){clif_skill_fail(sd,skillid,0,0);}
 			}
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(bl->type==BL_MOB && rand()%100 < (3+skilllv*2)) {
			skill_status_change_start( bl,
			SkillStatusChangeTable[skillid], skilllv, 0 );
		}
		break;

	case RG_INTIMIDATE:			/* ƒCƒ“ƒeƒBƒ~ƒfƒCƒg */
		if( sd ){
			if(map[sd->bl.m].flag.noteleport)	/* ƒeƒŒƒ|‹Ö~ */
				break;
			if( sd->skilllv==1 )
				pc_randomwarp(sd,3);
			else{
				clif_skill_warppoint(sd,sd->skillid,"Random",
					sd->status.save_point.map,"","");
			}
		}else if( bl->type==BL_MOB )
			mob_warp((struct mob_data *)bl,-1,-1,3);
		break;
	/* PotionPitcher added by Tato [17/08/03] */
	case AM_POTIONPITCHER:		/* ƒ|[ƒVƒ‡ƒ“ƒsƒbƒ`ƒƒ[ */
		{
        int a=0,i,bonus,c=0;
        bonus=100+(skilllv*10);
        if(skilllv==1){a=501;}
        else if(skilllv==2){a=500 > 503;}
        else if(skilllv==3){a=500 > 504;}
        else if(skilllv==4){a=500 > 506;}
        else if(skilllv==5){a=500 > 507;}
        if(a>0){
           for(i=0;i<MAX_INVENTORY;i++){
          if(sd->status.inventory[i].nameid==a){
                 pc_delitem(sd,i,1,0);
                 c=1;
                 if(a==501){a=rand()%15+30;}
                 else if(a==502){a=rand()%20+70;}
                 else if(a==503){a=rand()%60+175;}
                 else if(a==504){a=rand()%80+350;}
                 else if(a==506){a=1;}
                 if(a>1){
                    clif_skill_nodamage(src,bl,skillid,(bonus*a)/100,1);
                    battle_heal(NULL,bl,(bonus*a)/100,0);
                 }else{
                    skill_status_change_end(bl, SC_POISON , -1 );
                    skill_status_change_end(bl, SC_SILENCE , -1 );
                    skill_status_change_end(bl, SC_BLIND , -1 );
                    skill_status_change_end(bl, SC_CONFUSION , -1 );
                 }
          }
       }
       if(c==0){clif_displaymessage(sd->fd,"Not enough Potions.");}
        }
	}
		break;
	case SA_DISPELL:			/* ƒfƒBƒXƒyƒ‹ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_end(bl, SC_SILENCE	, -1 );
		skill_status_change_end(bl, SC_BLIND	, -1 );
		skill_status_change_end(bl, SC_CONFUSION, -1 );
		skill_status_change_end(bl, SC_FREEZE	, -1 );
		skill_status_change_end(bl, SC_STONE	, -1 );
		skill_status_change_end(bl, SC_SLEEP	, -1 );
		skill_status_change_end(bl, SC_STAN		, -1 );
		skill_status_change_end(bl, SC_POISON	, -1 );
		skill_status_change_end(bl, SC_CURSE	, -1 );		/* here and below added */
		skill_status_change_end(bl, SC_INCREASEAGI	, -1 );
		skill_status_change_end(bl, SC_SUFFRAGIUM	, -1 );
		skill_status_change_end(bl, SC_MAGNIFICAT	, -1 );
		skill_status_change_end(bl, SC_APPLEIDUN	, -1 );
		skill_status_change_end(bl, SC_CP_WEAPON	, -1 );
		skill_status_change_end(bl, SC_CP_ARMOR	, -1 );
		skill_status_change_end(bl, SC_CP_SHIELD	, -1 );
		skill_status_change_end(bl, SC_CP_HELM	, -1 );
		skill_status_change_end(bl, SC_DECREASEAGI	, -1 );
		skill_status_change_end(bl, SC_FROSTWEAPON,-1);
		skill_status_change_end(bl, SC_LIGHTNINGLOADER,-1);
		skill_status_change_end(bl, SC_SEISMICWEAPON,-1);
		skill_status_change_end(bl, SC_FLAMELAUNCHER,-1);
		skill_status_change_end(bl, SC_DIVINA	, -1 );
		skill_status_change_end(bl, SC_GLORIA	, -1 );
		skill_status_change_end(bl, SC_MAGNIFICAT, -1 );
		break;

	case TF_BACKSLIDING:		/* ƒoƒbƒNƒXƒeƒbƒv */
		battle_stopwalking(src,1);
		skill_blown(src,bl,5|0x10000);
		clif_fixpos(src);
		skill_addtimerskill(src,tick + 200,src->id,0,0,skillid,skilllv,0,flag);
		break;

	/* ƒ‰ƒ“ƒ_ƒ€‘®«•Ï‰»A…‘®«•Ï‰»A’nA‰ÎA•— */
	case NPC_ATTRICHANGE:
	case NPC_CHANGEWATER:
	case NPC_CHANGEGROUND:
	case NPC_CHANGEFIRE:
	case NPC_CHANGEWIND:
	/* “ÅA¹A”OAˆÅ */
	case NPC_CHANGEPOISON:
	case NPC_CHANGEHOLY:
	case NPC_CHANGEDARKNESS:
	case NPC_CHANGETELEKINESIS:
		if(md){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			md->def_ele=skill_get_pl(skillid);
			if(md->def_ele==0)			/* ƒ‰ƒ“ƒ_ƒ€•Ï‰»A‚½‚¾‚µA*/
				md->def_ele=rand()%9;	/* •s€‘®«‚Íœ‚­ */
			md->def_ele+=(1+rand()%4)*20;	/* ‘®«ƒŒƒxƒ‹‚Íƒ‰ƒ“ƒ_ƒ€ */
		}
		break;

	case NPC_SUICIDE:			/* ©Œˆ */
		if(md){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			mob_damage(NULL,md,md->hp);
		}
		break;

	case NPC_SUMMONSLAVE:		/* è‰º¢Š« */
	case NPC_SUMMONMONSTER:		/* MOB¢Š« */
		if(md)
			mob_summonslave(md,mob_db[md->class].skill[md->skillidx].val1,
				skilllv,(skillid==NPC_SUMMONSLAVE)?1:0);
		break;

	case NPC_EMOTION:			/* ƒGƒ‚[ƒVƒ‡ƒ“ */
		clif_emotion(&md->bl,mob_db[md->class].skill[md->skillidx].val1);
		break;

	case BD_ENCORE:				// Added by RoVeRT
		sd->skillitem = sd->last_skillid;
		sd->skillitemlv = sd->last_skilllv;
		clif_item_skill(sd,sd->last_skillid,sd->last_skilllv,sd->status.name);
		sd->last_skillid = BD_ENCORE;
		break;

	case SA_CASTCANCEL:			// Added by RoVeRT
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_castcancel(src);
		break;

	case SA_AUTOSPELL:			// Added by RoVeRT
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_status_change_start( bl, SkillStatusChangeTable[skillid], skilllv, 0 );
		break;
	}
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹AIDw’èj
 *------------------------------------------
 */
int skill_castend_id( int tid, unsigned int tick, int id,int data )
{
	struct map_session_data* sd=NULL/*,*target_sd=NULL*/;
	struct block_list *bl;
	
	if( (sd=map_id2sd(id))==NULL || sd->bl.prev == NULL)
		return 0;
	
	if( sd->skilltimer != tid )	/* ƒ^ƒCƒ}ID‚ÌŠm”F */
		return 0;
	if(sd->skilltimer != -1 && pc_checkskill(sd,SA_FREECAST) > 0) {
		sd->speed = sd->prev_speed;
		clif_updatestatus(sd,SP_SPEED);
	}
	sd->skilltimer=-1;
	bl=map_id2bl(sd->skilltarget);
	if(bl==NULL || bl->prev==NULL)
		return 0;
	if(sd->bl.m != bl->m || pc_isdead(sd))
		return 0;

	if(!battle_config.skill_out_range_consume) {
		if(skill_get_range(sd->skillid, sd->skilllv) + battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,bl->x,bl->y)) {
			clif_skill_fail(sd,sd->skillid,0,0);
			sd->canact_tick = tick;
			sd->canmove_tick = tick;
			return 0;
		}
	}
	if(!skill_check_condition( sd )) {		/* g—pğŒƒ`ƒFƒbƒN */
		sd->canact_tick = tick;
		sd->canmove_tick = tick;
		return 0;
	}
	if(battle_config.skill_out_range_consume) {
		if(skill_get_range(sd->skillid, sd->skilllv) + battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,bl->x,bl->y)) {
			clif_skill_fail(sd,sd->skillid,0,0);
			sd->canact_tick = tick;
			sd->canmove_tick = tick;
			return 0;
		}
	}

	printf("PC %d skill castend skill=%d\n",sd->bl.id,sd->skillid);

	if( (skill_get_inf(sd->skillid)&1) &&	// ”Ş‰ä“G‘ÎŠÖŒWƒ`ƒFƒbƒN
		battle_check_target(&sd->bl,bl, BCT_ENEMY)<=0 )
		return 0;

	switch( skill_get_nk(sd->skillid) )
	{
	/* UŒ‚Œn/‚«”ò‚Î‚µŒn */
	case 0:	case 2:
		skill_castend_damage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		break;
	case 1:/* x‰‡Œn */
		if( (sd->skillid==AL_HEAL || sd->skillid==ALL_RESURRECTION) && battle_get_elem_type(bl)==9 )
			skill_castend_damage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		else
			skill_castend_nodamage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		break;
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹AêŠw’è‚ÌÀÛ‚Ìˆ—j
 *------------------------------------------
 */
int skill_castend_pos2( struct block_list *src, int x,int y,int skillid,int skilllv,unsigned int tick,int flag)
{
	struct map_session_data *sd=NULL;
	int i,tmpx = 0,tmpy = 0, x1 = 0, y1 = 0;

	if(src->type==BL_PC)
		sd=(struct map_session_data *)src;
	if(skillid != WZ_METEOR)
		clif_skill_poseffect(src,skillid,skilllv,x,y,tick);

	switch(skillid)
	{
	case MG_THUNDERSTORM:		/* ƒTƒ“ƒ_[ƒXƒg[ƒ€ */
	case WZ_HEAVENDRIVE:		/* ƒwƒ”ƒ“ƒYƒhƒ‰ƒCƒu */
		skill_area_temp[1]=src->id;
		skill_area_temp[2]=x;
		skill_area_temp[3]=y;
		map_foreachinarea(skill_area_sub,
			src->m,x-2,y-2,x+2,y+2,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|2,
			skill_castend_damage_id);
		break;

	case PR_BENEDICTIO:			/* ¹‘Ì~•Ÿ */
		skill_area_temp[1]=src->id;
		map_foreachinarea(skill_area_sub,
			src->m,x-1,y-1,x+1,y+1,0,
			src,skillid,skilllv,tick, flag|BCT_NOENEMY|1,
			skill_castend_nodamage_id);
		map_foreachinarea(skill_area_sub,
			src->m,x-1,y-1,x+1,y+1,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
		break;

	case BS_HAMMERFALL:			/* ƒnƒ“ƒ}[ƒtƒH[ƒ‹ */
		skill_area_temp[1]=src->id;
		skill_area_temp[2]=x;
		skill_area_temp[3]=y;
		map_foreachinarea(skill_area_sub,
			src->m,x-2,y-2,x+2,y+2,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|2,
			skill_castend_nodamage_id);
		break;

	case BA_FROSTJOKE:			/* ƒnƒ“ƒ}[ƒtƒH[ƒ‹ */
		skill_area_temp[1]=src->id;
		skill_area_temp[2]=x;
		skill_area_temp[3]=y;
		map_foreachinarea(skill_area_sub,
			src->m,x-2,y-2,x+2,y+2,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|2,
			skill_castend_nodamage_id);
		break;

	case MG_SAFETYWALL:			/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
	case MG_FIREWALL:			/* ƒtƒ@ƒCƒ„[ƒEƒH[ƒ‹ */
	case AL_PNEUMA:				/* ƒjƒ…[ƒ} */
	case WZ_ICEWALL:			/* ƒAƒCƒXƒEƒH[ƒ‹ */
	case WZ_FIREPILLAR:			/* ƒtƒ@ƒCƒAƒsƒ‰[ */
	case WZ_QUAGMIRE:			/* ƒNƒ@ƒOƒ}ƒCƒA */
	case WZ_VERMILION:			/* ƒ[ƒhƒIƒuƒ”ƒ@[ƒ~ƒŠƒIƒ“ */
	case WZ_STORMGUST:			/* ƒXƒg[ƒ€ƒKƒXƒg */
	case PR_SANCTUARY:			/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
	case PR_MAGNUS:				/* ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€ */
	case CR_GRANDCROSS:			/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
	case HT_SKIDTRAP:			/* ƒXƒLƒbƒhƒgƒ‰ƒbƒv */
	case HT_LANDMINE:			/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
	case HT_ANKLESNARE:			/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
	case HT_SHOCKWAVE:			/* ƒVƒ‡ƒbƒNƒEƒF[ƒuƒgƒ‰ƒbƒv */
	case HT_SANDMAN:			/* ƒTƒ“ƒhƒ}ƒ“ */
	case HT_FLASHER:			/* ƒtƒ‰ƒbƒVƒƒ[ */
	case HT_FREEZINGTRAP:		/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
	case HT_BLASTMINE:			/* ƒuƒ‰ƒXƒgƒ}ƒCƒ“ */
	case HT_CLAYMORETRAP:		/* ƒNƒŒƒCƒ‚ƒA[ƒgƒ‰ƒbƒv */
	case AS_VENOMDUST:			/* ƒxƒmƒ€ƒ_ƒXƒg */
	case WZ_SIGHTRASHER:
	/* Test */
	case AM_DEMONSTRATION:
	case SA_VOLCANO:
	case SA_DELUGE:
	case SA_VIOLENTGALE:
	case SA_LANDPROTECTOR:
		if(skillid == WZ_SIGHTRASHER)
			skill_status_change_end(src, SC_SIGHT, -1);
		skill_unitsetting(src,skillid,skilllv,x,y,0);
		break;

	case WZ_METEOR:				//ƒƒeƒIƒXƒg[ƒ€
		for(i=0;i<2+(skilllv>>1);i++) {
			int j = 0, c;
			do {
				tmpx = x + (rand()%5 - 2);
				tmpy = y + (rand()%5 - 2);
				if(tmpx < 0)
					tmpx = 0;
				else if(tmpx >= map[src->m].xs)
					tmpx = map[src->m].xs - 1;
				if(tmpy < 0)
					tmpy = 0;
				else if(tmpy >= map[src->m].ys)
					tmpy = map[src->m].ys - 1;
				j++;
			} while(((c=map_getcell(src->m,x,y))==1 || c==5) && j<100);
			if(j >= 100)
				continue;

			if(i==0)
				clif_skill_poseffect(src,skillid,skilllv,tmpx,tmpy,tick);
			if(i > 0)
				skill_addtimerskill(src,tick+i*1000,0,tmpx,tmpy,skillid,skilllv,(x1<<16)|y1,flag);

			x1 = tmpx;
			y1 = tmpy;
		}
		skill_addtimerskill(src,tick+i*1000,0,tmpx,tmpy,skillid,skilllv,-1,flag);
		break;

	case AL_WARP:				/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
	case 336:
		if(map[sd->bl.m].flag.noteleport)	/* ƒeƒŒƒ|‹Ö~ */
			break;
		clif_skill_warppoint(sd,sd->skillid,sd->status.save_point.map,
			(sd->skilllv>1)?sd->status.memo_point[0].map:"",
			(sd->skilllv>2)?sd->status.memo_point[1].map:"",
			(sd->skilllv>3)?sd->status.memo_point[2].map:"");
		break;
	case MO_BODYRELOCATION:
		if(sd)
			pc_movepos(sd,x,y);
		break;
	case AM_CANNIBALIZE:		// Added by RoVeRT
		{
		int m,c;
		int i,x,y;

		int qty=skilllv*2/3;
		qty = (qty>1)?qty:1;

		struct mob_data *md;
		for(i=0;i<qty;i++){
			do{
				x = sd->skillx + (rand()%6 - 3);
				y = sd->skilly + (rand()%6 - 3);
			}while( ( (c=map_getcell(sd->bl.m,x,y))==1 || c==5) );

			if ((m=mob_once_spawn(sd,"this",x,y,"--ja--",1118, 1,""))){
				md=(struct mob_data*)map_id2bl(m);
				skill_status_change_start((struct block_list *)map_id2bl(m), SC_CANNIBALIZE, skilllv, 0 );
				md->hp = md->hp * skilllv * 10 / 100;
			}
		}
		}
		break;
	case AM_SPHEREMINE:		// Added by RoVeRT
		{
			int m,c;
			int i,x,y;

			int qty=skilllv*2/3;
			qty = (qty>1)?qty:1;

			struct mob_data *md;
			for(i=0;i<qty;i++){
				do{
					x = sd->skillx + (rand()%6 - 3);
					y = sd->skilly + (rand()%6 - 3);
				}while( ( (c=map_getcell(sd->bl.m,x,y))==1 || c==5) );

				if ((m=mob_once_spawn(sd,"this",x,y,"--ja--",1142, 1,""))){
					/* Permanent residence time of suicide bombing being 5 seconds, drift speed 300 (per 0.1 seconds becomes quick 5 at a time) */
					md=(struct mob_data*)map_id2bl(m);
					skill_status_change_start((struct block_list *)map_id2bl(m), SC_SPHEREMINE, skilllv, 0 );
					/* md->hp = md->hp * skilllv * 10 / 100;		/. == instant explode */
				}
			}
		}
		break;
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹Amapw’èj
 *------------------------------------------
 */
int skill_castend_map( struct map_session_data *sd,int skill_num, const char *map)
{
	int x=0,y=0,itid=0;
	
	if( sd==NULL || sd->bl.prev == NULL || pc_isdead(sd))
		return 0;

	if( sd->opt1>0 || sd->status.option&6 ||
		sd->sc_data[SC_DIVINA].timer!=-1 || sd->sc_data[SC_ROKISWEIL].timer!=-1)
		return 0;	/* ˆÙí‚â’¾–Ù‚È‚Ç */
	
	if( skill_num != sd->skillid)	/* •s³ƒpƒPƒbƒg‚ç‚µ‚¢ */
		return 0;

	pc_stopattack(sd);

	printf("PC %d skill castend skill =%d map=%s\n",sd->bl.id,skill_num,map);

	if(strcmp(map,"cancel")==0)
		return 0;

	switch(skill_num){
	case AL_TELEPORT:		/* ƒeƒŒƒ|[ƒg */
		if(strcmp(map,"Random")==0)
			pc_randomwarp(sd,3);
		else
			pc_setpos(sd,sd->status.save_point.map,
				sd->status.save_point.x,sd->status.save_point.y,3);
		break;

	case AL_WARP:			/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
		{
			const struct point *p[]={
				&sd->status.save_point,&sd->status.memo_point[0],
				&sd->status.memo_point[1],&sd->status.memo_point[2],
			};
			struct skill_unit_group *group;
			int i;
			for(i=0;i<sd->skilllv;i++){
				if(strcmp(map,p[i]->map)==0){
					x=p[i]->x;
					y=p[i]->y;
					break;
				}
			}
			if(x==0 || y==0)	/* •s³ƒpƒPƒbƒgH */
				return 0;

			if(!sd->special_state.no_gemstone && (itid=pc_search_inventory(sd, 717)) <= 0){		// Added by RoVeRT
				clif_skill_fail(sd,sd->skillid,8,0);
				return 0;
			}else if (!sd->special_state.no_gemstone)
				pc_delitem(sd, itid, 1, 0);

			group=skill_unitsetting(&sd->bl,sd->skillid,sd->skilllv,sd->skillx,sd->skilly,0);
			group->valstr=malloc(16);
			if(group->valstr==NULL){
				printf("skill_castend_map: out of memory !\n");
				exit(0);
			}
			memcpy(group->valstr,map,16);
			group->val2=(x<<16)|y;
		}
		break;

	case RG_INTIMIDATE:			/* ƒCƒ“ƒeƒBƒ~ƒfƒCƒg */
		if(strcmp(map,"Random")==0)
			pc_randomwarp(sd,3);
		else
			pc_setpos(sd,sd->status.save_point.map,
				sd->status.save_point.x,sd->status.save_point.y,3);
		break;
	case 336:	/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
		{
			const struct point *p[]={
				&sd->status.save_point,&sd->status.memo_point[0],
				&sd->status.memo_point[1],&sd->status.memo_point[2],
			};
			struct skill_unit_group *group;
			int i;
			for(i=0;i<sd->skilllv;i++){
				if(strcmp(map,p[i]->map)==0){
					x=p[i]->x;
					y=p[i]->y;
					break;
				}
			}
			if(x==0 || y==0)	/* •s³ƒpƒPƒbƒgH */
				return 0;
/*			
//			if(!pc_check_equip_dcard(sd,4132) && (itid=pc_search_inventory(sd, 717)) <= 0){
			if(!sd->no_gemstone && (itid=pc_search_inventory(sd, 717)) <= 0){
				clif_skill_fail(sd,sd->skillid,8,0);
				return 0;
			}else
				pc_delitem(sd, itid, 1, 0);*/

			group=skill_unitsetting(&sd->bl,sd->skillid,sd->skilllv,sd->skillx,sd->skilly,0);
			group->valstr=malloc(16);
			if(group->valstr==NULL){
				printf("skill_castend_map: out of memory !\n");
				exit(0);
			}
			memcpy(group->valstr,map,16);
			group->val2=(x<<16)|y;
		}
		break;
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgİ’èˆ—
 *------------------------------------------
 */
struct skill_unit_group *skill_unitsetting( struct block_list *src, int skillid,int skilllv,int x,int y,int flag)
{
	struct skill_unit_group *group;
	int i,count=1,limit=10000,val1=skilllv,val2=0;
	int target=BCT_ENEMY,interval=1000,range=0;
	int dir=0;

	if (1){

		struct map_session_data *sd=NULL;
		struct mob_data *md=NULL;
		int skill_limit=0;

		if(skillid == MG_FIREWALL)
			skill_limit = 4;
		else if(skillid == WZ_ICEWALL)	// after limit you get 1 column of ice
			skill_limit = 9;
		else if(skillid==WZ_QUAGMIRE)
			skill_limit=3;

		if (src->type == BL_PC){
			sd=(struct map_session_data *)src;
			if (skill_limit && sd->skill_limit[skillid] > skill_limit ) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return group;
			}
			sd->skill_limit[skillid]++;
		}
		else if(src->type==BL_MOB){
			md=(struct mob_data *)src;
			if (skill_limit && md->skill_limit[skillid] > skill_limit) {
				return group;
			}
		}
	}

	switch(skillid){	/* İ’è */

	case KN_BRANDISHSPEAR:	/*ƒuƒ‰ƒ“ƒfƒBƒbƒVƒ…ƒXƒsƒA*/
		if(skilllv >1 && skilllv <=3)
			range=1;
		else if(skilllv >3 && skilllv <=6)
			range=2;
		else if(skilllv >6 && skilllv <=9)
			range=3;
		else
			range=4;
		break;

	case MG_SAFETYWALL:			/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
		limit=5000*skilllv;
		val2=skilllv+1;
		target=(battle_config.defnotenemy)?BCT_NOENEMY:BCT_ALL;
		break;

	case MG_FIREWALL:			/* ƒtƒ@ƒCƒ„[ƒEƒH[ƒ‹ */
		dir=map_calc_dir(src,x,y);
		if(dir&1) count=5;
		else count=3;
		limit=1000*(val2=(4+skilllv));
		interval=100;
		break;

	case AL_PNEUMA:				/* ƒjƒ…[ƒ} */
		target=(battle_config.defnotenemy)?BCT_NOENEMY:BCT_ALL;
		range=1;
		break;

	case AL_WARP:				/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
		target=BCT_ALL;
		val1=skillid+6;
		if(flag==0)
			limit=2000;
		else
			limit=5000*(1+skilllv);
		break;

	case KN_SPEARBOOMERANG:	/* ƒ{ƒ‹ƒP[ƒm */		// Added by AppleGirl
		range=3;
		break;

	case PR_SANCTUARY:			/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
		count=21;
		limit=1000*(3*skilllv+1);
		val1=skilllv+3;
		val2=(skilllv>6)?777:skilllv*100;
		target=BCT_ALL;
		range=1;
		break;

	case PR_MAGNUS:				/* ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€ */
		count=33;
		limit=1000*(skilllv+4);
		interval=3000;
		break;

	case WZ_FIREPILLAR:			/* ƒtƒ@ƒCƒA[ƒsƒ‰[ */
		if(flag==0)
			limit=30000;
		else
			limit=1000;
		interval=2000;
		val1=skilllv+2;
		range=1;
		break;
	case WZ_METEOR:				/* ƒƒeƒIƒXƒg[ƒ€ */
		limit=500;
		interval=500;
		range=2;
		break;

	case WZ_VERMILION:			/* ƒ[ƒhƒIƒuƒ”ƒ@[ƒ~ƒŠƒIƒ“ */
		limit=3500;
		interval=1500;
		range=4;
		break;

	case WZ_ICEWALL:			/* ƒAƒCƒXƒEƒH[ƒ‹ */
		limit=4000*(1+skilllv);
		count=5;
		break;

	case WZ_STORMGUST:			/* ƒXƒg[ƒ€ƒKƒXƒg */
		limit=2000+skilllv*300;
		interval=450;
		range=4;
		break;

	case WZ_QUAGMIRE:			/* ƒNƒ@ƒOƒ}ƒCƒA */
		limit=5000*skilllv;
		interval=200;
		count=25;
		break;

	case HT_SKIDTRAP:			/* ƒXƒLƒbƒhƒgƒ‰ƒbƒv */
		limit=60000*(6-skilllv);
		range=1;
		break;

	case HT_LANDMINE:			/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
		limit=40000*(6-skilllv);
		range=1;
		break;

	case HT_ANKLESNARE:			/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
		limit=50000*(6-skilllv);
		range=1;
		val1=skilllv*5000;
		interval=val1+5000;
		break;

	case HT_SHOCKWAVE:			/* ƒVƒ‡ƒbƒNƒEƒF[ƒuƒgƒ‰ƒbƒv */
		limit=40000*(6-skilllv);
		range=1;
		val1=skilllv*15+10;
		break;

	case HT_SANDMAN:			/* ƒTƒ“ƒhƒ}ƒ“ */
		limit=30000*(6-skilllv);
		range=1;
		break;

	case HT_FLASHER:			/* ƒtƒ‰ƒbƒVƒƒ[ */
		limit=30000*(6-skilllv);
		range=1;
		break;

	case HT_FREEZINGTRAP:		/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
		limit=30000*(6-skilllv);
		range=1;
		break;

	case HT_BLASTMINE:			/* ƒuƒ‰ƒXƒgƒ}ƒCƒ“ */
		limit=5000*(6-skilllv);
		interval=3000;
		range=1;
		break;

	case HT_CLAYMORETRAP:		/* ƒNƒŒƒCƒ‚ƒA[ƒgƒ‰ƒbƒv */
		limit=20000*(skilllv);
		interval=3000;
		range=1;
		break;

	case AM_DEMONSTRATION:	/* ƒtƒ@ƒCƒ„[ƒsƒ‰[ */		// Added by AppleGirl
		limit=5000*skilllv;
		interval=2000;
		val1=skilllv+2;
		range=1;
		break;

	case CR_SHIELDBOOMERANG:	/* ƒ{ƒ‹ƒP[ƒm */		// Added by AppleGirl
		range=3;
		break;

	case AS_VENOMDUST:			/* ƒxƒmƒ€ƒ_ƒXƒg */
		limit=5000*skilllv;
		interval=1000;
		count=5;
		break;

	case CR_GRANDCROSS:			/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
		count=29;
		limit=900;
		interval=300;
		break;

	case MO_STEELBODY:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
		val1=skilllv;
		target=BCT_ALL;
		break;

	case MO_BLADESTOP:	/* ƒT[ƒrƒXƒtƒH[ƒ† */
		target=BCT_ALL;
		break;

	case SA_VOLCANO:			/* ƒ{ƒ‹ƒP[ƒm */
	case SA_DELUGE:				/* ƒfƒŠƒ…[ƒW */
	case SA_VIOLENTGALE:	/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
		//count=33;
		limit=180000;
		range=skilllv+skilllv%2+3;
		target=BCT_ALL;
		if(skilllv > 0 && skilllv <= 2)
			count=25;
		else if(skilllv > 2 && skilllv <= 4)
			count=49;
		else
			count=81;
		break;

	case SA_LANDPROTECTOR:	/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
//		count=33;
		limit=(90+skilllv*30)*1000;
		val1=skilllv*15+10;
		range=skilllv+skilllv%2+5;
		target=BCT_ALL;
		if(skilllv > 0 && skilllv <= 2)
			count=49;
		else if(skilllv > 2 && skilllv <= 4)
			count=81;
		else
			count=121;
		break;

	case BD_LULLABY:			/* qç‰S */
	case BD_ETERNALCHAOS:		/* ƒGƒ^[ƒiƒ‹ƒJƒIƒX */
	case BD_ROKISWEIL:			/* ƒƒL‚Ì‹©‚Ñ */
		count=81;
		limit=60*1000;
		range=skilllv+skilllv%2+3;
		target=BCT_ALL;
		break;

	case BD_DRUMBATTLEFIELD:	/* í‘¾ŒÛ‚Ì‹¿‚« */
	case BD_RINGNIBELUNGEN:		/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case BD_INTOABYSS:			/* [•£‚Ì’†‚É */
	case BD_SIEGFRIED:			/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
		count=81;
		limit=60*1000;
		range=skilllv+skilllv%2+3;
		target=BCT_PARTY;
		break;

	case BA_FROSTJOKE:	/* Œû“J */
		count=81;
		limit=skill_get_delay(skillid,skilllv);
		range=9;
		target=BCT_NOENEMY;
		break;

	case BA_WHISTLE:			/* Œû“J */
	case DC_HUMMING:			/* ƒnƒ~ƒ“ƒO */
		count=81;
		limit=60*1000;
		range=9;
		val1=skilllv;
		target=BCT_ALL;
		break;

	case BA_DISSONANCE:			/* •s‹¦˜a‰¹ */
	case DC_UGLYDANCE:			/* ©•ªŸè‚Èƒ_ƒ“ƒX */
		count=81;
		limit=30*1000;
		range=skilllv+skilllv%2+3;
		val1=skilllv*15+10;
		target=BCT_ENEMY;
		break;

	case DC_DONTFORGETME:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
		count=81;
		limit=120000;
		val1=skilllv;
		target=BCT_NOENEMY;
		break;

	case BA_POEMBRAGI:	/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
		count=81;
		limit=180000;
		val1=skilllv;
		target=BCT_NOENEMY;
		break;

	case BA_ASSASSINCROSS:	/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case DC_FORTUNEKISS:		/* K‰^‚ÌƒLƒX */
		count=81;
		limit=120*1000;
		val1=skilllv;
		target=BCT_NOENEMY;
		break;

	case BA_APPLEIDUN:			/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case DC_SERVICEFORYOU:		/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
		count=81;
		limit=180*1000;
		range=skilllv+skilllv%2+3;
		target=BCT_PARTY;
		break;

	case 336:	/* ƒ[ƒvƒ|[ƒ^ƒ‹ */
		target=BCT_ALL;
		val1=skillid+6;
		if(flag==0)
			limit=2000;
		else
			limit=5000*(1+skilllv);
		break;
	};

	group=skill_initunitgroup(src,count,skillid,skilllv,
		skill_get_unit_id(skillid,flag&1));
	group->limit=limit;
	group->val1=val1;
	group->val2=val2;
	group->target_flag=target;
	group->interval=interval;
	group->range=range;

	for(i=0;i<count;i++){
		struct skill_unit *unit;
		int ux=x,uy=y,val1=skilllv,val2=0,limit=group->limit,alive=1;
		int range=0;
		switch(skillid){	/* İ’è */
		case MG_FIREWALL:		/* ƒtƒ@ƒCƒ„[ƒEƒH[ƒ‹ */
		{
				if(dir&1){	/* Î‚ß”z’u */
					static const int dx[][5]={
						{ 1,1,0,0,-1 }, { -1,-1,0,0,1 },
					},dy[][5]={
						{ 1,0,0,-1,-1 }, { 1,0,0,-1,-1 },
					};
					ux+=dx[(dir>>1)&1][i];
					uy+=dy[(dir>>1)&1][i];
				}else{	/* ã‰º”z’u */
					if(dir%4==0)	/* ã‰º */
						ux+=i-1;
					else			/* ¶‰E */
						uy+=i-1;
				}
				val2=group->val2;
			}
			break;

		case PR_SANCTUARY:		/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
			{
				static const int dx[]={
					-1,0,1, -2,-1,0,1,2, -2,-1,0,1,2, -2,-1,0,1,2, -1,0,1 };
				static const int dy[]={
					-2,-2,-2, -1,-1,-1,-1,-1, 0,0,0,0,0, 1,1,1,1,1, 2,2,2, };
				ux+=dx[i];
				uy+=dy[i];
			}
			break;

		case PR_MAGNUS:			/* ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€ */
			{
				static const int dx[]={ -1,0,1, -1,0,1, -3,-2,-1,0,1,2,3,
					-3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -1,0,1, -1,0,1, };
				static const int dy[]={
					-3,-3,-3, -2,-2,-2, -1,-1,-1,-1,-1,-1,-1,
					0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2, 3,3,3 };
				ux+=dx[i];
				uy+=dy[i];
			}
			break;
/*
		case SA_VOLCANO: 
			{
				static const int dx[]={ -1,0,1, -1,0,1, -3,-2,-1,0,1,2,3,
					-3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -1,0,1, -1,0,1, };
				static const int dy[]={
					-3,-3,-3, -2,-2,-2, -1,-1,-1,-1,-1,-1,-1,
					0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2, 3,3,3 };
				ux+=dx[i];
				uy+=dy[i];
			} 
			break;
		case SA_DELUGE: 
			{
				static const int dx[]={ -1,0,1, -1,0,1, -3,-2,-1,0,1,2,3,
					-3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -1,0,1, -1,0,1, };
				static const int dy[]={
					-3,-3,-3, -2,-2,-2, -1,-1,-1,-1,-1,-1,-1,
					0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2, 3,3,3 };
				ux+=dx[i];
				uy+=dy[i];
			} 
			break;
		case SA_VIOLENTGALE: {
				static const int dx[]={ -1,0,1, -1,0,1, -3,-2,-1,0,1,2,3,
					-3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -1,0,1, -1,0,1, };
				static const int dy[]={
					-3,-3,-3, -2,-2,-2, -1,-1,-1,-1,-1,-1,-1,
					0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2, 3,3,3 };
				ux+=dx[i];
				uy+=dy[i];
			} 
			break;
		case SA_LANDPROTECTOR: 
			{
				static const int dx[]={ -1,0,1, -1,0,1, -3,-2,-1,0,1,2,3,
					-3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -1,0,1, -1,0,1, };
				static const int dy[]={
					-3,-3,-3, -2,-2,-2, -1,-1,-1,-1,-1,-1,-1,
					0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2, 3,3,3 };
				ux+=dx[i];
				uy+=dy[i];
			} 
			break;
			*/
		case WZ_SIGHTRASHER: 
			{
				static const int dx[]={ 3,2,1, -1,-2,-3, 0,0,0, 0,0,0, 
					1,-1,1,-1, 2,-2,2,-2, 3,-3,3,-3, };
				static const int dy[]={ 
					0,0,0, 0,0,0, 3,2,1, -1,-2,-3, 
						1,1,-1,-1, 2,2,-2,-2, 3,3,-3,-3 };
				ux+=dx[i];
				uy+=dy[i];
			} 
			break;

		case WZ_ICEWALL:		/* ƒAƒCƒXƒEƒH[ƒ‹ */
			{
				static const int dirx[8]={0,-1,-1,-1,0,1,1,1};
				static const int diry[8]={1,1,0,-1,-1,-1,0,1};
				dir=map_calc_dir(src,x,y);
				ux+=(2-i)*diry[dir];
				uy+=(i-2)*dirx[dir];
				val2=map_getcell(src->m,ux,uy);
				if(val2==5 || val2==1)
					alive=0;
				else {
					map_setcell(src->m,ux,uy,5);
					clif_changemapcell(src->m,ux,uy,5,0);
				}
			}
			break;

		case WZ_QUAGMIRE:		/* ƒNƒ@ƒOƒ}ƒCƒA */
			ux+=(i%5-2);
			uy+=(i/5-2);
			break;

		case AS_VENOMDUST:		/* ƒxƒmƒ€ƒ_ƒXƒg */
			{
				static const int dx[]={-1,0,0,0,1};
				static const int dy[]={0,-1,0,1,0};
				ux+=dx[i];
				uy+=dy[i];
			}
			break;

		case CR_GRANDCROSS:		/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
			{
				static const int dx[]={
					0, 0, -1,0,1, -2,-1,0,1,2, -4,-3,-2,-1,0,1,2,3,4, -2,-1,0,1,2, -1,0,1, 0, 0, };
				static const int dy[]={
					-4, -3, -2,-2,-2, -1,-1,-1,-1,-1, 0,0,0,0,0,0,0,0,0, 1,1,1,1,1, 2,2,2, 3, 4, };
				ux+=dx[i];
				uy+=dy[i];
			}
			break;

		case SA_VOLCANO:			/* ƒ{ƒ‹ƒP[ƒm */
		case SA_DELUGE:				/* ƒfƒŠƒ…[ƒW */
		case SA_VIOLENTGALE:	/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
		case SA_LANDPROTECTOR:	/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
			ux+=(i%9-4);
			uy+=(i/9-4);
			break;

		/* ƒ_ƒ“ƒX‚È‚Ç */
		case BD_LULLABY:		/* qç‰Ì */
		case BD_RICHMANKIM:		/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
		case BD_ETERNALCHAOS:	/* ‰i‰“‚Ì¬“× */
		case BD_DRUMBATTLEFIELD:/* í‘¾ŒÛ‚Ì‹¿‚« */
		case BD_RINGNIBELUNGEN:	/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
		case BD_ROKISWEIL:		/* ƒƒL‚Ì‹©‚Ñ */
		case BD_INTOABYSS:		/* [•£‚Ì’†‚É */
		case BD_SIEGFRIED:		/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
		case BA_DISSONANCE:		/* •s‹¦˜a‰¹ */
		case BA_WHISTLE:		/* Œû“J */
		case BA_ASSASSINCROSS:	/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
		case BA_POEMBRAGI:		/* ƒuƒ‰ƒM‚Ì */
		case BA_APPLEIDUN:		/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
		case DC_UGLYDANCE:		/* ©•ªŸè‚Èƒ_ƒ“ƒX */
		case DC_HUMMING:		/* ƒnƒ~ƒ“ƒO */
		case DC_DONTFORGETME:	/* „‚ğ–Y‚ê‚È‚¢‚Åc */
		case DC_FORTUNEKISS:	/* K‰^‚ÌƒLƒX */
		case DC_SERVICEFORYOU:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
			ux+=(i%9-4);
			uy+=(i/9-4);
			if(i==40)
				range=4;	/* ’†S‚Ìê‡‚Í”ÍˆÍ‚ğ4‚ÉƒI[ƒo[ƒ‰ƒCƒh */
			else
				range=-1;	/* ’†S‚¶‚á‚È‚¢ê‡‚Í”ÍˆÍ‚ğ-1‚ÉƒI[ƒo[ƒ‰ƒCƒh */
			break;
		}
		if(alive){
			unit=skill_initunit(group,i,ux,uy);
			unit->val1=val1;
			unit->val2=val2;
			unit->limit=limit;
			unit->range=range;
		}
	}

	return group;
}

/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‚Ì”­“®ƒCƒxƒ“ƒg
 *------------------------------------------
 */
int skill_unit_onplace(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg= src->group;
	struct block_list *ss=map_id2bl(sg->src_id);
	struct skill_unit_group_tickset *ts;
	int diff,goflag;

	if(bl == NULL ||  bl->prev==NULL || !src->alive)
		return 0;
	if( bl->type!=BL_PC && bl->type!=BL_MOB )
		return 0;

	if(ss==NULL)
		return 0;

	ts=skill_unitgrouptickset_search( bl, sg->group_id);
	diff=DIFF_TICK(tick,ts->tick);
	goflag=(diff>sg->interval || diff<0);
	if(!goflag)
		return 0;
	ts->tick=tick;
	ts->group_id=sg->group_id;

	switch(sg->unit_id){
	case 0x83:	/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
		{
			int *list=sg->vallist;
			int i,ei=0,race=battle_get_race(bl);
			int element=battle_get_elem_type(bl);

			if(!battle_config.sanctuary_type) {
				if( battle_get_hp(bl)>=battle_get_max_hp(bl) &&
					 race!=1 && race!=6  )
					break;

				for(i=0;i<16;i++)	/* l”§ŒÀ‚ÌŒvZ */
					if(list[i]==0)
						ei=i;
					else if(list[i]==bl->id)
						break;
				if(i==16){
					list[ei]=bl->id;
					if((sg->val1--)<=0){
						skill_delunitgroup(sg);
						return 0;
					}
				}
				if( race!=1 && race!=6 ){
					int heal=sg->val2;
					if( bl->type==BL_PC && ((struct map_session_data *)bl)->special_state.no_magic_damage)
						heal=0;	/* ‰©‹àå³ƒJ[ƒhiƒq[ƒ‹—Ê‚Oj */
					clif_skill_nodamage(&src->bl,bl,/*sg->skill_id*/28,heal,1);
					battle_heal(NULL,bl,heal,0);
				}else
					skill_attack(BF_MAGIC,ss,&src->bl,bl,
						sg->skill_id,sg->skill_lv,tick,0);
			}
			else {
				if( battle_get_hp(bl)>=battle_get_max_hp(bl) &&
					 element!=9 && race!=6  )
					break;

				for(i=0;i<16;i++)	/* l”§ŒÀ‚ÌŒvZ */
					if(list[i]==0)
						ei=i;
					else if(list[i]==bl->id)
						break;
				if(i==16){
					list[ei]=bl->id;
					if((sg->val1--)<=0){
						skill_delunitgroup(sg);
						return 0;
					}
				}
				if( element!=9 && race!=6 ){
					int heal=sg->val2;
					if( bl->type==BL_PC && ((struct map_session_data *)bl)->special_state.no_magic_damage)
						heal=0;	/* ‰©‹àå³ƒJ[ƒhiƒq[ƒ‹—Ê‚Oj */
					clif_skill_nodamage(&src->bl,bl,/*sg->skill_id*/28,heal,1);
					battle_heal(NULL,bl,heal,0);
				}else
					skill_attack(BF_MAGIC,ss,&src->bl,bl,
						sg->skill_id,sg->skill_lv,tick,0);
			}
		}
		break;

	case 0x84:	/* ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€ */
		{
			int race=battle_get_race(bl);
			int element=battle_get_elem_type(bl);
			if(!battle_config.sanctuary_type) {
				if( race!=1 && race!=6 )
					return 0;
			}
			else {
				if(element!=9 && race!=6  )
					return 0;
			}
			skill_attack(BF_MAGIC,ss,&src->bl,bl,
				sg->skill_id,sg->skill_lv,tick,0);
		}
		break;

	case 0x85:	/* ƒjƒ…[ƒ} */
	case 0x7e:	/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
		{
			struct skill_unit *unit2;
			struct status_change *sc_data=battle_get_sc_data(bl);
			int type=(sg->unit_id==0x85)?SC_PNEUMA:SC_SAFETYWALL;
			if(sc_data[type].timer==-1)
				skill_status_change_start(bl,type,src->group->val1,(int)src);
			else if((unit2=(struct skill_unit *)sc_data[type].val2)!=src){
				if( sg->unit_id==0x85 && DIFF_TICK(sg->tick,unit2->group->tick)>0 )
					skill_status_change_start(bl,type,src->group->val1,(int)src);
				if( sg->unit_id==0x7e && sg->val1 < unit2->group->val1 )
					skill_status_change_start(bl,type,src->group->val1,(int)src);
				ts->tick-=sg->interval;
			}
		} break;

	case 0x86:	/* ƒ[ƒhƒIƒuƒ”ƒ@[ƒ~ƒŠƒIƒ“(•ƒXƒg[ƒ€ƒKƒXƒg •ƒOƒ‰ƒ“ƒhƒNƒƒX) */
		skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
		break;

	case 0x7f:	/* ƒtƒ@ƒCƒ„[ƒEƒH[ƒ‹ */
		if( (src->val2--)>0)
			skill_attack(BF_MAGIC,ss,&src->bl,bl,
				sg->skill_id,sg->skill_lv,tick,0);
		if( src->val2<=0 )
			skill_delunit(src);
		break;

	case 0x87:	/* ƒtƒ@ƒCƒA[ƒsƒ‰[(”­“®‘O) */
		skill_delunit(src);
		skill_unitsetting(ss,sg->skill_id,sg->skill_lv,src->bl.x,src->bl.y,1);
		break;

	case 0x88:	/* ƒtƒ@ƒCƒA[ƒsƒ‰[(”­“®Œã) */
		skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
		break;

	case 0x90:	/* ƒXƒLƒbƒhƒgƒ‰ƒbƒv */
		if(sg->val2==0){
			int i;
			for(i=0;i<sg->skill_lv;i++)
				skill_blown(&src->bl,bl,1|0x30000);
			sg->limit=DIFF_TICK(tick,sg->tick)+3000;
			sg->val2=bl->id;
		}
		break;

	case 0x93:	/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
		skill_attack(BF_MISC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
		skill_delunit(src);
		break;

	case 0x8f:	/* ƒuƒ‰ƒXƒgƒ}ƒCƒ“ */
	case 0x97:	/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
	case 0x98:	/* ƒNƒŒƒCƒ‚ƒA[ƒgƒ‰ƒbƒv */
		skill_attack((sg->unit_id==0x97)?BF_WEAPON:BF_MISC,
			ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,(sg->val2)?0x0500:0);
		if(sg->val2==0){
			sg->limit=DIFF_TICK(tick,sg->tick)+500;
			sg->val2=bl->id;
		}
		break;

	case 0x95:	/* ƒTƒ“ƒhƒ}ƒ“ */
	case 0x96:	/* ƒtƒ‰ƒbƒVƒƒ[ */
	case 0x94:	/* ƒVƒ‡ƒbƒNƒEƒF[ƒuƒgƒ‰ƒbƒv */
	case 0x91:	/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
		skill_additional_effect(ss,bl,sg->skill_id,sg->skill_lv,BF_MISC,tick);
		if(sg->val2==0){
			sg->limit=DIFF_TICK(tick,sg->tick)
				+((sg->unit_id==0x91)?sg->val1/((battle_get_mode(bl)&0x20)?5:1):500);
			sg->val2=bl->id;
		}
		break;

	case 0x80:	/* ƒ[ƒvƒ|[ƒ^ƒ‹(”­“®Œã) */
		if(bl->type==BL_PC){
			if((sg->val1--)>0){
				pc_setpos((struct map_session_data *)bl,
					sg->valstr,sg->val2>>16,sg->val2&0xffff,0);
			}else
				skill_delunitgroup(sg);
		}
		break;

	case 0x8e:	/* ƒNƒ@ƒOƒ}ƒCƒA */
	case 0x92:	/* ƒxƒmƒ€ƒ_ƒXƒg */
		{
			int type=SkillStatusChangeTable[sg->skill_id];
			if( battle_get_sc_data(bl)[type].timer==-1 )
				skill_status_change_start(bl,type,src->group->val1,(int)src);
		}
		break;

	/* Volcano, Deluge, Gale, Land */
	case 0x9a:
	case 0x9b:
	case 0x9c:
	case 0x9d:
	case 0x9e:	/* qç‰S */
	case 0x9f:	/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
	case 0xa0:	/* ‰i‰“‚Ì¬“× */
	case 0xa1:	/* í‘¾ŒÛ‚Ì‹¿‚« */
	case 0xa2:	/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case 0xa3:	/* ƒƒL‚Ì‹©‚Ñ */
	case 0xa4:	/* [•£‚Ì’†‚É */
	case 0xa5:	/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
	case 0xa6:	/* •s‹¦˜a‰¹ */
	case 0xa7:	/* Œû“J */
	case 0xa8:	/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
	case 0xa9:	/* ƒuƒ‰ƒM‚Ì */
	case 0xaa:	/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case 0xab:	/* ©•ªŸè‚Èƒ_ƒ“ƒX */
	case 0xac:	/* ƒnƒ~ƒ“ƒO */
	case 0xad:	/* „‚ğ–Y‚ê‚È‚¢‚Åc */
	case 0xae:	/* K‰^‚ÌƒLƒX */
	case 0xaf:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
		{
			struct skill_unit *unit2;
			struct status_change *sc_data=battle_get_sc_data(bl);
			int type=SkillStatusChangeTable[sg->skill_id];
			if(sc_data[type].timer==-1)
				skill_status_change_start(bl,type,src->group->val1,(int)src);
			else if((unit2=(struct skill_unit *)sc_data[type].val4)!=src){
				if( DIFF_TICK(sg->tick,unit2->group->tick)>0 )
					skill_status_change_start(bl,type,0,(int)src);
/*				if( sg->unit_id==0x7e && sg->val1 < unit2->group->val1 )
					skill_status_change_start(bl,type,0,(int)src);*/
				ts->tick-=sg->interval;
			}
		} break;

	case 0xb2:
		if(bl->type==BL_PC){
			pc_setpos((struct map_session_data *)bl,
				sg->valstr,sg->val2>>16,sg->val2&0xffff,0);
		}
		break;

	/* Blah */
	case 0xb1:
	case 0xb3:
	case 0xb4:
	case 0xb5:
		{
			skill_attack(BF_MAGIC,ss,&src->bl,bl,
				sg->skill_id,sg->skill_lv,tick,0);
		} break;

/*	default:
		printf("skill_unit_onplace: Unknown skill unit id=%d block=%d\n",sg->unit_id,bl->id);
		break;*/
	}
	return 0;
}
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‚©‚ç—£’E‚·‚é(‚à‚µ‚­‚Í‚µ‚Ä‚¢‚é)ê‡
 *------------------------------------------
 */
int skill_unit_onout(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg= src->group;

	if(bl == NULL || bl->prev==NULL || !src->alive)
		return 0;
	if( bl->type!=BL_PC && bl->type!=BL_MOB )
		return 0;

	switch(sg->unit_id){
	case 0x83:	/* ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ */
		{
			int i,*list=sg->vallist;
			for(i=0;i<list[i];i++)
				if(list[i]==bl->id)
					list[i]=0;
		}
		break;

	case 0x7e:	/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
	case 0x85:	/* ƒjƒ…[ƒ} */
	case 0x8e:	/* ƒNƒ@ƒOƒ}ƒCƒA */
		{
			struct skill_unit *unit2;
			struct status_change *sc_data=battle_get_sc_data(bl);
			int type=
				(sg->unit_id==0x85)?SC_PNEUMA:
				((sg->unit_id==0x7e)?SC_SAFETYWALL:
				 SC_QUAGMIRE);
			if((type != SC_QUAGMIRE || bl->type != BL_MOB) &&
				sc_data[type].timer!=-1 && (unit2=(struct skill_unit *)sc_data[type].val2)==src){
				skill_status_change_end(bl,type,-1);
			}
		} break;

	case 0x91:	/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
		{
			struct block_list *target=map_id2bl(sg->val2);
			if( target==bl )
				skill_status_change_end(bl,SC_ANKLE,-1);
			sg->limit=DIFF_TICK(tick,sg->tick)+1000;
		}
		break;

	/* Volcano, Deluge, Gale, Land */
	case 0x9a:
	case 0x9b:
	case 0x9c:
	case 0x9d:
	case 0x9e:	/* qç‰S */
	case 0x9f:	/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
	case 0xa0:	/* ‰i‰“‚Ì¬“× */
	case 0xa1:	/* í‘¾ŒÛ‚Ì‹¿‚« */
	case 0xa2:	/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case 0xa3:	/* ƒƒL‚Ì‹©‚Ñ */
	case 0xa4:	/* [•£‚Ì’†‚É */
	case 0xa5:	/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
	case 0xa6:	/* •s‹¦˜a‰¹ */
	case 0xa7:	/* Œû“J */
	case 0xa8:	/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
	case 0xa9:	/* ƒuƒ‰ƒM‚Ì */
	case 0xaa:	/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case 0xab:	/* ©•ªŸè‚Èƒ_ƒ“ƒX */
	case 0xac:	/* ƒnƒ~ƒ“ƒO */
	case 0xad:	/* „‚ğ–Y‚ê‚È‚¢‚Åc */
	case 0xae:	/* K‰^‚ÌƒLƒX */
	case 0xaf:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
	/* Blah */
	case 0xb1:
	case 0xb2:
	case 0xb3:
	case 0xb4:
	case 0xb5:
		{
			struct skill_unit *unit2;
			struct status_change *sc_data=battle_get_sc_data(bl);
			int type=SkillStatusChangeTable[sg->skill_id];
			if(sc_data[type].timer!=-1 && (unit2=(struct skill_unit *)sc_data[type].val4)==src){
				skill_status_change_end(bl,type,-1);
			}
		}
		break;

/*	default:
		printf("skill_unit_onout: Unknown skill unit id=%d block=%d\n",sg->unit_id,bl->id);
		break;*/
	}
	skill_unitgrouptickset_delete(bl,sg->group_id);
	return 0;
}
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‚ÌíœƒCƒxƒ“ƒg
 *------------------------------------------
 */
int skill_unit_ondelete(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg= src->group;
	
	if(bl == NULL || bl->prev==NULL || !src->alive)
		return 0;
	if( bl->type!=BL_PC && bl->type!=BL_MOB )
		return 0;

	switch(sg->unit_id){
	/* Volcano, Deluge, Gale, Land */
	case 0x9a:
	case 0x9b:
	case 0x9c:
	case 0x9d:
	case 0x85:	/* ƒjƒ…[ƒ} */
	case 0x7e:	/* ƒZƒCƒtƒeƒBƒEƒH[ƒ‹ */
	case 0x8e:	/* ƒNƒ@ƒOƒ}ƒCƒ„ */
	case 0x9e:	/* qç‰S */
	case 0x9f:	/* ƒjƒˆƒ‹ƒh‚Ì‰ƒ */
	case 0xa0:	/* ‰i‰“‚Ì¬“× */
	case 0xa1:	/* í‘¾ŒÛ‚Ì‹¿‚« */
	case 0xa2:	/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
	case 0xa3:	/* ƒƒL‚Ì‹©‚Ñ */
	case 0xa4:	/* [•£‚Ì’†‚É */
	case 0xa5:	/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
	case 0xa6:	/* •s‹¦˜a‰¹ */
	case 0xa7:	/* Œû“J */
	case 0xa8:	/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
	case 0xa9:	/* ƒuƒ‰ƒM‚Ì */
	case 0xaa:	/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
	case 0xab:	/* ©•ªŸè‚Èƒ_ƒ“ƒX */
	case 0xac:	/* ƒnƒ~ƒ“ƒO */
	case 0xad:	/* „‚ğ–Y‚ê‚È‚¢‚Åc */
	case 0xae:	/* K‰^‚ÌƒLƒX */
	case 0xaf:	/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
	/* Blah */
	case 0xb1:
	case 0xb2:
	case 0xb3:
	case 0xb4:
	case 0xb5:
		return skill_unit_onout(src,bl,tick);

/*	default:
		printf("skill_unit_ondelete: Unknown skill unit id=%d block=%d\n",sg->unit_id,bl->id);
		break;*/
	}
	skill_unitgrouptickset_delete(bl,sg->group_id);
	return 0;
}
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‚ÌŒÀŠEƒCƒxƒ“ƒg
 *------------------------------------------
 */
int skill_unit_onlimit(struct skill_unit *src,unsigned int tick)
{
	struct skill_unit_group *sg= src->group;
	

	switch(sg->unit_id){
	case 0x81:	/* ƒ[ƒvƒ|[ƒ^ƒ‹(”­“®‘O) */
		{
			struct skill_unit_group *group=
				skill_unitsetting(map_id2bl(sg->src_id),sg->skill_id,sg->skill_lv,
					src->bl.x,src->bl.y,1);
			group->valstr=malloc(16);
			if(group->valstr==NULL){
				printf("skill_unit_ondelete: out of memory !\n");
				exit(0);
			}
			memcpy(group->valstr,sg->valstr,16);
			group->val2=sg->val2;
		}
		break;

	case 0x8d:	/* ƒAƒCƒXƒEƒH[ƒ‹ */
		map_setcell(src->bl.m,src->bl.x,src->bl.y,src->val2);
		clif_changemapcell(src->bl.m,src->bl.x,src->bl.y,src->val2,1);
		break;
			
	}
	return 0;
}
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‚Ìƒ_ƒ[ƒWƒCƒxƒ“ƒg
 *------------------------------------------
 */
int skill_unit_ondamaged(struct skill_unit *src,struct block_list *bl,
	int damage,unsigned int tick)
{
	struct skill_unit_group *sg= src->group;
	
	switch(sg->unit_id){
	case 0x8d:	/* ƒAƒCƒXƒEƒH[ƒ‹ */
		sg->limit-=damage;
		break;
	}
	return 0;
}


/*---------------------------------------------------------------------------- */



/*==========================================
 * ƒXƒLƒ‹g—pi‰r¥Š®—¹AêŠw’èj
 *------------------------------------------
 */
int skill_castend_pos( int tid, unsigned int tick, int id,int data )
{
	struct map_session_data* sd=NULL/*,*target_sd=NULL*/;
	
	if( (sd=map_id2sd(id))==NULL )
		return 0;
	
	if( sd->skilltimer != tid )	/* ƒ^ƒCƒ}ID‚ÌŠm”F */
		return 0;
	if(sd->skilltimer != -1 && pc_checkskill(sd,SA_FREECAST) > 0) {
		sd->speed = sd->prev_speed;
		clif_updatestatus(sd,SP_SPEED);
	}
	sd->skilltimer=-1;
	if(pc_isdead(sd))
		return 0;

	if(!battle_config.skill_out_range_consume) {
		if(skill_get_range(sd->skillid, sd->skilllv) + battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,sd->skillx,sd->skilly)) {
			clif_skill_fail(sd,sd->skillid,0,0);
			sd->canact_tick = tick;
			sd->canmove_tick = tick;
			return 0;
		}
	}
	if(!skill_check_condition( sd )) {		/* g—pğŒƒ`ƒFƒbƒN */
		sd->canact_tick = tick;
		sd->canmove_tick = tick;
		return 0;
	}
	if(battle_config.skill_out_range_consume) {
		if(skill_get_range(sd->skillid, sd->skilllv) + battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,sd->skillx,sd->skilly)) {
			clif_skill_fail(sd,sd->skillid,0,0);
			sd->canact_tick = tick;
			sd->canmove_tick = tick;
			return 0;
		}
	}

	printf("PC %d skill castend skill=%d\n",sd->bl.id,sd->skillid);

	skill_castend_pos2(&sd->bl,sd->skillx,sd->skilly,sd->skillid,sd->skilllv,tick,0);

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹g—pğŒi‹U‚Åg—p¸”sj
 *------------------------------------------
 */
int skill_check_condition( struct map_session_data *sd )
{
	unsigned int tick;
	int j=0,sp=0,hp=0,zeny=0,spiritball=0;
	int	i[3]={0,0,0},
		item_id[3]={0,0,0},
		item_amount[3]={0,0,0};

	if( sd->opt1>0) {
		clif_skill_fail(sd,sd->skillid,0,0);
		return 0;
	}

	if(sd->skillitem==sd->skillid) {	/* ƒAƒCƒeƒ€‚Ìê‡–³ğŒ¬Œ÷ */
		sd->skillitem = sd->skillitemlv = -1;

		if (sd->last_skillid==BD_ENCORE){
			sp = skill_get_sp(sd->skillid, sd->skilllv)/2;

			if( sp>0 && sd->status.sp < sp) {			/* SPƒ`ƒFƒbƒN */
				clif_skill_fail(sd,sd->skillid,1,0);		/* SP•s‘«F¸”s’Ê’m */
				return 0;
			}
			if(sp) {					/* SPÁ”ï */
				sd->status.sp-=sp;
				clif_updatestatus(sd,SP_SP);
			}
		}
	}else{
		if(sd->sc_data[SC_DIVINA].timer!=-1 || sd->sc_data[SC_ROKISWEIL].timer!=-1
		 || sd->sc_data[SC_STEELBODY].timer!=-1) {
			clif_skill_fail(sd,sd->skillid,0,0);
			return 0;
		}

		sp=skill_get_sp(sd->skillid, sd->skilllv);	/* Á”ïSP */
		if(sd->dsprate!=100)
			sp=sp*sd->dsprate/100;	/* Á”ïSPC³ */
		
		/* ƒXƒLƒ‹‚²‚Æ‚Ìg—pğŒ */
		switch(sd->skillid)
		{
		case SA_ABRACADABRA:
			item_amount[0]+=1;
			break;
		case SA_VOLCANO:
		case SA_DELUGE:
		case SA_VIOLENTGALE:
			item_id[0]=715;		//	yellow_gem = 715;
			item_amount[0]+=1;
			break;

		case SA_DISPELL:
			item_id[1]=715;		//	yellow_gem = 715;
			item_amount[1]+=1;
			break;
		case MG_STONECURSE:		// ƒXƒg[ƒ“ƒJ[ƒX
		case AS_VENOMDUST:		// ƒxƒiƒ€ƒ_ƒXƒg
			item_id[0]=716;		//	red_gem = 716;
			item_amount[0]+=1;
			break;

		case SA_LANDPROTECTOR:
			item_id[1]=715;		//	yellow_gem = 715;
			item_amount[1]+=1;
			break;
		case MG_SAFETYWALL:		// ƒZƒCƒtƒeƒBƒEƒH[ƒ‹
		case ALL_RESURRECTION:	// ƒŠƒUƒŒƒNƒVƒ‡ƒ“
		case PR_SANCTUARY:		// ƒTƒ“ƒNƒ`ƒ…ƒAƒŠ
		case PR_MAGNUS:			// ƒ}ƒOƒkƒXƒGƒNƒ\ƒVƒYƒ€
		case WZ_FIREPILLAR:		// ƒtƒ@ƒCƒA[ƒsƒ‰[
			item_id[0]=717;		//	blue_gem = 717;
			item_amount[0]+=1;
			break;

		case SA_FLAMELAUNCHER:
			item_id[0]=990;		// ‰ÎŒ´Î
			item_amount[0]+=1;
			break;

		case SA_FROSTWEAPON:
			item_id[0]=991;		// …Œ´Î
			item_amount[0]+=1;
			break;

		case SA_LIGHTNINGLOADER:
			item_id[0]=992;		// •—Œ´Î
			item_amount[0]+=1;
			break;

		case SA_SEISMICWEAPON:
			item_id[0]=993;		// ’nŒ´Î
			item_amount[0]+=1;
			break;

		case HT_TALKIEBOX:	/* ƒg[ƒL[ƒ{ƒbƒNƒX */
		case HT_BLASTMINE:	/* ƒuƒ‰ƒXƒgƒ}ƒCƒ“ */
		case HT_SKIDTRAP:	/* ƒXƒLƒbƒhƒgƒ‰ƒbƒv */
		case HT_ANKLESNARE:	/* ƒAƒ“ƒNƒ‹ƒXƒlƒA */
		case HT_LANDMINE:	/* ƒ‰ƒ“ƒhƒ}ƒCƒ“ */
		case HT_SHOCKWAVE:	/* ƒVƒ‡ƒbƒNƒEƒF[ƒuƒgƒ‰ƒbƒv */
		case HT_SANDMAN:	/* ƒTƒ“ƒhƒ}ƒ“ */
		case HT_FLASHER:	/* ƒtƒ‰ƒbƒVƒƒ[ */
		case HT_FREEZINGTRAP:	/* ƒtƒŠ[ƒWƒ“ƒOƒgƒ‰ƒbƒv */
		case HT_CLAYMORETRAP:	/* ƒNƒŒƒCƒ‚ƒA[ƒgƒ‰ƒbƒv */
			item_id[0]=1065;		//	İ’u—pƒgƒ‰ƒbƒv;
			item_amount[0]+=1;
			break;

		case AL_HOLYWATER:	/* ƒAƒNƒAƒxƒlƒfƒBƒNƒ^ */
			if(map_getcell(sd->bl.m,sd->bl.x,sd->bl.y)!=3){	//…ê”»’è
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			item_id[0]=713;			//	‹ó‚«ƒrƒ“;
			item_amount[0]+=1;
			break;

		case PR_ASPERSIO:	/* ƒAƒXƒyƒ‹ƒVƒI */
			item_id[0]=523;			//	¹…;
			item_amount[0]+=1;
			break;

		case WZ_WATERBALL:	/* ƒEƒH[ƒ^[ƒ{[ƒ‹ */
			if(map_getcell(sd->bl.m,sd->bl.x,sd->bl.y)!=3){	//…ê”»’è
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			break;

		case TF_THROWSTONE:
			item_id[0]=7049;
			item_amount[0]+=1;
			break;

		case MC_MAMMONITE:		/* ƒƒ}[ƒiƒCƒg */
			zeny = sd->skilllv*100;
			if( sd->status.zeny<zeny ) {
				clif_skill_fail(sd,sd->skillid,5,0);	/* Zeny•s‘«F¸”s’Ê’m */
				return 0;
			}
			break;

		case MC_VENDING:		// ˜I“XŠJİ	
			if(!pc_iscarton(sd)) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			break;

		case AC_DOUBLE:		// ƒ_ƒuƒ‹ƒXƒgƒŒƒCƒtƒBƒ“ƒO
		case AC_SHOWER:		// ƒAƒ[ƒVƒƒƒ[
			if( sd->status.weapon != 11) {
				clif_skill_fail(sd,sd->skillid,6,0);	// ‹|
		     	return 0; 
			}
			break;

		case KN_BRANDISHSPEAR:	// ƒuƒ‰ƒ“ƒfƒBƒbƒVƒ…ƒXƒsƒA
			if(!pc_isriding(sd)) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			else if(sd->status.weapon != 4 && sd->status.weapon != 5) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;		// ,ƒyƒRƒyƒRó‘Ô
			}
			break;

		case KN_PIERCE:			// ƒsƒA[ƒX
		case KN_SPEARSTAB:		// ƒXƒsƒAƒXƒ^ƒu
		case KN_SPEARBOOMERANG:	// ƒXƒsƒAƒu[ƒƒ‰ƒ“
			if(sd->status.weapon != 4 && sd->status.weapon != 5) {
				clif_skill_fail(sd,sd->skillid,6,0);	// ‘‹
				return 0; 
			}
			break;

		case KN_TWOHANDQUICKEN:	// ƒc[ƒnƒ“ƒhƒNƒCƒbƒPƒ“
			if( sd->status.weapon != 3) {
				clif_skill_fail(sd,sd->skillid,6,0);	// —¼èŒ•
				return 0;
			}
			break;
			
		case BS_ADRENALINE:		// ƒAƒhƒŒƒiƒŠƒ“ƒ‰ƒbƒVƒ…
		case BS_HAMMERFALL:		// ƒnƒ“ƒ}[ƒtƒH[ƒ‹
			if( sd->status.weapon != 6 && sd->status.weapon != 7 && sd->status.weapon != 8) {
				clif_skill_fail(sd,sd->skillid,6,0);	// •€E‚ÅƒƒCƒX
				return 0;
			}
			break;

		case BS_MAXIMIZE:		/* ƒ}ƒLƒVƒ}ƒCƒYƒpƒ[ */
		case NV_TRICKDEAD:		/* €‚ñ‚¾‚Ó‚è */
		case TF_HIDING:			/* ƒnƒCƒfƒBƒ“ƒO */
		case AS_CLOAKING:		/* ƒNƒ[ƒLƒ“ƒO */
			if(sd->sc_data[SkillStatusChangeTable[sd->skillid]].timer!=-1)
				sp=0;			/* ‰ğœ‚·‚éê‡‚ÍSPÁ”ï‚µ‚È‚¢ */
			break;

		case HT_BLITZBEAT:		/* ƒuƒŠƒbƒcƒr[ƒg */
			if(!pc_isfalcon(sd)) {		/* ‘é‚ª‚¢‚È‚¢ */
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			break;

		case AS_GRIMTOOTH:	/* ƒOƒŠƒ€ƒgƒD[ƒX */
			if(!pc_ishiding(sd)) {		// ƒnƒCƒfƒBƒ“ƒOó‘Ô
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			if( sd->status.weapon != 16)  {
				clif_skill_fail(sd,sd->skillid,6,0);    	// ƒJƒ^[ƒ‹Á”ï
				return 0; 
			}
			break;

		case AS_SONICBLOW:	/* SONICBLOW */
			if( sd->status.weapon != 16) {
				clif_skill_fail(sd,sd->skillid,6,0);	/* —¼è‘„ */
				return 0;
			}
			break;

		case AM_CANNIBALIZE:
			item_id[0]=7137;
			item_amount[0]+=1;
			break;
		
		case AM_DEMONSTRATION:
		case AM_SPHEREMINE:
			item_id[0]=7138;
			item_amount[0]+=1;
			break;

		case AM_ACIDTERROR:
			item_id[0]=7136;
			item_amount[0]+=1;
			break;

		case AM_CP_WEAPON:
		case AM_CP_SHIELD:
		case AM_CP_ARMOR:
		case AM_CP_HELM:
			item_id[0]=7139;
			item_amount[0]+=1;
			break;

		case CR_AUTOGUARD:	/* ƒXƒsƒAƒNƒCƒbƒPƒ“ */
		case CR_SHIELDCHARGE:
		case CR_SHIELDBOOMERANG:
		case CR_REFLECTSHIELD:
			if(!sd->status.shield) {
				clif_skill_fail(sd,sd->skillid,0,0);	/* —¼è‘„ */
				return 0;
			}
			break;

		case CR_SPEARQUICKEN:	// ƒXƒsƒAƒNƒCƒbƒPƒ“
			if( sd->status.weapon != 4 && sd->status.weapon != 5) {
				clif_skill_fail(sd,sd->skillid,6,0);	// —¼è‘„
				return 0;
			}
			break;

		case CR_GRANDCROSS:
			hp = sd->status.hp/5;
			break;

		case MO_CALLSPIRITS:
			if(sd->spiritball >= ((sd->skilllv > 10)? 10:sd->skilllv)) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			break;

		case MO_EXTREMITYFIST:					// ˆ¢C—…”e–PŒ
			tick = gettick();
			if( sd->sc_data[SC_EXPLOSIONSPIRITS].timer == -1) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			else {
				if(sd->combo_delay3 <= tick && tick <= sd->combo_delay3 + battle_config.asuradelay) 
					spiritball = 4;				// Ÿ†‹…
				else spiritball = 5;			// Ÿ†‹…
			}
			sd->skill_old = 0;
			break;

		case MO_FINGEROFFENSIVE:				//w’e
			spiritball = sd->skilllv;			// Ÿ†‹…
			if (sd->spiritball > 0 && sd->spiritball < spiritball) {
				spiritball = sd->spiritball;
				sd->spiritball_old = sd->spiritball;	
			}
			else sd->spiritball_old = sd->skilllv;	
			break;

		case MO_INVESTIGATE:					//”­™¤
			spiritball = 1;						// Ÿ†‹…
			break;

		case MO_BODYRELOCATION:
			if(!pc_can_reach(sd,sd->skillx,sd->skilly)) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			} else
				spiritball = 1;					// Ÿ†‹…
			break;
		
		case MO_STEELBODY:						// ‹à„
		case MO_EXPLOSIONSPIRITS:				// ”š—ô”g“®
				spiritball = 5;					// Ÿ†‹…
			break;

		case MO_CHAINCOMBO:						//˜A‘Å¶
			tick = gettick();
			if(sd->combo_delay1 <= tick && tick <= sd->combo_delay1 + 300) {
				if(sd->skill_old == sd->skillid)
					return 0;
				sd->skill_old = sd->skillid;
				break;
			}
			else {
				sd->skill_old = 0;
				return 0;
			}
			break;

		case MO_COMBOFINISH:					//–Ò—´Œ
			tick = gettick();
			spiritball = 1;
			if(sd->combo_delay2 <= tick && tick <= sd->combo_delay2 + 300) {
				if(sd->skill_old == sd->skillid)
					return 0;
				sd->skill_old = sd->skillid;
				break;
			}
			else {
				sd->skill_old = 0;
				return 0;
			}
			break;

//		case RG_BACKSTAP:	// ƒoƒbƒNƒXƒ^ƒu
		case RG_RAID:		// ƒTƒvƒ‰ƒCƒYƒAƒ^ƒbƒN
			if(!pc_ishiding(sd)) {		// ƒnƒCƒfƒBƒ“ƒOó‘Ô
				clif_skill_fail(sd,sd->skillid,0,0);
				return 0;
			}
			break;

		case SA_CASTCANCEL:
			sp=skill_get_sp(sd->cast_skillid, sd->cast_skilllv) / 2;	/* Á”ïSP */
			if(sd->dsprate!=100)
				sp=sp*sd->dsprate/100;	/* Á”ïSPC³ */

			sd->cast_skillid = -1;
			sd->cast_skilllv = -1;
			break;

		case BA_MUSICALSTRIKE:
			if( sd->status.weapon != 13) {
				clif_skill_fail(sd,sd->skillid,6,0);	/* —¼èŒ• */
				return 0;
			}
			break;

		case BA_DISSONANCE:
		case BA_WHISTLE:
		case BA_ASSASSINCROSS:
		case BA_POEMBRAGI:
		case BA_APPLEIDUN:
			if(sd->sc_data[SkillStatusChangeTable[sd->skillid]].timer!=-1)
				sp=0;			/* ‰ğœ‚·‚éê‡‚ÍSPÁ”ï‚µ‚È‚¢ */
			break;

		case DC_THROWARROW:
			if( sd->status.weapon != 14) {
				clif_skill_fail(sd,sd->skillid,6,0);	/* —¼èŒ• */
				return 0;
			}
			break;

		case DC_UGLYDANCE:
		case DC_HUMMING:
		case DC_DONTFORGETME:
		case DC_FORTUNEKISS:
		case DC_SERVICEFORYOU:
			if(sd->sc_data[SkillStatusChangeTable[sd->skillid]].timer!=-1)
				sp=0;			/* ‰ğœ‚·‚éê‡‚ÍSPÁ”ï‚µ‚È‚¢ */
			break;

/*		case MG_FIREWALL:
			if(sd->skill_limit[MG_FIREWALL] < 5 )
				sd->skill_limit[MG_FIREWALL]++;
			else {
				clif_skill_fail(sd,sd->skillid,0,0);
				break;
			}
			break;
		case WZ_ICEWALL:
			if(sd->skill_limit[WZ_ICEWALL] < 10 && !(sd->bl.x == sd->skillx && sd->bl.y == sd->skilly))
				sd->skill_limit[WZ_ICEWALL]++;
			else {
				clif_skill_fail(sd,sd->skillid,0,0);
				break;
			}
			break;*/

	}

		if( sp>0 && sd->status.sp < sp) {				/* SPƒ`ƒFƒbƒN */
			clif_skill_fail(sd,sd->skillid,1,0);		/* SP•s‘«F¸”s’Ê’m */
			return 0;
		}
		
		if( spiritball > 0 && sd->spiritball < spiritball) {
			clif_skill_fail(sd,sd->skillid,0,0);		// Ÿ†‹…•s‘«
			return 0;
		}

		if	(item_id[0] || item_id[1] || item_id[2]) {
			for(j=0;j<3;j++) {
				// ƒAƒCƒeƒ€–³‚µA‚RíƒWƒFƒ€•ƒ~ƒXƒgƒŒƒXƒJ[ƒhA‚Í–³ˆ—
				if(!item_id[j] ||
					 ((item_id[j] == 715 || item_id[j] == 716
					  || item_id[j] == 717) && sd->special_state.no_gemstone) )
					continue;
				// [•£‚Ì’†‚É‚ÍƒWƒFƒ€‚Æã©‚ª‚¢‚ç‚È‚¢
				if( ((item_id[j] == 715 || item_id[j] == 716 ||
					 item_id[j] == 717 || item_id[j] == 1065 )
					 && sd->sc_data[SC_INTOABYSS].timer!=-1) )
					continue;

				if((i[j]=pc_search_inventory(sd,item_id[j])) == -1 ||
						sd->status.inventory[i[j]].amount < item_amount[j]) {	// ƒAƒCƒeƒ€‚È‚µ
					if(item_id[j] == 716 || item_id[j] == 717)		// Ô•ÂƒWƒFƒ€‚Ìê‡‚Ì‚İ
						clif_skill_fail(sd,sd->skillid,(6+(item_id[j]-715)),0);
					else
						clif_skill_fail(sd,sd->skillid,0,0);
					return 0;
				}
			}
			for(j=0;j<3;j++) {
				if(i[j] != 0) {
					pc_delitem(sd,i[j],item_amount[j],0);		// ƒAƒCƒeƒ€Á”ï
				}
			}
		}

		if(sp) {					// SPÁ”ï
			sd->status.sp-=sp;
			clif_updatestatus(sd,SP_SP);
		}
		if(hp) {					// HPÁ”ï
			sd->status.hp-=hp;
			clif_updatestatus(sd,SP_HP);
		}
		if(zeny) {					// ZenyÁ”ï
			sd->status.zeny -= zeny;
			clif_updatestatus(sd,SP_ZENY);
		}
		if(spiritball)				// Ÿ†‹…Á”ï
			pc_delspiritball(sd,spiritball,0);

		if (sd->skillid != BD_ENCORE) {
			sd->last_skillid= sd->skillid;
			sd->last_skilllv= sd->skilllv;
		}
		if (sd->cast_skillid) {
			sd->cast_skillid = -1;
			sd->cast_skilllv = -1;
		}
	}
	return 1;
}


/*==========================================
 * ‰r¥ŠÔŒvZ
 *------------------------------------------
 */
int skill_castfix( struct block_list *bl, int time )
{
	struct status_change *sc_data;
	int dex=battle_get_dex(bl);
	int castrate=100;
	if(time==0)
		return 0;
	if(bl->type==BL_PC)
		castrate=((struct map_session_data *)bl)->castrate;
	
	time=time*castrate*(150- dex)/15000;
	time=time*battle_config.cast_rate/100;
	
	/* ƒTƒtƒ‰ƒMƒEƒ€ */
	if( (sc_data = battle_get_sc_data(bl))[SC_SUFFRAGIUM].timer!=-1 ){
		time=time*(100-sc_data[SC_SUFFRAGIUM].val1*15)/100;
		skill_status_change_end( bl, SC_SUFFRAGIUM, -1);
	}
	/* ƒuƒ‰ƒM‚Ì */
	if( (sc_data = battle_get_sc_data(bl))[SC_POEMBRAGI].timer!=-1 )
		time=time*(100-sc_data[SC_POEMBRAGI].val2)/100;

	return (time>0)?time:0;
}
/*==========================================
 * ƒfƒBƒŒƒCŒvZ
 *------------------------------------------
 */
int skill_delayfix( struct block_list *bl, int time )
{
	struct status_change *sc_data;
	if(time<=0)
		return 0;
	
	if( battle_config.delay_dependon_dex )	/* dex‚Ì‰e‹¿‚ğŒvZ‚·‚é */
		time=time*(150- battle_get_dex(bl))/150;

	time=time*battle_config.delay_rate/100;

	/* ƒuƒ‰ƒM‚Ì */
	if( (sc_data = battle_get_sc_data(bl))[SC_POEMBRAGI].timer!=-1 )
		time=time*(100-sc_data[SC_POEMBRAGI].val2)/100;
	
	return (time>0)?time:0;
}
/*==========================================
 * ƒXƒLƒ‹g—piIDw’èj
 *------------------------------------------
 */
int skill_use_id( struct map_session_data *sd, int target_id,
	int skill_num, int skill_lv)
{
	unsigned int tick;
	int casttime=0,delay=0,skill;
	struct map_session_data* target_sd=NULL;
	int target_fd=-1;
	int forcecast=0;
	struct block_list *bl;
	tick=gettick();

	bl=map_id2bl(target_id);
	if(bl==NULL){
/*		printf("skill target not found %d\n",target_id); */
		return 0;
	}
	if(sd->bl.m != bl->m || pc_isdead(sd))
		return 0;

	/* ’¾–Ù‚âˆÙíi‚½‚¾‚µAƒOƒŠƒ€‚È‚Ç‚Ì”»’è‚ğ‚·‚éj */
	if( sd->opt1>0 || sd->status.option&6 ||
		 sd->sc_data[SC_DIVINA].timer!=-1 || sd->sc_data[SC_ROKISWEIL].timer!=-1 ){
		if( (sd->status.option&4) && skill_num==AS_CLOAKING );	/* ƒNƒ[ƒLƒ“ƒO’† */
		else if( (sd->status.option&2) && (skill_num==TF_HIDING || skill_num==AS_GRIMTOOTH || skill_num==RG_BACKSTAP || skill_num==RG_RAID ));	/* ƒnƒCƒfƒBƒ“ƒO’† */
		else
			return 0;
	}

	/* ‰‰‘t/ƒ_ƒ“ƒX’† */
	if( skill_check_dancing(&sd->bl) && skill_num!=BD_ADAPTATION &&
		skill_num!=BA_MUSICALSTRIKE && skill_num!=DC_THROWARROW ){
		printf("dancing! %d\n",skill_num);
		return 0;
	}

	/* ‘é‚ª‚¢‚È‚¢ */
	if( skill_num==HT_BLITZBEAT && !(sd->status.option&0x10) ){	/* ‘é‚ª‚¢‚È‚¢ */
		return 0;
	}

	/* Ë’ö‚ÆáŠQ•¨ƒ`ƒFƒbƒN */
	if(!battle_check_range(&sd->bl,bl->x,bl->y,skill_get_range(skill_num, skill_lv)))
		return 0;

	if(bl->type==BL_PC){
		target_sd=(struct map_session_data*)bl;
		target_fd=target_sd->fd;
	}
	if(skill_num != MO_CHAINCOMBO && skill_num != MO_COMBOFINISH)
		pc_stopattack(sd);

	casttime=skill_castfix(&sd->bl, skill_get_cast( skill_num,skill_lv) );
	delay=skill_delayfix(&sd->bl, skill_get_delay( skill_num,skill_lv) );

	sd->state.skillcastcancel=1;

	switch(skill_num){	/* ‰½‚©“Áê‚Èˆ—‚ª•K—v */
	case AL_HEAL:	/* ƒq[ƒ‹ */
		if( battle_get_elem_type(bl)==9)
			forcecast=1;	/* ƒq[ƒ‹ƒAƒ^ƒbƒN‚È‚ç‰r¥ƒGƒtƒFƒNƒg—L‚è */
		break;
	case ALL_RESURRECTION:	/* ƒŠƒUƒŒƒNƒVƒ‡ƒ“ */
		if( battle_get_elem_type(bl)==9){	/* “G‚ªƒAƒ“ƒfƒbƒh‚È‚ç */
			forcecast=1;	/* ƒ^[ƒ“ƒAƒ“ƒfƒbƒg‚Æ“¯‚¶‰r¥ŠÔ */
			casttime=skill_castfix(&sd->bl, skill_get_cast(PR_TURNUNDEAD,skill_lv) );
		}
		break;
	/* ‰r¥ƒLƒƒƒ“ƒZƒ‹‚³‚ê‚È‚¢‚à‚Ì */
	case KN_BRANDISHSPEAR:	/* ƒuƒ‰ƒ“ƒfƒBƒbƒVƒ…ƒXƒsƒA */
	case KN_BOWLINGBASH:	/* ƒ{ƒEƒŠƒ“ƒOƒoƒbƒVƒ… */
	case AC_CHARGEARROW:	/* ƒ`ƒƒ[ƒWƒAƒ[ */
	case RG_STRIPWEAPON:	/* ƒXƒgƒŠƒbƒvƒEƒGƒ|ƒ“ */
	case RG_STRIPSHIELD:	/* ƒXƒgƒŠƒbƒvƒV[ƒ‹ƒh */
	case RG_STRIPARMOR:	/* ƒXƒgƒŠƒbƒvƒA[ƒ}[ */
	case RG_STRIPHELM:	/* ƒXƒgƒŠƒbƒvƒwƒ‹ƒ€ */
	case CR_GRANDCROSS:	/* ƒOƒ‰ƒ“ƒhƒNƒƒX */
	case MO_CALLSPIRITS:	// ‹CŒ÷
	case MO_INVESTIGATE:	/* ”­™¤ */
	case MO_STEELBODY:	/* ‹à„*/
		sd->state.skillcastcancel=0;
		break;
	case MO_FINGEROFFENSIVE:	/* w’e */
		casttime += casttime * ((skill_lv > sd->spiritball)? sd->spiritball:skill_lv);
		sd->state.skillcastcancel=0;
		break;
	case MO_CHAINCOMBO:		/*˜A‘Å¶*/
		sd->state.skillcastcancel=0;
		target_id = sd->attacktarget;
		break;
	case MO_COMBOFINISH:		/*–Ò—´Œ*/
		sd->state.skillcastcancel=0;
		target_id = sd->attacktarget;
		break;
	case MO_EXTREMITYFIST:	/*ˆ¢C—…”e–PŒ*/
		if(sd->combo_delay3 <= tick && tick <= sd->combo_delay3 + battle_config.asuradelay) {
			casttime = 0;
		}
		break;	
	}

	printf("PC %d skill use target_id=%d skill=%d lv=%d cast=%d\n"
		,sd->bl.id,target_id,skill_num,skill_lv,casttime);

	if(sd->skillitem == skill_num)
		casttime = delay = 0;

	if( casttime>0 || forcecast ){ /* ‰r¥‚ª•K—v */
		struct mob_data *md;
		clif_skillcasting( &sd->bl,
			sd->bl.id, target_id, 0,0, skill_num,casttime);

		/* ‰r¥”½‰ƒ‚ƒ“ƒXƒ^[ */
		if( bl->type==BL_MOB && mob_db[(md=(struct mob_data *)bl)->class].mode&0x10 &&
			md->state.state!=MS_ATTACK && sd->ghost_timer == -1){
				md->target_id=sd->bl.id;
				md->state.targettype = ATTACKABLE;
				md->min_chase=13;
		}
		sd->cast_skillid = skill_num;
		sd->cast_skilllv = skill_lv;
	}

	if( casttime<=0 )	/* ‰r¥‚Ì–³‚¢‚à‚Ì‚ÍƒLƒƒƒ“ƒZƒ‹‚³‚ê‚È‚¢ */
		sd->state.skillcastcancel=0;

	sd->skilltarget	= target_id;
/*	sd->cast_target_bl	= bl; */
	sd->skillx		= 0;
	sd->skilly		= 0;
	sd->skillid		= skill_num;
	sd->skilllv		= skill_lv;
	sd->canact_tick = tick + casttime + delay;
	sd->canmove_tick = tick;
	if(casttime > 0) {
		sd->skilltimer = add_timer( tick+casttime, skill_castend_id, sd->bl.id, 0 );
		if((skill = pc_checkskill(sd,SA_FREECAST)) > 0) {
			sd->prev_speed = sd->speed;
			sd->speed = sd->speed*(175 - skill*5)/100;
			clif_updatestatus(sd,SP_SPEED);
		}
		else
			pc_stop_walking(sd,0);
	}
	else {
		sd->skilltimer = -1;
		skill_castend_id(sd->skilltimer,tick,sd->bl.id,0);
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹g—piêŠw’èj
 *------------------------------------------
 */
int skill_use_pos( struct map_session_data *sd,
	int skill_x, int skill_y, int skill_num, int skill_lv)
{
	unsigned int tick;
	int casttime=0,delay=0,skill;
	
	if(pc_isdead(sd))
		return 0;

	if( sd->opt1>0 || sd->status.option&6 ||
		 sd->sc_data[SC_DIVINA].timer!=-1 || sd->sc_data[SC_ROKISWEIL].timer!=-1 )
		return 0;	/* ˆÙí‚â’¾–Ù‚È‚Ç */

	/* ‰‰‘t/ƒ_ƒ“ƒX’†‚©ƒ`ƒFƒbƒN */
	if( skill_check_dancing(&sd->bl) )
		return 0;

	/* Ë’ö‚ÆáŠQ•¨ƒ`ƒFƒbƒN */
	if(!battle_check_range(&sd->bl,skill_x,skill_y,skill_get_range(skill_num, skill_lv)))
		return 0;
	
	pc_stopattack(sd);

	casttime=skill_castfix(&sd->bl, skill_get_cast( skill_num,skill_lv) );
	delay=skill_delayfix(&sd->bl, skill_get_delay( skill_num,skill_lv) );

	sd->state.skillcastcancel=1;

	printf("PC %d skill use target_pos=(%d,%d) skill=%d lv=%d cast=%d\n"
		,sd->bl.id,skill_x,skill_y,skill_num,skill_lv,casttime);

	if(sd->skillitem == skill_num)
		casttime = delay = 0;

	if( casttime>0 )	/* ‰r¥‚ª•K—v */
		clif_skillcasting( &sd->bl,
			sd->bl.id, 0, skill_x,skill_y, skill_num,casttime);

	if( casttime<=0 )	/* ‰r¥‚Ì–³‚¢‚à‚Ì‚ÍƒLƒƒƒ“ƒZƒ‹‚³‚ê‚È‚¢ */
		sd->state.skillcastcancel=0;

	sd->skillx			= skill_x;
	sd->skilly			= skill_y;
	sd->skilltarget	= 0;
/*	sd->cast_target_bl	= NULL; */
	sd->skillid		= skill_num;
	sd->skilllv			= skill_lv;
	tick=gettick();
	sd->canact_tick = tick + casttime + delay;
	sd->canmove_tick = tick;
	if(casttime > 0) {
		sd->skilltimer = add_timer( tick+casttime, skill_castend_pos, sd->bl.id, 0 );
		if((skill = pc_checkskill(sd,SA_FREECAST)) > 0) {
			sd->prev_speed = sd->speed;
			sd->speed = sd->speed*(175 - skill*5)/100;
			clif_updatestatus(sd,SP_SPEED);
		}
		else
			pc_stop_walking(sd,0);
	}
	else {
		sd->skilltimer = -1;
		skill_castend_pos(sd->skilltimer,tick,sd->bl.id,0);
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹‰r¥ƒLƒƒƒ“ƒZƒ‹
 *------------------------------------------
 */
int skill_castcancel( struct block_list *bl )
{
	if(bl->type==BL_PC){
		struct map_session_data *sd=(struct map_session_data *)bl;
		unsigned long tick=gettick();
		sd->canact_tick=tick;
		sd->canmove_tick = tick;
		if( sd->skilltimer!=-1){
			if(pc_checkskill(sd,SA_FREECAST) > 0) {
				sd->speed = sd->prev_speed;
				clif_updatestatus(sd,SP_SPEED);
			}
			if( skill_get_inf( sd->skillid )&2 )
				delete_timer( sd->skilltimer, skill_castend_pos );
			else
				delete_timer( sd->skilltimer, skill_castend_id );
			sd->skilltimer=-1;
			clif_skillcastcancel(bl);
		}

		return 0;
	}else if(bl->type==BL_MOB){
		struct mob_data *md=(struct mob_data *)bl;
		if( md->skilltimer!=-1 ){
			if( skill_get_inf( md->skillid )&2 )
				delete_timer( md->skilltimer, mobskill_castend_pos );
			else
				delete_timer( md->skilltimer, mobskill_castend_id );
			md->skilltimer=-1;
			clif_skillcastcancel(bl);
		}
		return 0;
	}
	return 1;
}




/*----------------------------------------------------------------------------
 * ƒXƒe[ƒ^ƒXˆÙí
 *----------------------------------------------------------------------------
 */

/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙíƒ^ƒCƒ}[”ÍˆÍˆ—
 *------------------------------------------
 */
int skill_status_change_timer_sub(struct block_list *bl, va_list ap )
{
	struct block_list *src;
	int type;
	unsigned int tick;
	src=va_arg(ap,struct block_list*);
	type=va_arg(ap,int);
	tick=va_arg(ap,unsigned int);
	
	if(bl->type!=BL_PC && bl->type!=BL_MOB)
		return 0;
	
	switch( type ){
	case SC_SIGHT:	/* ƒTƒCƒg */
	case SC_RUWACH:	/* ƒ‹ƒAƒt */
		if( (*battle_get_option(bl))&6 ){
			skill_status_change_end( bl, SC_HIDDING, -1);
			skill_status_change_end( bl, SC_CLOAKING, -1);
			if( type==SC_RUWACH && battle_check_target( src,bl, BCT_ENEMY ) > 0)
				skill_attack(BF_MAGIC,src,src,bl,type,1,tick,0);
		}

		break;
	}
	return 0;
}

/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙíI—¹
 *------------------------------------------
 */
int skill_status_change_end( struct block_list* bl , int type,int tid )
{
	struct status_change* sc_data;
	int opt_flag=0;
	short *sc_count, *option, *opt1, *opt2;
	
	sc_data=battle_get_sc_data(bl);
	sc_count=battle_get_sc_count(bl);
	option=battle_get_option(bl);
	opt1=battle_get_opt1(bl);
	opt2=battle_get_opt2(bl);
	
	if(bl->type!=BL_PC && bl->type!=BL_MOB) {
		printf("skill_status_change_end: neither MOB nor PC !\n");
		return 0;
	}
	
	
	if((*sc_count)>0 && sc_data[type].timer!=-1 &&
		(sc_data[type].timer==tid || tid==-1) ){
		
		if(tid==-1)	/* ƒ^ƒCƒ}‚©‚çŒÄ‚Î‚ê‚Ä‚¢‚È‚¢‚È‚çƒ^ƒCƒ}íœ‚ğ‚·‚é */
			delete_timer(sc_data[type].timer,skill_status_change_timer);
	
		/* ŠY“–‚ÌˆÙí‚ğ³í‚É–ß‚· */
		sc_data[type].timer=-1;
		(*sc_count)--;

		if(bl->type==BL_PC){
			int ii = -1;
			if (type < 64) ii=type;
			else if (type == SC_EXPLOSIONSPIRITS)	ii = 0x56;
			else if (type == SC_STEELBODY)			ii = 0x57;
			else if (type == SC_SPEARSQUICKEN)		ii = 0x44;
			else if (type == SC_FLAMELAUNCHER)		ii = 0x5a;
			else if (type == SC_FROSTWEAPON)		ii = 0x5b;
			else if (type == SC_LIGHTNINGLOADER)	ii = 0x5c;
			else if (type == SC_SEISMICWEAPON)		ii = 0x5d;
			if (ii != -1)
				clif_status_change(bl,ii,0);	/* ƒAƒCƒRƒ“Á‹ */
		}

		switch(type){	/* ³í‚É–ß‚é‚Æ‚«‚È‚É‚©ˆ—‚ª•K—v */
		case SC_STONE:
		case SC_FREEZE:
		case SC_STAN:
		case SC_SLEEP:
			*opt1=0;
			opt_flag=1;
			break;

		case SC_POISON:
		case SC_CURSE:
		case SC_SILENCE:
		case SC_BLIND:
			*opt2&=~(1<<(type-SC_POISON));
			opt_flag=1;
			break;

		case SC_HIDDING:
		case SC_CLOAKING:
			*option&=~((type==SC_HIDDING)?2:4);
			opt_flag =1 ;
			break;

		case SC_SIGHT:
			*option&=~1;
			opt_flag=1;
			break;
		case SC_RUWACH:
			*option&=~8192;
			opt_flag=1;
			break;
		}

		if(opt_flag)	/* option‚Ì•ÏX‚ğ“`‚¦‚é */
			clif_changeoption(bl);

		if(bl->type==BL_PC)
			pc_calcstatus((struct map_session_data *)bl,0);	/* ƒXƒe[ƒ^ƒXÄŒvZ */
	}

	return 0;
}
/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙíI—¹ƒ^ƒCƒ}[
 *------------------------------------------
 */
int skill_status_change_timer(int tid, unsigned int tick, int id, int data)
{
	int type=data;
	struct block_list *bl;
	struct map_session_data *sd=NULL;
	struct status_change *sc_data;
	short *sc_count;
	
	bl=map_id2bl(id);
	if(bl==NULL)
		return 0;
	
	if(bl->type==BL_PC)
		sd=(struct map_session_data *)bl;

	sc_data=battle_get_sc_data(bl);
	sc_count=battle_get_sc_count(bl);
	
	switch(type){	/* “Áê‚Èˆ—‚É‚È‚éê‡ */
	case SC_MAXIMIZEPOWER:	/* ƒ}ƒLƒVƒ}ƒCƒYƒpƒ[ */
	case SC_CLOAKING:		/* ƒNƒ[ƒLƒ“ƒO */
		if(sd){
			if( sd->status.sp > 0 ){	/* SPØ‚ê‚é‚Ü‚Å‘± */
				sd->status.sp--;
				clif_updatestatus(sd,SP_SP);
				sc_data[type].timer=add_timer(	/* ƒ^ƒCƒ}[Äİ’è */
					sc_data[type].val2+tick, skill_status_change_timer,
					bl->id, data);
				return 0;
			}
		}
		break;

	case SC_HIDDING:		/* ƒnƒCƒfƒBƒ“ƒO */
		if(sd){		/* SP‚ª‚ ‚Á‚ÄAŠÔ§ŒÀ‚ÌŠÔ‚Í‘± */
			if( sd->status.sp > 0 && (--sc_data[type].val2)>0 ){
				if(sc_data[type].val2 % (sc_data[type].val1+3) ==0 ){
					sd->status.sp--;
					clif_updatestatus(sd,SP_SP);
				}
				sc_data[type].timer=add_timer(	/* ƒ^ƒCƒ}[Äİ’è */
					1000+tick, skill_status_change_timer,
					bl->id, data);
				return 0;
			}
		}
		break;

	case SC_SIGHT:	/* ƒTƒCƒg */
	case SC_RUWACH:	/* ƒ‹ƒAƒt */
		{
			const int range=AREA_SIZE-5;
			map_foreachinarea( skill_status_change_timer_sub,
				bl->m, bl->x-range, bl->y-range, bl->x+range,bl->y+range,0,
				bl,type,tick);
	
			if( (--sc_data[type].val2)>0 ){
				sc_data[type].timer=add_timer(	/* ƒ^ƒCƒ}[Äİ’è */
					250+tick, skill_status_change_timer,
					bl->id, data);
				return 0;
			}
		}
		break;

	case SC_PROVOKE:	/* ƒvƒƒ{ƒbƒN/ƒI[ƒgƒo[ƒT[ƒN */
		if(sc_data[type].val2!=0){	/* ƒI[ƒgƒo[ƒT[ƒNi‚P•b‚²‚Æ‚ÉHPƒ`ƒFƒbƒNj */
			if(sd!=NULL && sd->status.hp>sd->status.max_hp/4)	/* ’â~ */
				break;
			if(sc_data[type].timer==tid)
				sc_data[type].timer=add_timer( 1000+tick,
					skill_status_change_timer, bl->id, data );
		}
		break;

	case SC_WATERBALL:	/* ƒEƒH[ƒ^[ƒ{[ƒ‹ */
		{
			struct block_list *target=map_id2bl(sc_data[type].val2);
			if(target==NULL || target->prev==NULL)
				break;
			skill_attack(BF_MAGIC,bl,bl,target,86,sc_data[type].val1,tick,0);
			if((--sc_data[type].val3)>0)
				sc_data[type].timer=add_timer( 100+tick,
					skill_status_change_timer, bl->id, data );
		}
		break;

	case SC_ENDURE:	/* ƒCƒ“ƒfƒ…ƒA */
		if(sd && sd->special_state.infinite_endure) {
			if(sc_data[type].timer==tid) {
				sc_data[type].timer=add_timer( 1000*600+tick,
					skill_status_change_timer, bl->id, data );
				sc_data[type].val2=1;
			}
			return 0;
		}
		break;

	case SC_DISSONANCE:	/* •s‹¦˜a‰¹ */
		if( (--sc_data[type].val2)>0){
			struct skill_unit_group *group=
				(struct skill_unit_group *)sc_data[type].val4;
			if(!group)
				break;
			skill_attack(BF_MISC,(struct block_list *)&group->unit[0],
				(struct block_list *)&group->unit[0],
				bl,group->skill_id,sc_data[type].val1,tick,0);			
			if(sc_data[type].timer==tid)
				sc_data[type].timer=add_timer( 3000+tick,
					skill_status_change_timer, bl->id, data );			
		}break;
		
	case SC_LULLABY:	/* qç‰S */
		if( (--sc_data[type].val2)>0){
			skill_additional_effect(bl,bl,
				BD_LULLABY,sc_data[type].val1,BF_LONG|BF_SKILL|BF_MISC,tick);
			if(sc_data[type].timer==tid)
				sc_data[type].timer=add_timer( 6000+tick,
					skill_status_change_timer, bl->id, data );			
		}break;
	
	/* ŠÔØ‚ê–³‚µHH */
	case SC_AETERNA:
	case SC_TRICKDEAD:
	case SC_STONE:
/*	case SC_BLIND: */
	case SC_RIDING:
	case SC_FALCON:
	case SC_WEIGHT50:
	case SC_WEIGHT90:
		if(sc_data[type].timer==tid)
			sc_data[type].timer=add_timer( 1000*600+tick,
				skill_status_change_timer, bl->id, data );
		return 0;
	}

	return skill_status_change_end( bl,type,tid );
}

/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙíŠJn
 *------------------------------------------
 */
int skill_encchant_eremental_end(struct block_list *bl,int type)
{
	struct status_change *sc_data=battle_get_sc_data(bl);

	if( type!=SC_ENCPOISON && sc_data[SC_ENCPOISON].timer!=-1 )	/* ƒGƒ“ƒ`ƒƒƒ“ƒgƒ|ƒCƒYƒ“‰ğœ */
		skill_status_change_end(bl,SC_ENCPOISON,-1);
	if( type!=SC_ASPERSIO && sc_data[SC_ASPERSIO].timer!=-1 )	/* ƒAƒXƒyƒ‹ƒVƒI‰ğœ */
		skill_status_change_end(bl,SC_ASPERSIO,-1);
	if( type!=SC_FLAMELAUNCHER && sc_data[SC_FLAMELAUNCHER].timer!=-1 )	/* ƒtƒŒƒCƒ€ƒ‰ƒ“ƒ`ƒƒ‰ğœ */
		skill_status_change_end(bl,SC_FLAMELAUNCHER,-1);
	if( type!=SC_FROSTWEAPON && sc_data[SC_FROSTWEAPON].timer!=-1 )	/* ƒtƒƒXƒgƒEƒFƒ|ƒ“‰ğœ */
		skill_status_change_end(bl,SC_FROSTWEAPON,-1);
	if( type!=SC_LIGHTNINGLOADER && sc_data[SC_LIGHTNINGLOADER].timer!=-1 )	/* ƒ‰ƒCƒgƒjƒ“ƒOƒ[ƒ_[‰ğœ */
		skill_status_change_end(bl,SC_LIGHTNINGLOADER,-1);
	if( type!=SC_SEISMICWEAPON && sc_data[SC_SEISMICWEAPON].timer!=-1 )	/* ƒTƒCƒXƒ~ƒbƒNƒEƒFƒ|ƒ“‰ğœ */
		skill_status_change_end(bl,SC_SEISMICWEAPON,-1);

	return 0;
}
/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙíŠJn
 *------------------------------------------
 */
int skill_status_change_start(struct block_list *bl,int type,int val1,int val2)
{
	struct map_session_data *sd = NULL;
	struct mob_data *md = NULL;
	struct status_change* sc_data;
	unsigned int tick = 0;
	short *sc_count, *option, *opt1, *opt2;
	int opt_flag = 0;
	int val3=0,val4=val2;
	
	sc_data=battle_get_sc_data(bl);
	sc_count=battle_get_sc_count(bl);
	option=battle_get_option(bl);
	opt1=battle_get_opt1(bl);
	opt2=battle_get_opt2(bl);

	if(bl->type==BL_MOB){
		md=(struct mob_data *)bl;
		if( (mob_db[md->class].mode & 0x20 || mob_db[md->class].mexp > 0) &&
			(type==SC_STONE || type==SC_FREEZE || type==SC_STAN || type==SC_SLEEP)){
			/* ƒ{ƒX‚É‚ÍŒø‚©‚È‚¢ */
			return 0;
		}
	}else if(bl->type==BL_PC){
		sd=(struct map_session_data *)bl;

		if(SC_STONE<=type && type<=SC_BLIND){	/* ƒJ[ƒh‚É‚æ‚é‘Ï« */
			if(sd->reseff[type-SC_STONE] && rand()%100<sd->reseff[type-SC_STONE]){
				printf("PC %d skill_sc_start: card‚É‚æ‚éˆÙí‘Ï«”­“®\n",sd->bl.id);
				return 0;
			}
		}
	}else{
		printf("skill_status_change_start: neither MOB nor PC !\n");
		return 0;
	}
	if(type==SC_STONE || type==SC_FREEZE || type==SC_STAN || type==SC_SLEEP)
		battle_stopwalking(bl,1);

	if(sc_data[type].timer != -1){	/* ‚·‚Å‚É“¯‚¶ˆÙí‚É‚È‚Á‚Ä‚¢‚éê‡ƒ^ƒCƒ}‰ğœ */
		(*sc_count)--;
		delete_timer(sc_data[type].timer, skill_status_change_timer);
		sc_data[type].timer = -1;
	}
	
	switch(type){	/* ˆÙí‚Ìí—Ş‚²‚Æ‚Ìˆ— */
		case SC_PROVOKE:			/* ƒvƒƒ{ƒbƒN */
			if(val2==0)	tick = 1000 * 30;
			else		tick = 1000;/* (ƒI[ƒgƒo[ƒT[ƒN) */
			break;
		case SC_ENDURE:				/* ƒCƒ“ƒfƒ…ƒA */
			tick = 1000 * (7 + val1*3);
			break;
		case SC_CONCENTRATE:		/* W’†—ÍŒüã */
			tick = 1000 * (40 + val1 * 20);
			break;
		case SC_BLESSING:			/* ƒuƒŒƒbƒVƒ“ƒO */
			tick = 1000 * (40 + val1 * 20);
			if(md && battle_get_elem_type(bl) != 9 && battle_get_race(bl) != 6) {
				if(sc_data[SC_CURSE].timer!=-1 )
					skill_status_change_end(bl,SC_CURSE,-1);
				if(sc_data[SC_STONE].timer!=-1 )
					skill_status_change_end(bl,SC_STONE,-1);
			}
			break;
		case SC_ANGELUS:			/* ƒAƒ“ƒ[ƒ‹ƒX */
			tick = 1000 * (30 * val1);
			break;
		case SC_INCREASEAGI:		/* ‘¬“xã¸ */
			tick = 1000 * (40 + val1 * 20);
			if(sc_data[SC_DECREASEAGI].timer!=-1 )
				skill_status_change_end(bl,SC_DECREASEAGI,-1);
			break;
		case SC_DECREASEAGI:		/* ‘¬“xŒ¸­ */
			tick = 1000 * (30 + val1 * 10);
			if(sc_data[SC_INCREASEAGI].timer!=-1 )
				skill_status_change_end(bl,SC_INCREASEAGI,-1);
			break;
		case SC_TWOHANDQUICKEN:		/* 2HQ */
			tick = 1000 * (30 * val1);
			break;
		case SC_ADRENALINE:			/* ƒAƒhƒŒƒiƒŠƒ“ƒ‰ƒbƒVƒ… */
			tick = 1000 * (30 * val1);
			break;
		case SC_WEAPONPERFECTION:	/* ƒEƒFƒ|ƒ“ƒp[ƒtƒFƒNƒVƒ‡ƒ“ */
			tick = 1000 * (30 * val1);
			break;
		case SC_OVERTHRUST:			/* ƒI[ƒo[ƒXƒ‰ƒXƒg */
			tick = 1000 * (20 * val1);
			break;
		case SC_MAXIMIZEPOWER:		/* ƒ}ƒLƒVƒ}ƒCƒYƒpƒ[(SP‚ª1Œ¸‚éŠÔ,val2‚É‚à) */
			tick = 1000 * val1;
			val2 = tick;
			break;
		case SC_ENCPOISON:			/* ƒGƒ“ƒ`ƒƒƒ“ƒgƒ|ƒCƒYƒ“ */
			tick = 1000 * (15 + val1 * 15);
			val2=((val1 - 1) / 2) + 3;	/* “Å•t—^Šm—¦ */
			skill_encchant_eremental_end(bl,SC_ENCPOISON);
			break;
		case SC_IMPOSITIO:			/* ƒCƒ“ƒ|ƒVƒeƒBƒIƒ}ƒkƒX */
			tick = 1000 * 60;
			break;
		case SC_SUFFRAGIUM:			/* ƒTƒtƒ‰ƒMƒ€ */
			tick = 1000 * (40 - val1 * 10);
			break;
		case SC_ASPERSIO:			/* ƒAƒXƒyƒ‹ƒVƒI */
			tick = 1000 * (30 + val1 * 30);
			skill_encchant_eremental_end(bl,SC_ASPERSIO);
			break;
		case SC_BENEDICTIO:			/* ¹‘Ì */
			tick = 1000 * 40 * val1;
			break;
		case SC_KYRIE:				/* ƒLƒŠƒGƒGƒŒƒCƒ\ƒ“ */
			tick = 1000 * 120;
			val2 = ((sd)?sd->status.max_hp:mob_db[md->class].max_hp) *
				 (val1 * 2 + 10) / 100;/* ‘Ï‹v“x */
			val1 = (val1 / 2 + 5);	/* ‰ñ” */
			break;
		case SC_MAGNIFICAT:			/* ƒ}ƒOƒjƒtƒBƒJ[ƒg */
			tick = 1000 * 20 * val1;
			break;
		case SC_GLORIA:				/* ƒOƒƒŠƒA */
			tick = 1000 * (5 + val1 * 5);
			break;
		case SC_AETERNA:			/* ƒG[ƒeƒ‹ƒi */
			tick = 1000 * 600;		/* ‚Æ‚è‚ ‚¦‚¸10•ª‚É‚µ‚Ä‚İ‚éiŒã‚Íƒ^ƒCƒ}ˆ—‚Å‰„’·‚·‚éj */
			break;
		case SC_ENERGYCOAT:			/* ƒGƒiƒW[ƒR[ƒg */
			tick = 1000 * 6 * 50;
			break;
		case SC_LOUD:				/* ƒ‰ƒEƒhƒ{ƒCƒX */
			tick = 1000 * 60 * 5;
			break;
		case SC_TRICKDEAD:			/* €‚ñ‚¾‚Ó‚è */
			tick = 1000 * 600;		/* ‚Æ‚è‚ ‚¦‚¸10•ª‚É‚µ‚Ä‚İ‚éiŒã‚Íƒ^ƒCƒ}ˆ—‚Å‰„’·‚·‚éj */
			break;
		case SC_QUAGMIRE:			/* ƒNƒ@ƒOƒ}ƒCƒA */
			tick = 5000 * val1;
			if(sc_data[SC_CONCENTRATE].timer!=-1 )	/* W’†—ÍŒüã‰ğœ */
				skill_status_change_end(bl,SC_CONCENTRATE,-1);
			if(sc_data[SC_INCREASEAGI].timer!=-1 )	/* ‘¬“xã¸‰ğœ */
				skill_status_change_end(bl,SC_INCREASEAGI,-1);
			if(sc_data[SC_TWOHANDQUICKEN].timer!=-1 )
				skill_status_change_end(bl,SC_TWOHANDQUICKEN,-1);
			if(sc_data[SC_SPEARSQUICKEN].timer!=-1 )
				skill_status_change_end(bl,SC_SPEARSQUICKEN,-1);
			if(sc_data[SC_ADRENALINE].timer!=-1 )
				skill_status_change_end(bl,SC_ADRENALINE,-1);
			if(sc_data[SC_LOUD].timer!=-1 )
				skill_status_change_end(bl,SC_LOUD,-1);
			break;
		case SC_SIGNUMCRUCIS:		/* ƒVƒOƒiƒ€ƒNƒ‹ƒVƒX */
			tick = 5000 * 60 * 25;
			break;
		case SC_FLAMELAUNCHER:		/* ƒtƒŒ[ƒ€ƒ‰ƒ“ƒ`ƒƒ[ */
			tick = 1000 * (15 + val1 * 15);
			skill_encchant_eremental_end(bl,SC_FLAMELAUNCHER);
			break;
		case SC_FROSTWEAPON:		/* ƒtƒƒXƒgƒEƒFƒ|ƒ“ */
			tick = 1000 * (15 + val1 * 15);
			skill_encchant_eremental_end(bl,SC_FROSTWEAPON);
			break;
		case SC_LIGHTNINGLOADER:	/* ƒ‰ƒCƒgƒjƒ“ƒOƒ[ƒ_[ */
			tick = 1000 * (15 + val1 * 15);
			skill_encchant_eremental_end(bl,SC_LIGHTNINGLOADER);
			break;
		case SC_SEISMICWEAPON:		/* ƒTƒCƒYƒ~ƒbƒNƒEƒFƒ|ƒ“ */
			tick = 1000 * (15 + val1 * 15);
			skill_encchant_eremental_end(bl,SC_SEISMICWEAPON);
			break;
		case SC_PROVIDENCE:			/* ƒvƒƒ”ƒBƒfƒ“ƒX */
			tick = 1000 * 180;
			val2=val1*5;
			break;
		case SC_DEFENDER:			/* ƒfƒBƒtƒFƒ“ƒ_[ */
			tick = 1000 * 180;
			val2=val1*15+5;
			break;
		case SC_SPEARSQUICKEN:		/* ƒXƒsƒAƒNƒCƒbƒPƒ“ */
			tick = 1000 * 300;
			val2 = 20+val1;
			break;
		
		case SC_LULLABY:			/* qç‰S */
			tick = 1000 * 6;
			val2 = 11;
			break;
		case SC_ETERNALCHAOS:		/* ƒGƒ^[ƒiƒ‹ƒJƒIƒX */
			tick = 1000* 60;
			break;
		case SC_DRUMBATTLE:			/* í‘¾ŒÛ‚Ì‹¿‚« */
			tick = 1000 * 60;
			val2 = (val1+1)*25;
			val3 = (val1+1)*2;
			break;
		case SC_NIBELUNGEN:			/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
			tick = 1000 * 60;
			val2 = (val1+2)*50;
			break;
		case SC_ROKISWEIL:			/* ƒƒL‚Ì‹©‚Ñ */
			tick = 1000 * 60;
			break;
		case SC_INTOABYSS:			/* [•£‚Ì’†‚É */
			tick = 1000 * 60;
			break;
		case SC_SIEGFRIED:			/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
			tick = 1000 * 60;
			val2 = (val1+3)*10;
			break;
		case SC_DISSONANCE:			/* •s‹¦˜a‰¹ */
			tick = 1000 * 3;
			val2 = 10;
			val3 = (val1+3)*10;
			break;
		case SC_WHISTLE:			/* Œû“J */
			tick = 1000 * 60;
			break;
		case SC_ASSNCROS:			/* —[—z‚ÌƒAƒTƒVƒ“ƒNƒƒX */
			tick = 1000 * 120;
			val2 = 10+val1;
			break;
		case SC_POEMBRAGI:			/* ƒuƒ‰ƒM‚Ì */
			tick = 1000 * 180;
			val2 = val1 * 3;
		case SC_APPLEIDUN:			/* ƒCƒhƒDƒ“‚Ì—ÑŒç */
			tick = 1000 * 60 * 3;
			val2 = val1*2+5;
			break;
		case SC_UGLYDANCE:			/* ©•ªŸè‚Èƒ_ƒ“ƒX */
			tick = 1000 * 3;
			val2 = 10;
			val3 = (val1+1)*5;
			break;
		case SC_HUMMING:			/* ƒnƒ~ƒ“ƒO */
			tick = 1000 * 60;
			val2 = val1*2;
			break;
		case SC_DONTFORGETME:		/* „‚ğ–Y‚ê‚È‚¢‚Å */
			tick = 1000 * 180;
			val2 = val1*3;
			val3 = 100+val1*2;
			break;
		case SC_FORTUNE:			/* K‰^‚ÌƒLƒX */
			tick = 1000 * 60 * 2;
			break;
		case SC_SERVICE4U:			/* ƒT[ƒrƒXƒtƒH[ƒ†[ */
			tick = 1000 * 60 * 3;
			val2 = val1+10;
			break;

//----		case SC_ETERNALCHAOS:		/* ƒGƒ^[ƒiƒ‹ƒJƒIƒX */
//			tick = 1000* 60;
//			break;
//		case SC_NIBELUNGEN:			/* ƒj[ƒxƒ‹ƒ“ƒO‚Ìw—Ö */
//			tick = 1000 * 60;
//			val2 = (val1+2)*50;
//			break;
//		case SC_SIEGFRIED:			/* •s€g‚ÌƒW[ƒNƒtƒŠ[ƒh */
//			tick = 1000 * 60;
//			val2 = (val1+3)*10;
//			break;
//		case SC_DISSONANCE:
//			tick = 1000 * 30;
//			val2 = 30 + val1*10;
//			break;
//		case SC_UGLYDANCE:
//			tick = 1000 * 30;
//			val2 = 30 + val1*10;
//			break;
		case SC_EXPLOSIONSPIRITS:	// ”š—ô”g“®
			tick = 1000 * 60 * 3;
			val2 = 75 + 25*val1;
			break;
		case SC_STEELBODY:			// ‹à„
			tick = 1000 * 30 * val1;
			break;
		case SC_VOLCANO:
			if( sc_data[SC_DELUGE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_DELUGE,-1);
			if( sc_data[SC_VIOLENTGALE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VIOLENTGALE,-1);
			if( sc_data[SC_LANDPROTECTOR].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_LANDPROTECTOR,-1);
			break;
		case SC_DELUGE:
			if( sc_data[SC_LANDPROTECTOR].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_LANDPROTECTOR,-1);
			if( sc_data[SC_VIOLENTGALE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VIOLENTGALE,-1);
			if( sc_data[SC_VOLCANO].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VOLCANO,-1);
			break;
		case SC_VIOLENTGALE:
			if( sc_data[SC_DELUGE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_DELUGE,-1);
			if( sc_data[SC_LANDPROTECTOR].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_LANDPROTECTOR,-1);
			if( sc_data[SC_VOLCANO].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VOLCANO,-1);
			break;
		case SC_LANDPROTECTOR:
			tick=((struct skill_unit *)val2)->group->limit;
			if( sc_data[SC_DELUGE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_DELUGE,-1);
			if( sc_data[SC_VIOLENTGALE].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VIOLENTGALE,-1);
			if( sc_data[SC_VOLCANO].timer!=-1 )	/* EP‰ğœ */
				skill_status_change_end(bl,SC_VOLCANO,-1);
			break;
		case SC_STRIPWEAPON:				/* Strip Weapon */
			tick = 1000 * 60;
			break;
		case SC_STRIPHELM:				/* Strip Helmet */
			tick = 1000 * 60;
			break;
		case SC_STRIPARMOR:				/* Strip Armor */
			tick = 1000 * 60;
			break;
		case SC_STRIPSHIELD:				/* Strip Shield */
			tick = 1000 * 60;
			break;
		case SC_AUTOCOUNTER:
			tick = 300 * val1;
			if(val1 == 4)
				tick = 1500;
			if(val1 == 5)
				tick = 2000;
			break;
		case SC_AUTOGUARD:
			tick = 500 * val1;
			if(val1 == 4)
				tick = 1500;
			if(val1 == 5)
				tick = 2000;
			break;
		case SC_REFLECTSHIELD:
			tick = 1000 * val1;
			if(val1 == 4)
				tick = 1500;
			if(val1 == 5)
				tick = 2000;
			break;

		/* option1 */
		case SC_SPEEDPOTION0:		/* ‘‘¬ƒ|[ƒVƒ‡ƒ“ */
		case SC_SPEEDPOTION1:
		case SC_SPEEDPOTION2:
			tick = 1000 * val1;
			val2 = 5*(2+type-SC_SPEEDPOTION0);
			break;

		case SC_STONE:				/* Î‰» */
			if( (tick=val2)<=0 )
				tick = 1000 * 600;		/* ‚Æ‚è‚ ‚¦‚¸10•ª‚É‚µ‚Ä‚İ‚éiŒã‚Íƒ^ƒCƒ}ˆ—‚Å‰„’·‚·‚éj */
			break;
		case SC_SLEEP:				/* ‡–° */
			tick = val2;
			break;
		case SC_FREEZE:				/* “€Œ‹ */
			if( (tick=val2)<=0 )
				tick = 1000 * 3 * val1;
			break;
		case SC_STAN:				/* ƒXƒ^ƒ“ival2‚Éƒ~ƒŠ•bƒZƒbƒgj */
			tick = val2;
			break;

		/* option2 */
		case SC_POISON:				/* “Å */
			if( (tick=val2)<=0 )
				tick = 1000 * (10 + rand()%50);	/* “K“–‚È’·‚³ */
			break;
		case SC_SILENCE:			/* ’¾–ÙiƒŒƒbƒNƒXƒfƒr[ƒij */
			if( (tick=val2)<=0 )
				tick = 1000 * ( (val1>6)?60: 25 + val1 * 5);
			break;
		case SC_BLIND:				/* ˆÃ• */
			if( (tick=val2)<=0 )
				tick = 500*600;		/* ‚Æ‚è‚ ‚¦‚¸‚T•ª */
			break;
			
		/* option */
		case SC_HIDDING:		/* ƒnƒCƒfƒBƒ“ƒO */
			tick = 1000;			/* ‚P•b‚¸‚ÂŠÔƒ`ƒFƒbƒN */
			val2 = 30 * val1;		/* ‘±ŠÔ */
			break;
		case SC_CLOAKING:		/* ƒNƒ[ƒLƒ“ƒO */
			tick = 1000 * (3 + val1 );	/* SP‚ª1Œ¸‚éŠÔ(val2‚É‚àƒZƒbƒg) */
			val2 = tick;
			break;
		case SC_SIGHT:			/* ƒTƒCƒg/ƒ‹ƒAƒt */
		case SC_RUWACH:
			tick=10;
			val2=40;	/* 250*40=10•b */
			break;

		/* ƒZ[ƒtƒeƒBƒEƒH[ƒ‹Aƒjƒ…[ƒ} */
		case SC_SAFETYWALL:	case SC_PNEUMA:
			tick=((struct skill_unit *)val2)->group->limit;
			break;

		/* ƒAƒ“ƒNƒ‹ */
		case SC_ANKLE:
			tick=val2;
			break;

		/* ƒEƒH[ƒ^[ƒ{[ƒ‹ */
		case SC_WATERBALL:
			tick=100;
			val3= (val1|1)*(val1|1)-1;
			break;

		/* ƒXƒLƒ‹‚¶‚á‚È‚¢/ŠÔ‚ÉŠÖŒW‚µ‚È‚¢ */
		case SC_RIDING:	case SC_FALCON:	case SC_WEIGHT50:	case SC_WEIGHT90:
			tick=600*1000;
			break;

		case SC_CANNIBALIZE:		// Added by RoVeRT
		case SC_SPHEREMINE:
			tick = 10 * val1 * 2 * 1000;
			break;

		case SC_AUTOSPELL:
			tick = 1000 * (90 + 30 * val1); /* SC_AUTOSPELL); */
			val1 = 5 + 2 * val1;
			break;

		default:
			printf("UnknownStatusChange [%d]\n", type);
			return 0;
	}

	if(bl->type==BL_PC){
		int ii = -1;
		if (type < 64) ii=type;
		else if (type == SC_EXPLOSIONSPIRITS)	ii = 0x56;
		else if (type == SC_STEELBODY)			ii = 0x57;
		else if (type == SC_SPEARSQUICKEN)		ii = 0x44;
		else if (type == SC_FLAMELAUNCHER)		ii = 0x5a;
		else if (type == SC_FROSTWEAPON)		ii = 0x5b;
		else if (type == SC_LIGHTNINGLOADER)	ii = 0x5c;
		else if (type == SC_SEISMICWEAPON)		ii = 0x5d;
		if (ii != -1)
			clif_status_change(bl,ii,1);	/* ƒAƒCƒRƒ“•\¦ */
	}

	/* option‚Ì•ÏX */
	switch(type){
		case SC_STONE:	case SC_FREEZE:	case SC_STAN:	case SC_SLEEP:
			battle_stopattack(bl);	/* UŒ‚’â~ */
			battle_stopattack(bl);	/* UŒ‚’â~ */
			if(sd){	/* ‰‰‘t/ƒ_ƒ“ƒX‚Ì’†’f */
				struct skill_unit_group *group=skill_check_dancing(bl);
				if(group){
					skill_delunitgroup(group);
					pc_calcstatus(sd,0);
				}
			}
			{	/* “¯‚ÉŠ|‚©‚ç‚È‚¢ƒXƒe[ƒ^ƒXˆÙí‚ğ‰ğœ */
				int i;
				for(i = SC_STONE; i <= SC_SLEEP; i++){
					if(sc_data[i].timer != -1){
						(*sc_count)--;
						delete_timer(sc_data[i].timer, skill_status_change_timer);
						sc_data[i].timer = -1;
					}
				}
			}
			*opt1 = type - SC_STONE + 1;
			opt_flag = 1;
			break;
		case SC_POISON:	case SC_CURSE:	case SC_SILENCE:	case SC_BLIND:
			*opt2|= 1<<(type-SC_POISON);
			opt_flag = 1;
			break;
		case SC_HIDDING:	case SC_CLOAKING:
			battle_stopattack(bl);	/* UŒ‚’â~ */
			*option|= ((type==SC_HIDDING)?2:4);
			opt_flag =1 ;
			break;
		case SC_SIGHT:
			*option|=1;
			opt_flag=1;
			break;
		case SC_RUWACH:
			*option|=8192;
			opt_flag=1;
			break;
		case SC_BLADESTOP:
			pc_stop_walking(sd,1);	/* Blade Stop */
			break;
	}

	if(opt_flag)	/* option‚Ì•ÏX */
		clif_changeoption(bl);

	(*sc_count)++;	/* ƒXƒe[ƒ^ƒXˆÙí‚Ì” */

	sc_data[type].val1 = val1;
	sc_data[type].val2 = val2;
	sc_data[type].val3 = val3;
	sc_data[type].val4 = val4;
	/* ƒ^ƒCƒ}[İ’è */
	sc_data[type].timer = add_timer(
		gettick() + tick, skill_status_change_timer, bl->id, type);

	if(bl->type==BL_PC)
		pc_calcstatus(sd,0);	/* ƒXƒe[ƒ^ƒXÄŒvZ */

	return 0;
}
/*==========================================
 * ƒXƒe[ƒ^ƒXˆÙí‘S‰ğœ
 *------------------------------------------
 */
int skill_status_change_clear(struct block_list *bl)
{
	struct status_change* sc_data;
	short *sc_count, *option, *opt1, *opt2;
	int i;
	
	sc_data=battle_get_sc_data(bl);
	sc_count=battle_get_sc_count(bl);
	option=battle_get_option(bl);
	opt1=battle_get_opt1(bl);
	opt2=battle_get_opt2(bl);
	
	if(*sc_count == 0)
		return 0;
	for(i = 0; i < MAX_STATUSCHANGE; i++){
		if(sc_data[i].timer != -1){	/* ˆÙí‚ª‚ ‚é‚È‚çƒ^ƒCƒ}[‚ğíœ‚·‚é */
			delete_timer(sc_data[i].timer, skill_status_change_timer);
			sc_data[i].timer = -1;
			if(bl->type==BL_PC){
				int ii = -1;
				if (i < 64) ii=i;
				else if (i == SC_EXPLOSIONSPIRITS)	ii = 0x56;
				else if (i == SC_STEELBODY)			ii = 0x57;
				else if (i == SC_SPEARSQUICKEN)		ii = 0x44;
				else if (i == SC_FLAMELAUNCHER)		ii = 0x5a;
				else if (i == SC_FROSTWEAPON)		ii = 0x5b;
				else if (i == SC_LIGHTNINGLOADER)	ii = 0x5c;
				else if (i == SC_SEISMICWEAPON)		ii = 0x5d;
				if (ii != -1)
					clif_status_change(bl,ii,0);	/* ƒAƒCƒRƒ“Á‹ */
			}
		}
	}
	*sc_count = 0;
	*opt1 = 0;
	*opt2 = 0;
	*option &= 0xfff8;

	if( bl->type==BL_PC )
		clif_changeoption(bl);

	return 0;
}


/* ƒNƒ[ƒLƒ“ƒOŒŸ¸iü‚è‚ÉˆÚ“®•s‰Â”\’n‘Ñ‚ª‚ ‚é‚©j */
int skill_check_cloaking(struct block_list *bl)
{
	static int dx[]={-1, 0, 1,-1, 1,-1, 0, 1};
	static int dy[]={-1,-1,-1, 0, 0, 1, 1, 1};
	int end=1,i;
	for(i=0;i<sizeof(dx)/sizeof(dx[0]);i++){
		int c=map_getcell(bl->m,bl->x+dx[i],bl->y+dy[i]);
		if(c==1 || c==5) end=0;
	}
	if(end){
		skill_status_change_end(bl, SC_CLOAKING, -1);
		*battle_get_option(bl)&=~4;	/* ”O‚Ì‚½‚ß‚Ìˆ— */
	}
	return end;
}





/*
 *----------------------------------------------------------------------------
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg
 *----------------------------------------------------------------------------
 */

/* ‰‰‘t/ƒ_ƒ“ƒX’†‚©‚Ç‚¤‚©i‚»‚ÌƒOƒ‹[ƒv‚ğ•Ô‚·j */
struct skill_unit_group *skill_check_dancing(struct block_list *src)
{
	int i;
	struct skill_unit_group *list=NULL;
	int maxsug=0;

	if(src->type==BL_PC){
		list=((struct map_session_data *)src)->skillunit;
		maxsug=MAX_SKILLUNITGROUP;
	}else if(src->type==BL_MOB){
		list=((struct mob_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	}
	if(!list) return NULL;
	
	for(i=0;i<maxsug;i++){	/* ŒŸõ */
		int id;
		if(list[i].group_id==0)
			continue;
		id=list[i].skill_id;
		if( (id>=BD_LULLABY && id<=BD_RAGNAROK) ||
			(id>=BA_DISSONANCE && id<=BA_APPLEIDUN && id!=BA_FROSTJOKE ) ||
			(id>=DC_UGLYDANCE && id<=DC_SERVICEFORYOU && id!=DC_SCREAM) )
			return &list[i];
	}
	return NULL;
}

/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒg‰Šú‰»
 *------------------------------------------
 */
struct skill_unit *skill_initunit(struct skill_unit_group *group,int idx,int x,int y)
{
	struct skill_unit *unit=&group->unit[idx];
	
	if(!unit->alive)
		group->alive_count++;
	
	unit->bl.id=map_addobject(&unit->bl);
	unit->bl.type=BL_SKILL;
	unit->bl.m=group->map;
	unit->bl.x=x;
	unit->bl.y=y;
	unit->group=group;
	unit->val1=unit->val2=0;
	unit->alive=1;

	map_addblock(&unit->bl);
	clif_skill_setunit(unit);
	return unit;
}

int skill_unit_timer_sub_ondelete( struct block_list *bl, va_list ap );
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgíœ
 *------------------------------------------
 */
int skill_delunit(struct skill_unit *unit)
{
	struct skill_unit_group *group=unit->group;
	int range;
/*	printf("delunit %d\n",unit->bl.id); */

	if(!unit->alive || group==NULL)
		return 0;

	/* onlimitƒCƒxƒ“ƒgŒÄ‚Ño‚µ */
	skill_unit_onlimit( unit,gettick() );

	/* ondeleteƒCƒxƒ“ƒgŒÄ‚Ño‚µ */
	range=unit->group->range;
	map_foreachinarea( skill_unit_timer_sub_ondelete, unit->bl.m,
		unit->bl.x-range,unit->bl.y-range,unit->bl.x+range,unit->bl.y+range,0,
		&unit->bl,gettick() );

	clif_skill_delunit(unit);
	
	
	unit->group=NULL;
	unit->alive=0;
	map_delobjectnofree(unit->bl.id);
	if(group->alive_count>0 && (--group->alive_count)<=0) {
		if (1){
			struct block_list *bl=map_id2bl(group->src_id);
			struct map_session_data *sd=NULL;
			struct mob_data *md=NULL;
			if (bl->type == BL_PC){
				sd=(struct map_session_data *)bl;
				sd->skill_limit[group->skill_id]--;
			}
			else if(bl->type==BL_MOB){
				md=(struct mob_data *)bl;
				md->skill_limit[group->skill_id]--;
			}
		}
		skill_delunitgroup(group);
	}
	
	return 0;
}
/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒOƒ‹[ƒv‰Šú‰»
 *------------------------------------------
 */
static int skill_unit_group_newid=10;
struct skill_unit_group *skill_initunitgroup(struct block_list *src,
	int count,int skillid,int skilllv,int unit_id)
{
	int i;
	struct skill_unit_group *group=NULL, *list=NULL;
	int maxsug=0;

	if(src->type==BL_PC){
		list=((struct map_session_data *)src)->skillunit;
		maxsug=MAX_SKILLUNITGROUP;
	}else if(src->type==BL_MOB){
		list=((struct mob_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	}
	if(list){
		for(i=0;i<maxsug;i++)	/* ‹ó‚¢‚Ä‚¢‚é‚à‚ÌŒŸõ */
			if(list[i].group_id==0){
				group=&list[i];
				break;
			}

		if(group==NULL){	/* ‹ó‚¢‚Ä‚È‚¢‚Ì‚ÅŒÃ‚¢‚à‚ÌŒŸõ */
			int j=0;
			unsigned maxdiff=0,x,tick=gettick();
			for(i=0;i<maxsug;i++)
				if((x=DIFF_TICK(tick,list[i].tick))>maxdiff){
					maxdiff=x;
					j=i;
				}
			skill_delunitgroup(&list[j]);
			group=&list[j];
		}
	}

	if(group==NULL){
		printf("skill_initunitgroup: error unit group !\n");
		exit(0);
	}

	group->src_id=src->id;
	group->party_id=battle_get_party_id(src);
	group->guild_id=battle_get_guild_id(src);
	group->group_id=skill_unit_group_newid++;
	if(skill_unit_group_newid<=0)
		skill_unit_group_newid=10;

	group->unit=malloc(sizeof(struct skill_unit)*count);
	if(group->unit==NULL){
		printf("skill_initunitgroup: out of memory! \n");
		exit(0);
	}
	memset(group->unit,0,sizeof(struct skill_unit)*count);

	group->unit_count=count;
	group->val1=group->val2=0;
	group->skill_id=skillid;
	group->skill_lv=skilllv;
	group->unit_id=unit_id;
	group->map=src->m;
	group->range=0;
	group->limit=10000;
	group->interval=1000;
	group->tick=gettick();
	memset(group->vallist,0,sizeof(group->vallist));
	group->valstr=NULL;

	return group;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒOƒ‹[ƒvíœ
 *------------------------------------------
 */
int skill_delunitgroup(struct skill_unit_group *group)
{
	int i;
	if(group->unit_count<=0)
		return 0;

/*	printf("delunitgroup %d\n",group->group_id); */

	group->alive_count=0;
	if(group->unit!=NULL){
		for(i=0;i<group->unit_count;i++)
			if(group->unit[i].alive)
				skill_delunit(&group->unit[i]);
	}
	if(group->valstr!=NULL){
		map_freeblock(group->valstr);
		group->valstr=NULL;
	}
	
	map_freeblock(group->unit);	/* free()‚Ì‘Ö‚í‚è */
	group->unit=NULL;
	group->src_id=0;
	group->group_id=0;
	group->unit_count=0;
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒOƒ‹[ƒv‘Síœ
 *------------------------------------------
 */
int skill_clear_unitgroup(struct block_list *src)
{
	struct skill_unit_group *group=NULL;
	int maxsug=0;
	if(src->type==BL_PC){
		group=((struct map_session_data *)src)->skillunit;
		maxsug=MAX_SKILLUNITGROUP;
	}else if(src->type==BL_MOB){
		group=((struct mob_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	}
	if(group){
		int i;
		for(i=0;i<maxsug;i++)
			if(group[i].group_id>0)
				skill_delunitgroup(&group[i]);
	}
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒOƒ‹[ƒv‚Ì”í‰e‹¿tickŒŸõ
 *------------------------------------------
 */
struct skill_unit_group_tickset *skill_unitgrouptickset_search(
	struct block_list *bl,int group_id)
{
	int i,j=0,k,s=group_id%MAX_SKILLUNITGROUPTICKSET;
	struct skill_unit_group_tickset *set=NULL;
	if(bl->type==BL_PC){
		set=((struct map_session_data *)bl)->skillunittick;
	}else{
		set=((struct mob_data *)bl)->skillunittick;
	}
	if(set==NULL)
		return 0;
	for(i=0;i<MAX_SKILLUNITGROUPTICKSET;i++)
		if( set[(k=(i+s)%MAX_SKILLUNITGROUPTICKSET)].group_id == group_id )
			return &set[k];
		else if( set[k].group_id==0 )
			j=k;

	return &set[j];
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒOƒ‹[ƒv‚Ì”í‰e‹¿tickíœ
 *------------------------------------------
 */
int skill_unitgrouptickset_delete(struct block_list *bl,int group_id)
{
	int i,s=group_id%MAX_SKILLUNITGROUPTICKSET;
	struct skill_unit_group_tickset *set=NULL,*ts;
	if(bl->type==BL_PC){
		set=((struct map_session_data *)bl)->skillunittick;
	}else{
		set=((struct mob_data *)bl)->skillunittick;
	}
	
	if(set!=NULL){
	
		for(i=0;i<MAX_SKILLUNITGROUPTICKSET;i++)
			if( (ts=&set[(i+s)%MAX_SKILLUNITGROUPTICKSET])->group_id == group_id )
				ts->group_id=0;

	}
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒ^ƒCƒ}[”­“®ˆ——p(foreachinarea)
 *------------------------------------------
 */
int skill_unit_timer_sub_onplace( struct block_list *bl, va_list ap )
{
	struct block_list *src;
	unsigned int tick;
	src=va_arg(ap,struct block_list*);
	tick=va_arg(ap,unsigned int);


	if( ((struct skill_unit *)src)->alive) {
		if(battle_check_target(src,bl,((struct skill_unit *)src)->group->target_flag )>0 ||
			(((struct skill_unit *)src)->group->skill_id == CR_GRANDCROSS && ((struct skill_unit *)src)->group->src_id == bl->id) )
			skill_unit_onplace( (struct skill_unit *)src, bl, tick );
	}
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒ^ƒCƒ}[íœˆ——p(foreachinarea)
 *------------------------------------------
 */
int skill_unit_timer_sub_ondelete( struct block_list *bl, va_list ap )
{
	struct block_list *src;
	unsigned int tick;
	src=va_arg(ap,struct block_list*);
	tick=va_arg(ap,unsigned int);

	if( ((struct skill_unit *)src)->alive &&
		battle_check_target(src,bl,((struct skill_unit *)src)->group->target_flag )>0 )
			skill_unit_ondelete( (struct skill_unit *)src, bl, tick );
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒ^ƒCƒ}[ˆ——p(foreachobject)
 *------------------------------------------
 */
int skill_unit_timer_sub( struct block_list *bl, va_list ap )
{
	struct skill_unit *unit=(struct skill_unit *)bl;
	struct skill_unit_group *group;
	int range;
	unsigned int tick;
	tick=va_arg(ap,unsigned int);

	if(!unit->alive)
		return 0;

	group=unit->group;
	range=(unit->range!=0)?unit->range:group->range;

	/* onplaceƒCƒxƒ“ƒgŒÄ‚Ño‚µ */
	if(unit->alive && unit->range>=0)
		map_foreachinarea( skill_unit_timer_sub_onplace, bl->m,
			bl->x-range,bl->y-range,bl->x+range,bl->y+range,0,
			bl,tick );

	/* ŠÔØ‚êíœ */
	if(unit->alive &&
		(DIFF_TICK(tick,group->tick)>=group->limit ||
		 DIFF_TICK(tick,group->tick)>=unit->limit) ){
		skill_delunit(unit);
	}

	return 0;
}

/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgƒ^ƒCƒ}[ˆ—
 *------------------------------------------
 */
int skill_unit_timer( int tid,unsigned int tick,int id,int data)
{
	map_freeblock_lock();

	map_foreachobject( skill_unit_timer_sub, BL_SKILL, tick );

	map_freeblock_unlock();
	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgˆÚ“®ˆ——p(foreachinarea)
 *------------------------------------------
 */
int skill_unit_move_sub( struct block_list *bl, va_list ap )
{
	struct skill_unit *unit=(struct skill_unit *)bl;
	struct skill_unit_group *group;
	struct block_list *src;
	int range;
	unsigned int tick;
	src=va_arg(ap,struct block_list*);
	tick=va_arg(ap,unsigned int);

	if(!unit->alive || src->prev==NULL)
		return 0;

	group=unit->group;
	range=(unit->range!=0)?unit->range:group->range;

	if( range<0 || battle_check_target(bl,src,group->target_flag )<=0 )
		return 0;

	if( src->x >= bl->x-range && src->x <= bl->x+range &&
		src->y >= bl->y-range && src->y <= bl->y+range )
		skill_unit_onplace( unit, src, tick );
	else
		skill_unit_onout( unit, src, tick );

	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ƒ†ƒjƒbƒgˆÚ“®ˆ—
 *------------------------------------------
 */
int skill_unit_move( struct block_list *bl,unsigned int tick,int range)
{
	if(bl->prev==NULL)
		return 0;

	if(range<5)
		range=5;
	map_foreachinarea( skill_unit_move_sub,
		bl->m,bl->x-range,bl->y-range,bl->x+range,bl->y+range,BL_SKILL,
		bl,tick );

	return 0;
}


/*----------------------------------------------------------------------------
 * ƒAƒCƒeƒ€‡¬
 *----------------------------------------------------------------------------
 */

/*==========================================
 * ƒAƒCƒeƒ€‡¬‰Â”\”»’è
 *------------------------------------------
 */
int skill_can_produce_mix( struct map_session_data *sd, int nameid, int trigger )
{
	int i,j;

	if(nameid<=0)
		return 0;

	for(i=0;i<MAX_SKILL_PRODUCE_DB;i++){
		if(skill_produce_db[i].nameid == nameid )
			break;
	}
	if( i >= MAX_SKILL_PRODUCE_DB )	/* ƒf[ƒ^ƒx[ƒX‚É‚È‚¢ */
		return 0;

	if(trigger>=0){
		if(trigger==32 || trigger==16){
			if(skill_produce_db[i].itemlv!=trigger)	/* ƒtƒ@[ƒ}ƒV[–ƒ|[ƒVƒ‡ƒ“—Ş‚Æ—nz˜F–zÎˆÈŠO‚Í‚¾‚ß */
				return 0;
		}else{
			if(skill_produce_db[i].itemlv>=16)		/* •ŠíˆÈŠO‚Í‚¾‚ß */
				return 0;
			if( itemdb_wlv(nameid) > trigger )	/* •ŠíLv”»’è */
				return 0;
		}
	}
	if( (j=skill_produce_db[i].req_skill)>0 && pc_checkskill(sd,j)<=0 )
		return 0;		/* ƒXƒLƒ‹‚ª‘«‚è‚È‚¢ */

	for(j=0;j<5;j++){
		int id,x,y;
		if( (id=skill_produce_db[i].mat_id[j])==0 )	/* ‚±‚êˆÈã‚ÍŞ—¿—v‚ç‚È‚¢ */
			break;
		for(y=0,x=0;y<MAX_INVENTORY;y++)
			if( sd->status.inventory[y].nameid == id )
				x+=sd->status.inventory[y].amount;
		if(x<skill_produce_db[i].mat_amount[j])	/* ƒAƒCƒeƒ€‚ª‘«‚è‚È‚¢ */
			return 0;
	}
	return i+1;
}


/*==========================================
 * ƒAƒCƒeƒ€‡¬‰Â”\”»’è
 *------------------------------------------
 */
int skill_produce_mix( struct map_session_data *sd,
	int nameid, int slot1, int slot2, int slot3 )
{
	int slot[3];
	int i,sc,ele,idx,equip,wlv,make_per,flag;

	if( !(idx=skill_can_produce_mix(sd,nameid,-1)) )	/* ğŒ•s‘« */
		return 0;
	idx--;
	slot[0]=slot1;
	slot[1]=slot2;
	slot[2]=slot3;

	/* –„‚ß‚İˆ— */
	for(i=0,sc=0,ele=0;i<3;i++){
		int j;
		if( slot[i]<=0 )
			continue;
		j = pc_search_inventory(sd,slot[i]);
		if(j < 0)	/* •s³ƒpƒPƒbƒg(ƒAƒCƒeƒ€‘¶İ)ƒ`ƒFƒbƒN */
			continue;
		if(slot[i]==1000){	/* ¯‚Ì‚©‚¯‚ç */
			pc_delitem(sd,j,1,1);
			sc++;
		}
		if(slot[i]>=994 && slot[i]<=997 && ele==0){	/* ‘®«Î */
			static const int ele_table[4]={3,1,4,2};
			pc_delitem(sd,j,1,1);
			ele=ele_table[slot[i]-994];
		}
	}

	/* Ş—¿Á”ï */
	for(i=0;i<5;i++){
		int j,id,x;
		if( (id=skill_produce_db[idx].mat_id[i])<=0 )
			break;
		x=skill_produce_db[idx].mat_amount[i];	/* •K—v‚ÈŒÂ” */
		do{	/* ‚Q‚ÂˆÈã‚ÌƒCƒ“ƒfƒbƒNƒX‚É‚Ü‚½‚ª‚Á‚Ä‚¢‚é‚©‚à‚µ‚ê‚È‚¢ */
			int y=0;
			j = pc_search_inventory(sd,id);
			
			if(j >= 0){
				y = sd->status.inventory[j].amount;
				if(y>x)y=x;	/* ‘«‚è‚Ä‚¢‚é */
				if(!(skill_produce_db[idx].req_skill==AM_PHARMACY && i==0))
					pc_delitem(sd,j,y,0);
			}else
				printf("skill_produce_mix: material item error\n");

			x-=y;	/* ‚Ü‚¾‘«‚è‚È‚¢ŒÂ”‚ğŒvZ */
		}while( j>=0 && x>0 );	/* Ş—¿‚ğÁ”ï‚·‚é‚©AƒGƒ‰[‚É‚È‚é‚Ü‚ÅŒJ‚è•Ô‚· */
	}

	/* Šm—¦”»’è */
	equip = itemdb_isequip(nameid);
	if(!equip) {
		if(nameid>=501 && nameid<=504)
			make_per = 2000 + sd->status.job_level*20 + sd->paramc[4]*10 + sd->paramc[5]*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*100 + pc_checkskill(sd,AM_PHARMACY)*300 + pc_checkskill(sd,AM_POTIONPITCHER)*100;
		else if(nameid>=545 && nameid<=547)
			make_per = sd->status.job_level*20 + sd->paramc[4]*10 + sd->paramc[5]*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*100 + pc_checkskill(sd,AM_PHARMACY)*300;
		else if(nameid>=7135 && nameid<=7139)
			make_per = 1000 + sd->status.job_level*20 + sd->paramc[4]*10 + sd->paramc[5]*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*100 + pc_checkskill(sd,AM_PHARMACY)*300 + pc_checkskill(sd,AM_CANNIBALIZE)*100;
		else if(nameid == 998)
			make_per = 2000 + sd->status.base_level*30 + sd->paramc[4]*20 + sd->paramc[5]*10 + pc_checkskill(sd,skill_produce_db[idx].req_skill)*600;
		else if(nameid == 985)
			make_per = 1000 + sd->status.base_level*30 + sd->paramc[4]*20 + sd->paramc[5]*10 + (pc_checkskill(sd,skill_produce_db[idx].req_skill)-1)*500;
		else
			make_per = 1000 + sd->status.base_level*30 + sd->paramc[4]*20 + sd->paramc[5]*10 + pc_checkskill(sd,skill_produce_db[idx].req_skill)*500;
	}
	else {
		int add_per;
		if(pc_search_inventory(sd,989) >= 0) add_per = 750;
		else if(pc_search_inventory(sd,988) >= 0) add_per = 500;
		else if(pc_search_inventory(sd,987) >= 0) add_per = 250;
		else if(pc_search_inventory(sd,986) >= 0) add_per = 0;
		else add_per = -500;
		if(ele) add_per -= 500;
		add_per -= sc*500;
		wlv = itemdb_wlv(nameid);
		make_per = ((250 + sd->status.base_level*15 + sd->paramc[4]*10 + sd->paramc[5]*5 + pc_checkskill(sd,skill_produce_db[idx].req_skill)*500 +
			add_per) * (100 - (wlv - 1)*20))/100 + pc_checkskill(sd,107)*100 + ((wlv >= 3)? pc_checkskill(sd,97)*100 : 0);
	}

	if(make_per < 1) make_per = 1;

	if( battle_config.wp_rate!=100 )	/* Šm—¦•â³ */
		make_per=make_per*battle_config.wp_rate/100;

	/* debug code */
	/*printf("make success percent = %.2lf\n",(double)make_per/100.); */
	
	if(rand()%10000 < make_per){
		/* ¬Œ÷ */
		struct item tmp_item;
		memset(&tmp_item,0,sizeof(tmp_item));
		tmp_item.nameid=nameid;
		tmp_item.amount=1;
		tmp_item.identify=1;
		if(equip){	/* •Ší‚Ìê‡ */
			tmp_item.card[0]=0x00ff; /* »‘¢•Šíƒtƒ‰ƒO */
			tmp_item.card[1]=((sc*5)<<8)+ele;	/* ‘®«‚Æ‚Â‚æ‚³ */
			*((unsigned long *)(&tmp_item.card[2]))=sd->char_id;	/* ƒLƒƒƒ‰ID */
		}
		clif_produceeffect(sd,0,nameid);/* »‘¢ƒGƒtƒFƒNƒgƒpƒPƒbƒg */
		clif_misceffect(&sd->bl,3); /* ‘¼l‚É‚à¬Œ÷‚ğ’Ê’mi¸˜B¬Œ÷ƒGƒtƒFƒNƒg‚Æ“¯‚¶‚Å‚¢‚¢‚ÌHj */
		if((flag = pc_additem(sd,&tmp_item,1))) {
			clif_additem(sd,0,0,flag);
			map_addflooritem(&tmp_item,1,sd->bl.m,sd->bl.x,sd->bl.y);
		}
	}
	else {
		/* ¸”s */
		clif_produceeffect(sd,1,nameid);/* »‘¢ƒGƒtƒFƒNƒgƒpƒPƒbƒg */
		clif_misceffect(&sd->bl,2); /* ‘¼l‚É‚à¸”s‚ğ’Ê’m */
	}
	return 0;
}

int skill_arrow_create( struct map_session_data *sd,int nameid)
{
	int i,j,flag,index=-1;
	struct item tmp_item;

	if(nameid <= 0)
		return 1;

	for(i=0;i<MAX_SKILL_ARROW_DB;i++)
		if(nameid == skill_arrow_db[i].nameid) {
			index = i;
			break;
		}

	if(index < 0 || (j = pc_search_inventory(sd,nameid)) < 0)
		return 1;

	pc_delitem(sd,j,1,0);
	memset(&tmp_item,0,sizeof(tmp_item));
	tmp_item.identify = 1;
	for(i=0;i<5;i++) {
		tmp_item.nameid = skill_arrow_db[index].cre_id[i];
		tmp_item.amount = skill_arrow_db[index].cre_amount[i];
		if(tmp_item.nameid <= 0 || tmp_item.amount <= 0)
			continue;
		if((flag = pc_additem(sd,&tmp_item,tmp_item.amount))) {
			clif_additem(sd,0,0,flag);
			map_addflooritem(&tmp_item,tmp_item.amount,sd->bl.m,sd->bl.x,sd->bl.y);
		}
//		printf("arrow create %d -> %d:%d\n",mate,tmp_item.nameid,tmp_item.amount);
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * ‰Šú‰»Œn
 */

/*==========================================
 * ƒXƒLƒ‹ŠÖŒWƒtƒ@ƒCƒ‹“Ç‚İ‚İ
 * skill_db.txt ƒXƒLƒ‹ƒf[ƒ^
 * cast_db.txt ƒXƒLƒ‹‚Ì‰r¥ŠÔ‚ÆƒfƒBƒŒƒCƒf[ƒ^
 * produce_db.txt ƒAƒCƒeƒ€ì¬ƒXƒLƒ‹—pƒf[ƒ^
 * create_arrow_db.txt –îì¬ƒXƒLƒ‹—pƒf[ƒ^
 *------------------------------------------
 */
int skill_readdb(void)
{
	int i,j,k;
	FILE *fp;
	char line[1024],*p;

	/* ƒXƒLƒ‹ƒf[ƒ^ƒx[ƒX */
	memset(skill_db,0,sizeof(skill_db));
	fp=fopen("db/skill_db.txt","r");
	if(fp==NULL){
		printf("can't read db/skill_db.txt\n");
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50], *split2[16];
		if(line[0]=='/' && line[1]=='/')
			continue;
		for(j=0,p=line;j<10 && p;j++){
			split[j]=p;
			p=strchr(p,',');
			if(p) *p++=0;
		}
		if(split[9]==NULL || j<10)
			continue;

		i=atoi(split[0]);
		if(i<0 || i>MAX_SKILL_DB)
			continue;

/*		printf("skill id=%d\n",i); */
		memset(split2,0,sizeof(split2));
		for(j=0,p=split[1];j<10 && p;j++){
			split2[j]=p;
			p=strchr(p,':');
			if(p) *p++=0;
		}
		for(k=0;k<10;k++)
			skill_db[i].range[k]=(split2[k]?atoi(split2[k]):atoi(split2[0]));
//		skill_db[i].range=atoi(split[1]);
		skill_db[i].hit=atoi(split[2]);
		skill_db[i].inf=atoi(split[3]);
		skill_db[i].pl=atoi(split[4]);
		skill_db[i].nk=atoi(split[5]);
		skill_db[i].max=atoi(split[6]);

		memset(split2,0,sizeof(split2));
		for(j=0,p=split[7];j<10 && p;j++){
			split2[j]=p;
			p=strchr(p,':');
			if(p) *p++=0;
		}
		for(k=0;k<10;k++)
			skill_db[i].sp[k]=(split2[k]?atoi(split2[k]):atoi(split2[0]));

		memset(split2,0,sizeof(split2));
		for(j=0,p=split[8];j<10 && p;j++){
			split2[j]=p;
			p=strchr(p,':');
			if(p) *p++=0;
		}
		for(k=0;k<10;k++)
			skill_db[i].num[k]=(split2[k]?atoi(split2[k]):atoi(split2[0]));
		
		skill_db[i].inf2=atoi(split[9]);

	}
	fclose(fp);
	printf("read db/skill_db.txt done\n");

	/* ƒLƒƒƒXƒeƒBƒ“ƒOƒf[ƒ^ƒx[ƒX */
/*	for(i=0;i<MAX_SKILL;i++){
		for(j=0;j<10;j++){
			skill_db[i].cast[j]=1000;
			skill_db[i].delay[j]=1000;
		}
	}*/
	fp=fopen("db/cast_db.txt","r");
	if(fp==NULL){
		printf("can't read db/cast_db.txt\n");
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50], *split2[16];
		if(line[0]=='/' && line[1]=='/')
			continue;
		for(j=0,p=line;j<3 && p;j++){
			split[j]=p;
			p=strchr(p,',');
			if(p) *p++=0;
		}
		if(split[2]==NULL || j<3)
			continue;

		i=atoi(split[0]);
		if(i<0 || i>MAX_SKILL_DB)
			continue;

		memset(split2,0,sizeof(split2));
		for(j=0,p=split[1];j<10 && p;j++){
			split2[j]=p;
			p=strchr(p,':');
			if(p) *p++=0;
		}
		for(k=0;k<10;k++)
			skill_db[i].cast[k]=(split2[k]?atoi(split2[k]):atoi(split2[0]));

		memset(split2,0,sizeof(split2));
		for(j=0,p=split[2];j<10 && p;j++){
			split2[j]=p;
			p=strchr(p,':');
			if(p) *p++=0;
		}
		for(k=0;k<10;k++)
			skill_db[i].delay[k]=(split2[k]?atoi(split2[k]):atoi(split2[0]));
	}
	fclose(fp);
	printf("read db/cast_db.txt done\n");

	memset(skill_produce_db,0,sizeof(skill_produce_db));
	fp=fopen("db/produce_db.txt","r");
	if(fp==NULL){
		printf("can't read db/produce_db.txt\n");
		return 1;
	}
	k=0;
	while(fgets(line,1020,fp)){
		char *split[16];
		int x,y;
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		for(j=0,p=line;j<13 && p;j++){
			split[j]=p;
			p=strchr(p,',');
			if(p) *p++=0;
		}
		if(split[0]==NULL)
			continue;
		i=atoi(split[0]);
		if(i<=0)
			continue;

		skill_produce_db[k].nameid=i;
		skill_produce_db[k].itemlv=atoi(split[1]);
		skill_produce_db[k].req_skill=atoi(split[2]);

		for(x=3,y=0;split[x] && split[x+1] && y<5;x+=2,y++){
			skill_produce_db[k].mat_id[y]=atoi(split[x]);
			skill_produce_db[k].mat_amount[y]=atoi(split[x+1]);
		}
		k++;
		if(k >= MAX_SKILL_PRODUCE_DB)
			break;
	}
	fclose(fp);
	printf("read db/produce_db.txt done (count=%d)\n",k);

	memset(skill_arrow_db,0,sizeof(skill_arrow_db));
	fp=fopen("db/create_arrow_db.txt","r");
	if(fp==NULL){
		printf("can't read db/create_arrow_db.txt\n");
		return 1;
	}
	k=0;
	while(fgets(line,1020,fp)){
		char *split[16];
		int x,y;
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		for(j=0,p=line;j<13 && p;j++){
			split[j]=p;
			p=strchr(p,',');
			if(p) *p++=0;
		}
		if(split[0]==NULL)
			continue;
		i=atoi(split[0]);
		if(i<=0)
			continue;

		skill_arrow_db[k].nameid=i;

		for(x=1,y=0;split[x] && split[x+1] && y<5;x+=2,y++){
			skill_arrow_db[k].cre_id[y]=atoi(split[x]);
			skill_arrow_db[k].cre_amount[y]=atoi(split[x+1]);
		}
		k++;
		if(k >= MAX_SKILL_ARROW_DB)
			break;
	}
	fclose(fp);
	printf("read db/create_arrow_db.txt done (count=%d)\n",k);

	return 0;
}


/*==========================================
 * ƒXƒLƒ‹ŠÖŒW‰Šú‰»ˆ—
 *------------------------------------------
 */
int do_init_skill(void)
{
	skill_readdb();

	add_timer_func_list(skill_unit_timer,"skill_unit_timer");
	add_timer_func_list(skill_castend_id,"skill_castend_id");
	add_timer_func_list(skill_castend_pos,"skill_castend_pos");
	add_timer_func_list(skill_timerskill,"skill_timerskill");
	add_timer_func_list(skill_status_change_timer,"skill_status_change_timer");
	add_timer_interval(gettick()+SKILLUNITTIMER_INVERVAL,skill_unit_timer,0,0,SKILLUNITTIMER_INVERVAL);

	return 0;
}


