#include "LsdChk.h"

void LsdChk::chk( LsdMain &lsd1, LsdMain &lsd2 )
{
	chkHeroObject(lsd1.LocHero, lsd2.LocHero);
	for(int i=0; i<3; i++)
		chkRideObject(i, lsd1.LocRide[i], lsd2.LocRide[i]);

	for(int i=0; i<lsd1.Map.events.GetLength(); i++)
		chkMapObject(i, lsd1.Map.events[i], lsd2.Map.events[i]);
}

void LsdChk::chkSystem( LsdSystem &s1, LsdSystem &s2 )
{

}

void LsdChk::chkScreen( LsdScreen &lsd1, LsdScreen &lsd2 )
{

}