#ifndef __R2K_GRP_SYS_DOWN__
#define __R2K_GRP_SYS_DOWN__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpSysDown : public CCNode{
	CCSprite *m_pSprite;
	float m_width, m_height;

public:
	GrpSysDown(CCTexture2D *pTexSystem);
	~GrpSysDown();
	void draw();

private:
	void addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	void addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif