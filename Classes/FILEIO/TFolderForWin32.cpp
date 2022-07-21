#include "TFolder.h"

#include "../AppMulti.h"
#include "../DATA/TString.h"

#if (SR_WIN == 1)
#include <direct.h>
//#include <wchar.h>

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
	
	(openedPath+"*.*").getText_s(openedPathC, CCFOLDER_PATH_MAX);
	m_handle = _findfirst(openedPathC, &m_fd);
	if (m_handle == -1)
		return false;
	
	while (_findnext(m_handle, &m_fd) != -1) {
		if (strcmp(m_fd.name, "..") && strcmp(m_fd.name, ".")) {
			last_dot = false;
			break;
		}else
			last_dot = true;
	}
	
	//CCLOGT(_ST("finded firstfile : ") + m_fd.name);
	
	if (last_dot)
		return false;
	
	isFolderOpened = true;
	return true;
}

void TFolder::close()
{
	if (!isFolderOpened)
		return;
	
	_findclose(m_handle);
	isFolderOpened = false;
}

bool TFolder::next()
{
	if (!isFolderOpened)
		return false;
	
	int result = _findnext(m_handle, &m_fd);
	if (result == -1) {
		close();
		return false;
	}
	//CCLOGT(_ST("finded nextfile : ") + m_fd.name);
	return true;
}

bool TFolder::isfolder()
{
	if (!isFolderOpened)
		return false;
	
	return (m_fd.attrib == _A_SUBDIR);
}

TString TFolder::get()
{
	if (!isFolderOpened)
		return "";
	
	TString str;
	str = m_fd.name;
	return openedPath + str;
}

TString TFolder::getName()
{
	if (!isFolderOpened)
		return "";
	
	TString str;
	str = m_fd.name;
	return str;
}

void TFolder::chkFolder( TString path )
{
	for(int i=0; i<path.getLength(); i++) {
		if (path.charAt(i) == '/') {
			TString subpath = path.substring(0, i);
			//_wmkdir((wchar_t *)subpath.getTextUTF8());
			_mkdir(subpath.getTextUTF8());
		}
	}
}

#endif
