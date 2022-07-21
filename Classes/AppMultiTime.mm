#include "AppMultiTime.h"

#if (SR_IPHONE == 1)

#import <QuartzCore/CAAnimation.h>

double getTime(){
    return CACurrentMediaTime();
}

#endif