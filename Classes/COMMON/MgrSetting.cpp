#include "MgrSetting.h"

#include <fstream>
#include "ScreenCommon.h"
#include "ComMath.h"
#include "../CONTROL/MultiJoystick.h"
#include "../CONTROL/ctlMain.h"
#include "../FILEIO/lib_json/json.h"
#include "../SCENE/AppScaleTools.h"

#define MGR_SETTING_AUTO_INT -1
#define MGR_SETTING_AUTO_FLOAT -2
#define MGR_SETTING_AUTO_BOOL -3

using namespace std;

MgrSetting* MgrSetting::pInstance = NULL;

MgrSetting* MgrSetting::getInstance()
{
	if (pInstance == NULL) {
		pInstance = new MgrSetting();
	}

	return pInstance;
}

void MgrSetting::removeInstance()
{
	if (pInstance != NULL)
		delete pInstance;

	pInstance = NULL;
}

MgrSetting::MgrSetting()
{
	LoadAll();	
}

void MgrSetting::LoadDefault() {

	int i; 

	int sizeLong = _MAX(ScreenCommon::app_w, ScreenCommon::app_h);
	int sizeShort = _MIN(ScreenCommon::app_w, ScreenCommon::app_h);

	float mwidth = _MIN(ScreenCommon::app_w, ScreenCommon::app_h);

	for(i=0; i<R2K_KEYCODE_COUNT; i++)
		defVal[RST_KEY_USING + i].u_b = false;

	defVal[RST_KEY_SCALE + 0].u_f = AppScaleTools::cmToPx(2.5f) / R2K_KEY_ARROW_WIDTH;

	for(int i=1; i<3; i++)
		defVal[RST_KEY_SCALE + i].u_f = AppScaleTools::cmToPx(1.3f) / R2K_KEY_PUBLIC_WIDTH;

	for(int i=3; i<R2K_KEYCODE_COUNT; i++)
		defVal[RST_KEY_SCALE + i].u_f = AppScaleTools::cmToPx(1.1f) / R2K_KEY_PUBLIC_WIDTH;

	defVal[RST_KEY_USING+ 0].u_b = true;
	defVal[RST_KEY_POSX + 0].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 0].u_f*0.5f;
	defVal[RST_KEY_POSY + 0].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 0].u_f*0.5f;

	defVal[RST_KEY_USING+ 1].u_b = true;
	defVal[RST_KEY_POSX + 1].u_f	= sizeLong - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 1].u_f*1.0f;
	defVal[RST_KEY_POSY + 1].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 1].u_f*0.5f;

	defVal[RST_KEY_USING+ 2].u_b = true;
	defVal[RST_KEY_POSX + 2].u_f	= sizeLong - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 2].u_f*0.5f;
	defVal[RST_KEY_POSY + 2].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 2].u_f*1.2f;

	defVal[RST_KEY_USING+ 3].u_b = true;
	defVal[RST_KEY_POSX + 3].u_f	= sizeLong - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 3].u_f*0.5f;
	defVal[RST_KEY_POSY + 3].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_SCALE + 3].u_f*2.5f;

	defVal[RST_KEY_USING+19].u_b = true;
	defVal[RST_KEY_POSX +19].u_f	= R2K_KEY_PUBLIC_WIDTH*defVal[RST_KEY_SCALE + 19].u_f*0.5f;
	defVal[RST_KEY_POSY +19].u_f	= sizeShort - R2K_KEY_PUBLIC_WIDTH*defVal[RST_KEY_SCALE + 19].u_f*0.5f;



	for(i=0; i<R2K_KEYCODE_COUNT; i++)
		defVal[RST_KEY_V_USING + i].u_b = false;

	defVal[RST_KEY_V_SCALE + 0].u_f = AppScaleTools::cmToPx(2.5f) / R2K_KEY_ARROW_WIDTH;

	for(int i=1; i<3; i++)
		defVal[RST_KEY_V_SCALE + i].u_f = AppScaleTools::cmToPx(1.3f) / R2K_KEY_PUBLIC_WIDTH;

	for(int i=3; i<R2K_KEYCODE_COUNT; i++)
		defVal[RST_KEY_V_SCALE + i].u_f = AppScaleTools::cmToPx(1.1f) / R2K_KEY_PUBLIC_WIDTH;

	defVal[RST_KEY_V_USING+ 0].u_b	= true;
	defVal[RST_KEY_V_POSX + 0].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 0].u_f*0.5f;
	defVal[RST_KEY_V_POSY + 0].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 0].u_f*0.5f;

	defVal[RST_KEY_V_USING+ 1].u_b	= true;
	defVal[RST_KEY_V_POSX + 1].u_f	= sizeShort - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 1].u_f*1.0f;
	defVal[RST_KEY_V_POSY + 1].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 1].u_f*0.5f;

	defVal[RST_KEY_V_USING+ 2].u_b	= true;
	defVal[RST_KEY_V_POSX + 2].u_f	= sizeShort - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 2].u_f*0.5f;
	defVal[RST_KEY_V_POSY + 2].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 2].u_f*1.2f;

	defVal[RST_KEY_V_USING+ 3].u_b	= true;
	defVal[RST_KEY_V_POSX + 3].u_f	= sizeShort - R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 3].u_f*0.5f;
	defVal[RST_KEY_V_POSY + 3].u_f	= R2K_KEY_ARROW_WIDTH*defVal[RST_KEY_V_SCALE + 3].u_f*2.5f;

	defVal[RST_KEY_V_USING+19].u_b	= true;
	defVal[RST_KEY_V_POSX +19].u_f	= R2K_KEY_PUBLIC_WIDTH*defVal[RST_KEY_V_SCALE + 19].u_f*0.5f;
	defVal[RST_KEY_V_POSY +19].u_f	= sizeLong * 0.3f + R2K_KEY_PUBLIC_WIDTH*defVal[RST_KEY_V_SCALE + 19].u_f*0.5f;


	for(i=0; i<JOYSTICK_KEY_COUNT; i++)
		defVal[RST_JOYSTICK_KEYCODE + i].u_i = JOYSTICK_DEF_KEY_CODE[i];

	for(i=0; i<2; i++)
		defVal[RST_SND_MUSIC_VOL + i].u_i = 100;


	defVal[RST_ETC_DEBUG].u_b			= false;
	defVal[RST_ETC_HIDDEN_DEBUG].u_b	= false;

	defVal[RST_ETC_FRAMESKIP].u_b = true;
	defVal[RST_ETC_AUTOMSG].u_b	= false;
	defVal[RST_ETC_LITESHADER].u_b= false;
	defVal[RST_ETC_USESHADER].u_b	= true;
	defVal[RST_ETC_IS_VERTICAL].u_b	= false;

	defVal[RST_TRANSLATE_LDB].u_b	= false;
	defVal[RST_TRANSLATE_MSG].u_b	= false;
}

void MgrSetting::LoadAll()
{
	int i, id;
	char buf[128];
	Json::Reader reader;
	Json::Value root;

    const char *path = CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str();
	ifstream is(path , ios::binary);

	reader.parse(is, root);

	is.close();

	LoadDefault();

	for(i=0; i<R2K_KEYCODE_COUNT; i++) {
		id = RST_KEY_USING + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_USED");
		curVal[id].u_b = (root[buf].isNull())?defVal[id].u_b:root[buf].asBool();

		id = RST_KEY_POSX + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_POS_X");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();

		id = RST_KEY_POSY + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_POS_Y");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();

		id = RST_KEY_SCALE + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_SCALE");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();


		id = RST_KEY_V_USING + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_USED");
		curVal[id].u_b = (root[buf].isNull())?defVal[id].u_b:root[buf].asBool();

		id = RST_KEY_V_POSX + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_POS_X");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();

		id = RST_KEY_V_POSY + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_POS_Y");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();

		id = RST_KEY_V_SCALE + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_SCALE");
		curVal[id].u_f = (root[buf].isNull())?defVal[id].u_f:root[buf].asDouble();
	}

	for(i=0; i<JOYSTICK_KEY_COUNT; i++) {
		id = RST_JOYSTICK_KEYCODE + i;
		sprintf(buf, "%s%d", SETTING_JOYSTICK, i);
		curVal[id].u_i = (root[buf].isNull())?defVal[id].u_i:root[buf].asInt();
	}

	for(i=0; i<2; i++) {
		id = RST_SND_MUSIC_VOL + i;
		sprintf(buf, "%s%d", SETTING_SND, i);
		curVal[id].u_i = (root[buf].isNull())?defVal[id].u_i:root[buf].asInt();
	}

	for(i=0; i<6; i++) {
		id = RST_ETC_DEBUG + i;
		sprintf(buf, "%s%d", SETTING_ETC, i);
		curVal[id].u_b = (root[buf].isNull())?defVal[id].u_b:root[buf].asBool();
	}

	for(i=0; i<2; i++) {
		id = RST_TRANSLATE_LDB + i;
		sprintf(buf, "%s%d", SETTING_TRANSLATE, i);
		curVal[id].u_b = (root[buf].isNull())?defVal[id].u_b:root[buf].asBool();
	}
}


void MgrSetting::SaveAll()
{
	int i, id;
	char buf[128];
	Json::StyledStreamWriter writer;
	Json::Value root;

	for(i=0; i<R2K_KEYCODE_COUNT; i++) {
		id = RST_KEY_USING + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_USED");
		root[buf] = curVal[id].u_b;

		id = RST_KEY_POSX + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_POS_X");
		root[buf] = curVal[id].u_f;

		id = RST_KEY_POSY + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_POS_Y");
		root[buf] = curVal[id].u_f;

		id = RST_KEY_SCALE + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY, i, "_SCALE");
		root[buf] = curVal[id].u_f;


		id = RST_KEY_V_USING + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_USED");
		root[buf] = curVal[id].u_b;

		id = RST_KEY_V_POSX + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_POS_X");
		root[buf] = curVal[id].u_f;

		id = RST_KEY_V_POSY + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_POS_Y");
		root[buf] = curVal[id].u_f;

		id = RST_KEY_V_SCALE + i;
		sprintf(buf, "%s%d%s", SETTING_VIRTUALKEY_V, i, "_SCALE");
		root[buf] = curVal[id].u_f;
	}

	for(i=0; i<JOYSTICK_KEY_COUNT; i++) {
		id = RST_JOYSTICK_KEYCODE + i;
		sprintf(buf, "%s%d", SETTING_JOYSTICK, i);
		root[buf] = curVal[id].u_i;
	}

	for(i=0; i<2; i++) {
		id = RST_SND_MUSIC_VOL + i;
		sprintf(buf, "%s%d", SETTING_SND, i);
		root[buf] = curVal[id].u_i;
	}

	for(i=0; i<6; i++) {
		id = RST_ETC_DEBUG + i;
		sprintf(buf, "%s%d", SETTING_ETC, i);
		root[buf] = curVal[id].u_b;
	}

	for(i=0; i<2; i++) {
		id = RST_TRANSLATE_LDB + i;
		sprintf(buf, "%s%d", SETTING_TRANSLATE, i);
		root[buf] = curVal[id].u_b;
	}
    
    const char *path = CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str();
	ofstream os(path, ios::binary);
	writer.write(os, root);
}

void MgrSetting::init( int from, int len )
{
	int i, start, end;
	start = _MAX(from, 0);
	end = _MIN(from + len , R2K_SETTING_MAX);

	for(i = start; i < end; i++)
		curVal[i] = defVal[i];
}

int MgrSetting::getInt( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return curVal[type].u_i;
	else
		return 0;
}

int MgrSetting::getDefInt( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return defVal[type].u_i;
	else
		return 0;
}

void MgrSetting::setInt( int type, int val )
{
	if (0<=type && type <R2K_SETTING_MAX)
		curVal[type].u_i = val;
}

float MgrSetting::getFloat( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return curVal[type].u_f;
	else
		return 0.0f;
}

float MgrSetting::getDefFloat( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return defVal[type].u_f;
	else
		return 0.0f;
}

void MgrSetting::setFloat( int type, float val )
{
	if (0<=type && type <R2K_SETTING_MAX)
		curVal[type].u_f = val;
}

bool MgrSetting::getBool( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return curVal[type].u_b;
	else
		return false;
}

bool MgrSetting::getDefBool( int type )
{
	if (0<=type && type <R2K_SETTING_MAX)
		return defVal[type].u_b;
	else
		return false;
}

void MgrSetting::setBool( int type, bool val )
{
	if (0<=type && type <R2K_SETTING_MAX)
		curVal[type].u_b = val;
}

int MgrSetting::getAutoInt( int type )
{
	switch(getAutoType(type)) {
	case MGR_SETTING_AUTO_BOOL:
		return getBool(type)?1:0;

	case MGR_SETTING_AUTO_FLOAT:
		return (int)getFloat(type);

	case MGR_SETTING_AUTO_INT:
		return getInt(type);

	default:
		return -1;
	}
}

void MgrSetting::setAutoInt( int type, int value )
{
	switch(getAutoType(type)) {
	case MGR_SETTING_AUTO_BOOL:
		setBool(type, (value!=0));
		break;

	case MGR_SETTING_AUTO_FLOAT:
		setFloat(type, (float)value);
		break;

	case MGR_SETTING_AUTO_INT:
		setInt(type, value);
		break;
	}

	SaveAll();
}

int MgrSetting::getAutoType( int type )
{
	switch(type) {
	//FLOAT
	case RST_KEY_POSX:
	case RST_KEY_POSY:
	case RST_KEY_SCALE:
	case RST_KEY_V_POSX:
	case RST_KEY_V_POSY:
	case RST_KEY_V_SCALE:
		return MGR_SETTING_AUTO_FLOAT;

	//BOOL
	case RST_KEY_USING:
	case RST_KEY_V_USING:
	case RST_ETC_DEBUG:
	case RST_ETC_FRAMESKIP:
	case RST_ETC_AUTOMSG:
	case RST_ETC_LITESHADER:
	case RST_ETC_USESHADER:
	case RST_ETC_IS_VERTICAL:
	case RST_TRANSLATE_LDB:
	case RST_TRANSLATE_MSG:
	case RST_ETC_HIDDEN_DEBUG:
		return MGR_SETTING_AUTO_BOOL;

	//INT
	default:
		return MGR_SETTING_AUTO_INT;
	}
}

