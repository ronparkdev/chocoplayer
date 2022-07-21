#include "ScrNameInput.h"

#include "../../COMMON/GlobalLangFont.h"

ScrNameInput::ScrNameInput( GameMain *main, TString *defstr )
{
	pGM = main;
	pDefStr = defstr;

	CCScale9Sprite *sprite9 = CCScale9Sprite::create("input.png");
	sprite9->setPreferredSize(CCSizeMake(1600, 400));
	sprite9->setCapInsets(CCRect(80,80,352,352));
	sprite9->setPosition(ccp(160, 200));
	sprite9->setScale(1 / 8.0f);

	this->addChild(sprite9);


	float scale = ScreenCommon::game_s;

	pTxtBox = CCEditBox::create(CCSizeMake(170 * scale, 50 * scale), CCScale9Sprite::create("bg_alpha.png"));
	pTxtBox->setPosition(ccp(160, 200));
	
	pTxtBox->setScale(1.0f / scale);

	pTxtBox->setFontName(GlobalLangFont::getDefFontName());
	pTxtBox->setFontSize(20 * scale);
	pTxtBox->setFontColor(ccBLACK);
	
	pTxtBox->setText(defstr->getTextUTF8());
	pTxtBox->setMaxLength(8);
	
	pTxtBox->setReturnType(kKeyboardReturnTypeDone);
	pTxtBox->setDelegate(this);

	this->addChild(pTxtBox);
}

ScrNameInput::~ScrNameInput()
{

}


void ScrNameInput::Init()
{

}

void ScrNameInput::Draw( int framedelta )
{

}

void ScrNameInput::editBoxEditingDidBegin(CCEditBox* editBox) {
	//    CCLOG("editBox %p DidBegin !", editBox);
}
void ScrNameInput::editBoxEditingDidEnd(CCEditBox* editBox) {
	//    CCLOG("editBox %p DidEnd !", editBox);
}
void ScrNameInput::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
	//    CCLOG("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void ScrNameInput::editBoxReturn(CCEditBox* editBox) {
	if (pTxtBox == editBox && strlen(pTxtBox->getTextUTF8()) > 0)
	{
		pDefStr->setTextUTF8(pTxtBox->getTextUTF8());
		pGM->slm->setSettingScrMap();
		pGM->slm->setScreen(scr_map);
	}
}
