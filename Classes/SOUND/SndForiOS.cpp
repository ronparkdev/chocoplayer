#include "../COMMON/DebugingCode.h"

#include "SndMain.h"
#include "../AppMulti.h"
#include "../DATA/TString.h"

#if (SR_IPHONE == 1)

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool gisFadeIn = false;
int gfade_frame_total = 0;
int gfade_frame_left = 0;

void SndMain::PlaySound( const GlbSound &val )
{
	if (val.name == "(OFF)")
		return;
    
    TString path = findSoundPath(val.name);
    
    if (path.isEmpty()) {
		//CCLOG((_ST("SndMain:PlaySound cant find file <") + val.name + ">").getTextUTF8());
		return;
	}

    SimpleAudioEngine::sharedEngine()
        ->playEffect(path.getTextUTF8(),
                     val.volume / 100.0f,
                     val.tempo / 100.0f,
                     (50 - val.balance)/50.0f
                     );
    
//    SimpleAudioEngine::sharedEngine()->playEffect(path.getTextUTF8());
	//CCLOG(("Sound Play : " + path).getTextUTF8());
}

void SndMain::StopSound()
{
	SimpleAudioEngine::sharedEngine()
        ->stopAllEffects();
}

void SndMain::PlayMusic( const GlbMusic &val )
{
    
    bool isLoop = true;
    
    if (val.name.getLength() > 0 &&
       val.name.charAt(0) == 'J') {
        isLoop = false;
    }
    
	pLsd->System.music_current = val;
    
    if (val.name == "(OFF)") {
		StopMusic();
		return;
	}
    
	TString path = findMusicPath(val.name);
	
    SimpleAudioEngine::sharedEngine()
        ->setBackgroundMusicVolume(val.volume/100.0f);
    
    SimpleAudioEngine::sharedEngine()
    ->playBackgroundMusic(path.getTextUTF8(),
                 val.volume / 100.0f,
                 val.tempo / 100.0f,
                 (50 - val.balance)/50.0f,
                 isLoop
                 );

    gisFadeIn = true;
    gfade_frame_left = gfade_frame_total = val.fadein * 60 / 1000;
    
	//CCLOG(("Music Play : " + path).getTextUTF8());
}

void SndMain::StopMusic()
{
	pLsd->System.music_current.name = "(OFF)";
    
    SimpleAudioEngine::sharedEngine()
    	->stopBackgroundMusic();
    
	//CCLOG("Music Stop");
}

void SndMain::FadeoutMusic( int sec )
{
    gisFadeIn = false;
    gfade_frame_left = gfade_frame_total = sec * 60 / 1000;
}


void SndMain::onPause()
{
 	SimpleAudioEngine::sharedEngine()
 		->pauseBackgroundMusic();

 	SimpleAudioEngine::sharedEngine()
 		->pauseAllEffects();
}

void SndMain::onResume()
{
 	if (pLsd->System.music_current.name != "(OFF)")
        SimpleAudioEngine::sharedEngine()
            ->resumeBackgroundMusic();

 	SimpleAudioEngine::sharedEngine()
 		->resumeAllEffects();
}

void SndMain::onRelease()
{
 	SimpleAudioEngine::sharedEngine()
 		->stopAllEffects();
 
 	SimpleAudioEngine::sharedEngine()
 		->stopBackgroundMusic();
}

void SndMain::doCheck(int framedelta) {
    GlbMusic &mus = pLsd->System.music_current;
    if (gfade_frame_left > 0) {
        gfade_frame_left -= framedelta;
        if (gfade_frame_left > 0) {
            float per = (float)(gfade_frame_total - gfade_frame_left)/gfade_frame_total;
            
            if (!gisFadeIn)
                per = 1.0f - per;
            
            SimpleAudioEngine::sharedEngine()
                ->setBackgroundMusicVolume(mus.volume / 100.0f * per);
        } else {
            if (gisFadeIn) {
                SimpleAudioEngine::sharedEngine()
                    ->setBackgroundMusicVolume(mus.volume / 100.0f);
            } else {
                SimpleAudioEngine::sharedEngine()
                    ->stopBackgroundMusic();
                
                mus.name = "(OFF)";
            }
        }
    }
}

#endif
