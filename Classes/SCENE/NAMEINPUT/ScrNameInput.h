#ifndef __R2K_SCENE_NAME_INPUT__
#define __R2K_SCENE_NAME_INPUT__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "../ScreenLayer.h"
#include "../../GameMain.h"

class ScrNameInput : public ScreenLayer, public CCEditBoxDelegate
{
	GameMain *pGM;
	CCEditBox* pTxtBox;

	TString *pDefStr;
public:

	ScrNameInput( GameMain *main, TString *defstr );
	~ScrNameInput();

	void Init();

	void Draw(int framedelta);

private:
	void editBoxEditingDidBegin(CCEditBox* editBox);
	void editBoxEditingDidEnd(CCEditBox* editBox);
	void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
	void editBoxReturn(CCEditBox* editBox);
private:

};

#endif 
