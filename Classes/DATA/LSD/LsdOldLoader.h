#ifndef __R2K__LSD_OLD_LOADER__
#define __R2K__LSD_OLD_LOADER__

#include "StructControler.h"

#include "../LSD/LsdMain.h"
#include "../LDB/LdbMain.h"

class LsdOldLoader : public StructControler{
	LsdMain *m_lsd;
	LdbMain *m_ldb;

public:
	bool ReadData( const TString &path, LdbMain *ldb, LsdMain *lsd );
	void ReadData();

	void ReadPreview( int StructIdLimit );

	void ReadSystem( int StructIdLimit );

	void ReadScreen( int StructIdLimit );

	void ReadPicture( int StructIdLimit );
	void ReadPictureOne( LsdPicture &pic, long StructLimit );

	void ReadPublicEvent( int StructIdLimit, int id );

	void ReadCharacter( int StructIdLimit );

	void ReadCharacterOne(int index, LsdCharacter &lsdCh, int structIdLimit );

	void ReadStatus( int StructIdLimit );

	void ReadTeleport( int StructIdLimit );

	void ReadMap( int StructIdLimit );
	void ReadMapEvent( int StructIdLimit );

	void ObjectInit(int index, LsdObjectCommon *obj);
};

#endif