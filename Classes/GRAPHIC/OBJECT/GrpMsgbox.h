#ifndef __R2K_GRP_MSGBOX__
#define __R2K_GRP_MSGBOX__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../../GameMain.h"

class GrpMsgbox : public CCNode{
	CCTexture2D* m_pTexture;
	CCSprite *m_pSprite[16];
	int m_SpriteLen;
	float m_width, m_height;
public:
	GrpMsgbox( GameMain *main, float width, float height);
	~GrpMsgbox();

private:
	void addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty);
	void addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth);
};

#endif