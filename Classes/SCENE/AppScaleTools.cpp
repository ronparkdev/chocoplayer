#include "AppScaleTools.h"

#include "../COMMON/ComMath.h"
#include <cmath>

float AppScaleTools::getScale()
{
	CCSize rect = CCDirector::sharedDirector()->getVisibleSize();
	return _MIN(rect.width, rect.height) / 1440.0f;
}

float AppScaleTools::dpToPx( float dp )
{
	CCSize rect = CCDirector::sharedDirector()->getVisibleSize();
	if (dp == SCALE_TOOL_WIDTH_FULL) {
		return rect.width;
	} else if (dp == SCALE_TOOL_HEIGHT_FULL) {
		return rect.height;
	} else {
 		return getScale() * dp * 4;//dp * (CCDevice::getDPI() / 160.0f);
	}
}

float AppScaleTools::cmToPx( float cm ) {
	float inch = cm / 2.54f;

	float pxPerInch = (float)CCDevice::getDPI();

	return inch * pxPerInch;
}