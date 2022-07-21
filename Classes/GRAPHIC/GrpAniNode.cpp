#include "GrpAniNode.h"

#include "../COMMON/ScreenCommon.h"

GrpAniNode::GrpAniNode()
{
	this->autorelease();  
}

GrpAniNode::~GrpAniNode()
{

}

void GrpAniNode::Init( CCTexture2D * pbatchtex )
{
	if (pTex != pbatchtex) {
		isNeedInit = true;
		pTex = pbatchtex;	
	}
}

void GrpAniNode::Draw( int targetid, LdbAniCell *cell, int posx, int posy )
{
	if (isNeedInit) {
		this->initWithTexture(pTex);
		isNeedInit = false;
	}
	this->setVisible(true);

	int c = (cell->cell_id) % GRP_ANIMATION_COL;
	int r = (cell->cell_id) / GRP_ANIMATION_COL;

	this->setPosition(ccp(	(cell->x + posx),
							(-cell->y + posy)));

	//CCLOG("%d %d", cell->x, cell->y);

	this->setTextureRect(CCRect(
		c * GRP_ANIMATION_WIDTH, 
		r * GRP_ANIMATION_HEIGHT,
		GRP_ANIMATION_WIDTH,
		GRP_ANIMATION_HEIGHT));

	this->setScale(cell->zoom/100.0f);
	this->setOpacity((100.0f-cell->transparency)/100.0f*255.0f);
}

void GrpAniNode::Clear()
{
	this->setVisible(false);
}


