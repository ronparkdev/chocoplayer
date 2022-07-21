#include "ScrAppJoy.h"
#include "../../COMMON/GlobalLangFont.h"
#include "../../COMMON/GlobalLangFile.h"
#include "../../COMMON/MgrSetting.h"
#include "../../CONTROL/MultiJoystick.h"
#include "../AppScaleTools.h"
#include "../ScreenLayerManager.h"

#define MENU_SUB_BEFORE 0
#define MENU_SUB_DEFAULT 1
#define MENU_SUB_NEXT 2
#define MENU_SUB_OK 3

bool ScrAppJoy::init()
{
	if ( !CCLayer::init() )
		return false;

	isMenuClosing = false;

	m_last_pushed = -1;
	curKey = 0;

	initBtns(0.0f);

	refreshKey();

	return true;
}

void ScrAppJoy::initBtns( float menu_scale )
{
	sz = CCDirector::sharedDirector()->getVisibleSize();

	GlobalLangFile *lang = GlobalLangFile::getInstance();
	float min_width = _MIN(sz.width, sz.height);

	pBg = CCSprite::create("bg_black.png");
	pBg->setScale(99999.0f);
	pBg->setOpacity(125.0f);
	this->addChild(pBg, -100);

	float title_size = sz.width/25.0f;
	CCLabelTTF *pTitle = CCLabelTTF::create(lang->getStringForKey("APPMENU_SET_JOY"), GlobalLangFont::getDefFontName(), title_size);
	pTitle->enableStroke(ccc3(0, 0, 0), title_size/20.0f);
	pTitle->setPosition(ccp(sz.width/2, sz.height * 0.93f));
	this->addChild(pTitle, 2);

	float item_size = min_width/25.0f;
	float item_scale = min_width/500.0f;

	//Caption
	pText = CCLabelTTF::create("testtest", GlobalLangFont::getDefFontName(), item_size);
	pText->enableStroke(ccc3(0, 0, 0), item_size/20.0f);
	pText->setPosition(ccp(sz.width*0.5f, sz.height*0.7f));
	this->addChild(pText, 1);

	pText2 = CCLabelTTF::create("testtest", GlobalLangFont::getDefFontName(), item_size);
	pText2->enableStroke(ccc3(0, 0, 0), item_size/20.0f);
	pText2->setPosition(ccp(sz.width*0.5f, sz.height*0.6f));
	this->addChild(pText2, 1);

	CCMenuItemImage* item;
	{
		CCArray *items = CCArray::create();

		float menu_scale = min_width*0.15f/128.0f;

		item = CCMenuItemImage::create(
			"menu_previous.png", "menu_previous_push.png", 
			this, menu_selector(ScrAppJoy::doMenuClicked) );
		item->setScale(AppScaleTools::cmToPx(1.2f) / item->getContentSize().height);
		item->setTag(MENU_SUB_BEFORE);
		items->addObject(item);
		pBtnPrevious = item;

		item = CCMenuItemImage::create(
			"menu_none.png", "menu_none_push.png", 
			this, menu_selector(ScrAppJoy::doMenuClicked) );
		item->setScale(AppScaleTools::cmToPx(1.2f) / item->getContentSize().height);
		item->setTag(MENU_SUB_DEFAULT);
		items->addObject(item);
		pBtnNext = item;

		item = CCMenuItemImage::create(
			"menu_next.png", "menu_next_push.png", 
			this, menu_selector(ScrAppJoy::doMenuClicked) );
		item->setScale(AppScaleTools::cmToPx(1.2f) / item->getContentSize().height);
		item->setTag(MENU_SUB_NEXT);
		items->addObject(item);
		pBtnNext = item;

		CCMenu* pMenu = CCMenu::createWithArray(items);
		if (MgrSetting::getInstance()->getBool(RST_ETC_IS_VERTICAL))
			pMenu->alignItemsVertically();
		else
			pMenu->alignItemsHorizontally();
		pMenu->setPositionY(sz.height*0.45f);
		this->addChild(pMenu, 1);
	}

	{
		item = CCMenuItemImage::create(
			"gamelist_btn_ok.png", "gamelist_btn_ok.png", 
			this, menu_selector(ScrAppJoy::doMenuClicked) );
		item->setScale(AppScaleTools::cmToPx(1.0f) / item->getContentSize().width);

		float width = item->getContentSize().width;
		float radius = item->getScale() * width * 0.5f;

		item->setTag(MENU_SUB_OK);

		CCMenu* pMenu = CCMenu::createWithItem(item);
		pMenu->alignItemsHorizontally();
		pMenu->setPosition(ccp(sz.width/2, radius * 1.3f));
		this->addChild(pMenu, 1);
	}
}

void ScrAppJoy::refreshKey()
{
	MgrSetting *s = MgrSetting::getInstance();
	int key = s->getInt(RST_JOYSTICK_KEYCODE + curKey);

	pText->setString((_ST("Current Key : ") + _ST(JOYSTICK_KEY_STRING[curKey])).getTextUTF8());
	pText2->setString((_ST("Assigned Key : ") + _ST(JOYSTICK_KEY_REAL_STRING[key])).getTextUTF8());

	if (curKey <= 0)
		pBtnPrevious->setOpacity(125);
	else
		pBtnPrevious->setOpacity(255);

	if (curKey >= JOYSTICK_KEY_COUNT-1)
		pBtnNext->setOpacity(125);
	else
		pBtnNext->setOpacity(255);
}

void ScrAppJoy::draw() {
	MgrSetting *s = MgrSetting::getInstance();

	if (m_last_pushed > 0) {
		for(int i=0; i<JOYSTICK_KEY_COUNT; i++)
			if (s->getInt(RST_JOYSTICK_KEYCODE + i) == m_last_pushed)
				s->setInt(RST_JOYSTICK_KEYCODE + i, 0);

		s->setInt(RST_JOYSTICK_KEYCODE + curKey, m_last_pushed);

		refreshKey();

		m_last_pushed = -1;
	}
}
void ScrAppJoy::doMenuClicked(CCObject *pSender)
{
	CCMenuItem* pTempItem = (CCMenuItem*)pSender;
	MgrSetting *s = MgrSetting::getInstance();

	int i = pTempItem->getTag();

	if (isMenuClosing)
		return;

	switch (i) {
	case MENU_SUB_BEFORE:
		if (curKey > 0) {
			curKey--;
			refreshKey();
		}
		break;

	case MENU_SUB_DEFAULT:
		s->setInt(RST_JOYSTICK_KEYCODE + curKey, 0);
		refreshKey();
		break;

	case MENU_SUB_NEXT:
		if (curKey < JOYSTICK_KEY_COUNT-1) {
			curKey++;
			refreshKey();
		}
		break;


	case MENU_SUB_OK:
		MgrSetting::getInstance()->SaveAll();

		ScreenLayerManager::getInstance()->setSceneJoyVisible(false);

	default:
		break;
	}
}
