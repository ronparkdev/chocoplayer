
#ifndef __R2K_COMMON_MANAGER_SETTING__
#define __R2K_COMMON_MANAGER_SETTING__
#include "DebugingCode.h"

#include <cocos2d.h>
USING_NS_CC;

#define R2K_SETTING_MAX 350

#define SETTING_VIRTUALKEY	"SETTING_TOUCH_"
#define SETTING_VIRTUALKEY_V	"SETTING_TOUCH_V_"
#define SETTING_JOYSTICK	"SETTING_JOYSTICK_"
#define SETTING_SND			"SETTING_SND_"
#define SETTING_ETC			"SETTING_ETC_"
#define SETTING_TRANSLATE	"SETTING_TRANSLATE_"

enum R2K_SETTING_TYPE{
	RST_KEY_USING = 0,			//BOOL
	RST_KEY_POSX = 30,			//FLOAT
	RST_KEY_POSY = 60,			//FLOAT
	RST_KEY_SCALE = 90,			//FLOAT

	RST_KEY_V_USING = 200,		//BOOL
	RST_KEY_V_POSX = 230,		//FLOAT
	RST_KEY_V_POSY = 260,		//FLOAT
	RST_KEY_V_SCALE = 290,		//FLOAT

	RST_JOYSTICK_KEYCODE = 120,	//INT

	RST_SND_MUSIC_VOL = 150,	//INT
	RST_SND_SOUND_VOL,			//INT

	RST_ETC_DEBUG = 160,		//BOOL
	RST_ETC_FRAMESKIP,			//BOOL
	RST_ETC_AUTOMSG,			//BOOL
	RST_ETC_LITESHADER,			//BOOL not use
	RST_ETC_USESHADER,			//BOOL
	RST_ETC_IS_VERTICAL,		//BOOL

	RST_TRANSLATE_LDB = 180,	//BOOL
	RST_TRANSLATE_MSG,			//BOOL

	RST_ETC_HIDDEN_DEBUG = 190,	//BOOL
};

union uSettingVal{
	int u_i;
	float u_f;
	bool u_b;
};

class MgrSetting{
	MgrSetting(); // constructor is private
	static MgrSetting* pInstance;

	uSettingVal curVal[R2K_SETTING_MAX];
	uSettingVal defVal[R2K_SETTING_MAX];
public:
	static MgrSetting* getInstance();
	static void removeInstance();

	void	LoadAll();
	void	SaveAll();

	void	init(int from, int len);

	int		getAutoType(int type);
	int		getAutoInt(int type);
	void	setAutoInt(int type, int value);

	int		getInt(int type);
	int		getDefInt(int type);
	void	setInt(int type, int val);

	float	getFloat(int type);
	float	getDefFloat(int type);
	void	setFloat(int type, float val);
	
	bool	getBool(int type);
	bool	getDefBool(int type);
	void	setBool(int type, bool val);

private:
	void	LoadDefault();
};

#endif
