#ifndef __R2K_GRP_NUMBER_FOR_BATTLE__
#define __R2K_GRP_NUMBER_FOR_BATTLE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;


#define GRP_BAT_NUM_MAX 6

#define GRP_BAT_NUM_MAX_FRAME 70
#define GRP_BAT_NUM_HIDE_FRAME 10

class GrpBatNum : public CCNode{
	CCSpriteBatchNode *m_pSpriteBatch;
	CCSprite* m_pSptite[GRP_BAT_NUM_MAX];

	int m_using_sprite_cnt;
public:
	GrpBatNum( CCTexture2D *pTexSystem2 );
	~GrpBatNum();

	void set( int num ) ;

private:
	void init( int id, int num );
};

#endif