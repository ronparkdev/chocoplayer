#include "GlobalLang.h"

#include "../DATA/StructLoader.h"
#include "../DATA/BaseStruct.h"
#include "../FILEIO/TFolder.h"
#include "../FILEIO/UnzipCTL.h"

int GlobalLang::getEncoding( const TString &path, bool isarchive ) {

	memset(countEncoding, 0, sizeof(int)*ENCODING_COUNT);

	if (!isarchive) {
		TFolder folder;
		if (folder.open(path)) {
			do {
				if (folder.isfolder()) {
					getEncFromFolder(folder.get());
				}
			}while(folder.next());
		}
	} else {
		return -1;
	}

	int maxval = 0;
	int maxid = -1;
	for(int i=0; i<ENCODING_COUNT; i++) {
		int val = countEncoding[i]*EncodingWeight[i];
		if (val > maxval) {
			maxval = val;
			maxid = i;
		}
	}

	return maxid;
}

void GlobalLang::getEncFromFolder( const TString &path ) {
	TFolder folder;
	if (folder.open(path)) {
		do {
			if (!folder.isfolder()) {
				getEncFromString(folder.getName());
			}
		}while(folder.next());
	}
}

void GlobalLang::getEncFromString( const TString &str ) {
	if (str.isEmpty())
		return;

	char *s = str.getTextUTF8();
	int slen = strlen(s);
	int c, pos = 0;
	while(pos < slen) {
		if ((s[pos] & 0x80) == 0x00) {
			c = s[pos] & 0x7F;
			pos += 1;
		} else if ((s[pos] & 0xE0) == 0xC0) {
			if (pos+1 >= slen)return;
			c = ((s[pos] & 0x1F) << 6) | (s[pos+1] & 0x3F);
			pos += 2;
		} else if ((s[pos] & 0xF0) == 0xE0) {
			if (pos+2 >= slen)return;
			c = ((s[pos] & 0x0F) << 12) | ((s[pos+1] & 0x3F) << 6) | (s[pos+2] & 0x3F);
			pos += 3;
		} else if ((s[pos] & 0xF8) == 0xF0) {
			if (pos+3 >= slen)return;
			c = ((s[pos] & 0x07) << 18) | ((s[pos+1] & 0x3F) << 12) | ((s[pos+2] & 0x3F) << 6) | (s[pos+3] & 0x3F);
			pos += 4;
		} else {
			return;
		}

		if ( (0x1100 <= c && c <0x11FF) ||
			(0x3130 <= c && c <0x318F) ||
			(0xA960 <= c && c <0xA97F) ||
			(0xAC00 <= c && c <0xD7AF) ||
			(0xD7B0 <= c && c <0xD7FF) )
			countEncoding[LKOREAN]++;
		else if (
			(0x3040 <= c && c <0x309F) ||
			(0x30A0 <= c && c <0x30FF))
			countEncoding[LJAPANESE]++;
		else if (
			(0x2FF0 <= c && c <0x2FFF) ||
			(0x31C0 <= c && c <0x31EF) ||
			(0x3400 <= c && c <0x4DBF) ||
			(0x4E00 <= c && c <0x9FBF) ||
			(0xF900 <= c && c <0xFAFF) ||
			(0x1F200 <= c && c <0x1F251) ||
			(0x20000 <= c && c <0x2A6DF) ||
			(0x2A700 <= c && c <0x2B73F) ||
			(0x2B740 <= c && c <0x2B81F) ||
			(0x2F800 <= c && c <0x2FA1F))
			countEncoding[LCHINESE]++;
		else if (
			(0x0370 <= c && c <0x058F) ||
			(0x0E00 <= c && c <0x109F) ||
			(0x1F00	<= c && c <0x1FFF) ||
			(0x2C00 <= c && c <0x2C5F) ||
			(0x2C80 <= c && c <0x2CFF))
			countEncoding[LMSEURO]++;
		else if (
			(0x0400 <= c && c <0x052F))//Cyrillic script
			countEncoding[LRUSSIAN]++;
	}
}
