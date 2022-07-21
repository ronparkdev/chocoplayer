#ifndef __R2K_GRAPHIC__FILE_TYPE__
#define __R2K_GRAPHIC__FILE_TYPE__
#include "../COMMON/DebugingCode.h"

#include "../DATA/TString.h"

#define R2K_FILE_TYPES 19

enum File_Type_Name{
	fileBackdrop		= 0,
	fileBattle			= 1,
	fileBattleCharSet	= 2,
	fileBattleWeapon	= 3,
	fileCharSet			= 4,
	fileChipSet			= 5,
	fileFaceSet			= 6,
	fileGameOver		= 7,
	fileMonster			= 8,
	fileMusic			= 9,
	filePanorama		= 10,
	fileSound			= 11,
	fileSystem			= 12,
	fileSystem2			= 13,
	fileTitle			= 14,
	fileFrame			= 15,
	filePicture			= 16,
	fileMovie			= 17,
	fileFolder			= 18
};

const TString File_Type_Folder[R2K_FILE_TYPES] = {
	"Backdrop"		,
	"Battle"		,
	"BattleCharSet"	,
	"BattleWeapon"	,
	"CharSet"		,
	"ChipSet"		,
	"FaceSet"		,
	"GameOver"		,
	"Monster"		,
	"Music"			,
	"Panorama"		,
	"Sound"			,
	"System"		,
	"System2"		,
	"Title"			,
	"Frame"			,
	"Picture"		,
	"Movie"			,
	""	
};

#endif
