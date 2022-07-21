#include "ScrBattleBattlerManager.h"

ScrBattleBattlerMgr::ScrBattleBattlerMgr( GameMain *main, int battle_id, BatPos pos)
	:mchar(main)
{
	m_main = main;

	ArrayList<short> &hero_list = main->lsd.State.party;
	LdbEnemyGroup &enemy_group = main->ldb.dEnemys[battle_id-1];

	int state_length = main->ldb.dCond.GetLength();

	m_hero_length = min(hero_list.GetLength(), SCR_BATTLE_MAX_HEROS);
	m_enemy_length = min(enemy_group.members.GetLength(), SCR_BATTLE_MAX_ENEMIES);

	for(int i=0; i<m_hero_length; i++) {
		int hero_id = hero_list[i];
		ScrBattleBattlerInfo &d = data[i];
		LdbCharacter &ldb = m_main->ldb.dChar[hero_id-1];

		d.invisible = false;

		d.data_id = hero_id;

		d.mhp = mchar.get(hero_id, MAXHP);
		d.mmp = mchar.get(hero_id, MAXMP);
		d.chp = mchar.get(hero_id, HP);
		d.cmp = mchar.get(hero_id, MP);
		d.catk = mchar.get(hero_id, ATK);
		d.cdef = mchar.get(hero_id, DEF);
		d.cspt = mchar.get(hero_id, SPT);
		d.cagi = mchar.get(hero_id, AGI);

		d.ccond.SetLength(state_length);
		for(int j=0; j<state_length; j++)
			d.ccond[j] = mchar.getState(hero_id, j+1);

		d.defence_type = 0;

		if (!ldb.critical_hit)
			d.critical_chance = 0;
		else
			d.critical_chance = 100 / ldb.critical_hit_chance;

		int weaponID = getEquip(i).weapon_id;
		if (weaponID > 0) {
			LdbItem &item = m_main->ldb.dItem[weaponID-1];
			d.critical_chance += item.critical_hit;
		}

		d.is_power_up = false;

		if (!m_main->ldb.dBatCmd.enemy_auto_arrange) {
			d.def_pos_x = ldb.battle_x;
			d.def_pos_y = ldb.battle_y;
			switch(pos) {
				case BPOS_BACKATTACK_ENEMY:
				case BPOS_SIDEATTACK_ENEMY:
					d.def_pos_x = 320 - d.def_pos_x;
					break;
			}
		} else {
			if (m_hero_length == 1) {
				d.def_pos_x = 264;
				d.def_pos_y = 107;
			} else {
				d.def_pos_x = 273*i/(m_hero_length-1) + 256*(m_hero_length-1-i)/(m_hero_length-1);
				d.def_pos_y = 132*i/(m_hero_length-1) + 88*(m_hero_length-1-i)/(m_hero_length-1);
			}
		}
		d.def_pos_y = 240 - d.def_pos_y;

		d.turn_count = 0;
	}

	for(int i=0; i<m_enemy_length; i++) {
		int enemy_id = enemy_group.members[i].enemy_id;
		LdbEnemy &ldb = main->ldb.dEnemy[enemy_id-1];
		ScrBattleBattlerInfo &d = data[i+SCR_BATTLE_MAX_HEROS];

		d.invisible = enemy_group.members[i].invisible;

		d.data_id = enemy_id;

		d.mhp = d.chp = ldb.max_hp;
		d.mmp = d.cmp = ldb.max_mp;
		d.catk = ldb.attack;
		d.cdef = ldb.defense;
		d.cspt = ldb.spirit;
		d.cagi = ldb.agility;

		d.ccond.SetLength(state_length);
		for(int j=0; j<state_length; j++)
			d.ccond[j] = false;

		d.defence_type = 0;

		if (!ldb.critical_hit)
			d.critical_chance = 0;
		else
			d.critical_chance = 100 / ldb.critical_hit_chance;

		d.is_power_up = false;

		if (!m_main->ldb.dBatCmd.enemy_auto_arrange) {
			d.def_pos_x = enemy_group.members[i].x;
			d.def_pos_y = enemy_group.members[i].y;

			switch(pos) {
			case BPOS_BACKATTACK_ENEMY:
			case BPOS_SIDEATTACK_ENEMY:
				d.def_pos_x = 320 - d.def_pos_x;
				break;
			}
		} else {
			if (m_enemy_length == 1) {
				d.def_pos_x = 56;
				d.def_pos_y = 107;
			} else {
				d.def_pos_x = 47*i/(m_enemy_length-1) + 64*(m_enemy_length-1-i)/(m_enemy_length-1);
				d.def_pos_y = 132*i/(m_enemy_length-1) + 88*(m_enemy_length-1-i)/(m_enemy_length-1);
			}
		}
		d.def_pos_y = 240 - d.def_pos_y;

		d.turn_count = 0;
	}
}

int ScrBattleBattlerMgr::getID( int id )
{
	if ( (0<=id && id<m_hero_length) ||
		(SCR_BATTLE_MAX_HEROS<=id && id<SCR_BATTLE_MAX_HEROS+m_enemy_length))
		return data[id].data_id;
	else
		return 0;
}

void ScrBattleBattlerMgr::setID( int id, int mID )
{
	if (id < SCR_BATTLE_MAX_HEROS || SCR_BATTLE_MAX_HEROS+m_enemy_length<=id)
		return;

	ScrBattleBattlerInfo &d = data[id];

	d.data_id = mID;
	
	LdbEnemy &ldb = m_main->ldb.dEnemy[mID-1];

	d.mhp = ldb.max_hp;
	d.mmp = ldb.max_mp;
	d.catk = ldb.attack;
	d.cdef = ldb.defense;
	d.cspt = ldb.spirit;
	d.cagi = ldb.agility;
}

int ScrBattleBattlerMgr::get( int id, CMGR_CMD cmd )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return 0;

	ScrBattleBattlerInfo &d = data[id];
	
	int ldbID = getID(id);

	int val = 0;
	switch(cmd) {
	case HP:
	case MP:
		{
			if (isHero(id)) {
				val = mchar.get(ldbID, cmd);
			} else {
				switch(cmd) {
				case HP:	val = d.chp;	break;
				case MP:	val = d.cmp;	break;
				}
			}
			break;
		}
		
	case MAXHP:	val = d.mhp;	break;
	case MAXMP:	val = d.mmp;	break;
	case ATK:	val = d.catk;	break;
	case DEF:	val = d.cdef;	break;
	case AGI:	val = d.cagi;	break;
	case SPT:	val = d.cspt;	break;

	case BAT_INVISIBLE:			val = d.invisible;break;
	case BAT_CRITICAL_CHANCE:	val = d.critical_chance;break;
	case BAT_DEFENCE_TYPE:		val = d.defence_type;break;
	case BAT_POWER_UP:			val = d.is_power_up;break;
	case BAT_TURN_COUNT:		val = d.turn_count;break;
	}

	return chk(id, cmd, val);
}

TString ScrBattleBattlerMgr::getS( int id, CMGR_CMD cmd )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return "";

	ScrBattleBattlerInfo &d = data[id];

	switch(cmd) {
	case NAME:
		{
			if (id<SCR_BATTLE_MAX_HEROS) {
				return m_main->lsd.Character[d.data_id-1].name;
			} else {
				return m_main->ldb.dEnemy[d.data_id-1].name;
			}
		};

	case STATE:	return getStateStr(id);

	case HP:
		{
			if (get(id, HP) <= get(id, MAXHP)/4)
				return _ST("\\C[4]") + get(id, HP);
			else
				return _ST("\\C[0]") + get(id, HP);
		}

	case MP:
		{
			if (get(id, MP) <= get(id, MAXMP)/4)
				return _ST("\\C[4]") + get(id, MP);
			else
				return _ST("\\C[0]") + get(id, MP);
		}

	default:return "";
	}
}

int ScrBattleBattlerMgr::add( int id, CMGR_CMD cmd, int val )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return 0;

	ScrBattleBattlerInfo &d = data[id];

	int ldbID = getID(id);

	int newval = 0;

	switch(cmd) {
	case EXP:
		{
			newval = mchar.add(ldbID, cmd, val);
			return newval;
		}
	case HP:
	case MP:
		{
			if (isHero(id))
				return mchar.add(ldbID, cmd, val);
		}
	case MAXHP:
	case MAXMP:
	case ATK:
	case DEF:
	case AGI:
	case SPT:
		newval = get(id, cmd)	+ val;	
		break;

	case BAT_INVISIBLE:		
	case BAT_DEFENCE_TYPE:
	case BAT_POWER_UP:		
		newval = val;break;

	case BAT_TURN_COUNT: 
		newval = d.turn_count + val;break;
	}

	newval = chk(id, cmd, newval);

	set(id, cmd, newval);

	if (get(id, HP) <= 0)
		setState(id, 1, true);

	return newval - val;
}

int ScrBattleBattlerMgr::getTopState( int id )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return 0;

	if (isHero(id)) {
		int ldbID = getID(id);

		return mchar.getTopState(ldbID);
	} else {
		ArrayList<bool> &conds = data[id].ccond;
		ArrayList<LdbCondition> &dconds = m_main->ldb.dCond;

		int topProrityVal = 0, state = -1;
		for(int i=0; i<conds.GetLength(); i++)
			if (conds[i])
				if (state == -1 || topProrityVal < dconds[i].priority) {
					topProrityVal = dconds[i].priority;
					state = i;
				}

		return state+1;
	}
}

TString ScrBattleBattlerMgr::getStateStr( int id )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return "";

	int state = getTopState(id);

	if (state <= 0) {
		return m_main->ldb.dVoca.normal_status;
	} else {
		ArrayList<LdbCondition> &dconds = m_main->ldb.dCond;
		return (_ST("\\C[") + dconds[state-1].color + _ST("]") + dconds[state-1].name);
	}
}

bool ScrBattleBattlerMgr::getState( int id, int stateID )
{
	if (	id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES ||
		stateID<=0 || stateID>=m_main->ldb.dCond.GetLength())
		return false;

	if (isHero(id)) {
		int ldbID = getID(id);

		return mchar.getState(id, stateID);
	} else {
		ArrayList<bool> &conds = data[id].ccond;

		return conds[stateID-1];
	}
}

void ScrBattleBattlerMgr::setState( int id, int stateID, bool val )
{
	if (	id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES ||
		stateID<=0 || stateID>=m_main->ldb.dCond.GetLength())
		return;

	if (isHero(id)) {
		int ldbID = getID(id);

		mchar.setState(ldbID, stateID, val);
	} else {
		ArrayList<bool> &conds = data[id].ccond;
		ArrayList<LdbCondition> &dconds = m_main->ldb.dCond;

		conds[stateID-1] = val;

		int topPrority = 0;
		for(int i=0; i<conds.GetLength(); i++)
			if (conds[i])
				if (topPrority < dconds[i].priority)
					topPrority = dconds[i].priority;

		for(int i=0; i<conds.GetLength(); i++)
			if (conds[i])
				if (topPrority - dconds[i].priority >= 10)
					conds[i] = false;
	}
}

void ScrBattleBattlerMgr::clearState( int id )
{
	if (	id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return;

	if (isHero(id)) {
		int ldbID = getID(id);

		mchar.clearState(ldbID);
	} else {
		ArrayList<bool> &conds = data[id].ccond;

		for(int i=0; i<conds.GetLength(); i++)
			conds[i] = false;
	}
}

void ScrBattleBattlerMgr::set( int id, CMGR_CMD cmd, int val )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return;

	ScrBattleBattlerInfo &d = data[id];

	switch(cmd) {
	case HP:	d.chp	= val;	break;
	case MP:	d.cmp	= val;	break;
	case MAXHP:	d.mhp	= val;	break;
	case MAXMP:	d.mmp	= val;	break;
	case ATK:	d.catk	= val;	break;
	case DEF:	d.cdef	= val;	break;
	case AGI:	d.cagi	= val;	break;
	case SPT:	d.cspt	= val;	break;

	case BAT_INVISIBLE:		d.invisible = val;break;
	case BAT_DEFENCE_TYPE:	d.defence_type = val;break;
	case BAT_POWER_UP:		d.is_power_up = val;break;
	case BAT_TURN_COUNT:	d.turn_count = val;break;
	}
}

int ScrBattleBattlerMgr::chk( int id, CMGR_CMD cmd, int val )
{
	switch(cmd) {
	case MAXHP:
	case MAXMP:
		{
			if (val < 1)
				val = 1;
			if (val > 999999)
				val = 999999;
			break;
		}
	case HP:
	case MP:
		{
			if (val < 0)
				val = 0;
			if (val > 999999)
				val = 999999;
			break;
		}

	case ATK:
	case DEF:
	case SPT:
	case AGI:
		{
			if (val < 1)
				val = 1;
			if (val > 999)
				val = 999;
			break;
		}

	default:
		break;
	}

	return val;
}

GlbEquipment ScrBattleBattlerMgr::getEquip( int id )
{
	GlbEquipment empty;
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS)
		return empty;

	return mchar.getEquip(data[id].data_id);
}

CCPoint ScrBattleBattlerMgr::getDefPos( int id )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return CCPoint();

	return CCPoint(ccp(data[id].def_pos_x, data[id].def_pos_y));
}

void ScrBattleBattlerMgr::setDefPos( int id, const CCPoint &point )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return;

	data[id].def_pos_x = point.x;
	data[id].def_pos_y = point.y;
}	

bool ScrBattleBattlerMgr::isHero( int id )
{
	return (id < SCR_BATTLE_MAX_HEROS);
}

bool ScrBattleBattlerMgr::isPreventCritical( int id )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS)
		return false;

	ArrayList<LdbItem> &items = m_main->ldb.dItem;
	GlbEquipment equip = getEquip(id);

	bool val = false;

	if (equip.shield_id > 0)
		val |= items[equip.shield_id-1].prevent_critical;

	if (equip.helmet_id > 0)
		val |= items[equip.helmet_id-1].prevent_critical;

	if (equip.armor_id > 0)
		val |= items[equip.armor_id-1].prevent_critical;

	if (equip.accessory_id > 0)
		val |= items[equip.accessory_id-1].prevent_critical;

	return val;
}

bool ScrBattleBattlerMgr::isRaiseEvasion( int id )
{
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS)
		return false;

	int LdbId = getID(id);
	bool isTwoHandWeapon = m_main->ldb.dChar[LdbId-1].use_two_hand;

	ArrayList<LdbItem> &items = m_main->ldb.dItem;
	GlbEquipment equip = getEquip(id);

	bool val = false;

	if (!isTwoHandWeapon && equip.shield_id > 0)
		val |= items[equip.shield_id-1].raise_evasion;

	if (equip.helmet_id > 0)
		val |= items[equip.helmet_id-1].raise_evasion;

	if (equip.armor_id > 0)
		val |= items[equip.armor_id-1].raise_evasion;

	if (equip.accessory_id > 0)
		val |= items[equip.accessory_id-1].raise_evasion;

	return val;
}

bool ScrBattleBattlerMgr::isStateEff(int id, int stateID, bool isDeffence) {
	if (id < 0 || id >= SCR_BATTLE_MAX_HEROS)
		return false;

	int LdbId = getID(id);
	bool isTwoHandWeapon = m_main->ldb.dChar[LdbId-1].use_two_hand;

	ArrayList<LdbItem> &items = m_main->ldb.dItem;
	GlbEquipment equip = getEquip(id);

	if (!isDeffence) {
		if (equip.weapon_id > 0 && 
			items[equip.weapon_id-1].attribute_set.GetLength() > stateID &&
			items[equip.weapon_id-1].attribute_set[stateID])
			return true;

		if (isTwoHandWeapon && equip.shield_id > 0 && 
			items[equip.shield_id-1].attribute_set.GetLength() > stateID &&
			items[equip.shield_id-1].attribute_set[stateID])
			return true;
	} else {
		if (!isTwoHandWeapon && equip.shield_id > 0 && 
			items[equip.shield_id-1].attribute_set.GetLength() > stateID &&
			items[equip.shield_id-1].attribute_set[stateID])
			return true;

		if (!isTwoHandWeapon && equip.armor_id > 0 && 
			items[equip.armor_id-1].attribute_set.GetLength() > stateID &&
			items[equip.armor_id-1].attribute_set[stateID])
			return true;

		if (!isTwoHandWeapon && equip.accessory_id > 0 && 
			items[equip.accessory_id-1].attribute_set.GetLength() > stateID &&
			items[equip.accessory_id-1].attribute_set[stateID])
			return true;
	}
	return false;
}

bool ScrBattleBattlerMgr::isAlive( int id )
{
	if ((id<0) ||
		((m_hero_length <= id) && (id < SCR_BATTLE_MAX_HEROS)) ||
		(SCR_BATTLE_MAX_HEROS + m_enemy_length <= id))
			return false;

	int stateID = getTopState(id);

	if (stateID == 1)
		return false;

	if (!isHero(id) && get(id, BAT_INVISIBLE) != 0)
		return false;

	return true;
}

void ScrBattleBattlerMgr::doEvent(const GlbEventNode &edata )
{
	m_event_queue.Add(edata);
}

bool ScrBattleBattlerMgr::getEvent(GlbEventNode &node)
{
	if (m_event_queue.GetLength() == 0)
		return false;

	node = m_event_queue[0];
	m_event_queue.Remove(0);

	return true;
}

int ScrBattleBattlerMgr::getLevel(int hid, int exp) {
	LdbCharacter &dc = m_main->ldb.dChar[hid-1];
	int val = 0;
	for(int i=1; i<=99; i++) {
		val +=	dc.exp_base + 
			dc.exp_correction + 
			dc.exp_inflation * i;
		if (exp < val)
			return i-1;
	}
	return 1;
}

void ScrBattleBattlerMgr::getLastLevelUpMsg(ArrayList<TString>& msg)
{
	mchar.getLastLevelUpMsg(msg);
}

