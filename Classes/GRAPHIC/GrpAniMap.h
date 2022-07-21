#ifndef __R2K_GRP_ANIMATION__
#define __R2K_GRP_ANIMATION__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpAniOne.h"
#include "../GameMain.h"

#define GRP_ANIMATION_MAX_NODE 40

class GrpAniMap : public CCNode{
private:
	GameMain * pGM;
	GrpAniOne *pAnis[9];
	LsdScreen* pLsdScr;
public:
	GrpAniMap(GameMain *main);
	~GrpAniMap();

	void reloadShaders();

	void Draw(int framedelta); 
};

#endif