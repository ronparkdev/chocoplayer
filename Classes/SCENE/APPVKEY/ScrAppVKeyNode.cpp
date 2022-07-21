#include "ScrAppVKeyNode.h"

#include <cmath>
#include "../../COMMON/GlobalLangFont.h"
#include "../../COMMON/MgrSetting.h"

const char *VK_KEY_TEXT[] = {"", "A", "B", "S", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+", "-", "*", "/", "o", "", "", ""};

#define VKEY_NODE_Z_ACTIVE 50
#define VKEY_NODE_Z_INACTIVE 49

void ScrAppVKeyNode::init( CCNode *layer, int index )
{
	pLayer = layer;
	id = index;
	unusePosX = 0.0f;
	unusePosY = 0.0f;

	switch(index) {
	case  0:pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("button_arrow_x.png"));break;
	case 19:pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("button_menu_p.png"));break;
	case 20:pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("button_turbo_p.png"));break;
	case 21:pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("button_qsave_p.png"));break;
	default:pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("button_public_p.png"));break;
	}

	pLayer->addChild(pSpr, VKEY_NODE_Z_INACTIVE);
	width = pSpr->getContentSize().width;

	if (index < 19) {
		CCLabelTTF *pLabel;
		const char *font = GlobalLangFont::getDefFontName();
		pLabel = CCLabelTTF::create(VK_KEY_TEXT[index], font, width*0.5f);
		pLabel->setPosition(ccp(width*0.5f, width*0.5f));
		pSpr->addChild(pLabel, 0);
	}

	load();

	radius = scaleUse * width * 0.5f;
}

void ScrAppVKeyNode::setUsing( bool val )
{
	isUsing_ = val;
}

void ScrAppVKeyNode::setRandomPosition( ArrayList<ScrAppVKeyNode> &m_nodes, ScrAppVKeyStore &m_store )
{
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
	float x, y, r = radius;
	float xFrom = r;
	float xTo = sz.width - r;
	float yFrom = r;
	float yTo = sz.height - r;
	int i, j, len;

	len = id;

	for(i=0; i<100; i++) {
		if (i == 0) {
			x = unusePosX;
			y = unusePosY;

			if (x < xFrom || xTo < x || y < yFrom || yTo < y)
				continue;

		} else {
			x = (rand() % (int)(xTo - xFrom)) + xFrom;
			y = (rand() % (int)(yTo - yFrom)) + yFrom;
		}
		for(j=0; j<len; j++) {
			if ((j == id) || m_nodes[j].isUsing())
				continue;

			if (m_nodes[j].isCycleDup(x, y, r))
				break;
		}

		if (j == len && !m_store.isCycleDup(x, y, r))
			break;
	}

	unusePosX = x;
	unusePosY = y;

	pSpr->setPosition(ccp(x, y));
}

bool ScrAppVKeyNode::isUsing() {
	return isUsing_;
}

bool ScrAppVKeyNode::isCycleIn( float x, float y )
{
	float dx = (x - pSpr->getPositionX());
	float dy = (y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	return (d<radius);
}

bool ScrAppVKeyNode::isCycleDup( ScrAppVKeyStore &n )
{
	CCPoint p = n.getPosition();
	float dx = (p.x - pSpr->getPositionX());
	float dy = (p.y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	//CCLOG("(%d, %d) <=> %d(%d,%d) = %d", (int)x, (int)y, m_index, (int)mx, (int)my, (int)d);

	return (d<(radius+n.getRadius()));
}

bool ScrAppVKeyNode::isCycleDup( float x, float y, float r )
{
	float dx = (x - pSpr->getPositionX());
	float dy = (y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	return (d<(radius+r));
}

void ScrAppVKeyNode::doOpenStore( float x, float y, float time ) {
	float tx, ty;
	tx = pSpr->getPositionX();
	ty = pSpr->getPositionY();
	 
	pSpr->setPosition(ccp(x, y));
	pSpr->stopAllActions();
	pSpr->runAction(CCFadeIn::create(time));
	pSpr->runAction(CCEaseElasticOut::create(CCMoveTo::create(time, ccp(tx, ty)), time));
}

void ScrAppVKeyNode::doCloseStore( float x, float y, float time ) {
	pSpr->stopAllActions();
	pSpr->runAction(CCEaseOut::create(CCMoveTo::create(time, ccp(x, y)), time));
	pSpr->runAction(CCFadeOut::create(time));
}

void ScrAppVKeyNode::doSetForeground( float isForeground, float time ) {
	int r = isForeground?255:50;
	pSpr->stopAllActions();
	pSpr->runAction(CCFadeTo::create(time, r));

	pLayer->reorderChild(pSpr, (isForeground?VKEY_NODE_Z_ACTIVE:VKEY_NODE_Z_INACTIVE));
}

void ScrAppVKeyNode::doSetUse( bool isUse, float time ) {
	pSpr->stopAllActions();

	if (isUse)
		pSpr->runAction(CCFadeIn::create(time));
	else
		pSpr->runAction(CCFadeOut::create(time));

	pSpr->runAction(CCScaleTo::create(time, isUse?scaleUse:scaleUnuse));

	isUsing_ = isUse;
}

void ScrAppVKeyNode::load() {
	MgrSetting *s = MgrSetting::getInstance();
	bool v = s->getBool(RST_ETC_IS_VERTICAL);
	float x, y;

	isUsing_ = s->getBool((v?RST_KEY_V_USING:RST_KEY_USING) + id);
	x = s->getFloat((v?RST_KEY_V_POSX:RST_KEY_POSX) + id);
	y = s->getFloat((v?RST_KEY_V_POSY:RST_KEY_POSY) + id);

	scaleUse = s->getFloat(RST_KEY_SCALE + id);
	scaleUnuse = scaleUse;

	pSpr->setPosition(ccp(x, y));

	doSetUse(isUsing_, 0.0f);
}

void ScrAppVKeyNode::save() {
	MgrSetting *s = MgrSetting::getInstance();
	bool v = s->getBool(RST_ETC_IS_VERTICAL);
	float x, y;

	x = pSpr->getPositionX();
	y = pSpr->getPositionY();

	s->setBool((v?RST_KEY_V_USING:RST_KEY_USING) + id, isUsing_);
	if (isUsing_) {
		s->setFloat((v?RST_KEY_V_POSX:RST_KEY_POSX) + id, x);
		s->setFloat((v?RST_KEY_V_POSY:RST_KEY_POSY) + id, y);
		s->setFloat((v?RST_KEY_V_SCALE:RST_KEY_SCALE) + id, scaleUse);
	}
}

void ScrAppVKeyNode::addPosition( float x, float y )
{
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
	float r = radius;
	float xFrom = r;
	float xTo = sz.width - r;
	float yFrom = r;
	float yTo = sz.height - r;

	x += pSpr->getPositionX();
	y += pSpr->getPositionY();

	if (x < xFrom)x = xFrom;
	if (x > xTo)x = xTo;
	if (y < yFrom)y = yFrom;
	if (y > yTo)y = yTo;

	pSpr->setPosition(ccp(x,y));
}
