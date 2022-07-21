#include "MgrCharacter.h"

bool MgrCharacter::getState( int hID, int stateID )
{
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength() ||
		stateID<=0 || stateID > m_main->ldb.dCond.GetLength())
		return false;

	return m_main->lsd.Character[hID-1].ccond[stateID-1];
}

int MgrCharacter::getTopState(int hID) {
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength())
		return 0;

	ArrayList<bool> &conds = m_main->lsd.Character[hID-1].ccond;
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


TString MgrCharacter::getStateStr(int hID) {
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength())
		return "";

	int state = getTopState(hID) - 1;

	if (state == -1) {
		return m_main->ldb.dVoca.normal_status;
	} else {
		ArrayList<LdbCondition> &dconds = m_main->ldb.dCond;
		return (_ST("\\C[") + dconds[state].color + _ST("]") + dconds[state].name);
	}
}

void MgrCharacter::setState( int hID, int stateID, bool val )
{
	if (	hID < 0 || hID > m_main->ldb.dChar.GetLength() ||
		stateID<=0 || stateID>=m_main->ldb.dCond.GetLength())
		return;

	if (hID == 0) {
		for(int i=0; i<m_main->lsd.State.party.GetLength(); i++) {
			int hID2 = m_main->lsd.State.party[i];
			setState(hID2, stateID, val);
		}
		return;
	}

	ArrayList<bool> &conds = m_main->lsd.Character[hID-1].ccond;
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

void MgrCharacter::clearState(int hID) {
	if (	hID <= 0 || hID > m_main->ldb.dChar.GetLength())
		return;

	ArrayList<bool> &conds = m_main->lsd.Character[hID-1].ccond;

	for(int i=0; i<conds.GetLength(); i++)
		conds[i] = false;
}