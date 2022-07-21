#ifndef __R2K__GRP_SHADER__
#define __R2K__GRP_SHADER__
#include "../COMMON/DebugingCode.h"
#include "../DATA/ArrayList.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpShader{
private:
	CCGLProgram *m_glProgram;

	int ul_AddR;
	int ul_AddG;
	int ul_AddB;
	int ul_GrPer;
	int ul_Alpha;
    
	ArrayList<CCSprite *> m_sprites;

	float m_last_r;
	float m_last_g;
	float m_last_b;
	float m_last_gr;
	float m_last_alpha;
public:
	GrpShader();
	~GrpShader();

	void Init();
	void reload();
    
	void chkLite();
	bool isLite();
    
	void setShader( CCNode *pSpriteBatch );

	void setColor(float r, float g, float b, float gr);
	void setAlpha(float alpha);
};

#endif