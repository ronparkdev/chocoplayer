#include "ScrAppVKeyStore.h"

#include "../AppScaleTools.h"

#define VKEY_STORE_Z_ACTIVE 100
#define VKEY_STORE_Z_INACTIVE 99

ScrAppVKeyStore::ScrAppVKeyStore() {
	aniShake = CCRepeat::create(CCSequence::createWithTwoActions(CCEaseSineIn::create(CCRotateTo::create(0.1f, -15.0f)), CCEaseSineIn::create(CCRotateTo::create(0.1f, 15.0f))), -1);
	aniShake->retain();
}

ScrAppVKeyStore::~ScrAppVKeyStore() {
	aniShake->autorelease();
}

void ScrAppVKeyStore::init( CCNode *layer )
{
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	pLayer = layer;

	pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("gamelist_btn_none.png"));
	pSpr->setScale(AppScaleTools::cmToPx(1.4f) / pSpr->getContentSize().width);
	pSpr->setPosition(ccp(sz.width/2, sz.height/2));

	CCSize c = pSpr->getContentSize();

	pSprPlus = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("gamelist_btn_none_plus.png"));
	pSprPlus->setPosition(ccp(c.width/2, c.height/2));
	pSprRemove = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("gamelist_btn_none_garb.png"));
	pSprRemove->setPosition(ccp(c.width/2, c.height/2));
	pSprRemove->setOpacity(0);

	pSpr->addChild(pSprPlus, 0);
	pSpr->addChild(pSprRemove, 1);

	pLayer->addChild(pSpr, VKEY_STORE_Z_INACTIVE);

	isShaking = false;

	state = VKEY_STORE_STATE_PLUS;
	doChange(state, 0.0f);

	width = pSpr->getContentSize().width;
	radius = pSpr->getScale() * width * 0.5f;
}

bool ScrAppVKeyStore::isCycleIn( float x, float y )
{
	float dx = (x - pSpr->getPositionX());
	float dy = (y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	return (d<radius);
}

bool ScrAppVKeyStore::isCycleDup( float x, float y, float r )
{
	float dx = (x - pSpr->getPositionX());
	float dy = (y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	return (d<(radius+r));
}

void ScrAppVKeyStore::doChange( ScrAppVKeyStoreState state, float time /*= 1.0f*/ )
{
	CCSprite *sprFrom;
	CCSprite *sprTo;


	int r=255, g=255, b=255;
	float rotate = 0.0f;

	switch(this->state) {
		case VKEY_STORE_STATE_PLUS:		sprFrom = pSprPlus;	break;
		case VKEY_STORE_STATE_CANCEL:	sprFrom = pSprPlus;	break;
		case VKEY_STORE_STATE_REMOVE:	sprFrom = pSprRemove;	break;
	}

	switch(state) {
	case VKEY_STORE_STATE_PLUS:		sprTo = pSprPlus;		r=0;g=228;b=255;					break;
	case VKEY_STORE_STATE_CANCEL:	sprTo = pSprPlus;		r=g=b=80;			rotate=45.0f;	break;
	case VKEY_STORE_STATE_REMOVE:	sprTo = pSprRemove;	r=255;g=0;b=102;					break;
	}

	isShaking = false;
	this->state = state;

	sprFrom->stopAllActions();
	sprTo->stopAllActions();
	if (sprTo == pSprRemove)
		sprTo->setRotation(0.0f);
	else
		sprTo->runAction(CCRotateTo::create(time, rotate));

	pSpr->runAction(CCTintTo::create(time, r, g, b));

	if (sprFrom != sprTo) {
		sprFrom->runAction(CCFadeOut::create(time));
		sprTo->runAction(CCFadeIn::create(time));
	}

}

CCPoint ScrAppVKeyStore::getPosition()
{
	return pSpr->getPosition();
}

ScrAppVKeyStoreState ScrAppVKeyStore::getState()
{
	return state;
}

float ScrAppVKeyStore::getRadius()
{
	return radius;
}

void ScrAppVKeyStore::setShake(bool val) {
	if (isShaking == val)
		return;

	isShaking = val;
	
	if (val) {
		pSprRemove->runAction(aniShake);
	} else {
		pSprRemove->stopAction(aniShake);
		pSprRemove->runAction(CCRotateTo::create(0.1f, 0.0f));
	}
}
