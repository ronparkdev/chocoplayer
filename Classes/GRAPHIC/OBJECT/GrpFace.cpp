#include "GrpFace.h"

#include "../../COMMON/ScreenCommon.h"

GrpFace::GrpFace( GameMain *main )
{
	m_main = main;

	m_pSprite = new CCSprite();
	m_pSprite->autorelease();
	this->addChild(m_pSprite);
	this->setVisible(false);
	this->autorelease();
}

GrpFace::~GrpFace()
{
	this->removeChild(m_pSprite);
}

void GrpFace::set( int characterID ) {
	if (characterID <= 0 || characterID > m_main->ldb.dChar.GetLength())
		return;

	LsdCharacter &c = m_main->lsd.Character[characterID-1];

	set(c.face_name, c.face_index);
}

void GrpFace::set( TString face_name, int face_index, bool face_flip)
{
	if (face_name.getLength() == 0) {
		m_pic = "";
		this->setVisible(false);
		return;
	}

	if (m_pic != face_name) {
		int texID = IBuffer::getInstance()->get(imgFaceSet, face_name);
		CCTexture2D *pTex = IBuffer::getInstance()->get(imgFaceSet, texID);

		m_pSprite->initWithTexture(pTex);
		m_pic = face_name;
	}

	int x = ((int)(face_index % GRAPHIC_FACESET_COL)) * GRAPHIC_FACESET_WIDTH;
	int y = ((int)(face_index / GRAPHIC_FACESET_ROW)) * GRAPHIC_FACESET_HEIGHT;

	m_pSprite->setTextureRect(CCRect(x, y, GRAPHIC_FACESET_WIDTH, GRAPHIC_FACESET_HEIGHT));
	m_pSprite->setFlipX(face_flip);
	this->setVisible(true);
}


void GrpFace::GetFace(GameMain *main, CCSprite *pSprite, TString face_name, int face_index, bool isFlip)
{
	int imgid = IBuffer::getInstance()->get(imgFaceSet, face_name);

	if (imgid == -1) {
		pSprite->setVisible(false);
	} else {

		CCTexture2D *pTexture = IBuffer::getInstance()->get(imgFaceSet, imgid);
		pSprite->initWithTexture(pTexture);

		int x = ((int)(face_index % GRAPHIC_FACESET_COL)) * GRAPHIC_FACESET_WIDTH;
		int y = ((int)(face_index / GRAPHIC_FACESET_ROW)) * GRAPHIC_FACESET_HEIGHT;

		pSprite->setTextureRect(CCRect(x, y, GRAPHIC_FACESET_WIDTH, GRAPHIC_FACESET_HEIGHT));
		pSprite->setFlipX(isFlip);
		pSprite->setVisible(true);
	}
}

