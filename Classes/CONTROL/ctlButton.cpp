#include "ctlButton.h" 

#include "../COMMON/ScreenCommon.h"
#include "../COMMON/GlobalLangInGame.h"
#include "../COMMON/GlobalLangFont.h"
#include "../COMMON/MgrSetting.h"
#include "../DATA/TString.h"

ctlButton::ctlButton( R2K_KEYCODE keytype, int keyindex )
{
	keyType = keytype;
	keyId = keyindex;
	settingId = keyindex;
	width = height = 0;
	scale = 1.0f;
	selId = 0;
	touchId = -1;
	isSelected = false;
	pText = NULL;

	if (keyType == rkc_arrow) {
		for(int i=0; i<5; i++) {
			pSprite[i] = CCSprite::create(button_arrow_path[i]);
			if (pSprite[i] == NULL)
				return;

			this->addChild(pSprite[i], 1);
		}
		pSprite[0]->setOpacity(80.0f);
	} else {
		for(int i=0; i<2; i++) {
			if (keyType == rkc_menu)
				pSprite[i] = CCSprite::create(button_menu_path[i]);
			else if (keyType == rkc_turbo)
				pSprite[i] = CCSprite::create(button_turbo_path[i]);
			else if (keyType == rkc_qsave)
				pSprite[i] = CCSprite::create(button_qsave_path[i]);
			else
				pSprite[i] = CCSprite::create(button_public_path[i]);

			if (pSprite[i] == NULL)
				return;

			this->addChild(pSprite[i], 1);
		}
	}

	CCSize size = pSprite[0]->getTexture()->getContentSize();
	width = size.width;
	height = size.height;

	isSelected = 0;

	this->autorelease();
}


bool ctlButton::onTouch( Touch_Type type, int touch_id, float x, float y )
{
	if (!isUsed || ((touchId != -1) && (touchId != touch_id)))
		return false;

	float rx = posX - x;
	float ry = posY - y;
	int r = (((width + height)*scale)/4);
	float dist = sqrt(rx*rx + ry*ry);

	if (type == touchtype_down) {
		if (dist > r)
			return false;

		touchId = touch_id;
	} else if (type == touchtype_move) {
		if (touchId == -1)
			return false;

	} else if (type == touchtype_up || type == touchtype_err) {
		if (touchId == -1)
			return false;

		touchId = -1;
		isSelected = false;
		chkVisible();
		return false;
	}

	if (keyType == rkc_arrow && 
		(type == touchtype_down || r/3 < dist))
	{
		if (ry>rx) {
			if (ry>-rx)
				selId = rkca_down;
			else
				selId = rkca_right;
		} else {
			if (ry>-rx)
				selId = rkca_left;
			else
				selId = rkca_up;
		}
	}
	
	//CCLOG("x:%f y:%f touch:%d id:%d", x, y, (int)type, touch_id);
	isSelected = true;

	chkVisible();
	return true;
}

void ctlButton::chkPosition()
{
	int but_len = (keyType == rkc_arrow)?5:2;

	CCPoint p = ccp(posX, posY);
	float s = scale;
	for(int i=0; i<but_len; i++) {
		CCSprite *pSp = pSprite[i];
		this->setPosition( p );
		this->setScale( s );
	}
}

void ctlButton::chkVisible() {

	if (!isUsed) {
		this->setVisible(false);
	} else {
		this->setVisible(true);

		if (keyType == rkc_arrow) {
			if (!isSelected) {
				for(int i=0; i<5; i++)
					pSprite[i]->setVisible(i==0);
			} else {
				for(int i=0; i<5; i++)
					pSprite[i]->setVisible(i==selId);
			}
			
		} else {
			for(int i=0; i<2; i++)
				pSprite[i]->setVisible((i!=0) == isSelected);
		}
	}
}

int ctlButton::getID()
{
	if (!isSelected)
		return 0;
	else{
		return selId;
	}
}

void ctlButton::setID( bool selected )
{
	isSelected = selected;
	if (!selected)
		touchId = -1;
}

void ctlButton::setID( int selected ) {
	if (selected == 0) {
		isSelected = false;
	} else {
		isSelected = true;
		selId = selected;
	}
}

void ctlButton::updateSetting()
{
	TString cap;
	switch(keyType) {
	case rkc_enter:	cap	= KEYSTR_ENTER;
		selId	= KEYCODE_ENTER;
		settingId	= 1;
		break;

	case rkc_esc:	cap = KEYSTR_ESC;
		selId	= KEYCODE_ESC;
		settingId	= 2;
		break;

	case rkc_shift:	cap = KEYSTR_SHIFT;
		selId	= kEYCODE_SHIFT;
		settingId	= 3;
		break;

	case rkc_num:	cap = KEYSTR_NUMBER[keyId];
		selId	= KEYCODE_NUMBER[keyId];
		settingId	= 4+keyId;
		break;

	case rkc_special:cap= KEYSTR_SPECIAL[keyId];
		selId	= KEYCODE_SPECIAL[keyId];
		settingId	= 14+keyId;
		break;

	case rkc_menu:
		selId	= 1;
		settingId	= 19;
		break;

	case rkc_turbo:
		selId	= 1;
		settingId	= 20;
		break;

	case rkc_qsave:
		selId	= 1;
		settingId	= 21;
		break;
	}

	if (cap.getLength() > 0) {
		if (pText != NULL)
			this->removeChild(pText);
		pText = CCLabelTTF::create(cap.getTextUTF8(), GlobalLangFont::getDefFontName(), width*0.5f);
		//pText->setPosition(ccp(m_width*0.5f, m_width*0.5f));
		this->addChild(pText, 0);
	}

	MgrSetting *s = MgrSetting::getInstance();
	if (!ScreenCommon::game_is_vertical) {
		isUsed  = s->getBool(RST_KEY_USING + settingId);
		posX = s->getFloat(RST_KEY_POSX + settingId);
		posY = s->getFloat(RST_KEY_POSY + settingId);
		scale = s->getFloat(RST_KEY_SCALE + settingId);
	} else {
		isUsed  = s->getBool(RST_KEY_V_USING + settingId);
		posX = s->getFloat(RST_KEY_V_POSX + settingId);
		posY = s->getFloat(RST_KEY_V_POSY + settingId);
		scale = s->getFloat(RST_KEY_V_SCALE + settingId);
	}

	chkPosition();
	chkVisible();
}
