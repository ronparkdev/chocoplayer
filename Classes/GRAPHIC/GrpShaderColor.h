#ifndef __R2K__GRP_SHADER_COLOR__
#define __R2K__GRP_SHADER_COLOR__
#include "../COMMON/DebugingCode.h"
#include "../DATA/ArrayList.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpShaderColor{
private:
	static int g_grpshader_need_lite;
	static int g_grpshader_count;

	CCGLProgram *glProg;

	int ul_AddR;
	int ul_AddG;
	int ul_AddB;
	int ul_GrPer;
	int ul_Alpha;
    
	float lastR;
	float lastG;
	float lastB;
	float lastGray;
	float lastA;
public:
	GrpShaderColor();
	~GrpShaderColor();

	void Init();
	void reload();
    
	void chkLite();
	bool isLite();
    
	void setShader( CCNode *pSpriteBatch );

	void setColor(float r, float g, float b, float gr);
	void setAlpha(float alpha);
};

#endif