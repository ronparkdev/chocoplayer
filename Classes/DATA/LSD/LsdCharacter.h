#ifndef __R2K_DATA__LSD_CHARACTER__
#define __R2K_DATA__LSD_CHARACTER__

#include <iostream>
#include "../BaseStruct.h"
#include "../LDB/LdbMain.h"

class LsdCharacter : public BaseStruct
{
private:
	LdbCharacter * m_link;

public: 
	/*01*/TString name;
	/*02*/TString title;

	/*0B*/TString pic_name;
	/*0C*/int pic_id;

	/*0D*/int pic_default_trans;

	/*15*/TString face_name;
	/*16*/int face_index;

	/*1F*/int clevel;
	/*20*/int cexp;

	/*21*/int mhp;
	/*22*/int mmp;
	/*29*/int catk;
	/*2A*/int cdef;
	/*2B*/int cspt;
	/*2C*/int cagi;

	/*33*/int skills_size;
	/*34*/ArrayList<int> skills;//byte->int VER2.10
	/*3D*/GlbEquipment cequip;
	/*47*/int chp;
	/*48*/int cmp;
	/*50*/ArrayList<int> battle_commands;
	/*51*/int ccond_size;
	/*52*/ArrayList<bool> ccond;
	/*5A*/int cjob;

	/*5B*/bool is_back_attack;
	/*5C*/bool is_two_hand_sword;
	/*5D*/bool is_equip_fixed;
	/*5E*/bool is_auto_battle;
	/*5F*/bool is_super_gard;

	/*60*/unsigned short eq_weapon;
	/*61*/unsigned short eq_shield;
	/*62*/unsigned short eq_helmet;
	/*63*/unsigned short eq_armor;
	/*64*/unsigned short eq_accessory;
	/*82*/int what82;

public:
	LsdCharacter() {
		m_link = NULL;
	}

	void newgame(LdbMain &ldb, LdbCharacter &n) ;

	void serialize(StructLoader &sl) {
		if (m_link == NULL) {
			BaseStruct::serialize (sl, 0x01, name		, ""	, "" , true);
			BaseStruct::serialize (sl, 0x02, title	, ""	, "" , true);
		} else {
			BaseStruct::serialize (sl, 0x01, name		, m_link->name	, "" , true);
			BaseStruct::serialize (sl, 0x02, title	, m_link->title	, "" , true);
		}
		BaseStruct::serialize (sl, 0x0B, pic_name	, "");
		BaseStruct::serialize (sl, 0x0C, pic_id	, 0);

		BaseStruct::serialize (sl, 0x0D, pic_default_trans	,3);//semitrans = 3, notrans = 0

		BaseStruct::serialize (sl, 0x15, face_name	,"");
		BaseStruct::serialize (sl, 0x16, face_index	,0);

		BaseStruct::serialize (sl, 0x1F, clevel		,0);
		BaseStruct::serialize (sl, 0x20, cexp);

		BaseStruct::serialize (sl, 0x21, mhp);
		BaseStruct::serialize (sl, 0x22, mmp);
		BaseStruct::serialize (sl, 0x29, catk			,0);
		BaseStruct::serialize (sl, 0x2A, cdef			,0);
		BaseStruct::serialize (sl, 0x2B, cspt			,0);
		BaseStruct::serialize (sl, 0x2C, cagi			,0);

		BaseStruct::serialize (sl, 0x33, skills_size);
		BaseStruct::serialize (sl, 0x34, skills);
		BaseStruct::serialize (sl, 0x3D, &cequip);
		BaseStruct::serialize (sl, 0x47, chp			,0);
		BaseStruct::serialize (sl, 0x48, cmp			,0);
		BaseStruct::serialize (sl, 0x50, battle_commands);
		BaseStruct::serialize (sl, 0x51, ccond_size	,0);
		BaseStruct::serialize (sl, 0x52, ccond);
		BaseStruct::serialize (sl, 0x5A, cjob			,0);

		BaseStruct::serialize (sl, 0x5B, is_back_attack	,false);
		if (m_link == NULL) {
			BaseStruct::serialize (sl, 0x5C, is_two_hand_sword,false);
			BaseStruct::serialize (sl, 0x5D, is_equip_fixed	,false);
			BaseStruct::serialize (sl, 0x5E, is_auto_battle	,false);
			BaseStruct::serialize (sl, 0x5F, is_super_gard	,false);
		} else {
			BaseStruct::serialize (sl, 0x5C, is_two_hand_sword,m_link->use_two_hand);
			BaseStruct::serialize (sl, 0x5D, is_equip_fixed	,m_link->fix_equip);
			BaseStruct::serialize (sl, 0x5E, is_auto_battle	,m_link->auto_battle);
			BaseStruct::serialize (sl, 0x5F, is_super_gard	,m_link->super_guard);
		}

		if (sl.getMode() == StructLoaderModeWrite) {
			eq_weapon	= cequip.weapon_id;
			eq_shield	= cequip.shield_id;
			eq_helmet	= cequip.helmet_id;
			eq_armor	= cequip.armor_id;
			eq_accessory= cequip.accessory_id;
		}

		if (m_link == NULL) {
			BaseStruct::serialize (sl, 0x60, eq_weapon		,(unsigned short)0);
			BaseStruct::serialize (sl, 0x61, eq_shield		,(unsigned short)0);
			BaseStruct::serialize (sl, 0x62, eq_helmet		,(unsigned short)0);
			BaseStruct::serialize (sl, 0x63, eq_armor			,(unsigned short)0);
			BaseStruct::serialize (sl, 0x64, eq_accessory		,(unsigned short)0);
		} else {
			BaseStruct::serialize (sl, 0x60, eq_weapon		,m_link->init_equip.weapon_id);
			BaseStruct::serialize (sl, 0x61, eq_shield		,m_link->init_equip.shield_id);
			BaseStruct::serialize (sl, 0x62, eq_helmet		,m_link->init_equip.helmet_id);
			BaseStruct::serialize (sl, 0x63, eq_armor			,m_link->init_equip.armor_id);
			BaseStruct::serialize (sl, 0x64, eq_accessory		,m_link->init_equip.accessory_id);
		}
		sl.finishStruct();
	}

	void link( LdbCharacter *dChar ) 
	{
		m_link = dChar;	
	}
};

#endif
