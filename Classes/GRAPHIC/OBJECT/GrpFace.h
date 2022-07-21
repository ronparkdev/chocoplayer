#ifndef __R2K_GRP_FACE__
#define __R2K_GRP_FACE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../../GameMain.h"

#define GRAPHIC_FACESET_WIDTH 48
#define GRAPHIC_FACESET_HEIGHT 48
#define GRAPHIC_FACESET_ROW 4
#define GRAPHIC_FACESET_COL 4

class GrpFace: public CCNode{
	GameMain *m_main;
	CCSprite *m_pSprite;

	TString m_pic;
	int m_num;
public:
	GrpFace(GameMain *main);
	~GrpFace();

	void set(int characterID);
	void set( TString face_name, int face_index, bool face_flip = false);
public:
	static void GetFace(GameMain *main, CCSprite *pSprite, TString face_name, int face_index, bool isFlip = false);
};

#endif