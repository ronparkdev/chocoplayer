#ifndef __R2K_GRP_ANIMATION_FOR_BATTLE_ANIMATION__
#define __R2K_GRP_ANIMATION_FOR_BATTLE_ANIMATION__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpAniOne.h"
#include "../GameMain.h"
#include "../SCENE/BATTLE/ScrBattleHueManager.h"

class GrpAniBat : public CCNode{
private:
	GameMain * pGM;
	GrpAniOne *pAnis;
	int posX, posY;
	int height;
	bool isChar;

public:
	GrpAniBat( GameMain *main, ScrBattleHueMgr *hue , int objID);
	~GrpAniBat();

	void set( int aniID, int posX, int posY, int height, bool isLoop = false);
	void Draw(int framedelta); 
};

#endif