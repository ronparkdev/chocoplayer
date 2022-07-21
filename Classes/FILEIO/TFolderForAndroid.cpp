#include "TFolder.h"

#include "../AppMulti.h"
#include "../DATA/TString.h"

#if (SR_ANDROID == 1 || SR_IPHONE == 1)
#include <sys/stat.h>
#include <cocos2d.h>
#include <errno.h>
USING_NS_CC;

TFolder::TFolder()
:isFolderOpened(false)
{
	
}

TFolder::~TFolder()
{
	if (isFolderOpened)
		close();
}

bool TFolder::open( TString path )
{
	if (path.isEmpty())
		return false;
	
	bool last_dot = true;
	if (isFolderOpened)
		close();
	
	char c = path.getTextUTF8()[strlen(path.getTextUTF8())-1];
	if (c != '/' && c != '\\')
		openedPath = path + "/";
	else
		openedPath = path;
	
	//CCLOGT("opening path : " + path);
	
	m_dp = opendir(path.getTextUTF8());
	if (m_dp == NULL)
		return false;
	
	while ((m_dirp = readdir(m_dp)) != NULL) {
		if ((strcmp(m_dirp->d_name, "..") == 0) || 
			(strcmp(m_dirp->d_name, ".") == 0) ||
			(strcmp(m_dirp->d_name, "../") == 0) ||
			(strcmp(m_dirp->d_name, "./") == 0))
		{
			last_dot = true;
		} else {
			last_dot = false;
			break;
		}
	}
	
	if (!last_dot)
		;//CCLOGT(_ST("finded firstfile : ") + m_dirp->d_name);
	
	if (last_dot)
		return false;
	
	isFolderOpened = true;
	return true;
}

void TFolder::close()
{
	if (!isFolderOpened)
		return;
	
#if (SR_WIN == 1)
	_findclose(m_handle);
#else if (SR_ANDROID == 1)
	closedir(m_dp);
#endif
	isFolderOpened = false;
}

bool TFolder::next()
{
	if (!isFolderOpened)
		return false;

	m_dirp = readdir(m_dp);
	if (m_dirp == NULL) {
		close();
		return false;
	}
	//CCLOGT(_ST("finded nextfile : ") + m_dirp->d_name);
	
	return true;
}

bool TFolder::isfolder()
{
	if (!isFolderOpened)
		return false;
	
	struct stat buf;
	
	TString str;
	str.setTextUTF8(m_dirp->d_name);
	
	if (lstat((openedPath + str).getTextUTF8(), &buf) < 0) {
		CCLOG((_ST("LSTAT ERROR : ") + errno).getTextUTF8());
		return false;
	}
	
	return S_ISDIR(buf.st_mode);
}

TString TFolder::get()
{
	if (!isFolderOpened)
		return "";
	
	TString str;
	
	str.setTextUTF8(m_dirp->d_name);
	
	return openedPath + str;
}

TString TFolder::getName()
{
	if (!isFolderOpened)
		return "";
	
	TString str;
	
	str.setTextUTF8(m_dirp->d_name);
	
	return str;
}

void TFolder::chkFolder( TString path )
{
	for(int i=0; i<path.getLength(); i++) {
		if (path.charAt(i) == '/') {
			TString subpath = path.substring(0, i);
			
			mkdir(subpath.getTextUTF8(), 0777); // notice that 777 is different than 0777
		}
	}
}

#endif
