#include "MgrCharacter.h"

#include "MgrItem.h"
#include "MgrSkill.h"

MgrCharacter::MgrCharacter( GameMain *main )
{
	m_main = main;
	lastAddedId = 0;
}

int MgrCharacter::get( int id, CMGR_CMD cmd )
{
	if (id <= 0 || id > m_main->ldb.dChar.GetLength())
		return 0;

	MgrItem mitem(m_main);

	LsdCharacter *c = &(m_main->lsd.Character[id-1]);

	int val = 0;
	switch(cmd) {
		case LEVEL:	val = c->clevel;break;
		case EXP:	val = c->cexp;	break;
		case HP:	val = c->chp;	break;
		case MP:	val = c->cmp;	break;
		case MAXHP:	val = c->mhp	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case MAXMP:	val = c->mmp	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case ATK:	val = c->catk	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case DEF:	val = c->cdef	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case AGI:	val = c->cagi	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case SPT:	val = c->cspt	+ mitem.getEffectVal(c->cequip, cmd);	break;
		case JOB:	val = c->cjob;	break;
		case NEXTEXP:
		{	
			LdbCharacter &dc = m_main->ldb.dChar[id-1];

			if (m_main->getVersion() == RPG2000) {
				//값1 = ((기본치-10)/40*3+1)*((증가도-10)/40*19+1)
				//다음레벨 = 기본치*A5+값1*POWER(1.2,A5-1)-값1+보정치
				int ebase, einf;
				ebase = (dc.exp_base < 10)?10:dc.exp_base;
				einf = (dc.exp_inflation < 10)?10:dc.exp_inflation;

				float t = ((ebase-10.0f)/40.0f*3.0f+1.0f)*((einf-10.0f)/40.0f*19.0f+1.0f);

				for(int i=1; i<=c->clevel; i++)
					val += (ebase*i + t*(pow(1.2f, i-1) - 1.0f) + dc.exp_correction);
			
			} else {
				for(int i=1; i<=c->clevel; i++)
					val +=	dc.exp_base + 
							dc.exp_correction + 
							dc.exp_inflation * i;
			}
			break;
		}
	}

	return chk(id, cmd, val);
}

TString MgrCharacter::getS( int id, CMGR_CMD cmd )
{
	if (id <= 0 || id > m_main->ldb.dChar.GetLength())
		return 0;

	MgrItem mitem(m_main);

	LsdCharacter *c = &(m_main->lsd.Character[id-1]);

	switch(cmd) {
		case NAME:	return c->name;
		case TITLE:	return c->title;
		case JOB:	return c->cjob;
		case STATE:	return getStateStr(id);

		default:return "";
	}
}

int MgrCharacter::add( int id, CMGR_CMD cmd, int val )
{
	if (id < 0 || id > m_main->ldb.dChar.GetLength())
		return 0;
	
	if (val == 0)
		return 0;

	if (id == 0) {
		for(int i=0; i<m_main->lsd.State.party.GetLength(); i++) {
			int hid = m_main->lsd.State.party[i];
			add(hid, cmd, val);
		}
		return 0;
	}
	
	LsdCharacter *c = &(m_main->lsd.Character[id-1]);
	int lastval, newval;
	
	lastAddedId = 0;
	lastval = get(id, cmd);

	switch(cmd) {
		case JOB:	newval = val;				break;
		default:
			newval = lastval + val;break;
	}

	newval = chk(id, cmd, newval);

	switch(cmd) {
	case LEVEL:
		{
			LdbCharacter &n = m_main->ldb.dChar[id-1];

			lastAddedId = id;
			lastLevel = newval;
			lastAddedSkill.RemoveAll();

			for(int lev=0; lev<n.skills.GetLength(); lev++) {
				GlbSkillLearning &s = n.skills[lev];
				if (lastval < s.level && s.level <= newval) //TODO NEED USING BUT IT IS NEED NOWDAYS FOR FIX CURRENT BUG STUCKED USER
					m_main->lsdbuf.SetSkill(id, s.skill_id, true);
			}


			//TODO NEED CHECK
			add(id, MAXHP	, n.parameters.mhp[newval-1] - n.parameters.mhp[lastval-1]);
			add(id, MAXMP	, n.parameters.mmp[newval-1] - n.parameters.mmp[lastval-1]);
			add(id, ATK		, n.parameters.atk[newval-1] - n.parameters.atk[lastval-1]);
			add(id, DEF		, n.parameters.def[newval-1] - n.parameters.def[lastval-1]);
			add(id, AGI		, n.parameters.agi[newval-1] - n.parameters.agi[lastval-1]);
			add(id, SPT		, n.parameters.spt[newval-1] - n.parameters.spt[lastval-1]);

			break;
		}
	case EXP:
		{
			int lastLevel = getLevel(id, lastval);
			int curLevel = getLevel(id, newval);	

			if (curLevel > lastLevel)
				add(id, LEVEL, curLevel - lastLevel);

			break;
		}
	}

	set(id, cmd, newval);

	return newval - val;
}

void MgrCharacter::set( int id, CMGR_CMD cmd, int val )
{
	if (id <= 0 || id > m_main->ldb.dChar.GetLength())
		return;

	LsdCharacter *c = &(m_main->lsd.Character[id-1]);

	switch(cmd) {
	case LEVEL:	c->clevel=val;	break;
	case EXP:	c->cexp	= val;	break;
	case HP:	c->chp	= val;	break;
	case MP:	c->cmp	= val;	break;
	case MAXHP:	c->mhp	= val;	break;
	case MAXMP:	c->mmp	= val;	break;
	case ATK:	c->catk	= val;	break;
	case DEF:	c->cdef	= val;	break;
	case AGI:	c->cagi	= val;	break;
	case SPT:	c->cspt	= val;	break;
	case JOB:	c->cjob	= val;	break;
	}
}

int MgrCharacter::chk( int id, CMGR_CMD cmd, int val )
{
	LdbCharacter &dc = m_main->ldb.dChar[id-1];

	switch(cmd) {
	case LEVEL:
		{
			if (val < 1)
				val = 1;
			if (val > dc.final_level)
				val = dc.final_level;
			if (m_main->getVersion() == RPG2000 && val > 50)
				val = 50;
			break;
		}

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
		{
			if (val <= 0) {
				val = 0;
				setState(id, 1, true);
			} else {
				setState(id, 1, false);
			}
			if (val > 999999)
				val = 999999;
			if (val > get(id, MAXHP))
				val = get(id, MAXHP);
			break;
		}

	case MP:
		{
			if (val < 0)
				val = 0;
			if (val > 999999)
				val = 999999;
			if (val > get(id, MAXMP))
				val = get(id, MAXMP);
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

int MgrCharacter::getLevel(int hid, int exp) {
	LdbCharacter &dc = m_main->ldb.dChar[hid-1];
	int val = 0;
	if (m_main->getVersion() == RPG2000) {
		//값1 = ((기본치-10)/40*3+1)*((증가도-10)/40*19+1)
		//다음레벨 = 기본치*A5+값1*POWER(1.2,A5-1)-값1+보정치
		int ebase, einf;
		ebase = (dc.exp_base < 10)?10:dc.exp_base;
		einf = (dc.exp_inflation < 10)?10:dc.exp_inflation;

		float t = ((ebase-10.0f)/40.0f*3.0f+1.0f)*((einf-10.0f)/40.0f*19.0f+1.0f);

		for(int i=1; i<=dc.final_level; i++) {
			val += (ebase*i + t*(pow(1.2f, i-1) - 1.0f) + dc.exp_correction);

			if (exp < val)
				return i-1;
		}
	} else {
		for(int i=1; i<=dc.final_level; i++) {
			val +=	dc.exp_base + 
			dc.exp_correction + 
			dc.exp_inflation * i;

			if (exp < val)
				return i-1;
		}
	}
	return dc.final_level;
}

void MgrCharacter::getLastLevelUpMsg(ArrayList<TString>& msg)
{
	MgrSkill mskill(m_main);
	LdbVoca &voca = m_main->ldb.dVoca;
	GlobalLangInGame &lang = m_main->inlang;
	
	int id = lastAddedId;

	if (id == 0)
		return;

	msg.Add(getS(id, NAME) + lang.getLang(Game_LvUp) + voca.level + lang.getLang(Game_LvUp2) + lastLevel + voca.level_up);

	ArrayList<byte> &askill = lastAddedSkill;

	for(int i=0; i<askill.GetLength(); i++)
		msg.Add(getS(id, NAME) + lang.getLang(Game_LvUp) + mskill.getName(askill[i]) + voca.skill_learned);
}