#include "ScreenLayer.h"

void ScreenLayer::Init()
{
#ifdef DEBUG
	CCLog("ScreenLayer::Init()");
#endif
}

void ScreenLayer::onTouch( Touch_Type type, int id, float x, float y )
{
#ifdef DEBUG
	CCLog("ScreenLayer::onTouch( Touch_Type type, int id, float x, float y )");
#endif
}

void ScreenLayer::setEnable( bool isEnable )
{
#ifdef DEBUG
	CCLog("ScreenLayer::setEnable( bool isEnable )");
#endif
}

void ScreenLayer::Draw( int framedelta )
{
#ifdef DEBUG
	CCLog("ScreenLayer::Draw( int framedelta )");
#endif
}

void ScreenLayer::reloadSetting()
{
#ifdef DEBUG
	CCLog("ScreenLayer::reloadSetting()");
#endif
}

void ScreenLayer::reloadShaders()
{
#ifdef DEBUG
	CCLog("ScreenLayer::reloadShaders()");
#endif
}

bool ScreenLayer::isFullScreen()
{
	return false;
}
