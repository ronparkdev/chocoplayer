#include "SysTime.h"

double SysTime::getTime()
{
	time_t utc_time, loc_time;
	tm *utc_time_t, *loc_time_t; 

	time(&utc_time);

	utc_time_t = new tm;
	memcpy(utc_time_t, gmtime(&utc_time), sizeof(tm));

	loc_time_t = localtime(&utc_time);

	loc_time = mktime(loc_time_t);
	utc_time = mktime(utc_time_t);
	
	delete utc_time_t;

	return (double)(time(NULL) + loc_time - utc_time) / (double)(24 * 60 * 60) + 25569.0f;
}

struct tm* SysTime::getTM( double dtime )
{
	time_t ttime;

	ttime = (dtime - 25569.0f) * (24 * 60 * 60);
	if (ttime < 0)
		ttime = 0;

	return gmtime(&ttime);
}

