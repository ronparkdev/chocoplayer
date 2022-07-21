#include "GrpMsgbox.h"

GrpMsgbox::GrpMsgbox( GameMain *main, float w, float h )
{
	int texIDnoa = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + main->lsd.System.system_name); 
	int texID = IBuffer::getInstance()->get(imgSystem, main->lsd.System.system_name); 

	m_SpriteLen = 0;
	m_width = w;
	m_height = h;

	m_pTexture = IBuffer::getInstance()->get(imgSystem, texIDnoa);

	addSprite(0, 0, 32, 32, 0, 0, w, h);

	m_pTexture = IBuffer::getInstance()->get(imgSystem, texID);

	addSprite(32, 0, 8, 8, 0,	0);
	addSprite(56, 0, 8, 8, w-8, 0);
	addSprite(32,24, 8, 8, 0,	h-8);
	addSprite(56,24, 8, 8, w-8, h-8);

	addSprite(40, 0, 16, 8, 8,	0	, w - 16, 8);
	addSprite(40,24, 16, 8, 8,	h-8	, w - 16, 8);
	addSprite(32, 8,  8,16, 0,	8	, 8		, h - 16);
	addSprite(56, 8,  8,16, w-8,8	, 8		, h - 16);

	CCNode::autorelease();
}

GrpMsgbox::~GrpMsgbox() {
	for(int i=0; i<m_SpriteLen; i++)
		this->removeChild(m_pSprite[i]);
}

void GrpMsgbox::addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty) {
	addSprite(srcx, srcy, srcw, srch, dstx, dsty, srcw, srch);
}

void GrpMsgbox::addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth) {
	CCSprite *pSprite;
	pSprite = CCSprite::createWithTexture(m_pTexture);
	pSprite->setTextureRect(CCRect(srcx, srcy, srcw, srch));
	pSprite->setPosition(ccp(dstx + (dstw - m_width)/2, -(dsty + (dsth - m_height)/2) ));
	pSprite->setScaleX((float)dstw/srcw);
	pSprite->setScaleY((float)dsth/srch);
	m_pSprite[m_SpriteLen++] = pSprite;
	this->addChild(pSprite, 0);
}