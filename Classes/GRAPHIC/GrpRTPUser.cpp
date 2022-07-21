#include "GrpRTPUser.h"

#include <iostream>
#include <fstream>
#include <string>
#include "../COMMON/GlobalLang.h"

bool GrpRTPUser::Init( const TString &rootpath )
{
	nodes.RemoveAll();
	nodes.ChangeMode(ListMode);

	if (!fctl.open(rootpath + GRP_RTP_USER_DATA, true))
		return false;

	char data[102400];
	int datasize;

	TString tstr;
	int dstart, dpos, dlen;

	char buf[1024];

	std::string path_name = CCFileUtils::sharedFileUtils()->fullPathForFilename("rtp_name.dat");

    std::ifstream is(path_name.c_str());

	if (!is.is_open())
		return false;

	is.seekg (0, is.end);
	datasize = is.tellg();
	is.seekg (0, is.beg);

	is.read(data, datasize);

	dstart = dpos = 0;

	GrpRTPNode node;

	while(dpos < datasize) { 
		if (data[dpos] == '\n') {
			if (dpos - dstart > 1024) {
				return false;
			}

			//LineFeed : ODOA (2byte), 0A = '\n'
			dlen = dpos - dstart;
			memcpy(buf, data + dstart, sizeof(char)*(dlen));
			buf[dlen] = '\0';

			tstr.setTextUTF8(buf);

			int lasti = 0, i = -1, pos = 0;
			while((i = tstr.indexOf('\t', i+1)) != -1) {
				int val = tstr.substring(lasti, i).getInt();
				if (pos == 0) {//Lang
					node.lang = val;
				} else if (pos == 1) {//Ver
					node.ver = val;
				} else if (pos == 2) {//folder
					node.folder = val;
				} else if (pos == 3) {//index
					node.id = val;
				}
				lasti = i+1;
				pos++;
			}

			node.str = tstr.substring(lasti, tstr.getLength());

			nodes.Add(node);

			dstart = dpos + 1;
		}
		dpos++;
	}

	nodes.ChangeMode(ArrayMode);
	nodeLen = nodes.GetLength();
	return true;
}

TString GrpRTPUser::findName(File_Type_Name ctype, const TString &str )
{
	bool find_same_version = false;
	int last_i;

	int clang = 0;
	int cid = 0;
	int cver = 0;

	switch((Encoding)str.getEncoding()) {
	case LKOREAN:	clang = 1; break; 
	case LJAPANESE:	clang = 2; break; 
	case LCHINESE:	clang = 3; break; 
	case LMSEURO:	clang = 4; break; 
	case LRUSSIAN:	clang = 5; break; 
	default:
		return "";
	}

	int i = 0;
	//find folder head index
	while(true) {
		if (nodeLen <= i)
			return "";
		else if (nodes[i].folder == ctype)
			break;
		i++;
	}

	last_i = i;

	//find same filename in target version 
	while(true) {
		if (nodeLen <= i) {
			find_same_version = false;
			break;
		}

		GrpRTPNode &t = nodes[i];

		if (t.folder != ctype) {
			find_same_version = false;
			break;
		}

		char buf[1024];
		char buf2[1024];
		t.str.getText_s(buf, 1024);
		str.getText_s(buf2, 1024);

		if (t.ver == ver && t.str == str) {
			cid = t.id;
			find_same_version = true;
			break;
		}
		i++;
	}

	if (!find_same_version) {
		i = last_i;
		//find same filename in non target version 
		while(true) {
			if (nodeLen <= i)
				return "";

			GrpRTPNode &t = nodes[i];

			if (t.folder != ctype)
				return "";

			if (t.str == str) {
				cid = t.id;
				break;
			}
			i++;
		}
	}

	//find fixed filename
	while(true) {
		if (nodeLen <= i)
			return "";

		GrpRTPNode &t = nodes[i];

		if (t.folder != ctype)
			return "";

		else if (t.id == cid && t.ver == 0) {
			return t.str;
		}
		i++;
	}

	return clang;
}

void GrpRTPUser::setVersion( GameVersion game_ver )
{
	ver = game_ver;
}

bool GrpRTPUser::getData( File_Type_Name ctype, const TString &name, char **data, int *datasize )
{
	return fctl.getData(ctype, findName(ctype, name), data, datasize);
}
