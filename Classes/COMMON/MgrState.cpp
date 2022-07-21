#include "MgrState.h"

MgrState::MgrState( GameMain *main )
{
	pGM = main;
}

int MgrState::getVar( int index ) {
	ArrayList<int> &vars = pGM->lsd.System.variables;
	if (index < 1 || index > vars.GetLength())
		return 0;
	else
		return vars[index-1];
}

void MgrState::setVar( int index, int val )
{
	ArrayList<int> &vars = pGM->lsd.System.variables;
	if (index < 1 || index > vars.GetLength())
		return;
	else
		vars[index-1] = val;
}

bool MgrState::getSw( int index )
{
	ArrayList<bool> &sws = pGM->lsd.System.switches;
	if (index < 1 || index > sws.GetLength())
		return false;
	else
		return sws[index-1];
}

void MgrState::setSw( int index, bool val )
{
	ArrayList<bool> &sws = pGM->lsd.System.switches;
	if (index < 1 || index > sws.GetLength())
		return;
	else
		sws[index-1] = val;
}

bool MgrState::isPartyIn(int charID )
{
	ArrayList<short> party = pGM->lsd.State.party;
	for(int i=0; i<party.GetLength(); i++) {
		if (party[i] == charID)
			return true;
	}
	return false;
}

void MgrState::setPartyIn(int hID, bool val )
{
	ArrayList<short> party = pGM->lsd.State.party;
	
	int i;
	for(i=0; i<party.GetLength(); i++) {
		if (hID < party[i]) {
			party.Add(i, hID);
			break;
		} else if (party[i] == hID)
			break;
	}
	
	if (i == party.GetLength())
		party.Add(hID);
}

int MgrState::getPartyLen()
{
	return pGM->lsd.State.party.GetLength();
}

bool MgrState::getAbleMenu()
{
	return pGM->lsd.System.able_menu;
}

void MgrState::setAbleMenu( bool val )
{
	pGM->lsd.System.able_menu = val;
}

bool MgrState::getAbleSave()
{
	return pGM->lsd.System.able_save;
}

void MgrState::setAbleSave( bool val )
{
	pGM->lsd.System.able_save = val;
}

bool MgrState::getAbleTeleport()
{
	return pGM->lsd.System.able_teleport;
}

void MgrState::setAbleTeleport( bool val )
{
	pGM->lsd.System.able_teleport = val;
}

bool MgrState::getAbleEscape()
{
	return pGM->lsd.System.able_escape;
}

void MgrState::setAbleEscape( bool val )
{
	pGM->lsd.System.able_escape = val;
}

int MgrState::getMoney()
{
	return pGM->lsd.State.money;
}

void MgrState::addMoney( int val )
{
	int newval = getMoney() + val;

	if (newval < 0)
		newval = 0;

	if (newval > 999999)
		newval = 999999;

	pGM->lsd.State.money = newval;
}
