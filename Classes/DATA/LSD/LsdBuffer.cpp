#include "LsdBuffer.h"

#include "../../GameMain.h"

LsdBuffer::LsdBuffer( GameMain *main ) :m_main(main)
	,m_skill(NULL)
	,m_itemcnt(NULL)
	,m_itemused(NULL)
	,m_itemcnt_bak(NULL)
	,m_itemused_bak(NULL)
{

}

LsdBuffer::~LsdBuffer()
{
	if (m_skill != NULL)
		delete []m_skill;

	if (m_itemcnt != NULL)
		delete []m_itemcnt;

	if (m_itemused != NULL)
		delete []m_itemused;
}

void LsdBuffer::Init()
{
	LdbMain &ldb = m_main->ldb;
	m_heroLen	= ldb.dChar.GetLength();
	m_skillLen	= ldb.dSkill.GetLength();
	m_itemLen	= ldb.dItem.GetLength();

	if (m_skill != NULL)
		delete []m_skill;

	if (m_itemcnt != NULL)
		delete []m_itemcnt;

	if (m_itemused != NULL)
		delete []m_itemused;

	m_skill		= new bool[m_heroLen * m_skillLen];
	m_itemcnt	= new int[m_itemLen]; 
	m_itemused	= new int[m_itemLen];

	memset(m_skill		, 0, sizeof(bool)	* m_heroLen * m_skillLen);
	memset(m_itemcnt	, 0, sizeof(int)	* m_itemLen);
	memset(m_itemused	, 0, sizeof(int)	* m_itemLen);
}

void LsdBuffer::Save()
{
	int i, j, id;
	LsdState &state = m_main->lsd.State;

	id = 0;
	for(i=0; i<m_heroLen; i++) {
		LsdCharacter &lsdCh = m_main->lsd.Character[i];
		lsdCh.skills.RemoveAll();
		for(j=0; j<m_skillLen; j++)
			if (m_skill[id++])
				lsdCh.skills.Add(j+1);
		lsdCh.skills_size = lsdCh.skills.GetLength();
	}

	state.item_index.RemoveAll();
	state.item_count.RemoveAll();
	state.item_used.RemoveAll();
	for(i=0; i<m_itemLen; i++) {
		if (m_itemcnt[i] <= 0)
			continue;

		state.item_index.Add(i+1);
		state.item_count.Add(m_itemcnt[i]);
		state.item_used	.Add(m_itemused[i]);
	}
	state.item_length = state.item_index.GetLength();
}

void LsdBuffer::Load()
{
	int i, j, id;
	LsdState &state = m_main->lsd.State;

	memset(m_skill		, 0, sizeof(bool)	* m_heroLen * m_skillLen);
	memset(m_itemcnt	, 0, sizeof(int)	* m_itemLen);
	memset(m_itemused	, 0, sizeof(int)	* m_itemLen);

	int lastHeroLen = m_main->lsd.Character.GetLength();
	if (lastHeroLen < m_heroLen) {
		m_main->lsd.Character.SetLength(m_heroLen);

		StructLoader sl_init;
		sl_init.setInitMode();

		for(i=lastHeroLen; i<m_heroLen; i++) {
			LsdCharacter &lsdCh = m_main->lsd.Character[i];

			lsdCh.link(&m_main->ldb.dChar[i]);
			lsdCh.serialize(sl_init);
			
		}
	}

	id = 0;


	for(i=0; i<m_heroLen; i++) {

		LdbCharacter &ldbCh = m_main->ldb.dChar[i];
		LsdCharacter &lsdCh = m_main->lsd.Character[i];

		for(j=0; j<lsdCh.skills.GetLength(); j++)
			m_skill[i * m_skillLen + lsdCh.skills[j]-1] = true;

		//TODO NO NEED BUT IT IS NEED NOWDAYS FOR FIX CURRENT BUG STUCKED USER
// 		for(int id=0; id<dch.skills.GetLength(); id++) {
// 			GlbSkillLearning &s = dch.skills[id];
// 			if (s.level <= ch.clevel) 
// 				m_skill[i * m_skillLen + (s.skill_id-1)] = true;
// 		}
	}

	for(i=0; i<state.item_index.GetLength(); i++) {
		id = state.item_index[i]-1;
		m_itemcnt[id]	= state.item_count[i];
		m_itemused[id]	= state.item_used[i];
	}
}

bool LsdBuffer::GetSkill(int hID, int sID) {
	if (hID<=0 || hID>m_heroLen ||
		sID<=0 || sID>m_skillLen)
		return false;

	return m_skill[(hID-1) * m_skillLen + (sID-1)];
}

void LsdBuffer::SetSkill(int hID, int sID, bool val) {
	if (hID<=0 || hID>m_heroLen ||
		sID<=0 || sID>m_skillLen)
		return;

	m_skill[(hID-1) * m_skillLen + (sID-1)] = val;
}

void LsdBuffer::ClsSkill( int heroID )
{
	memset(m_skill + sizeof(bool) * m_skillLen * heroID	, 0 , sizeof(bool)	* m_skillLen);
}


int LsdBuffer::GetItem(int iID) {
	if (iID<=0 || iID>m_itemLen)
		return 0;

	return m_itemcnt[iID-1];
}

void LsdBuffer::SetItem(int iID, int val) {
	if (iID<=0 || iID>m_itemLen)
		return;

	if (val < 0)
		val = 0;

	if (val > 99)
		val = 99;

	m_itemcnt[iID-1] = val;

	if (val == 0)
		m_itemused[iID-1] = 0;
}

void LsdBuffer::AddItem(int iID, int val) {
	if (iID<=0 || iID>m_itemLen)
		return;

	SetItem(iID, GetItem(iID) + val);
}

void LsdBuffer::UseItem(int iID) {
	if (iID<=0 || iID>m_itemLen)
		return;

	LdbItem &item = m_main->ldb.dItem[iID-1];

	if (item.uses == 0)
		return;

	m_itemused[iID-1]++;

	if (item.uses <= m_itemused[iID-1]) {
		m_itemused[iID-1] = 0;
		if (m_itemcnt[iID-1] > 0)
			m_itemcnt[iID-1]--;
	}
}

void LsdBuffer::ClsUseItem(int iID) {
	if (iID<=0 || iID>m_itemLen)
		return;

	m_itemused[iID-1] = 0;
}

void LsdBuffer::BackupItem()
{	
	if (m_itemcnt_bak != NULL)
		delete []m_itemcnt_bak;
	
	if (m_itemused_bak != NULL)
		delete []m_itemused_bak;
	
	m_itemcnt_bak	= new int[m_itemLen]; 
	memcpy(m_itemcnt_bak,	m_itemcnt,	sizeof(int) * m_itemLen);

	m_itemused_bak	= new int[m_itemLen];
	memcpy(m_itemused_bak,	m_itemused,	sizeof(int) * m_itemLen);
}

void LsdBuffer::RestoreItem() {
	if (m_itemcnt_bak != NULL) {
		memcpy(m_itemcnt,	m_itemcnt_bak,	sizeof(int) * m_itemLen);
		delete []m_itemcnt_bak;
		m_itemcnt_bak = NULL;
	}
	
	if (m_itemused_bak != NULL) {
		memcpy(m_itemused,	m_itemused_bak,	sizeof(int) * m_itemLen);
		delete []m_itemused_bak;
		m_itemused_bak = NULL;
	}
}
