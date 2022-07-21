#ifndef __R2K_APP_MULTI_TIME__
#define __R2K_APP_MULTI_TIME__

#include "AppMulti.h"

#if (SR_WIN == 1)
#define _SLEEP(x) Sleep(x)
#else
#define _SLEEP(x) usleep(x*1000)
#endif

double getTime();

#endif
