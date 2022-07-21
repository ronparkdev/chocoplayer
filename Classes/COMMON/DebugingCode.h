#ifndef __R2K__DEBUGING_CODE__
#define __R2K__DEBUGING_CODE__

#include <cocos2d.h>
#include <time.h>
class TString;

//For catch memory reak

//#include "AppMulti.h"
// #if (defined( DEBUG ) || defined( _DEBUG )) && (SR_WIN == 1)
// #	define _CRTDBG_MAP_ALLOC
// #	include <crtdbg.h>
// #	define DEBUG_NORMALBLOCK new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
// 
// #   ifdef new
// #      undef new
// #   endif
// 
// #	define new DEBUG_NORMALBLOCK
// #	define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #	define calloc(c, s) _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #	define realloc(p, s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #	define _expand(p, s) _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #	define free(p) _free_dbg(p, _NORMAL_BLOCK)
// #	define _msize(p) _msize_dbg(p, _NORMAL_BLOCK)
// #endif // DEBUG

#define TLOG(str) CCLOG("%s", str);

#define __LOG_HERE CCLOG((__FILE__ + _ST("\t") + __LINE__).getTextUTF8())
#define __TIME_HERE CCLOG((__FILE__ + _ST("\t") + __LINE__ + _ST("\t TIME:") + clock()).getTextUTF8());

class TString;

inline void CCLOGT(const TString &str);

void doToast(const TString &str);

void initClock();

int defClock();

void defClockPrint(TString name);

#endif
