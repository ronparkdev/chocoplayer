#ifndef __R2K_GRP_HAND_LEFT__
#define __R2K_GRP_HAND_LEFT__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpHandLeft : public CCNode{
	CCSpriteBatchNode *m_pSpriteBatch;
	float m_width, m_height;

	CCSprite* m_pSprites[3];
public:
	GrpHandLeft(CCTexture2D *pTexSystem2);
	~GrpHandLeft();
	void draw();

private:
	CCSprite* getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	CCSprite* getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif