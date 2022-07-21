#include "CCLayerCTL.h"

#include "../COMMON/ScreenCommon.h"

bool CCLayerCTL::init()
{
	if ( !CCLayer::init())
	{
		return false;
	}

	this->setTouchEnabled(true);

	pControl = NULL;

	return true;
}

void CCLayerCTL::setCtl( ctlMain *pCtl )
{
	if (pControl != pCtl) {
		pControl = pCtl;
		this->addChild(pCtl, sz_controler);
	}
}

void CCLayerCTL::ccTouchesBegan( CCSet* pTouches, CCEvent* event )
{
	int i, id;
	float x, y;
	CCSetIterator it = pTouches->begin();
	CCTouch* pTouch;
	for(i=0; i<pTouches->count(); i++, it++) {
		pTouch = (CCTouch*)(*it);
		id = pTouch->getID();
		x = pTouch->getLocation().x;
		y = pTouch->getLocation().y;
		if (!pControl->onTouch(touchtype_down, id, x, y)) {
			onTouch(touchtype_down, id, x, y);
		}
	}
}

void CCLayerCTL::ccTouchesMoved( CCSet* pTouches, CCEvent* event )
{
	int i, id;
	float x, y;
	CCSetIterator it = pTouches->begin();
	CCTouch* pTouch;
	for(i=0; i<pTouches->count(); i++, it++) {
		pTouch = (CCTouch*)(*it);
		id = pTouch->getID();
		x = pTouch->getLocation().x;
		y = pTouch->getLocation().y;
		if (!pControl->onTouch(touchtype_move, id, x, y)) {
			onTouch(touchtype_move, id, x, y);
		}
	}
}

void CCLayerCTL::ccTouchesEnded( CCSet* pTouches, CCEvent* event )
{
	int i, id;
	float x, y;
	CCSetIterator it = pTouches->begin();
	CCTouch* pTouch;
	for(i=0; i<pTouches->count(); i++, it++) {
		pTouch = (CCTouch*)(*it);
		id = pTouch->getID();
		x = pTouch->getLocation().x;
		y = pTouch->getLocation().y;
		if (!pControl->onTouch(touchtype_up, id, x, y)) {
			onTouch(touchtype_up, id, x, y);
		}
	}
}

void CCLayerCTL::ccTouchesCancelled( CCSet* pTouches, CCEvent* event )
{
	int i, id;
	float x, y;
	CCSetIterator it = pTouches->begin();
	CCTouch* pTouch;
	for(i=0; i<pTouches->count(); i++, it++) {
		pTouch = (CCTouch*)(*it);
		id = pTouch->getID();
		x = pTouch->getLocation().x;
		y = pTouch->getLocation().y;
		if (!pControl->onTouch(touchtype_err, id, x, y)) {
			onTouch(touchtype_err, id, x, y);
		}
	}
}

ctlMain * CCLayerCTL::getCtl() const
{
	return pControl;
}

void CCLayerCTL::onTouch( Touch_Type type, int id, float x, float y )
{

}
