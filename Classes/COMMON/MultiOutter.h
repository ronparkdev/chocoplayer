#ifndef __R2K__MULTI_OUTTER__
#define __R2K__MULTI_OUTTER__

#include "../DATA/TString.h"
#include "../SOUND/SndMain.h"

class GameMain;

// JAVA / Object C단에서 Native의 정보를 얻어와야 하거나, 
// UI 스레드가 아닌, Cocos2d-x 스레드에서 작동해야하는 명령일경우... 
enum IncomingCommand{
	INCOMING_CMD_NONE = -1,
	INCOMING_CMD_OPEN_VKEY = 0,
	INCOMING_CMD_OPEN_JOY,
	INCOMING_CMD_GET_GAMES_INFO,
	INCOMING_CMD_GET_SUPPORT_MENUS,

	INCOMING_CMD_CLOSE_GAME_ADD = 50,

	INCOMING_CMD_GOTO_TITLE = 100,
	INCOMING_CMD_GOTO_GAMELIST,
	INCOMING_CMD_GOTO_SAVE,
	INCOMING_CMD_QUIT_APP,
};

class MultiOutter{
public:
	static void openMenu(const TString &stack);
	static void openWeb(const TString &url);
	static void openCPWeb( const TString &param1 );

	static void openMovie(const TString &path);
	static void openMovieReal(const TString &path);

	static void openGameAdd();
	static void closedGameAdd();

    static TString getModel();
	static TString getUniqueID();
	
	static bool isProA();
	static bool isProI();
	static bool isPro();
	static void buyPro();
	static TString getInfo( int type );
	
	static void deleteFolder( const TString &spath );
};


#endif
