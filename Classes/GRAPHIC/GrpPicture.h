#ifndef __R2K_GRP_PICTURE__
#define __R2K_GRP_PICTURE__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpPictureNode.h"
#include "../GameMain.h"

#define GRP_PICTURE_MAX 50

class GrpPicture : public CCNode{
	GameMain *pGM;

	GrpPictureNode* pNodes[GRP_PICTURE_MAX];
	int lastFrame;

public:
	GrpPicture(GameMain *main);
	~GrpPicture();

	void draw(int framedelta);

	static void setPic(GameMain *main,
		int id, int x, int y, 
		TString pic_name, bool fixMap, bool useBackcolor, 
		float scale, float upAlpha, float downAlpha, 
		int r, int g, int b, int gr, 
		int efftype, int effval );

	static void movePic(GameMain *main,
		int id, int x, int y, 
		float scale, float upAlpha, float downAlpha, 
		int r, int g, int b, int gr, 
		int efftype, int effval, 
		int runtime );

	static void removePic(GameMain *main, int id);
};

#endif