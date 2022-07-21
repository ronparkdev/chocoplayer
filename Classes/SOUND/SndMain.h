#ifndef __R2K_SOUND_MAIN__
#define __R2K_SOUND_MAIN__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../AppMulti.h"
#include "../DATA/TString.h"
#include "../FILEIO/TFolder.h"
#include "../DATA/LSD/LsdMain.h"
#include "../DATA/GLB/GlbMusic.h"
#include "../DATA/GLB/GlbSound.h"
#include "../FILEIO/FileCTL.h"
#include "../FILEIO/ConvWAV.h"

#define SND_BUFFER_MUSIC 20
#define SND_FAILED_SOUND 50
#define SND_BUFFER_SOUND 100
#define SND_BUFFER_VIDEO 20

class SndMain{
private:
	LsdMain *pLsd;
	TString gamePath;
	FileCTL *pFCtl;


	TString tmpSound[SND_BUFFER_SOUND];
	int tmpSoundId, tmpSoundLen;

	TString failSound[SND_FAILED_SOUND];
	int failSoundId, failSoundLen;

	TString tmpMusic[SND_BUFFER_MUSIC];
	TString tmpMusicPath[SND_BUFFER_MUSIC];
	int tmpMusicId, tmpMusicLen;

	TString tmpVideo[SND_BUFFER_VIDEO];
	TString tmpVideoPath[SND_BUFFER_VIDEO];
	int tmpVideoId, tmpVideoLen;

	TString tmpPath;

	ConvWAV cvtWav;
public:
	SndMain();
	~SndMain();

	void Init( LsdMain *lsd, FileCTL* filectl, TString tmp_path );
	void setPath( TString path );

	void PlayMusic( const GlbMusic &val);
	void FadeoutMusic(int sec);
	void StopMusic();

	void PlaySound( const GlbSound &val);
	void StopSound();

	void removeTemp();

	void doCheck(int framedelta);
	void onPause();
	void onResume();
private:
	TString findSoundPath( const TString & name );
	TString findMusicPath( const TString & name );
public:
	TString findMoviePath( const TString & name );

	void onRelease();
};

#endif