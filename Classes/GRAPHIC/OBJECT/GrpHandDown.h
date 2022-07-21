#ifndef __R2K_GRP_HAND_DOWN__
#define __R2K_GRP_HAND_DOWN__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpHandDown : public CCNode{
	CCSpriteBatchNode *m_pSpriteBatch;
	float m_width, m_height;

	CCSprite* m_pSprites[3];
public:
	GrpHandDown(CCTexture2D *pTexSystem2);
	~GrpHandDown();
	void draw();

private:
	CCSprite* getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	CCSprite* getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif