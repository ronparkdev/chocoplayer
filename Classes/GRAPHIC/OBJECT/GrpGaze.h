#ifndef __R2K_GRP_GAZE__
#define __R2K_GRP_GAZE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

class GrpGaze : public CCNode{
	CCSpriteBatchNode *m_pSpriteBatch;
	int m_type;

	CCSprite* m_pSprite[5];
	int m_SpriteLen;

public:
	GrpGaze( CCTexture2D *pTexSystem, int type );
	~GrpGaze();

	void refresh(float per);

private:
	void addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	void addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif