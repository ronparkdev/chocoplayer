#ifndef __R2K_COMMON_MATH_TOOLS__
#define __R2K_COMMON_MATH_TOOLS__

#define RANDF() ((rand() % 10000)/10000.0f)

#define PI 3.141592f

#define _MIN(a,b) (((a)>(b))?(b):(a))
#define _MAX(a,b) (((a)<(b))?(b):(a))

#define _ISALPHA(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))

#endif