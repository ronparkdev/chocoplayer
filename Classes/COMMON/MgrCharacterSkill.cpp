#include "MgrCharacter.h"

bool MgrCharacter::getSkill( int hID, int skillID )
{
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength() ||
		skillID <= 0 || skillID > m_main->ldb.dSkill.GetLength())
		return false;

	return m_main->lsdbuf.GetSkill(hID, skillID);
}

void MgrCharacter::setSkill( int hID, int skillID, bool val )
{
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength() ||
		skillID <= 0 || skillID > m_main->ldb.dSkill.GetLength())
		return;

	m_main->lsdbuf.SetSkill(hID, skillID, val);
}
