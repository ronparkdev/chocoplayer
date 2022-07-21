#ifndef __R2K_CONTROL_BUTTON__
#define __R2K_CONTROL_BUTTON__
#include "../COMMON/DebugingCode.h"

#include <cocos2d.h>
USING_NS_CC;

#include "../DATA/TString.h"

enum Touch_Type{
	touchtype_down,
	touchtype_move,
	touchtype_up,
	touchtype_err
};

enum R2K_KEYCODE{
	rkc_arrow	= 0,
	rkc_enter	= 1,
	rkc_esc		= 2,
	rkc_shift	= 3,
	rkc_num		= 4,
	rkc_special = 5,
	rkc_menu	= 6,
	rkc_turbo	= 7,
	rkc_qsave	= 8,
};

enum R2K_KEYCODE_ARROW{
	rkca_none	= 0,
	rkca_down	= 1,
	rkca_left	= 2,
	rkca_right	= 3,
	rkca_up		= 4,
};

#define		KEYSTR_ENTER	"A"
#define		KEYCODE_ENTER	5
#define		KEYSTR_ESC		"B"
#define		KEYCODE_ESC		6
#define		KEYSTR_SHIFT	"S"
#define		kEYCODE_SHIFT	7

const TString	KEYSTR_NUMBER[]		={"0","1","2","3","4","5","6","7","8","9"};
const int		KEYCODE_NUMBER[]	={ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
const TString	KEYSTR_SPECIAL[]	={"+","-","*","/","◎"};
const int		KEYCODE_SPECIAL[]	={20,21,22,23,24};

const char button_arrow_path[5][50] = 
{
	"button_arrow_x.png", 
	"button_arrow_d.png", 
	"button_arrow_l.png", 
	"button_arrow_r.png", 
	"button_arrow_u.png",
};

const char button_public_path[2][50] = 
{
	"button_public_n.png", 
	"button_public_p.png"
};

const char button_menu_path[2][50] = 
{
	"button_menu_n.png", 
	"button_menu_p.png"
};

const char button_turbo_path[2][50] = 
{
	"button_turbo_n.png", 
	"button_turbo_p.png"
};

const char button_qsave_path[2][50] = 
{
	"button_qsave_n.png", 
	"button_qsave_p.png"
};

class ctlButton: public CCNode{
private:
	R2K_KEYCODE keyType;
	int keyId;
	int selId;
	
	bool isSelected;
	int touchId;
	int settingId;

	bool isUsed;
	int posX, posY;
	int width, height;
	float scale;

	CCSprite* pSprite[5];
	CCLabelTTF *pText;
	
public:
	ctlButton( R2K_KEYCODE keytype, int keyindex );
	bool onTouch( Touch_Type type, int touch_id, float x, float y );

	void chkPosition();
	void chkVisible();
	int getID();
	void setID( bool selected );
	void setID( int selected );
	void updateSetting();
};

#endif