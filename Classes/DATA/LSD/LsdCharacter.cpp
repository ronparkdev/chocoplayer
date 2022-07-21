#include "LsdCharacter.h"

#include "../../COMMON/DebugingCode.h"

void LsdCharacter::newgame( LdbMain &ldb, LdbCharacter &n )
{
	name = n.name;
	title = n.title;

	pic_name = n.pic_name;
	pic_id = n.pic_id;
	pic_default_trans = (n.transparent?3:0);

	face_name = n.face_name;
	face_index = n.face_index;

	clevel = n.init_level;
	cexp = 0;

	LdbCharacterParameter &p = n.parameters;

	mhp = chp = p.mhp[clevel-1];
	mmp = cmp = p.mmp[clevel-1];
	catk = p.atk[clevel-1];
	cdef = p.def[clevel-1];
	cspt = p.spt[clevel-1];
	cagi = p.agi[clevel-1];

	skills.RemoveAll();
	skills.ChangeMode(ArrayMode);

	for(int i=0; i<n.skills.GetLength(); i++) {
		GlbSkillLearning &s = n.skills[i];
		if (s.level <= clevel) {

			int j;
			for(j=0; j<skills.GetLength(); j++) {
				if (s.skill_id < skills[j]) {
					skills.Add(j, s.skill_id);
					break;
				} else if (s.skill_id == skills[j]) {
					break;
				}
			}

			if (j == skills.GetLength())
				skills .Add(s.skill_id);
		}
	}

	ccond.RemoveAll();
	ccond.ChangeMode(ArrayMode);
	ccond.SetLength(ldb.dCond.GetLength());
	for(int i=0; i<ccond.GetLength(); i++)
		ccond[i] = false;

	cjob = n.job_id;

	//is_back_attack = n.
	is_two_hand_sword = n.use_two_hand;
	is_equip_fixed = n.fix_equip;
	is_auto_battle = n.auto_battle;
	is_super_gard = n.super_guard;

	GlbEquipment &e = n.init_equip;
	cequip = e;
	eq_weapon = e.weapon_id;
	eq_shield = e.shield_id;
	eq_helmet = e.helmet_id;
	eq_armor = e.armor_id;
	eq_accessory = e.accessory_id;

	battle_commands = n.battle_commends;
	
}