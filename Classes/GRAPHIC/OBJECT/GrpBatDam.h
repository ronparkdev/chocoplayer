#ifndef __R2K_GRP_DAMAGE_FOR_BATTLE__
#define __R2K_GRP_DAMAGE_FOR_BATTLE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../GrpFontL.h"

#define GRP_BAT_NUM_MAX 6

#define GRP_BAT_NUM_MAX_FRAME 70
#define GRP_BAT_NUM_HIDE_FRAME 10

class GrpBatDam : public CCNode{
	GrpFontL* m_pFont;

	bool m_is_set;
	int m_cur_frame;
	int m_using_sprite_cnt;

	CCPoint m_def_pos;
	float m_move_x;
public:
	GrpBatDam( GameMain *m_main );
	~GrpBatDam();

	void reloadShaders();

	void set( float posX, float posY, int num );
	void Draw(int framedelta);
	bool isSet();

private:
	void init(int num);

	float getCalPosY(float x);
};

#endif