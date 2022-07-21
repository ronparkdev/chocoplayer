#include "ScrAppMainGameNode.h"

#include "../../GameMain.h"
#include "../../AppMultiTime.h"
#include "../../FILEIO/GraphicIO.h"
#include "../../FILEIO/FileCTL.h"

ScrAppMainGameNode::ScrAppMainGameNode()
{
	type = GameNodeNothing;

	pTexTitle = NULL;
	pImgTitle = NULL;
	isLoadFailed = false;

	stateTitle = GameNodeTitleNotLoaded;
	pImgTitle = new CCImage();

	sTitleEncID = -1;
	sEncId = -1;
	sColor = -1;
}

ScrAppMainGameNode::~ScrAppMainGameNode()
{
	//pImgTitle->release();
}

void ScrAppMainGameNode::setHelp()
{
	type = GameNodeHelp;
}

void ScrAppMainGameNode::set( const TString & path, bool isArchive )
{
	if (isArchive)
		setArchive(path);
	else
		setFolder(path);
}

void ScrAppMainGameNode::setFolder( const TString &path )
{
	type = GameNodeFolder;
	this->path = path;
	pathSetting = path + "chocoplayer.dat";
}

void ScrAppMainGameNode::setArchive( const TString &path )
{
	int lastPointPos = path.lastIndexOf('.');
	int lastSlushPos = path.lastIndexOf('/');
	if (lastPointPos == -1)
		lastPointPos = path.getLength();
	TString filePath = path.substring(0, lastSlushPos);
	TString fileName = path.substring(lastSlushPos+1, lastPointPos);

	type = GameNodeArchive;
	this->path = path;
	pathSetting = filePath + "/Setting/" + fileName + ".dat";
}

void ScrAppMainGameNode::reload()
{
	loadSettings();
	reloadSettings();
	saveSettings();

	stateTitle = GameNodeTitleNotLoaded;
	loadDataWithSubThread();
}

GameNodeType ScrAppMainGameNode::getType() {return type;}

GameNodeState ScrAppMainGameNode::getState() {return stateTitle;}

CCTexture2D* ScrAppMainGameNode::getTitleTex() {return pTexTitle;}

bool ScrAppMainGameNode::isNeedLoading()
{
	switch(type) {
	case GameNodeArchive:
	case GameNodeFolder:
		break;

	default:
		return false;
	}

	return (pImgTitle == NULL && pTexTitle);
}

void ScrAppMainGameNode::loadDataWithSubThread()
{
	if (!isLoadFailed && stateTitle == GameNodeTitleNotLoaded) {
		GameMain *g = GameMain::getInstance();
		GraphicIO gio;
		char *data;
		int datalen;
	
		loadSettings();

		isLoadFailed = true;

		//CCLOG("NODE IMAGE PREPARE %s", getShortPath().getTextUTF8());
		
		if (g->fctl.open(path, type == GameNodeArchive)) { 
			if (g->fctl.getData(fileTitle, sTitleName, &data, &datalen) &&
				gio.loadPicture(data, datalen, *pImgTitle, false))
			{
				stateTitle = GameNodeTitlePrepared;
				isLoadFailed = false;

				//CCLOG("NODE IMAGE READY %s IMG 0x%X", getShortPath().getTextUTF8(), pImgTitle);
			}
			g->fctl.close();
		}
	}
}

void ScrAppMainGameNode::loadTitleWithMainThread()
{
	if (stateTitle == GameNodeTitlePrepared) {
		pTexTitle = new CCTexture2D();
		pTexTitle->initWithImage(pImgTitle);
		stateTitle = GameNodeTitleLoaded;

		loadedTime = getTime();

		//CCLOG("NODE TEXTURE READY %s IMG 0x%X TEX 0x%X", getShortPath().getTextUTF8(), pImgTitle, pTexTitle);
	}
}

float ScrAppMainGameNode::getLoadedTime()
{
	return loadedTime;
}

void ScrAppMainGameNode::getGameData( TString &gamepath, bool &archive, TString &hashCode, int &encodingID )
{
	gamepath = path;
	archive = (type == GameNodeArchive);
	hashCode = sHashCode;
	encodingID = sEncId;
}

TString ScrAppMainGameNode::getName()
{
	return sGameName;
}

TString ScrAppMainGameNode::getPath()
{
	return path;
}

TString ScrAppMainGameNode::getHash()
{
	return sHashCode;
}

int ScrAppMainGameNode::getEncID()
{
	return sEncId;
}

TString ScrAppMainGameNode::getShortPath()
{
	GameMain *gm = GameMain::getInstance();
	if (gm->pathGames.getLength() > 0 && path.indexOf(gm->pathGames) == 0)
		return path.substring(gm->pathGames.getLength());
	else
		return path;
}

bool ScrAppMainGameNode::isArchive()
{
	return (type == GameNodeArchive);
}

void ScrAppMainGameNode::setEncID( int encId )
{
	sEncId = encId;
}
