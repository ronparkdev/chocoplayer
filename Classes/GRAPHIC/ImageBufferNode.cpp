#include "ImageBuffer.h"

#include "../FILEIO/GraphicIO.h"

IBufferNode::IBufferNode()
	:isUsed(false)
	,isMapChanged(false)
	,texName("")
	,pTex(NULL)
	,pImg(NULL)
{
}

IBufferNode::~IBufferNode()
{
	Unset();
}

void IBufferNode::setFileCTL( FileCTL *filectl )
{
	this->pFCtl = filectl;
}

char Toupper(char c) {
	if ('a' <= c && c <= 'z')
		return (c + 'A' - 'a');
	else
		return c;
}

bool IBufferNode::Set(bool isReload) {
	GraphicIO gio;
	char *data;
	int datasize = 0;

	if	(isNoFile || 
			(	
				pFCtl->getData(Image_Type_To_File_Type[(int)typeId], realFileName, &data, &datasize) &&
				gio.loadPicture(data, datasize, *pImg, isUseAlpha)
			)		
		 )
	{
		isUsed = true;
		isMapChanged = false;


		CCRenderTexture *pRenTex;
		CCImage *target_image;
		if (isNoFile) {
			pRenTex = CCRenderTexture::create(
				Image_Type_Default_Size_W[typeId],
				Image_Type_Default_Size_H[typeId]
			);
			pRenTex->clear(0.0f,0.0f,0.0f,0.0f);
			target_image = pRenTex->newCCImage();
		} else {
			target_image = pImg;
		}

// 		if (m_texture == NULL)
// 			m_texture = new CCTexture2D();

		texKey = Image_Type_Folder[typeId] + "|" + texName;

		if (isReload)
			pTex->initWithImage(target_image);
		else	
			pTex = CCTextureCache::sharedTextureCache()->
					addUIImage(target_image , texKey.getTextUTF8());

		pTex->setAliasTexParameters();
		
		if (isNoFile) {
			//target_image->release();
			pRenTex->release();
		}

//		CCLOG("Texture Set:%s <%s> mem 0x%X size:%dbyte", Image_Type_Folder[typeId].getTextUTF8(), texName.getTextUTF8(), (int)pTex, datasize);
		return true;
	} else {

//		CCLOG("Texture Fail:%s <%s>", Image_Type_Folder[typeId].getTextUTF8(), texName.getTextUTF8());
		return false;
	}
}

bool IBufferNode::Set(Image_Type_Name type_id, const TString &name)
{
	if (isUsed)
		return false;

	isUseAlpha = (name.indexOf(IMAGE_BUFFER_NOALPHA_PRESTR) != 0);
	isNoFile = (name.indexOf(IMAGE_BUFFER_NOFILE) == 0);

	Unset(); 
	
	if (pImg == NULL)
		pImg = new CCImage();

	texName = name;
	typeId = type_id;

	if (!isUseAlpha)
		realFileName = name.substring(strlen(IMAGE_BUFFER_NOALPHA_PRESTR));
	else
		realFileName = name;

	return Set(false);
}

void IBufferNode::Reload() {
	if (!isUsed)
		return;
	
	Set(true);

//	CCLOG("Reload OK : 0x%X", (int)this);
}

void IBufferNode::Unset()
{
	if (isUsed) {
		isUsed = false;

		CCLOG("Texture Unset:%s <%s> RefCnt:%d", Image_Type_Folder[typeId].getTextUTF8(), texName.getTextUTF8(), pTex->getReferenceCount());

		CCTextureCache::sharedTextureCache()->removeTextureForKey(texKey.getTextUTF8());
		//m_texture->release();
		pTex = NULL;
	}

	if (pImg != NULL) {
		pImg->release();
		pImg = NULL;
	}
}
