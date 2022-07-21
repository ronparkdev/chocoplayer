#ifndef __R2K_GRAPHIC__BITMAP_BUFFER_TYPE__
#define __R2K_GRAPHIC__BITMAP_BUFFER_TYPE__
#include "../COMMON/DebugingCode.h"

#include "../DATA/TString.h"
#include "../FILEIO/FileType.h"

#define IMAGE_BUFFER_TYPES 15
#define IMAGE_BUFFER_PER_TYPE 512
#define IMAGE_BUFFER_PER_TYPE_SHIFT 9
#define IMAGE_BUFFER_PATH_BUF 256

#define IMAGE_BUFFER_NOALPHA_PRESTR "#noalpha#"
#define IMAGE_BUFFER_NOFILE "#nofile#"

enum Image_Type_Name{
	 imgBackdrop	,imgBattle		,imgBattleCharSet,imgBattleWeapon
	,imgCharSet		,imgChipSet		,imgFaceSet		,imgFrame		
	,imgGameOver	,imgMonster		,imgPanorama	,imgPicture		
	,imgSystem		,imgSystem2		,imgTitle
};

const File_Type_Name Image_Type_To_File_Type[] = {
	 fileBackdrop	,fileBattle		,fileBattleCharSet,fileBattleWeapon
	,fileCharSet	,fileChipSet	,fileFaceSet	,fileFrame		
	,fileGameOver	,fileMonster	,filePanorama	,filePicture		
	,fileSystem		,fileSystem2	,fileTitle
};

const bool Image_Type_Release_By_Scene[] = {
	 true			,true			,true			,true			
	,false			,false			,true			,false			
	,true			,true			,false			,false			
	,false			,false			,true
};

const TString Image_Type_Folder[] = {
	 "Backdrop"		,"Battle"		,"BattleCharSet","BattleWeapon"	
	,"CharSet"		,"ChipSet"		,"FaceSet"		,"Frame"		
	,"GameOver"		,"Monster"		,"Panorama"		,"Picture"		
	,"System"		,"System2"		,"Title"
};

const int Image_Type_Default_Size_W[] = {
	 320			,480			,480			,192
	,288			,480			,192			,320
	,320			,1				,320			,1
	,160			,80				,320		
};

const int Image_Type_Default_Size_H[] = {
	 240			,480			,480			,512
	,256			,256			,192			,240
	,240			,1				,240			,1
	,80				,96				,240			
};

#endif
