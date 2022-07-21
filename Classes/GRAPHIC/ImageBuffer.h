#ifndef __R2K_GRAPHIC__BITMAP_BUFFER__
#define __R2K_GRAPHIC__BITMAP_BUFFER__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ImageType.h"
#include "../COMMON/ScreenCommon.h"
#include "../DATA/TString.h"
#include "../FILEIO/FileCTL.h"

using namespace std;

class IBufferNode{
	bool isUsed;
	bool isMapChanged;
	int typeId;

	bool isUseAlpha;
	bool isNoFile;
	
	TString texName;
	TString realFileName;
	TString texKey;
	
	CCImage *pImg;
	CCTexture2D *pTex;

	FileCTL * pFCtl;
public:
	IBufferNode();
	~IBufferNode();

	void setFileCTL( FileCTL *filectl );
	bool Set(bool reload = false);
	bool Set(Image_Type_Name type_id, const TString &name);
	void Reload();
	void Unset();
	friend class IBuffer;
};

class IBuffer
{
private:
	IBufferNode m_nodes[IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE];
	int m_nodelen[IMAGE_BUFFER_TYPES];

	static IBuffer *m_pInstance;

	IBuffer();
public:
	static IBuffer *getInstance();
	static void removeInstance();

	void setPath(TString path);
	void setFileCTL( FileCTL *filectl );

	int get(Image_Type_Name type, const TString &name); 

	CCTexture2D* get( Image_Type_Name type, int index );

	void mapChanging();

	void mapChanged();

	void reload(const char *key);
	void release();
};

#endif