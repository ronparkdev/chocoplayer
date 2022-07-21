#include "UnzipCTL.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../AppMulti.h"
#include "../DATA/StrConv.h"

#define FILE_MAX_NAME 512

#define UNZ_BUFFER_SIZE 1024

UnzipCTL::UnzipCTL() :isOpened(false)
{
	bufSize = UNZ_BUFFER_SIZE;
	pBuf = new char[bufSize];
}

UnzipCTL::~UnzipCTL()
{
	close();
	delete []pBuf;
}

bool UnzipCTL::open( const TString &path, bool isRTP /*= false */ )
{
	if (isOpened)
		close();

#if (SR_WIN == 1)
	char tmp[1024];
	path.getText_s(tmp, 1024);

	uf_ = unzOpen(tmp);
#else
	uf_ = unzOpen(path.getTextUTF8());
#endif

	if (uf_ == NULL) {

		CCLOG("Fail to open Zip file <%s>", path.getTextUTF8());
		return false;
	}

	if (!findGameFolder(isRTP)) {
		CCLOG("Fail to check R2K Zip file <%s>", path.getTextUTF8());
		unzClose(uf_);
		return false;
	}
	
    //printFileList();
    
	isRTP = true;
	isOpened = true;
	return true;
}

void UnzipCTL::close()
{
	if (!isOpened)
		return;

	unzClose(uf_);
	isOpened = false;
}

bool UnzipCTL::findGameFolder(bool isRTP) {
	int ret;
	int i, pos, pos2, curtype, lasttype;
	unz_file_info info;
	char str[FILE_MAX_NAME];
	std::string fullpath, filename, foldername;
	unz_file_pos file_pos;
	
	bool is_found = false;
	gamePath = "";

	if (UNZ_OK != (ret = unzGoToFirstFile(uf_)))
		return false;

	for(i=0; i<R2K_FILE_TYPES; i++) {
		folders[i].pos_start.RemoveAll();
		folders[i].pos_length.RemoveAll();
	}

	if (!isRTP) {
		do{
			unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

			fullpath = str;

			pos = fullpath.find_last_of('/');

			if (pos != -1 && pos >= fullpath.length()-1)
				continue;

			TString filename;
			filename.setText(fullpath.substr(pos+1).c_str());
			filename = filename.toUpper();

			if (filename == "RPG_RT.LDB" || filename == "RPG_RT.LMT") {
				gamePathLen = pos+1;
				gamePath = fullpath.substr(0, gamePathLen);

				is_found = true;
				break;
			}
		}while(UNZ_OK == unzGoToNextFile(uf_));

		if (!is_found)
			return false;
	} else {
		do{
			unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

			fullpath = str;

			pos2 = fullpath.find_last_of('/');
			pos = fullpath.find_last_of('/', 2);

			if (pos2 == -1)
				continue;

			if (pos == -1)
				pos = 0;
			else
				pos++;

			foldername = fullpath.substr(pos, pos2 - pos);

			for(int i=0; i<R2K_FILE_TYPES; i++) {
				if (foldername == File_Type_Folder[i]) {
					gamePathLen = pos;
					is_found = true;
					break;
				}
			}

			if (is_found)
				break;

		}while(UNZ_OK == unzGoToNextFile(uf_));

		if (!is_found)
			return false;
	}

	if (UNZ_OK != (ret = unzGoToFirstFile(uf_)))
		return false;

	lasttype = -1;

	do{
		unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

		fullpath = str;

		if (fullpath.find(gamePath) != 0 && gamePath.length() > 0)
			continue;

		pos = fullpath.find_first_of('/', gamePathLen);

		if (pos == -1) {
			foldername = "";
			filename = fullpath.substr(gamePathLen);
		} else {
			foldername = fullpath.substr(gamePathLen, pos - gamePathLen);
			filename = fullpath.substr(pos+1);
		}
		
		if (filename.empty())
			continue;

		curtype = -1;
		for(i=0; i<R2K_FILE_TYPES; i++) {
			//if (m_folders[i].name == foldername) {
			if (File_Type_Folder[i] == foldername) {
                curtype = i;
				break;
			}
		}
      
        
		if (curtype != lasttype) {
			unzGetFilePos(uf_, &file_pos);

			if (curtype != -1) {
				folders[curtype].pos_start.Add(file_pos);
				folders[curtype].pos_length.Add(0);
			}
		} else {
			if (lasttype != -1)
				(*folders[lasttype].pos_length.GetTail())++;
		}

		lasttype = curtype;

		if (i == R2K_FILE_TYPES)
			continue;


	}while(UNZ_OK == unzGoToNextFile(uf_));

	return true;
}

bool UnzipCTL::getData(File_Type_Name type, const TString &name, char** data, int* datasize) {
	UnzipFolder &folder = folders[(int)type];
	int pos;
	unz_file_info info;
	char str[FILE_MAX_NAME];
	std::string fullpath, filename;
	
	TString nameU = name.toUpper();
	TString tfilename;

	int i, j, ret;
	for(i=0; i<folder.pos_start.GetLength(); i++) {
		if (UNZ_OK != (ret = unzGoToFilePos(uf_, &folder.pos_start[i])))
			continue;

		j = 0;
		do{
			unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

			fullpath = str;

			pos = fullpath.find_last_of('/');

			if (pos != -1 && pos >= fullpath.length()-1)
				continue;

			filename = fullpath.substr(pos+1);

			if (isUTF8(filename.c_str()))			
				tfilename.setTextUTF8(filename.c_str());
			else	
				tfilename.setText(filename.c_str());

			if (type == fileFolder) {
				if (tfilename.toUpper() != nameU)
					continue;
			} else {
				if (tfilename.indexOf(name + ".") != 0)
					continue;
			}

			int pointpos = tfilename.lastIndexOf('.');

			lastFileExt = tfilename.substring(pointpos);

			if (bufSize < info.uncompressed_size) {
				delete []pBuf;
				bufSize = 1;
				while(bufSize < info.uncompressed_size)
					bufSize <<= 1;
				pBuf = new char[bufSize];
			}

			if (UNZ_OK != unzOpenCurrentFile(uf_))
				return false;

			unzReadCurrentFile(uf_, pBuf, bufSize);

			unzCloseCurrentFile(uf_);

			*data = pBuf;
			*datasize = (int)info.uncompressed_size;

			return true;
		}while(UNZ_OK == unzGoToNextFile(uf_) &&
				j++ < folder.pos_length[i]);
	}

	return false;
}

bool UnzipCTL::isReadable( File_Type_Name type, const TString & name )
{
	UnzipFolder &folder = folders[(int)type];
	int pos;
	unz_file_info info;
	char str[FILE_MAX_NAME];
	std::string fullpath, filename;

	int i, j, ret;
	for(i=0; i<folder.pos_start.GetLength(); i++) {
		if (UNZ_OK != (ret = unzGoToFilePos(uf_, &folder.pos_start[i])))
			continue;

		j = 0;
		do{
			unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

			fullpath = str;

			pos = fullpath.find_last_of('/');

			if (pos != -1 && pos >= fullpath.length()-1)
				continue;

			filename = fullpath.substr(pos+1);

			if (type == fileFolder) {
				if (_ST(filename.c_str()) != name)
					continue;
			} else {
				if (_ST(filename.c_str()).indexOf(name + ".") != 0)
					continue;
			}

			return true;
		}while(UNZ_OK == unzGoToNextFile(uf_) &&
			j++ < folder.pos_length[i]);
	}

	return false;
}

void UnzipCTL::getFileList( ArrayList<TString> &fileList )
{
	int ret, pos;
	unz_file_info info;
	char str[FILE_MAX_NAME];
	std::string fullpath, filename, foldername;

	if (UNZ_OK != (ret = unzGoToFirstFile(uf_)))
		return;

	do{
		unzGetCurrentFileInfo(uf_, &info, str, FILE_MAX_NAME, NULL, 0, NULL, 0);

		fullpath = str;

		if (fullpath.find(gamePath) != 0 && gamePath.length() > 0)
			continue;

		pos = fullpath.find_first_of('/', gamePathLen);

		if (pos == -1)
			filename = fullpath.substr(gamePathLen);
		else
			filename = fullpath.substr(pos+1);

		if (filename.empty())
			continue;

		fileList.Add(filename);
	}while(UNZ_OK == unzGoToNextFile(uf_));
}

void UnzipCTL::printFileList() {
    for(int i=0; i<R2K_FILE_TYPES; i++) {
        UnzipFolder &folder = folders[i];
        
		CCLOG("----- %s cnt:%d", File_Type_Folder[i].getTextUTF8(), folder.pos_start.GetLength());
        
        for(int j=0; j<folder.pos_start.GetLength(); j++)
            CCLOG("%d -> %d", (int)folder.pos_start[j].num_of_file, (int)folder.pos_start[j].num_of_file + (int)folder.pos_length[j]);
    }
}

TString UnzipCTL::getLastExt()
{
	return lastFileExt;
}
