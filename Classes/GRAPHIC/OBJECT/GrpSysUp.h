#ifndef __R2K_GRP_SYS_UP__
#define __R2K_GRP_SYS_UP__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpSysUp : public CCNode{
	CCSprite *m_pSprite;
	float m_width, m_height;

public:
	GrpSysUp(CCTexture2D *pTexSystem);
	~GrpSysUp();
	void draw();

private:
	void addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	void addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif