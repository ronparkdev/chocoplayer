#ifndef __R2K_COMMON_SYS_TIME__
#define __R2K_COMMON_SYS_TIME__
#include "../COMMON/DebugingCode.h"

#include <iostream>
#include <ctime>

class SysTime{
public:
	static double getTime();

	static struct tm* getTM(double dtime);
};

#endif