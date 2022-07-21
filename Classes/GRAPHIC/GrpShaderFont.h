#ifndef __R2K__GRP_SHADER_FONT__
#define __R2K__GRP_SHADER_FONT__
#include "../COMMON/DebugingCode.h"
#include "../DATA/ArrayList.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpShaderFont{
private:
	bool isExtra;

	CCGLProgram *glProg;

	int m_ul_bgx;
	int m_ul_bgy;
	int m_uMask;

	float lastBgX;
	float lastBgY;
	CCTexture2D *pLastTexBg;
public:
	GrpShaderFont(bool isExtra);
	~GrpShaderFont();

	void Init();
	void reload();

	void setShader( CCNode *pSprite );

	void setPos( int id, CCTexture2D * texBG );
};

#endif