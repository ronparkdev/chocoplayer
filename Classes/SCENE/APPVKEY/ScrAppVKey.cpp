#include "ScrAppVKey.h"
#include "../ScreenLayerManager.h"
#include "../../COMMON/MgrSetting.h"
#include "../../AD/AdMain.h"

bool ScrAppVKey::init()
{
	if ( !CCLayer::init() )
		return false;

	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	pSprBg = CCSprite::create("bg_black.png");
	pSprBg->setOpacity(200);
	pSprBg->setPosition(ccp(sz.width/2, sz.height/2));
	pSprBg->setScaleX(sz.width);
	pSprBg->setScaleY(sz.height);
	this->addChild(pSprBg, 0);

	nodes.ChangeMode(ArrayMode);
	nodes.SetLength(VKEY_CNT);

	btnStore.init(this);
	btnOK.init(this);
	for(int i=0; i<VKEY_CNT; i++)
		nodes[i].init(this, i);

	m_selNodeID = -1;

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	this->autorelease();

	AdMain::hideLineAd();

	return true;
}

ScrAppVKey::~ScrAppVKey()
{
	AdMain::chkAd(scr_map);
}

void ScrAppVKey::draw()
{

}

bool ScrAppVKey::ccTouchBegan( CCTouch *pTouch, CCEvent* event )
{
	CCPoint t = pTouch->getLocation();

	if (btnStore.isCycleIn(t.x, t.y)) {
		switch(btnStore.getState()) {
		case VKEY_STORE_STATE_PLUS:
			{
				btnStore.doChange(VKEY_STORE_STATE_CANCEL);
				btnOK.hide();

				CCPoint p = btnStore.getPosition();

				for(int i=0; i<VKEY_CNT; i++)
					if (!nodes[i].isUsing())
						nodes[i].setRandomPosition(nodes, btnStore);

				for(int i=0; i<VKEY_CNT; i++)
					if (!nodes[i].isUsing()) {
						nodes[i].doOpenStore(p.x, p.y, 0.8f);
					} else {
						nodes[i].doSetForeground(false);
					}

				break;
			}

		case VKEY_STORE_STATE_CANCEL:
			{
				btnStore.doChange(VKEY_STORE_STATE_PLUS);
				btnOK.show(nodes, btnStore);

				CCPoint p = btnStore.getPosition();

				for(int i=0; i<VKEY_CNT; i++)
					if (!nodes[i].isUsing()) {
						nodes[i].doCloseStore(p.x, p.y);
					} else {
						nodes[i].doSetForeground(true);
					}

					break;
			}
		}
	} else if (btnOK.isCycleIn(t.x, t.y)) {
        doClose();
	} else {
		if (btnStore.getState() != VKEY_STORE_STATE_PLUS) {
			for(int i=0; i<nodes.GetLength(); i++) {
				if (!nodes[i].isUsing() && nodes[i].isCycleIn(t.x, t.y)) {

					m_selNodeID = i;

					nodes[i].setUsing(true);
					nodes[i].doSetForeground(true);

					btnStore.doChange(VKEY_STORE_STATE_REMOVE);
					btnOK.hide();

					CCPoint p = btnStore.getPosition();

					for(int i=0; i<VKEY_CNT; i++)
						if (!nodes[i].isUsing()) {
							nodes[i].doCloseStore(p.x, p.y);
						} else {
							nodes[i].doSetForeground(true);
						}

					break;
				}
			}
		} else {
			for(int i=0; i<nodes.GetLength(); i++) {
				if (nodes[i].isUsing() && nodes[i].isCycleIn(t.x, t.y)) {
					m_selNodeID = i;
					btnStore.doChange(VKEY_STORE_STATE_REMOVE);
					btnOK.hide();
					break;
				}
			}
		}
	}

	return true;
}

void ScrAppVKey::ccTouchMoved( CCTouch *pTouch, CCEvent* event )
{
	CCPoint t = pTouch->getDelta();

	if (m_selNodeID != -1) {
		if (btnStore.getState() == VKEY_STORE_STATE_REMOVE) {
			ScrAppVKeyNode &n = nodes[m_selNodeID];
			btnStore.setShake(n.isCycleDup(btnStore));
		}

		nodes[m_selNodeID].addPosition(t.x, t.y);
	}
}

void ScrAppVKey::ccTouchEnded( CCTouch *pTouch, CCEvent* event )
{
	CCPoint t = pTouch->getLocation();
	if (m_selNodeID != -1) {
		if (btnStore.getState() == VKEY_STORE_STATE_REMOVE) {
			ScrAppVKeyNode &n = nodes[m_selNodeID];
			if (n.isCycleDup(btnStore)) {
				CCPoint p = btnStore.getPosition();
				n.setUsing(false);
				n.doCloseStore(p.x, p.y);
			}

			btnStore.doChange(VKEY_STORE_STATE_PLUS);
			btnOK.show(nodes, btnStore);
		}
	}
	btnStore.setShake(false);
	m_selNodeID = -1;
}

void ScrAppVKey::doClose()
{
	for(int i=0; i<nodes.GetLength(); i++)
		nodes[i].save();

	MgrSetting::getInstance()->SaveAll();

	ScreenLayerManager::getInstance()->setSceneVKeyVisible(false);
}