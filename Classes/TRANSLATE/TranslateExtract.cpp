#include "TranslateMain.h"

#include "../GameMain.h"
#include "../DATA/LMU/LmuMain.h"
#include "../EVENT/EvtCodes.h"
#include "../FILEIO/TSecure.h"

/*
void TranslateMain::saveDataToJson( const char *path, TRANSLATE_NET_STATE *state, int *cnt, int *cnttot)
{
	GameMain *main = GameMain::getInstance();

	if (main == NULL)
		return;

	TString mappath, str;
	LmuMain map;
	int needchk = 20;

	StructLoader sl;
	char *data;
	int datasize;

	Json::Value root;
	Json::Value jsaying;


	FileCTL *fctl = &(main->fctl);
	LdbMain *ldb = &(main->ldb);

	const TRANSLATE_NET_STATE runstate = TNETS_Processing_CollectingData;

	ArrayList<int> maps;
	ArrayList<LdbItem> &items = ldb->dItem;
	ArrayList<LdbSkill> &skills = ldb->dSkill;
	ArrayList<LdbCommonEvent> &coms = ldb->dComEvt;

	*cnttot = 1;
	*cnt = 0;

	maps.ChangeMode(ListMode);
	for(int mapid=1; mapid<10000 && needchk >0 ; mapid++, needchk--) {
		mappath.setText(mapid, 4);
		mappath = "Map" + mappath + ".lmu";

		if (fctl->isReadable(fileFolder, mappath) ) {
			maps.Add(mapid);
			needchk = 20;
			continue;
		}
	}
	maps.ChangeMode(ArrayMode);

	*cnttot = maps.GetLength() + items.GetLength() + skills.GetLength() + coms.GetLength();
	*cnt = 0;
	for(int i=0; i < maps.GetLength() && (*state == runstate); i++, (*cnt)++) {
		mappath.setText(maps[i], 4);
		mappath = "Map" + mappath + ".lmu";

		try{
			if (!fctl->getData(fileFolder, mappath, &data, &datasize) )
				continue;

			if (!sl.OpenWithNoFree(data, datasize))
				continue;

			map.Serialize(sl);
			sl.Close();
		}catch(...) {
			continue;
		}

		for(int evtID=0; evtID<map.Events.GetLength() && (*state == runstate); evtID++) {
			LmuEvent &evt = map.Events[evtID];

			for(int pageID=0; pageID<evt.Page.GetLength() && (*state == runstate); pageID++) {
				ArrayList<GlbEventNode> &ds = evt.Page[pageID].event_data.data;

				for(int dID=0; dID<ds.GetLength() && (*state == runstate); dID++) {
					GlbEventNode &d = ds[dID];

					Json::Value jdata;
					if (d.code == ecShowMessage) {
						TString target;
						
						target = d.string + "\n";

						for(int dID2=dID+1; dID2<ds.GetLength() && (*state == runstate); dID2++) {
							GlbEventNode &d2 = ds[dID2];
							if (d2.code == ecShowMessage_2) {
								target = target + d2.string + "\n";
							} else {
								break;
							}
						}

						chkMsg(target);

						TString hash = TSequre::getMD5(target);

						if (!target.isEmpty())
							jsaying[hash.getTextUTF8()] = target.getTextUTF8();
					} else if (d.code == ecShowChoiceOption) {

						TString target = " " + d.string;
						chkMsg(target);

						TString hash = TSequre::getMD5(target);

						if (!d.string.isEmpty())
							jsaying[hash.getTextUTF8()] = target.getTextUTF8();
					}
				}
			}
		}
	}

	for(int comID=0; comID < coms.GetLength() && (*state == runstate); comID++, (*cnt)++) {
		LdbCommonEvent &com = coms[comID];

		Json::Value jcom;
		ArrayList<GlbEventNode> &ds = com.event_data.data;
		for(int dID=0; dID<ds.GetLength() && (*state == runstate); dID++) {
			GlbEventNode &d = ds[dID];

			Json::Value jdata;
			if (d.code == ecShowMessage) {
				TString target;

				target = d.string + "\n";

				for(int dID2=dID+1; dID2<ds.GetLength() && (*state == runstate); dID2++) {
					GlbEventNode &d2 = ds[dID2];
					if (d2.code == ecShowMessage_2) {
						target = target + d2.string + "\n";
					} else {
						break;
					}
				}

				chkMsg(target);

				TString hash = TSequre::getMD5(target);

				if (!target.isEmpty())
					jsaying[hash.getTextUTF8()] = target.getTextUTF8();
			} else if (d.code == ecShowChoiceOption) {

				TString target = " " + d.string;
				chkMsg(target);
				
				TString hash = TSequre::getMD5(target);

				if (!d.string.isEmpty())
					jsaying[hash.getTextUTF8()] = target.getTextUTF8();
			}
		}
	}

	if (!jsaying.isNull())
		root["SAYING"] = jsaying;

	Json::Value jitems;
	jitems.resize(items.GetLength());
	for(int i=0; i < items.GetLength() && (*state == runstate); i++, (*cnt)++) {
		LdbItem &item = items[i];

		Json::Value jitem;

		if (!item.name.isEmpty())
			jitem["NAME"] = item.name.getTextUTF8();

		if (!item.desc.isEmpty())
			jitem["DESC"] = item.desc.getTextUTF8();

		str.setText(i, 3);
		if (!jitem.isNull())
			jitems[i] = jitem;
	}

	root["ITEM"] = jitems;

	Json::Value jskills;
	jskills.resize(skills.GetLength());
	for(int i=0; i < skills.GetLength() && (*state == runstate); i++, (*cnt)++) {
		LdbSkill &skill = skills[i];

		Json::Value jskill;

		if (!skill.name.isEmpty())
			jskill["NAME"] = skill.name.getTextUTF8();

		if (!skill.desc.isEmpty())
			jskill["DESC"] = skill.desc.getTextUTF8();

		str.setText(i, 3);
		if (!jskill.isNull())
			jskills[i] = jskill;
	}

	root["SKILL"] = jskills;

	if (*state != runstate)
		return;

	Json::StyledWriter writer;
	std::string strJSON = writer.write(root);

	ofstream os(path);

	os<<strJSON.c_str();

	os.close();
}
*/


void TranslateMain::chkMsg(TString &str) {
	int c2, c1, c;

	TString str2, outstr;

	int i, lastpos, pos, pos2;

	if (str.isEmpty())
		return;

	lastpos = 0;

	c2 = c1 = c = -1;

	for(i=0; i<str.getLength(); i++) {
		c2 = c1;
		c1 = c;
		c = str.charAt(i);

		if (c1 == '.' && _ISALPHA(c)) {
			str2 = str2 + str.substring(lastpos, i) + " ";
			lastpos = i;
		} else if (c == '\n') {
			str2 = str2 + str.substring(lastpos, i) + " ";
			lastpos = i + 1;
		}
	}
	str2 = str2 + str.substring(lastpos);

	lastpos = 0;

	while(true) {
		if (((pos = str2.indexOf('\\', lastpos)) == -1) && 
			((pos = str2.indexOf(0xA5, lastpos)) == -1))
				break;

		if (pos + 1 >= str2.getLength())
			break;

		c = str2.charAt(pos+1);

		switch(c) {
		case '\\':
		case 0xA5:
			outstr = outstr + str2.substring(lastpos, pos) + "\\";
			lastpos = pos + 2;
			break;


		case '^':
		case '$':
		case '.':
		case '|':
		case '!':
		case '>':
		case '<':
		case '_':
			outstr = outstr + str2.substring(lastpos, pos);
			lastpos = pos + 2;
			break;

		case 'C':
		case 'c':
		case 'S':
		case 's':
		case 'N':
		case 'n':
		case 'V':
		case 'v':
			{
				if (	(pos + 2 >= str2.getLength()) ||
					(str2.charAt(pos+2) != '[') ||
					((pos2 = str2.indexOf(']', pos+2)) == -1) )
				{
					outstr = outstr + str2.substring(lastpos, pos) + "\\";
					lastpos = pos + 1;
					break;
				}

				switch(c) {
				case 'N':
				case 'n':
					outstr = outstr + str2.substring(lastpos, pos) + "[#1#]";
					break;

				case 'V':
				case 'v':
					outstr = outstr + str2.substring(lastpos, pos) + "[#2#]";
					break;

				default:
					outstr = outstr + str2.substring(lastpos, pos);
				}

				lastpos = pos2 + 1;
				break;
			}
		default:
			outstr = str2.substring(0, pos+1);
			lastpos = pos+1;
		}
	}

	if (lastpos == 0)
		str = str2;
	else
		str = outstr + str2.substring(lastpos);
}
