#include "LmtMain.h"

#define LMT_HEADER "LcfMapTree" 
#define LMT_HEADER2 "MdgNbqUsff"

void LmtMain::init()
{
	header = LMT_HEADER;

	Info.init();
}

void LmtMain::serialize( StructLoader &sl )
{
	init();
	BaseStruct::serialize(sl, header);
	if (header == LMT_HEADER) {
		if (sl.getMode() == StructLoaderModeRead) {
			int mapCount, mapID, infosize;
			sl >> mapCount;

			MInfo.RemoveAll();
			MInfo.ChangeMode(ArrayMode);
			MInfo.SetLength(mapCount);

			for(int i=0; i<=mapCount; i++) {
				sl >> mapID;
				if (i == mapCount) {
					int *buf = new int[mapCount];
					int mapIDMax = -1;
					for(int j=0; j<mapCount;j++) {
						sl >> buf[j];
						if (mapIDMax < buf[j])
							mapIDMax = buf[j];
					}

					MTable.RemoveAll();
					MTable.ChangeMode(ArrayMode);
					MTable.SetLength(mapIDMax+1);
					for(int j=0; j<=mapIDMax; j++)
						MTable[j] = -1;

					for(int j=0; j<mapCount; j++)
						MTable[buf[j]] = j;

					delete []buf;

					sl >> infosize;
					BaseStruct::serialize(sl,&Info);
				} else {
					MInfo[i].serialize(sl);
				}
			}
		}
	}
}

