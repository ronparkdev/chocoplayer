#include "SndMain.h"

#include <stdio.h>

SndMain::SndMain()
{
	gamePath = "";
	failSoundId = failSoundLen = 0;
	tmpSoundId = tmpSoundLen = 0;
	tmpMusicId = tmpMusicLen = 0;
	tmpVideoId = tmpVideoLen = 0;
}

SndMain::~SndMain()
{

}

void SndMain::Init( LsdMain *lsd, FileCTL* filectl, TString tmp_path )
{
	pLsd = lsd;
	pFCtl = filectl;
	tmpPath = tmp_path;
}

void SndMain::setPath( TString path )
{
	gamePath = path;

	int pos_slush = path.lastIndexOf('/');

	//m_temp_path = path.substring(0, pos_slush) + "/TEMP/";
	TFolder::chkFolder(tmpPath);

	removeTemp();
	tmpSoundLen = 0;
	tmpMusicLen = 0;
	tmpVideoLen = 0;
}

TString SndMain::findSoundPath( const TString & name )
{
	int i = 0;
	char *data;
	int datasize;
	
	for(int i=0; i<tmpSoundLen; i++) {
		if (tmpSound[i] == name)
			return tmpPath + "sound" + i + ".wav";
	}

	for(int i=0; i<failSoundLen; i++) {
		if (failSound[i] == name)
			return "";
	}

	
	if (!pFCtl->getData(fileSound, name, &data, &datasize)) {
		failSound[failSoundId] = name;
		failSoundId = (failSoundId+1)%SND_FAILED_SOUND;
		failSoundLen = min(failSoundLen+1, SND_FAILED_SOUND);

		return "";
	}

	TString path = tmpPath + "sound" + tmpSoundId + ".wav";

	char pathW[STRUCT_LOADER_PATH_MAX];
#ifdef _WIN32
	path.getText_s(pathW,STRUCT_LOADER_PATH_MAX);
#else
	strcpy(pathW, path.getTextUTF8());
#endif

	if (!cvtWav.ChkAndFix(data, datasize, pathW)) {
		std::ofstream os;
		os.open( pathW , ios::trunc | ios::binary);

		if (!os.is_open())
			return "";

		os.write(data, datasize);
		os.close();
	}

	tmpSound[tmpSoundId] = name;
	tmpSoundId = (tmpSoundId+1)%SND_BUFFER_SOUND;
	tmpSoundLen = min(tmpSoundLen+1, SND_BUFFER_SOUND);
	return path;
}

TString SndMain::findMusicPath( const TString & name )
{
	int i = 0;

	for(int i=0; i<tmpMusicLen; i++) {
		if (tmpMusic[i] == name)
			return tmpMusicPath[i];
	}

	char *data;
	int datasize;

	if (!pFCtl->getData(fileMusic, name, &data, &datasize))
		return "";

	TString path = tmpPath + "music" + tmpMusicId + pFCtl->getLastExt();

	char pathW[STRUCT_LOADER_PATH_MAX];
#ifdef _WIN32
	path.getText_s(pathW,STRUCT_LOADER_PATH_MAX);
#else
	strcpy(pathW, path.getTextUTF8());
#endif

	std::fstream m_fs;
	m_fs.open( pathW , ios::out | ios::trunc | ios::binary);

	if (!m_fs.is_open())
		return "";
		
	m_fs.write(data, datasize);
	m_fs.close();

	tmpMusic[tmpMusicId] = name;
	tmpMusicPath[tmpMusicId] = path;
	tmpMusicId = (tmpMusicId+1)%SND_BUFFER_MUSIC;
	tmpMusicLen = min(tmpMusicId+1, SND_BUFFER_MUSIC);
	return path;	
}

TString SndMain::findMoviePath( const TString & name )
{
	int i = 0;

	for(int i=0; i<tmpVideoLen; i++) {
		if (tmpVideo[i] == name)
			return tmpVideoPath[i];
	}

	char *data;
	int datasize;

	if (!pFCtl->getData(fileMovie, name, &data, &datasize))
		return "";

	TString path = tmpPath + "video" + tmpVideoId + pFCtl->getLastExt();

	char pathW[STRUCT_LOADER_PATH_MAX];
#ifdef _WIN32
	path.getText_s(pathW,STRUCT_LOADER_PATH_MAX);
#else
	strcpy(pathW, path.getTextUTF8());
#endif

	std::fstream m_fs;
	m_fs.open( pathW , ios::out | ios::trunc | ios::binary);

	if (!m_fs.is_open())
		return "";

	m_fs.write(data, datasize);
	m_fs.close();

	tmpVideo[tmpVideoId] = name;
	tmpVideoPath[tmpVideoId] = path;
	tmpVideoId = (tmpVideoId+1)%SND_BUFFER_VIDEO;
	tmpVideoLen = min(tmpVideoId+1, SND_BUFFER_VIDEO);
	return path;	
}

void SndMain::removeTemp() {
	
	if (gamePath.isEmpty())
		return;
	
	//onRelease();

	TFolder folder;
	TString path;
	char pathW[STRUCT_LOADER_PATH_MAX];
	if (folder.open(tmpPath)) {
		do{
			path = folder.get();

#ifdef _WIN32
			path.getText_s(pathW,STRUCT_LOADER_PATH_MAX);
#else
			strcpy(pathW, path.getTextUTF8());
#endif
			remove(pathW);

		}while(folder.next());
	}
}
