#include "ScrMap.h"

#include "../../COMMON/ComMath.h"

void ScrMap::initEffect(bool init) {
	LsdScreen &scr = pGM->lsd.Screen;
	LsdMap &map = pGM->lsd.Map;

	if (init) {
		scr.pan_x = 0;
		scr.pan_y = 0;
		scr.pan_move_x = 0.0f;
		scr.pan_move_y = 0.0f;
		scr.pan_moved_x = 0.0f;
		scr.pan_moved_y = 0.0f;
		map.is_pan_lock = false;
	}
	isEffShakelastFrameUsed = false;
	effShakeRunFrame = 0;
	isEffShakeAddX = 0.0f;

	pEffShakeHider = CCSprite::create("bg_black.png");
	pEffShakeHider->setScaleX(32);
	pEffShakeHider->setScaleY(240);
	pEffShakeHider->setPositionY(120.0f);
	pEffShakeHider->setVisible(false);
	this->addChild(pEffShakeHider, sz_animation-1);


	//Snow
	float per = ScreenCommon::game_s;

	pParticleSnow = CCParticleSnow::create();
	pParticleSnow->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle-snow.png"));
	pParticleSnow->setTotalParticles(400);
	pParticleSnow->setStartSize(10*per);
	pParticleSnow->setStartSizeVar(2*per);
	pParticleSnow->setEndSize(5*per);
	pParticleSnow->setEndSizeVar(2*per);
	pParticleSnow->setSpeed(200*per);
	pParticleSnow->setSpeedVar(100*per);
	pParticleSnow->setLife(3);
	pParticleSnow->setLifeVar(2);
	pParticleSnow->setEmissionRate(0);
	pParticleSnow->setPosition(ccp(ScreenCommon::game_w/2 , ScreenCommon::game_h));
	pParticleSnow->setScale(1.0f/ScreenCommon::game_s);
	this->addChild(pParticleSnow, sz_weather);


	//Rain
	pParticleRain = CCParticleRain::create();
	pParticleRain->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle-fire.png"));
	pParticleRain->setTotalParticles(400);
	pParticleRain->setStartSize(5*per);
	pParticleRain->setStartSizeVar(2*per);
	pParticleRain->setEndSize(5*per);
	pParticleRain->setEndSizeVar(2*per);
	pParticleRain->setSpeed(400*per);
	pParticleRain->setLife(2);
	pParticleRain->setLifeVar(2);
	pParticleRain->setEmissionRate(0);
	pParticleRain->setPosition(ccp(ScreenCommon::game_w/2 , ScreenCommon::game_h));
	pParticleRain->setScale(1.0f/ScreenCommon::game_s);
	this->addChild(pParticleRain, sz_weather);

	//Fog
	CCSpriteBatchNode *pSprBat;
	CCTexture2D *pTex;
	CCSprite *pSpr;

	for(int i=0; i<2; i++) {
		pSprBat = CCSpriteBatchNode::create("particle-fog2.png");
		pTex = pSprBat->getTexture();

		for(int x=0; x<3; x++) {
			for(int y=0; y<3; y++) {
				pSpr = CCSprite::createWithTexture(pTex);
				pSpr->setPosition(ccp(pTex->getContentSize().width * x, pTex->getContentSize().height * y));
				pSprBat->addChild(pSpr);
			}
		}

		pSprBat->setVisible(false);
		this->addChild(pSprBat, sz_weather);

		pParticleFog[i] = pSprBat;
	}

	//YellowFog

	setEffect(scr.weather_type, scr.weather_power);
}

void ScrMap::destroyEffect() {
	this->removeChild(pEffShakeHider);
	this->removeChild(pParticleRain);
	this->removeChild(pParticleSnow);

	for(int i=0; i<2; i++)
		this->removeChild(pParticleFog[i]);
}

void ScrMap::setEffect(int type, int power) {
	LsdScreen &scr = pGM->lsd.Screen;
	scr.weather_type = type;
	scr.weather_power = power;

	effWeatherLastScrX = scrX;
	effWeatherLastScrY = scrY;

	switch(scr.weather_type) {
	case 0:
		{
			pParticleRain->setEmissionRate(0.0f);
			pParticleSnow->setEmissionRate(0.0f);
			break;
		}

	case 1:
		{
			pParticleRain->setEmissionRate(60.0f * (power+1));
			pParticleSnow->setEmissionRate(0.0f);
			for(int i=0; i<2; i++)
				pParticleFog[i]->setVisible(false);
			break;
		}
	case 2:
		{
			pParticleRain->setEmissionRate(0.0f);
			pParticleSnow->setEmissionRate(30.0f * (power+1));
			for(int i=0; i<2; i++)
				pParticleFog[i]->setVisible(false);

			break;
		}
	case 3:
		{
			CCSprite *pSpr;
			pParticleRain->setEmissionRate(0.0f);
			pParticleSnow->setEmissionRate(0.0f);
			for(int i=0; i<2; i++) {
				pParticleFog[i]->setVisible(true);
				for(int j=0; j<pParticleFog[i]->getChildrenCount(); j++) {
					pSpr = ((CCSprite *)(pParticleFog[i]->getChildren()->data->arr[j]));
					pSpr->setOpacity(80*(power+1));
					pSpr->setColor(ccc3(255,255,255));
				}
			}
			break;
		}
	case 4:
		{
			CCSprite *pSpr;
			pParticleRain->setEmissionRate(0.0f);
			pParticleSnow->setEmissionRate(0.0f);
			for(int i=0; i<2; i++) {
				pParticleFog[i]->setVisible(true);
				for(int j=0; j<pParticleFog[i]->getChildrenCount(); j++) {
					pSpr = ((CCSprite *)(pParticleFog[i]->getChildren()->data->arr[j]));
					pSpr->setOpacity(80*(power+1));
					pSpr->setColor(ccc3(255,255,0));
				}
			}
			break;
		}
	}
}

void ScrMap::drawEffect( int framedelta )
{
	LsdScreen &scr = pGM->lsd.Screen;
	LsdMap &map = pGM->lsd.Map;

	//SHAKE!!
	bool isshake = false;
	if (scr.shake_continuous == 0) {
		if (scr.shake_timeleft > 0) {
			isshake = true;
			scr.shake_timeleft -= framedelta;

			if (scr.shake_timeleft <= 0) {
				scr.shake_timeleft = 0;

				float t = 0.0f;
				while(effShakeRunFrame/60.0f * (scr.shake_speed+1) > t)
					t += 2.0f;

				t *= 60.0f / (scr.shake_speed+1);

				effShakeEndFrame = effShakeRunFrame - (int)t;
			}
		}
	} else if (scr.shake_continuous == 1) {
		isshake = true;
	}

	if (isshake) {
		if (!isEffShakelastFrameUsed) {
			isEffShakelastFrameUsed = true;
			effShakeRunFrame = 0;
		}

		effShakeRunFrame += framedelta;
		isEffShakeAddX = sin(effShakeRunFrame/20.0f * PI * (scr.shake_speed+1)) * scr.shake_power  * 1.5f;

	} else {
		if (isEffShakelastFrameUsed) {
			if (effShakeEndFrame < 0) {
				isEffShakeAddX = sin(effShakeEndFrame/20.0f * PI * (scr.shake_speed+1)) * scr.shake_power  * 1.5f;
				effShakeEndFrame += framedelta;
			} else {
				isEffShakelastFrameUsed = false;
				isEffShakeAddX = 0.0f;
			}
		}
	}

	if (scr.weather_type > 0) {
		if (scr.weather_type < 3) {
			float defX = (scrX - effWeatherLastScrX)*(-80.0f);
			float defY = (scrY - effWeatherLastScrY)*(50.0f);

			pParticleRain->setGravity(ccp(defX, defY));
			pParticleSnow->setGravity(ccp(defX, defY));

			effWeatherLastScrX = scrX;
			effWeatherLastScrY = scrY;
		} else {
			int frame = pGM->lsd.System.frame_count;
			for(int i=0; i<2; i++) {
				pParticleFog[i]->setVisible(true);
				pParticleFog[i]->setPositionX(sin((frame+174*i)/60.0f)*(13+30*i) - 45*i);
				pParticleFog[i]->setPositionY(sin((frame+375*i)/300.0f)*(30+13*i) - 192*i);
			}
		}
	}
}