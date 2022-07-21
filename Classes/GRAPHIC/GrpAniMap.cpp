#include "GrpAniMap.h"

#include "../COMMON/ScreenCommon.h"
#include "../EVENT/EvtMain.h"
#include "../SCENE/MAP/ScrMapPublic.h"

GrpAniMap::GrpAniMap(GameMain *main)
{
	pGM = main;
	pLsdScr = &(main->lsd.Screen);
	
	for(int i=0; i<9; i++) {
		pAnis[i] = new GrpAniOne(main);
		this->addChild(pAnis[i]);
	}

	pAnis[0]->setPosition(ccp(0,		0));
	pAnis[1]->setPosition(ccp(-320,	160));
	pAnis[2]->setPosition(ccp(0,		160));
	pAnis[3]->setPosition(ccp(320,		160));
	pAnis[4]->setPosition(ccp(-320,	0));
	pAnis[5]->setPosition(ccp(320,		0));
	pAnis[6]->setPosition(ccp(-320,	-160));
	pAnis[7]->setPosition(ccp(0,		-160));
	pAnis[8]->setPosition(ccp(320,		-160));

	this->autorelease();
}

GrpAniMap::~GrpAniMap()
{
	for(int i=0; i<4; i++)
		this->removeChild(pAnis[i]);
}

void GrpAniMap::reloadShaders()
{
	//TODO why not use shader???
}

void GrpAniMap::Draw(int framedelta)
{
	int posx, posy;
	LdbAni *ani = pAnis[0]->getCurrentAni();

	if (ani != NULL) {

		if (!pLsdScr->ani_allscreen) {
			ScrMapPublic::getEventScreenPos(&(pGM->lsd), pGM->lsd.Screen.ani_target, posx, posy);

			if (ani->scope == 0) {
				//Personal
				switch(ani->position) {
				case 0://head
					posy += -12 - 4;
					break;

				case 1://center
					posy += -4;
					break;

				case 2://foot
					posy += 12 - 4;
					break;
				}
			} else {
				//Group
				posx = 320 / 2;
				posy = 240 / 2;
			}
			this->setPosition(ccp(SCR_POS_X(posx), SCR_POS_Y(posy)));

			pAnis[0]->Draw(framedelta);

			for(int i=1; i<9; i++)
				pAnis[i]->setVisible(false);
		} else {
			LsdScreen &screen = pGM->lsd.Screen;

			int screen_x_int = (int)ScreenCommon::game_screen_x;
			int screen_y_int = (int)ScreenCommon::game_screen_y;

			float screen_x_float = ScreenCommon::game_screen_x - screen_x_int;
			float screen_y_float = ScreenCommon::game_screen_y - screen_y_int;

			screen_x_int = (-(screen_x_int + screen.screen_adjust_x))%320;
			screen_y_int = (-(screen_y_int + screen.screen_adjust_y))%160;

			while(screen_x_int < 0)
				screen_x_int += 320;

			while(screen_y_int < 0)
				screen_y_int += 160;

			this->setPosition(ccp(SCR_POS_X(screen_x_int + screen_x_float),
									SCR_POS_Y(screen_y_int + screen_y_float)));

			for(int i=0; i<9; i++)
				pAnis[i]->Draw(framedelta, i != 8);

			for(int i=1; i<9; i++)
				pAnis[i]->setVisible(true);
		}

	}

}