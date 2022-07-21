#include "FileCTL.h"

#include "FileType.h"
#include "TFolder.h"
#include "../AppMulti.h"

#define FILE_MAX_NAME 512

#define UNZ_BUFFER_SIZE 1024

FileCTL::FileCTL()
{
	pRTP = NULL;

	isOpened = false;

	bufSize = UNZ_BUFFER_SIZE;
	pBuf = new char[bufSize];
}

FileCTL::~FileCTL()
{
	close();
	delete []pBuf;
}

void FileCTL::Init( GrpRTP *rtp, GrpRTPUser *rtpu )
{
	pRTP = rtp;
	pRTPUser = rtpu;
}

bool FileCTL::open( const TString &path, bool is_archive )
{
	if (isOpened) {
		if (openedPath != path)	
			close();
		else
			return true;
	}

	isArchive = is_archive;

	if (isArchive) {
		isOpened = unzCtl.open(path);
		openedPath = path;

		return isOpened;
	} else {
		if (path.charAt(path.getLength()-1) == '/')
			openedPath = path;
		else
			openedPath = path + "/";
		isOpened = true;
		return true;
	}
}

void FileCTL::close()
{
	if (!isOpened)
		return;

	if (isArchive) {
		unzCtl.close();
	} else {

	}

	isOpened = false;
}

bool FileCTL::getData( File_Type_Name type, const TString &name, char** data, int* datasize )
{

	//CCLOG("getData %s=>%s", File_Type_Folder[(int)type].getTextUTF8(), name.getTextUTF8());
	if (isArchive) {
		if (unzCtl.getData(type, name, data, datasize)) {
			lastFileExt = unzCtl.getLastExt();
			return true;
		}
	} else {
		TString path;

		if (type == fileFolder) {
			path = openedPath + name;
		} else if (type == fileSound) {
			path = findSoundFullPath(openedPath + File_Type_Folder[(int)type], name);
		} else if (type == fileMusic) {
			path = findMusicFullPath(openedPath + File_Type_Folder[(int)type], name);
		} else if (type == fileMovie) {
			path = findMovieFullPath(openedPath + File_Type_Folder[(int)type], name);
		} else if (type == fileFolder) {
			path = findEtcFullPath(openedPath, name);
		} else {
			path = findPictureFullPath(openedPath + File_Type_Folder[(int)type], name);
		}

		int pointpos = path.lastIndexOf('.');

		lastFileExt = path.substring(pointpos);

		if (!path.isEmpty() && loadData(path, data, datasize))
			return true;
	}

	if (type != fileFolder) {
		if (pRTPUser->getData(type, name, data, datasize) ||
			pRTP->getData(type, name, data, datasize))
				return true;
	}

	if (!name.isEmpty()) {
		CCLOG("Fail to load %s <%s>", File_Type_Folder[(int)type].getTextUTF8(), name.getTextUTF8());
	}
	return false;
}

bool FileCTL::loadData( const TString &path, char** data, int* datasize ) {
	std::ifstream file;
	int filesize;

#if (SR_WIN == 1)
	char tmp[1024];
	path.getText_s(tmp, 1024);

	file.open( tmp , ios::in | ios::binary);

#else
	file.open( path.getTextUTF8() , ios::in | ios::binary);
#endif

	if (file.fail() || !file.good() || !file.is_open())
		return false;

	file.seekg(0, ios::end);
	filesize = (int)file.tellg();

	if (bufSize < filesize) {
		delete []pBuf;
		pBuf = new char[filesize];
		bufSize = filesize;
	}

	file.clear(); 
	file.seekg(0, ios::beg);

	file.read(pBuf, filesize);

	file.close();

	*data = pBuf;
	*datasize = filesize;
	return true;
}

bool FileCTL::isReadable( File_Type_Name type, const TString &name )
{
	TString path;
	if (isArchive) {
		if (unzCtl.isReadable(type, name))
			return true;
	} else {
		if (type != fileFolder) {
			path = findPictureFullPath(openedPath + File_Type_Folder[(int)type], name);

			if (path.isEmpty())
				return false;
		} else {
			path = openedPath + name;
		}

		return true;
	}

	if (type != fileFolder) {
		path = pRTPUser->findName(type, name);

		if (!path.isEmpty()) {
			return true;
		} else {
			path = pRTP->findName(type, name);

			if (!path.isEmpty())
				return true;
		}
	}


	return false;
}

TString FileCTL::findEtcFullPath( const TString & path, const TString & name )
{
	TFolder folder;
	TString findingfilename = name.toUpper();
	TString pathes[3];

	if (folder.open(path)) {
		do{
			TString filepath = folder.getName();
			TString filenameU = filepath.toUpper();

			if (findingfilename == filenameU) {
				if (path.charAt(path.getLength()-1) == '/')
					return path + filepath;
				else
					return path + "/" + filepath;
			}

		}while(folder.next());

		folder.close();
	}
	return "";
}

TString FileCTL::findPictureFullPath( const TString & path, const TString & name )
{
	TFolder folder;
	TString findingfilename = name.toUpper() + ".";
	TString pathes[3];

	if (folder.open(path)) {
		do{
			TString filepath = folder.getName();
			TString filenameU = filepath.toUpper();
			if (filenameU.indexOf(findingfilename) != -1) {
				if (filenameU.indexOf(".BMP") != -1) {
					pathes[0] = filepath;
				} else if (filenameU.indexOf(".PNG") != -1) {
					pathes[1] = filepath;
				} else if (filenameU.indexOf(".XYZ") != -1) {
					pathes[2] = filepath;
				}
			}

		}while(folder.next());

		folder.close();
	}
	for(int i=0; i<3; i++) {
		if (!pathes[i].isEmpty()) {
			if (path.charAt(path.getLength()-1) == '/')
				return path + pathes[i];
			else
				return path + "/" + pathes[i];
		}
	}
	return "";
}


TString FileCTL::findSoundFullPath( const TString & path, const TString & name )
{
	TFolder folder;
	TString findingfilename = name.toUpper() + ".";
	if (folder.open(path)) {
		do{
			TString filepath = folder.getName();
			TString filenameU = filepath.toUpper();
			if (filenameU.indexOf(findingfilename) != -1) {
				if (filenameU.indexOf(".WAV") != -1) {
					if (path.charAt(path.getLength()-1) == '/')
						return path + filepath;
					else
						return path + "/" + filepath;
				}
			}

		}while(folder.next());

		folder.close();
	}
	return "";
}

TString FileCTL::findMusicFullPath( const TString & path, const TString & name )
{
	TFolder folder;
	TString findingfilename = name.toUpper() + ".";
	TString pathes[3];
	if (folder.open(path)) {
		do{
			TString filepath = folder.getName();
			TString filenameU = filepath.toUpper();
			if (filenameU.indexOf(findingfilename) != -1) {
				if (filenameU.indexOf(".MP3") != -1) {
					pathes[0] = filepath;
				} else if (filenameU.indexOf(".MID") != -1) {
					pathes[1] = filepath;
				} else if (filenameU.indexOf(".MIDI") != -1) {
					pathes[2] = filepath;
				}
			}

		}while(folder.next());

		folder.close();
	}
	for(int i=0; i<3; i++) {
		if (!pathes[i].isEmpty()) {
			if (path.charAt(path.getLength()-1) == '/')
				return path + pathes[i];
			else
				return path + "/" + pathes[i];
		}
	}

	return "";
}

TString FileCTL::findMovieFullPath( const TString & path, const TString & name )
{
	TFolder folder;
	TString findingfilename = name.toUpper() + ".";
	TString pathes[3];
	if (folder.open(path)) {
		do{
			TString filepath = folder.getName();
			TString filenameU = filepath.toUpper();
			if (filenameU.indexOf(findingfilename) != -1) {
				if (filenameU.indexOf(".AVI") != -1) {
					pathes[0] = filepath;
				} else if (filenameU.indexOf(".WMV") != -1) {
					pathes[1] = filepath;
				} else if (filenameU.indexOf(".MP4") != -1) {
					pathes[2] = filepath;
				}
			}

		}while(folder.next());

		folder.close();
	}
	for(int i=0; i<3; i++) {
		if (!pathes[i].isEmpty()) {
			if (path.charAt(path.getLength()-1) == '/')
				return path + pathes[i];
			else
				return path + "/" + pathes[i];
		}
	}

	return "";
}

TString FileCTL::getLastExt()
{
	return lastFileExt;
}
