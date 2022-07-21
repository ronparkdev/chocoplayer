#ifndef __R2K_SCREEN_APP_SCALE_TOOLS__
#define __R2K_SCREEN_APP_SCALE_TOOLS__

#include "cocos2d.h"
USING_NS_CC;

#define SCALE_TOOL_WIDTH_FULL (-1.0f)
#define SCALE_TOOL_HEIGHT_FULL (-2.0f)

class AppScaleTools
{
public:
	static float dpToPx(float dp);
	static float getScale();
	static float cmToPx( float cm );
};

#endif