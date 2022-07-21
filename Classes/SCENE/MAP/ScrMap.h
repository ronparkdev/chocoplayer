#ifndef __R2K_SCENE_MAP__
#define __R2K_SCENE_MAP__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include <iostream>
#include <queue>
using namespace std;

#include "ScrMapChipSetTile.h"
#include "ScrMapEventNode.h"
#include "ScrMapPublic.h"

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../COMMON/ScreenCommon.h"
#include "../../DATA/LMU/LmuMain.h"
#include "../../DATA/LDB/LdbMain.h"
#include "../../DATA/LSD/LsdMain.h" 
#include "../../EVENT/EvtMain.h"
#include "../../GRAPHIC/GrpFont.h"
#include "../../GRAPHIC/GrpPicture.h"
#include "../../GRAPHIC/GrpShaderColor.h"
#include "../../GRAPHIC/GrpAniMap.h"
#include "../../GRAPHIC/GrpComTimer.h"

USING_NS_CC;

#define MAP_TILE_MAX 1500
#define CHARSET_SEMITRANS 4

//const int DELAY_MOVE[] =	{0,	4,	6,	15,	30,	60,	120,0};

const int DELAY_MOVE[] =	{20,	5,	10,	19,	37,	75,	150,0};
const int DELAY_ANI[] =		{7,		12,	9,	7,	5,	4,	3,	0};

//const int DELAY_WAIT[] =	{0,	256,128,64,	32,	16,	8,	4,	0};
const int DELAY_WAIT[] =	{0,	96,	48,	24,	12,	6,	3,	2,	0};
const int DELAY_WAIT_MOMENT[] = {0, 148, 84, 52, 36, 28, 24, 22, 20};
class ScrMap : public ScreenLayer
{
	GameMain *pGM;

	//ETC

	EvtMain *pEvt;
	GrpPicture *pPic;
	GrpAniMap *pAni;
	GrpComTimer *pTimer;

	//Map Data

	int curMapId;
	LmuMain mapData;
	bool isInited;

	//MapInformation

	int mapWidth;
	int mapHeight;
	int mapTileEnabledLen;

	//BackGround

	CCSpriteBatchNode *pBgSprBN;
	CCSprite *pBgSprs[1024];
	CCNode *pFg;

	TString bgTexName;
	int bgR;
	int bgC;
	float bgX;
	float bgY;
	int bgW;
	int bgH;

	float bgLastScrX;
	float bgLastScrY;

	bool isBgNeedRefresh;

	GrpShaderColor bgShader;

	//Chipset

	int chipsetBufId;
	int chipsetId;

	int seaAniType;
	int seaIndex;
	int seaTime;
	int seaTimeDivide;
	int seaModeX;
	int seaFrameLeft;

	queue<int> mapTileQueue;
	ScrMapChipSetTile mapTile[MAP_TILE_MAX];
	byte *pMapTileChecked;
	int mapTileLen;

	CCSpriteBatchNode *pMapChipsetLayer[4];

	GrpShaderColor mapShader;

	//Map Event

	ArrayList<ScrMapEventNode> mapEvtSprs;

	int keyLastEnter;
	int keyLastEsc;
	int keyLastQSave;

	bool isEventShaderNeedRefresh;

	//Screen

	bool isScrNeedInit;
	float scrX;
	float scrY;
	float lastOriginalScrX;
	float lastScrX;
	float lastScrY;
	int maxScrX;
	int maxScrY;

	int mapTilesLen;

	int mapWalkCnt;

	CCSprite *pEffShakeHider;
	bool isEffShakelastFrameUsed;
	int effShakeRunFrame;
	float isEffShakeAddX;
	int effShakeEndFrame;

	CCParticleSystem* pParticleSnow;
	CCParticleSystem* pParticleRain;
	
	float effWeatherLastScrX;
	float effWeatherLastScrY;
	CCSpriteBatchNode *pParticleFog[2];

	//debug
	int debug_time_doevents;
	int debug_time_checkkeys;
	int debug_time_drawevents;
	int debug_time_drawchipset;
	int debug_time_drawevents2;
	int debug_time_draweffect;
	int debug_time_drawbg;
	int debug_time_chkride;
	int debug_time_drawpicture;
	int debug_time_drwanimation;
	int debug_time_drawtimer;
	int debug_time_last;

	CCLabelTTF *m_debuglabel;
public:

	ScrMap(GameMain *main , int mapid , bool init);
	~ScrMap();

	void Init();
	void reloadShaders();
	void reloadSetting();
	void Draw(int framedelta);
	void chkKeys();

	void initDebug();
	void drawDebug();

	//BackGround

	void createBackGround();
	void initBackGround(bool init);
	void destroyBackGround();

	void setBackGround(const TString &name, bool hs, bool vs, bool hsa, bool vsa, int hsa_speed, int vsa_speed);
	void setBackGround(const TString &name);

	void drawBackGround(int framedelta);
	//Chipset

	void createMapChipset();
	void initMapChipset();
	void destroyMapChipset();
	void chkChipSet();

	void drawChipSet(int framedelta);
	inline void drawChipsetTile(int pid);
	inline void drawChipsetPart(int chipID);
	int getChipsetID(int chipID);
	inline void DrawPart(int chipset_x, int chipset_y, int w, int h, int tile_x, int tile_y);
	inline void DrawPart(int chipset_x, int chipset_y);
	void refreshChipSetShader(int framedelta, bool force);
	//MapEvent

	void createMapEvents(bool init);
	void initMapEventsChipset();
	inline void initMapEvent(bool init, int mapid);
	void destroyMapEvents();

	void drawEvents(int framedelta);
	void drawEventsAfterChipset();

	inline void drawEventOne(int index, long framedelta, LsdObjectCommon *obj);
	void refreshEventShader(bool force, int index, LsdObjectCommon *obj);
	bool isFixedDirection( LsdObjectCommon *obj );

	void chkEventMove(int framedelta, int index, LsdObjectCommon *obj);
	void doEventMoveToHeroAI( int index, LsdObjectCommon * obj );
	bool doEventMoving(int index, int move_cmd, LsdObjectCommon *obj, GlbMoveRoute *rt = NULL, bool is_custom_move = false);
	int getEventFlag(int x, int y, int exid, bool isfrom);
	int getChipsetFlag(int posX, int posY);
	int getChipsetFlag(int x, int y, int exid, bool isfrom);
	int getTerrainID(int x, int y);
	bool chkEventMoveAble(int fromX, int fromY, int direction, int index, LsdObjectCommon *obj, bool eventRun);
	bool chkEventJumpAble(int toX, int toY, int index);
	TString getStringFromRoute( ArrayList<int> &move_commands, int &move_route_index );

	void refreshScrPos(float cX, float cY, int framedelta, bool init);
	void chkScrPosX( ScrollType scroll_type, float &x );
	void chkScrPosY( ScrollType scroll_type, float &y );
	
	bool chkPos(int &toX, int &toY);
	void chkHeroPic(bool init = false);
	static void chkHeroPic(GameMain *main, bool init);
	//Ride
	void initRide();
	void doRide();
	void updateRide();
	int getRideType();
	void setRideType(int val);
	void doRideLeave(int toX, int toY);
	bool chkRideLeaveable( int posX, int posY );

	//Effect
	void initEffect(bool init);
	void destroyEffect();
	void setEffect(int type, int power);	
	void drawEffect(int framedelta);

	//Walk
	LsdObjectCommon* getHeroObject();
	void initWalk(bool init);
	void doWalk();

	//Event
	void chkEventClick();
	bool isMoveableToHero(int evtID, int direction);
};

#endif 
