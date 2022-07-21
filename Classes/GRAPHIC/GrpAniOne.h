#ifndef __R2K_GRP_ANIMATION_ONE__
#define __R2K_GRP_ANIMATION_ONE__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpAniNode.h"
#include "../GameMain.h"
#include "../DATA/LDB/LdbAni.h"
#include "../SCENE/BATTLE/ScrBattleHueManager.h"

class GrpAniOne : public CCNode{
private:
	SndMain *pSnd;
	LsdMain *pLsd;
	LdbAni *pLdbAni;
	LsdScreen* pLsdScr;
	ScrBattleHueMgr* pHue;
	
	IBuffer *m_ibuf;

	bool isBattle;
	int objID;

	ArrayList<LdbAni> *anis;
	GrpAniNode *pNodes[GRP_ANIMATION_MAX_NODE];

	TString texName;
	int texId;
	int aniId;

	bool isLoop;
	int skippedFrames;

public:
	GrpAniOne(GameMain *main);
	GrpAniOne(GameMain *main, ScrBattleHueMgr *pHue, int objID);
	~GrpAniOne();

	void play( int aniID );
	void playLoop( int aniID );
	bool isPlaying();

	void Draw(int framedelta, bool isValFix = false);
	LdbAni* getCurrentAni();

	static int getAnimationFrame( GameMain *main, int id );
};

#endif