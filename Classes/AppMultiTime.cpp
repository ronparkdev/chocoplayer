#include "AppMultiTime.h"

#if (SR_IPHONE != 1)

#include <time.h>

double getTime() {
    return (double)clock()/CLOCKS_PER_SEC;
}

#endif