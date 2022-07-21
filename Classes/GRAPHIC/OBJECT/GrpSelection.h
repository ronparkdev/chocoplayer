#ifndef __R2K_GRP_SELECTION__
#define __R2K_GRP_SELECTION__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpSelection : public CCNode{
	CCSprite *m_pSprite[16];
	int m_SpriteLen;
	CCSpriteBatchNode *m_pSpriteBatch[2];
	float m_width, m_height;

public:
	GrpSelection(CCTexture2D *pTexSystem, float width, float height);
	~GrpSelection();

	void draw();

private:
	void addSprite(int i, int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	void addSprite(int i, int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif