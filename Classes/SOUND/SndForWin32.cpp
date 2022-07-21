#include "SndMain.h"

#if (SR_WIN == 1)

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

void SndMain::PlaySound( const GlbSound &val )
{
	if (val.name == "(OFF)")
		return;

	char pathC[1024];
	findSoundPath(val.name).getText_s(pathC, 1024);

// 	SimpleAudioEngine::sharedEngine()
// 		->playEffect(pathC);	

	//CCLOG((_ST("Sound Play : ") + pathC).getTextUTF8());
}

void SndMain::StopSound()
{
	SimpleAudioEngine::sharedEngine()
		->stopAllEffects();	
}

void SndMain::PlayMusic( const GlbMusic &val )
{
	pLsd->System.music_current = val;

	if (val.name == "(OFF)") {
		StopMusic();
		return;
	}

	char pathC[1024];
	findMusicPath(val.name).getText_s(pathC, 1024);
	
// 	SimpleAudioEngine::sharedEngine()
// 		->playBackgroundMusic(pathC, true);

	//CCLOG((_ST("Music Play : ") + pathC).getTextUTF8());
}

void SndMain::StopMusic()
{

	pLsd->System.music_current.name = "(OFF)";

// 	SimpleAudioEngine::sharedEngine()
// 		->stopBackgroundMusic();

	//CCLOG("Music Stop");
}

void SndMain::FadeoutMusic( int sec )
{

}


void SndMain::onPause()
{
// 	SimpleAudioEngine::sharedEngine()
// 		->pauseBackgroundMusic();
// 
// 	SimpleAudioEngine::sharedEngine()
// 		->pauseAllEffects();
}

void SndMain::onResume()
{
// 	SimpleAudioEngine::sharedEngine()
// 		->resumeBackgroundMusic();
// 
// 	SimpleAudioEngine::sharedEngine()
// 		->resumeAllEffects();
}

void SndMain::onRelease()
{
// 	SimpleAudioEngine::sharedEngine()
// 		->stopAllEffects();
// 
// 	SimpleAudioEngine::sharedEngine()
// 		->stopBackgroundMusic();
}

void SndMain::doCheck(int framedelta) {

}

#endif
