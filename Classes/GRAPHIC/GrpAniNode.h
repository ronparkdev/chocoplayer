#ifndef __R2K_GRP_ANIMATION_NODE__
#define __R2K_GRP_ANIMATION_NODE__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../GameMain.h"
#include "../DATA/LSD/LsdScreen.h"
#include "../DATA/LDB/LdbAni.h"

#define GRP_ANIMATION_MAX_NODE 40
#define GRP_ANIMATION_COL 5
#define GRP_ANIMATION_WIDTH 96
#define GRP_ANIMATION_HEIGHT 96

class GrpAniNode: public CCSprite{
private:
	bool isNeedInit;
	CCTexture2D * pTex;

public:
	GrpAniNode();
	~GrpAniNode();

	void Init( CCTexture2D * pbatchtex );
	void Draw( int targetid, LdbAniCell *cell, int posx, int posy );
	void Clear();
};

#endif