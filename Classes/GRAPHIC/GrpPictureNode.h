#ifndef __R2K_GRP_PICTURE_NODE__
#define __R2K_GRP_PICTURE_NODE__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ImageBuffer.h"
#include "GrpShaderColor.h"
#include "../DATA/TString.h"
#include "../DATA/LSD/LsdPicture.h"
#include "../COMMON/ScreenCommon.h"


class GrpPictureNode : public CCNode{
private:
	GrpShaderColor shaderUpper;
	GrpShaderColor shaderLower;

	CCTexture2D *pTex;
	
	CCSprite *pSprUpper;
	CCSprite *pSprLower;

	TString texName;
	bool isNoBackground;

	bool isNeedCheck;
	float lastScrX;
	float lastScrY;

	float curRot;
	float curWave;

public:
	GrpPictureNode();
	~GrpPictureNode();

	void reloadShaders();

	void Draw(int framedelta, LsdPicture *d);
	bool isUsing();
};

#endif