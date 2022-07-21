#include "GrpAniBat.h"
 
#include "../COMMON/ScreenCommon.h"
#include "../EVENT/EvtMain.h"
#include "../SCENE/MAP/ScrMapPublic.h"

GrpAniBat::GrpAniBat( GameMain *main, ScrBattleHueMgr *hue , int objID)
{
	pGM = main;
	pAnis = new GrpAniOne(main, hue, objID);
	this->addChild(pAnis);
	this->autorelease();
}

GrpAniBat::~GrpAniBat()
{
	this->removeChild(pAnis);
}

void GrpAniBat::set( int aniID, int posX, int posY, int height, bool isChar )
{
	this->posX = posX;
	this->posY = posY;
	this->height = height;
	this->isChar = isChar;
	if (isChar)
		pAnis->playLoop(aniID);
	else
		pAnis->play(aniID);
}

void GrpAniBat::Draw(int framedelta)
{
	LdbAni *ani = pAnis->getCurrentAni();

	int posx, posy;

	if (ani != NULL) {
		if (!isChar) {
			if (ani->scope == 0) {
				//Personal
				posx = posX;
				posy = posY;
				switch(ani->position) {
				case 0://head
					posy += height/2;
					break;

				case 1://center
					break;

				case 2://foot
					posy -= height/2;
					break;

				}
			} else {
				//Group
				posx = 160;
				posy = 160;
			}
		} else {
			posx = 0;
			posy = 0;
		}
		this->setPosition(ccp(posX, posY));
	}

	pAnis->Draw(framedelta);
}