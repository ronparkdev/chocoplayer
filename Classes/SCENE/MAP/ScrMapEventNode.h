#ifndef __R2K_SCENE_EVENT_NODE__
#define __R2K_SCENE_EVENT_NODE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../../DATA/TString.h"
#include "../../GRAPHIC/ImageBuffer.h"
#include "../../GRAPHIC/GrpShaderColor.h"

enum NeedMirror{
	NeedMirror_None		 = 0,
	NeedMirror_Vertical	 = 1,
	NeedMirror_Horizonal = 2,
	NeedMirror_Both		 = 3
};

const int NeedMirror_NeedSprites[] = {1, 2, 2, 4};

class ScrMapEventNode{
private:
	CCNode *pLayer;
	CCSprite *pSprite[4];
	CCTexture2D *m_pTextureChipset;

	GrpShaderColor m_shader;

	int m_event_id;
	int m_imgbuf_id;
	
	TString m_pic_name;
	
	int m_last_posZ;
	bool m_last_visible;
	int m_last_trans;

	NeedMirror m_mirror_type;
	CCTexture2D* m_pTexture;

	int m_map_height;
	int m_map_width;
public:
	ScrMapEventNode();
	~ScrMapEventNode();

	void init(CCNode *layer, int index, NeedMirror mirror_type, int map_width, int map_height);
	void init(CCTexture2D *textureChipset);

	void reloadShaders();

	void check();
	void set(const TString &name);
	void unset();

	void setPos(float npos_x, float npos_y);
	void setZPos(int posZ );
	void setVisible(bool val);
	void setTextureRect(int x, int y, int w, int h);
	bool operator!=(const TString &str);
	void setTrans(int val);
	void setColor(float r, float g, float b, float gr);
};

#endif