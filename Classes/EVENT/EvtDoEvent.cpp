#include "EvtMain.h"

#include "../COMMON/ScreenCommon.h"
#include "../GRAPHIC/GrpPicture.h"
#include "../GRAPHIC/GrpAniMap.h"
#include "../SCENE/MAP/ScrMap.h"
#include "../SCENE/MAP/ScrMapPublic.h"
#include "../SCENE/MAP/ScrMapChipsetTile.h"
#include "../SCENE/SHOP/ScrShop.h"
#include "../COMMON/MultiOutter.h"

int g_evt_do_move_cnt[30000];

bool EvtMain::doEvent(LsdMapEventStack *stack) {
	LsdSystem &sys = pGM->lsd.System;
	LsdState &state = pGM->lsd.State;
	LsdScreen &screen = pGM->lsd.Screen;
	LsdMap &map = pGM->lsd.Map;

	LsdCharacter* c;
	//LsdMapEventData &edatas = stack->data.[stack->data.GetLength()-1];
	LsdMapEventData &edatas = *(stack->data.GetTail());

	GlbEventNode &edata = edatas.event.data[edatas.eventpos];
	
// 	if (m_isBattle)
//	CCLog((_ST("EventID:")+edatas.event_id+"\tStack:"+stack->data.GetLength()+"\tCode:"+edata.code+"("+getString((evtcode)edata.code)+")/tPos:"+(edatas.eventpos-1)).getTextUTF8());
	
	//LogFile::log(_ST("EventID:")+edatas.event_id+"/tStack:"+stack->data.GetLength()+"/tCode:"+edata.code+"("+getString((evtcode)edata.code)+")/tPos:"+(edatas.eventpos-1));

	g_event_run_count++;
	g_evt_do_move_cnt[(int)edata.code]++;
	
	switch(edata.code) {
	case ecChangeClass:
		{
			int heroID = GetPara(edata, 1);
			c = getCharacter(heroID);
			if (c != NULL) {
				int jobID = GetPara(edata, 2);
				bool change_to_lv1 = (GetPara(edata, 3) == 1);
				int skillLearnType = GetPara(edata, 4);//0:not add 1:last delete and add 2:add
				int statType = GetPara(edata, 5);
				c->cjob = jobID;

				LdbCharacterParameter p;
				ArrayList<GlbSkillLearning> sl;
				if (jobID == -1) {
					LdbCharacter &lc = pGM->ldb.dChar[heroID];
					sl = lc.skills;
					p = lc.parameters;

				} else {
					LdbJob &j = pGM->ldb.dJob[jobID-1];
					sl = j.skills;
					p = j.parameters;

				}

				if (change_to_lv1)
					c->clevel = 1;

				if (skillLearnType == 1)
					pGM->lsdbuf.ClsSkill(heroID);

				if (skillLearnType > 0)
					for(int n=0; n<sl.GetLength(); n++)
						if ( sl[n].level <= c->clevel )
							pGM->lsdbuf.SetSkill(heroID, sl[n].skill_id, true);


				switch(statType) {
				case 1:
					{
						c->mhp /= 2;
						c->mmp /= 2;
						c->catk /= 2;
						c->cdef /= 2;
						c->cagi /= 2;
						c->cspt /=2;
						break;
					}
				case 2:
					{
						c->mhp = p.mhp[0];
						c->mmp = p.mmp[0];
						c->catk = p.atk[0];
						c->cdef = p.def[0];
						c->cagi = p.agi[0];
						c->cspt = p.spt[0];
						break;
					}
				case 3:
					{
						c->mhp = 0;
						c->mmp = 0;
						c->catk = 0;
						c->cdef = 0;
						c->cagi = 0;
						c->cspt = 0;
						for(int n=0; n<=c->clevel-1; n++) {
							c->mhp += p.mhp[n];
							c->mmp += p.mmp[n];
							c->catk += p.atk[n];
							c->cdef += p.def[n];
							c->cagi += p.agi[n];
							c->cspt += p.spt[n];
						}
						break;
					}
				}
			}
			break;
		}

	case ecChangeBattleCommands: 
		{
			int hID = GetPara(edata, 0);
			int cmd = GetPara(edata, 2);
			ArrayList<int> &bat = pGM->lsd.Character[hID].battle_commands;

			if (GetPara(edata, 3) == 0) {//Delete
				if (cmd == 0)
					bat.RemoveAll();
				else{
					for(int i=0; i<bat.GetLength(); i++)
						if (bat[i] == cmd) {
							bat.Remove(i);
							break;
						}
				}
			} else {//Insert
				for(int i=0; i<bat.GetLength(); i++) {
					if (bat[i] == cmd)
						break;
					else if (cmd < bat[i]) {
						bat.Add(i, cmd);
						break;
					} else if (i == bat.GetLength()-1) {
						bat.Add(cmd);
						break;
					}
				}
			}
			break;
		}
				
	case ecMessageOptions: 
		{
			sys.msg_visiable	= (GetPara(edata, 0) == 1);
			sys.msg_pos			= GetPara(edata, 1);
			sys.msg_automove	= (GetPara(edata, 2) == 1);
			sys.msg_moveevent	= (GetPara(edata, 3) == 1);
			break;
		}
				
	case ecShowMessage:
		{
			TString s1, s2;
			int p1, p2;
			int result = getRequest(stack, edatas.eventpos);

			if (result == EMR_REQUEST_MSG) {

 				s1 = edata.string + "\n";

				int str_count = 1;

				for(p1 = edatas.eventpos + 1; p1 < edatas.event.data.GetLength() ;p1++)
				{
					GlbEventNode &node = edatas.event.data[p1];

					if (node.code != ecShowMessage_2)
						break;

					s1 = s1 + node.string + "\n";
					str_count++;
				}

				msgLastPos = p1;

				int nextcode = edatas.event.data[p1].code;

				if (nextcode == ecShowChoice) {
					GlbEventNode &header = edatas.event.data[p1];

					int optionlen = 0;
					for(p2 = p1 + 1; p2 < edatas.event.data.GetLength() - 1; p2++) {
						GlbEventNode &node = edatas.event.data[p2];

						if (node.indent < header.indent || node.code == ecShowChoiceEnd)
							break;

						if (node.indent == header.indent && node.code == ecShowChoiceOption) {
							s2 = s2 + " " + node.string + "\n";
							optionlen++;
						}
					}

					bool canCancel = (GetPara(header, 0) != 0);

					if (str_count + optionlen <= 4)
						reqtChoice(s1 + s2, optionlen, canCancel, str_count);
					else
						reqtMessage(s1);
					
				} else if (nextcode == ecInputNumber && str_count < 4) {
					GlbEventNode &header = edatas.event.data[p1];
					reqtNumber(s1, GetPara(header,0), str_count);
				} else {
					reqtMessage(s1);
				}

				return false;

			} else if (result >= 0 || (result == -1 && msgType == EMT_CHOICE)) {
				if (msgType == EMT_ONLY_MSG) {
					edatas.eventpos = msgLastPos-1;
				} else if (msgType == EMT_CHOICE) {
					p1 = msgLastPos;

					GlbEventNode &header = edatas.event.data[p1];
					bool canCancel = (GetPara(header, 0) != 0);

					if (result == -1)
						result = GetPara(header, 0)-1;//cancel
					else
						result -= 1;//choice

					for(p2 = p1 + 1;p2 < edatas.event.data.GetLength(); p2++) {
						GlbEventNode &node = edatas.event.data[p2];

						if (node.indent < header.indent || node.code == ecShowChoiceEnd)
							break;

						if (node.indent == header.indent && node.code == ecShowChoiceOption &&
							node.parameters[0] == result) {
								edatas.eventpos = p2;
								break;
						}
					}
				} else if (msgType == EMT_NUMBER) {
					edatas.eventpos = p1 = msgLastPos;

					GlbEventNode &header = edatas.event.data[p1];
					mst.setVar(GetPara(header,1), result);
				}
			} else {
				return false;
			}

			break;
		}

	case ecShowMessage_2: break;
		
	case ecShowChoice:
		{
			TString s1;
			int p1;
			int result = getRequest(stack, edatas.eventpos);

			if (result == EMR_REQUEST_MSG) {

				int optionlen = 0;
				for(p1 = edatas.eventpos + 1; p1 < edatas.event.data.GetLength() - 1; p1++) {
					GlbEventNode &node = edatas.event.data[p1];

					if (node.indent < edata.indent || node.code == ecShowChoiceEnd)
						break;

					if (node.indent == edata.indent && node.code == ecShowChoiceOption) {
						s1 = s1 + " " + node.string + "\n";
						optionlen++;
					}
				}

				bool canCancel = (GetPara(edata, 0) != 0);

				if (optionlen <= 4)
					reqtChoice(s1, optionlen, canCancel, 0);

				return false;

			} else if (result >= -1) {
				bool canCancel = (GetPara(edata, 0) != 0);

				if (result == -1)
					result = GetPara(edata, 0)-1;//cancel
				else
					result -= 1;//choice

				for(p1 = edatas.eventpos + 1;p1 < edatas.event.data.GetLength(); p1++) {
					GlbEventNode &node = edatas.event.data[p1];

					if (node.indent < edata.indent || node.code == ecShowChoiceEnd)
						break;

					if (node.indent == edata.indent && node.code == ecShowChoiceOption &&
						node.parameters[0] == result) {
							edatas.eventpos = p1;
							break;
					}
				}
			} else {
				return false;
			}

			break;
		}

	case ecInputNumber:
		{
			int result = getRequest(stack, edatas.eventpos);

			if (result == EMR_REQUEST_MSG) {
				reqtNumber("", GetPara(edata,0), 0);
				return false;
			} else if (result >= 0) {
				mst.setVar(GetPara(edata,1), result);
			} else {
				return false;
			}

			break;
		}

	case ecChangeFaceGraphic: /*ChangeFaceGraphic*/
		{
			sys.face_name = edata.string;
			sys.face_index = GetPara(edata, 0);
			sys.face_right = (GetPara(edata, 1)==1);
			sys.face_flip = (GetPara(edata, 2)==1);
			break;
		}

	case ecControlSwitches: /*ControlSwitches*/
		{
			int vfrom = GetPara(edata, 1), vto = GetPara(edata, 2);
			switch(GetPara(edata, 0)) {
			case 0:
				vto = vfrom;
				break;

			case 2:
				vfrom = vto = mst.getVar(vfrom);
				break;
			}

			for(int k=vfrom ; k<=vto ; k++) {
				bool val = mst.getSw(k);
				switch(GetPara(edata, 3)) {
				case 0:val = true;break;
				case 1:val = false;break;
				case 2:val = !val;break;
				}

				mst.setSw(k, val);
			}
			break;
		}

	case ecControlVars: /*ControlVars*/
		{
 			int vfrom = GetPara(edata, 1), vto = GetPara(edata, 2);
			switch(GetPara(edata, 0)) {
			case 0:
				vto = vfrom;
				break;

			case 2:
				vfrom = vto = mst.getVar(vfrom);
				break;
			}
					
			for(int k=vfrom ; k<=vto ; k++) {
				int curVal = mst.getVar(k);
				int inVal = 0;
				switch(GetPara(edata, 4)) {
				case 0:inVal = GetPara(edata, 5);break;
				case 1:inVal = mst.getVar(GetPara(edata, 5));break;
				case 2:inVal = mst.getVar(mst.getVar(GetPara(edata, 5)));break;
				case 3:inVal = (int)(rand()%(GetPara(edata, 6)-GetPara(edata, 5)+1) + GetPara(edata, 5));break;
				case 4:
					{
						if (GetPara(edata, 5)==0)
							inVal = mit.getItemCount(GetPara(edata, 5));
						break;
					}
				case 5:
					{
						LsdCharacter *c = getCharacter(GetPara(edata, 5));
						GlbEquipment equip = mch.getEquip(GetPara(edata, 5));
						if (c != NULL) {
							switch(GetPara(edata, 6)) {
								case 0:inVal = c->clevel;break;
								case 1:inVal = c->cexp;break;
								case 2:inVal = c->chp;break;
								case 3:inVal = c->cmp;break;
								case 4:inVal = c->mhp;break;
								case 5:inVal = c->mmp;break;
								case 6:inVal = c->catk;break;
								case 7:inVal = c->cdef;break;
								case 8:inVal = c->cspt;break;
								case 9:inVal = c->cagi;break;
								case 10:inVal = equip.weapon_id;break;
								case 11:inVal = equip.shield_id;break;
								case 12:inVal = equip.armor_id;break;
								case 13:inVal = equip.helmet_id;break;
								case 14:inVal = equip.accessory_id;break;
							}
						} else {
							inVal = 0;
						}
						break;
					}
				case 6:
					{
						LsdObjectCommon *o = getEvent(edatas.event_id, GetPara(edata, 5));
		
						if (o == NULL)
							break;

						switch(GetPara(edata, 6)) {
							case 0:inVal = o->map_id;break;
							case 1:inVal = o->posX;break;
							case 2:inVal = o->posY;break;
							case 3:{
								switch(o->direction) {
									case 1:/*U*/inVal = 8;break;
									case 2:/*R*/inVal = 6;break;
									case 3:/*D*/inVal = 2;break;
									case 4:/*L*/inVal = 4;break;
								}
								break;
							}
							case 4:inVal = o->screen_pos_x;break;
							case 5:inVal = o->screen_pos_y+8;break;
						}
						break;
					}
				case 7:
					{
						switch(GetPara(edata, 5)) {
							case 0:inVal = state.money;break;
							case 1:inVal = state.timer1_leftframe/FRAME_PER_SEC;break;//TODO
							case 2:inVal = state.timer2_leftframe/FRAME_PER_SEC;break;
							case 3:inVal = state.party.GetLength()-1;break;
							case 4:inVal = sys.save_count;break;
							case 5:inVal = state.count_battle;break;
							case 6:inVal = state.count_victory;break;
							case 7:inVal = state.count_lose;break;
							case 9:inVal = state.count_escape;break;
							case 8:inVal = 0;break;//TODO midi - ticktok cant not make..
						}	
						break;
					}
				}
						
				int outVal = 0;
				switch(GetPara(edata, 3)) {
					case 0:outVal = inVal;break;
					case 1:outVal = curVal + inVal;break;
					case 2:outVal = curVal - inVal;break;
					case 3:outVal = curVal * inVal;break;
					case 4:{
						if (inVal != 0)
							outVal = curVal / inVal;
						else
							outVal = curVal;
						break;
					}
					case 5:{
						if (inVal != 0)
							outVal = curVal % inVal;
						else
							outVal = curVal;
						break;
					}
				}
				mst.setVar(k, outVal);
			}
			break;
		}

	case ecTimerOperation: /*TimerOperation*/
		{
			int id = GetPara(edata, 5);
			switch(GetPara(edata, 0)) {
			case 0:{
				if (id == 0)
					state.timer1_leftframe = (GetPara(edata, 1)*60 + GetPara(edata, 2) + 1)*FRAME_PER_SEC;
				else
					state.timer2_leftframe = (GetPara(edata, 1)*60 + GetPara(edata, 2) + 1)*FRAME_PER_SEC;
				break;
				   }
			case 1:{
				if (id == 0) {
					state.timer1_using = true;
					state.timer1_visible = (GetPara(edata, 3)==1);
					state.timer1_runinbattle = (GetPara(edata, 4)==1);
				} else {
					state.timer2_using = true;
					state.timer2_visible = (GetPara(edata, 3)==1);
					state.timer2_runinbattle = (GetPara(edata, 4)==1);
				}
				break;
				   }
			case 2:{
				if (id == 0) {
					state.timer1_using = false;
				} else {
					state.timer2_using = false;
				}
				break;
				   }
			}
			break;
		}

	case ecChangeGold: /*ChangeGold*/
		{
			int inVal;
			if (GetPara(edata, 1) == 0)
				inVal = GetPara(edata, 2);
			else
				inVal = mst.getVar(GetPara(edata, 2));

			if (GetPara(edata, 0) != 0)
				inVal = -inVal;

			LsdState &stat = pGM->lsd.State;

			inVal += state.money;
			inVal = max(inVal, 0);
			inVal = min(inVal, 999999);
			state.money = inVal;

			break;
		}

	case ecChangeItems: /*ChangeItems*/
		{
			int itemID, inVal;
			if (GetPara(edata, 1) == 0)
				itemID = GetPara(edata, 2);
			else
				itemID = mst.getVar(GetPara(edata, 2));
					
			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));
	
			if (GetPara(edata, 0) != 0)
				inVal = -inVal;

			LsdState &stat = pGM->lsd.State;

			inVal += mit.getItemCount(itemID);
			inVal = max(inVal, 0);
			inVal = min(inVal, 999);
			inVal -= mit.getItemCount(itemID);

			mit.addItemCount(itemID, inVal);

			break;
		}

	case ecChangePartyMembers: /*ChangePartyMembers*/
		{
	
			int inVal;
			if (GetPara(edata, 1) == 0)
				inVal = GetPara(edata, 2);
			else
				inVal = mst.getVar(GetPara(edata, 2));

			LsdState &stat = pGM->lsd.State;

			ArrayList<short> &party = state.party;
			if (GetPara(edata, 0) == 0) {
				int i;
				for(i=0; i<party.GetLength(); i++)
					if (party[i] == inVal)
						break;

				if (i < party.GetLength())
					break;

				party.ChangeMode(ListMode);
				party.Add(inVal);
			} else {
				for(int i=0; i<party.GetLength(); i++)
					if (party[i] == inVal) {
						party.ChangeMode(ListMode);
						party.Remove(i);
						break;
					}
			}
			party.ChangeMode(ArrayMode);
			state.party_length = party.GetLength();

			if (party.GetLength() > 0) {
				LdbCharacter &c = pGM->ldb.dChar[party[0]-1];
				LsdLocationHero &h = pGM->lsd.LocHero;
				h.pic_name	= c.pic_name;
				h.pic_index	= c.pic_id;
				h.need_picname_check = true;
			}

			if (party.GetLength() <= 1)
				ScrMap::chkHeroPic(pGM, false);

			break;
		}

	case ecChangeExp: /*ChangeExp*/
		{
			int inID = 0,inVal;
			switch(GetPara(edata, 0)) {
				case 0:inID=0; break;
				case 1:inID = GetPara(edata, 1);break;
				case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}
					
			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));
					
			if (GetPara(edata, 2) != 0)
				inVal = -inVal;

			ArrayList<TString> msg;

			if (inID == 0) {
				for(int i=0; i<pGM->lsd.State.party.GetLength(); i++) {
					int hid = pGM->lsd.State.party[i];
					mch.add(hid, EXP, inVal);

					mch.getLastLevelUpMsg(msg);
				}
			} else {
				mch.add(inID, EXP, inVal);
				mch.getLastLevelUpMsg(msg);
			}

			if (GetPara(edata, 5) == 1)
				addMsgInStack(edatas, edata, msg); 
			break;
		}
				
	case ecChangeLevel: /*ChangeLevel*/
		{
			int inID = 0,inVal;
			switch(GetPara(edata, 0)) {
				case 0:inID=0; break;
				case 1:inID = GetPara(edata, 1);break;
				case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}
					
			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));
					
			if (GetPara(edata, 2) != 0)
				inVal = -inVal;

			ArrayList<TString> msg;

			if (inID == 0) {
				for(int i=0; i<pGM->lsd.State.party.GetLength(); i++) {
					int hid = pGM->lsd.State.party[i];
					mch.add(hid, LEVEL, inVal);

					mch.getLastLevelUpMsg(msg);
				}
			} else {
				mch.add(inID, LEVEL, inVal);
				mch.getLastLevelUpMsg(msg);
			}
					
			if (GetPara(edata, 5) == 1)
				addMsgInStack(edatas, edata, msg);
					
			break;
		}
				
	case ecChangeParameters: /*ChangeParameters*/
		{

			int inID = 0,inVal;
			switch(GetPara(edata, 0)) {
			case 0:inID = 0; break;
			case 1:inID = GetPara(edata, 1);break;
			case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}

			if (GetPara(edata, 4) == 0)
				inVal = GetPara(edata, 5);
			else
				inVal = mst.getVar(GetPara(edata, 5));

			if (GetPara(edata, 2) != 0)
				inVal = -inVal;

			LsdCharacter *c = getCharacter(inVal);
			if (c != NULL) {
				CMGR_CMD type;
				switch(GetPara(edata, 3)) {
				case 0:type = MAXHP;break;
				case 1:type = MAXMP;break;
				case 2:type = ATK;break;
				case 3:type = DEF;break;
				case 4:type = SPT;break;
				case 5:type = AGI;break;
				}
				mch.add(inID, type, inVal);
			}

			break;
		}

	case ecChangeSkills: /*ChangeSkills*/
		{
			int inID = 0, inVal;
			bool inVal2;

			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));

			inVal2 = ((GetPara(edata, 2)==0)?true:false);

			if (GetPara(edata, 0) == 0) {
				for(int n=0; n<state.party.GetLength(); n++)
					mch.setSkill(state.party[n], inVal, inVal2);

			} else {
				if (GetPara(edata, 0) == 1)
					inID = GetPara(edata, 1);
				else
					inID = mst.getVar(GetPara(edata, 1));

				mch.setSkill(inID, inVal, inVal2);
			}
			break;
		}

	case ecChangeEquipment: /*ChangeEquipment*/
		{
			int inID = 0, inVal, unEquipType;
			bool isEquip;
	
			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));
				
			isEquip = ((GetPara(edata, 2)==0)?true:false);
			unEquipType = GetPara(edata, 5);
					
			if (GetPara(edata, 0) == 0) {
				for(int n=0; n<state.party.GetLength(); n++) {
					int heroID = state.party[n];
					if (isEquip)
						mch.setEquip(heroID, inVal);
					else
						mch.unsetEquip(heroID, unEquipType);
				}

			} else {
				if (GetPara(edata, 0) == 1)
					inID = GetPara(edata, 1);
				else
					inID = mst.getVar(GetPara(edata, 1));

				if (isEquip)
					mch.setEquip(inID, inVal);
				else
					mch.unsetEquip(inID, unEquipType);
			}
		}

	case ecChangeHP: /*ChangeHP*/
		{
			int inID = 0,inVal;
			switch(GetPara(edata, 0)) {
			case 0:inID=0; break;
			case 1:inID = GetPara(edata, 1);break;
			case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}

			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));

			if (GetPara(edata, 2) != 0)
				inVal = -inVal;

			if (GetPara(edata, 0) == 0) {
				for(int n=0; n<state.party.GetLength(); n++) {
					int heroID = state.party[n];
					c = getCharacter(heroID);
					if (c != NULL) {
						int curHP = mch.get(heroID, HP);
						if (GetPara(edata, 5) == 0 && curHP + inVal < 1) {
							inVal = 1 - curHP;
						}
						mch.add(heroID, HP, inVal);
					}
				}
			} else {
				if (GetPara(edata, 0) == 1)
					inID = GetPara(edata, 1);
				else
					inID = mst.getVar(GetPara(edata, 1));

				c = getCharacter(inID);
				if (c != NULL) {
					int curHP = mch.get(inID, HP);
					if (GetPara(edata, 5) == 0 && curHP + inVal < 1) {
						inVal = 1 - curHP;
					}
					mch.add(inID, HP, inVal);
				}
			}

			break;
		}

	case ecChangeMP: /*ChangeMP*/
		{
			int inID = 0,inVal;
			switch(GetPara(edata, 0)) {
				case 0:inID=0; break;
				case 1:inID = GetPara(edata, 1);break;
				case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}
					
			if (GetPara(edata, 3) == 0)
				inVal = GetPara(edata, 4);
			else
				inVal = mst.getVar(GetPara(edata, 4));
					
			if (GetPara(edata, 2) != 0)
				inVal = -inVal;

			mch.add(inID, MP, inVal);
					
			break;
		}

	case ecChangeCondition: /*ChangeCondition*/
		{
			int inID = 0;
			switch(GetPara(edata, 0)) {
				case 0:inID=0; break;
				case 1:inID = GetPara(edata, 1);break;
				case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}
			mch.setState(inID, GetPara(edata, 3), (GetPara(edata, 2) == 0?true:false));
			break;
		}

	case ecFullHeal: /*FullHeal*/
		{
			int inID = 0;
			switch(GetPara(edata, 0)) {
				case 0:inID=0; break;
				case 1:inID = GetPara(edata, 1);break;
				case 2:inID = mst.getVar(GetPara(edata, 1));break;
			}
					
			mch.add(inID, HP, 999999);
			mch.add(inID, MP, 999999);
			mch.clearState(inID);
					
			break;
		}
				
	case ecSimulatedAttack: /*SimulatedAttack*/
		{
			//TODO
			break;
		}

	case ecChangeHeroName: /*ChangeHeroName*/
		{
			LsdCharacter *c = getCharacter(GetPara(edata, 0));
			if (c != NULL)
				c->name = edata.string;
			break;
		}

	case ecChangeHeroTitle: /*ChangeHeroTitle*/
		{
			c = getCharacter(GetPara(edata, 0));
			if (c != NULL)
				c->title = edata.string;
			break;
		}

	case ecChangeSpriteAssociation: /*ChangeSpriteAssociation*/
		{
			LsdCharacter &lc = pGM->lsd.Character[GetPara(edata, 0)-1];
			edata.string;
			lc.pic_name	= edata.string;
			lc.pic_id		= GetPara(edata, 1);
			lc.pic_default_trans = ((GetPara(edata, 2)==1)?3:0);

			ScrMap::chkHeroPic(pGM, false);

			break;
		}

	case ecChangeActorFace: /*ChangeActorFace*/
		{
			c = getCharacter(GetPara(edata, 0));
			if (c != NULL) {
				c->face_name = edata.string;
				c->face_index = GetPara(edata, 1);
				c->pic_default_trans = ((GetPara(edata, 2)==1)?3:0);
			}
			break;
		}

	case ecChangeVehicleGraphic: /*ChangeVehicleGraphic*/
		{
			int n=GetPara(edata, 0);

			if (n<0 || n>=3)
				break;

			LsdObjectCommon *o = &(pGM->lsd.LocRide[n]);

			o->pic_name = edata.string;
			o->pic_index = GetPara(edata, 1);
			break;
		}

	case ecChangeSystemBGM: /*ChangeSystemBGM*/
		{
			GlbMusic mus;
			mus.name	= edata.string;
			mus.fadein	= GetPara(edata, 1);
			mus.volume	= GetPara(edata, 2);
			mus.tempo	= GetPara(edata, 3);
			mus.balance	= GetPara(edata, 4);

			switch(GetPara(edata, 0)) {
				case 0: sys.music_battle		= mus;	break;
				case 1: sys.music_battle_end	= mus;	break;
				case 2: sys.music_inn			= mus;	break;
				case 3: sys.music_boat			= mus;	break;
				case 4: sys.music_ship			= mus;	break;
				case 5: sys.music_airship		= mus;	break;
				case 6: sys.music_gameover		= mus;	break;
			}
			break;
		}

	case ecChangeSystemSFX: /*ChangeSystemSFX*/
		{
			GlbSound snd;
			snd.name	= edata.string;
			snd.volume	= GetPara(edata, 1);
			snd.tempo	= GetPara(edata, 2);
			snd.balance	= GetPara(edata, 3);

			switch(GetPara(edata, 0)) {
			case 0: sys.se_cursor			= snd;	break;
			case 1: sys.se_decision			= snd;	break;
			case 2: sys.se_cancel			= snd;	break;
			case 3: sys.se_buzzer			= snd;	break;
			case 4: sys.se_battle			= snd;	break;
			case 5: sys.se_escape			= snd;	break;
			case 6: sys.se_enemy_attack		= snd;	break;
			case 7: sys.se_enemy_damaged	= snd;	break;
			case 8: sys.se_actor_damaged	= snd;	break;
			case 9: sys.se_dodge			= snd;	break;
			case 10:sys.se_enemy_death		= snd;	break;
			case 11:sys.se_item				= snd;	break;
			}
			break;
		}

	case ecChangeSystemGraphics:/*ChangeSystemGraphics*/
		{
			//TODO
			//GetPara(edata, 0) 확대 나란히
			//GetPara(edata, 1) 고딕 명조
			changeSystem(edata.string);
			break;
		}

	case ecChangeScreenTransitions: /*ChangeScreenTransitions*/
		{
			int inVal = GetPara(edata, 1);
			if (GetPara(edata, 0) == 0)
				sys.transition_out = inVal;
			else
				sys.transition_in = inVal;
			break;
		}

	case ecEnemyEncounter: /*EnemyEncounter*/
		{
			int groupID = GetPara(edata, 1);
			if (GetPara(edata, 0)==1)
				groupID = mst.getVar(groupID);

			int encounterType = GetPara(edata, 6); 

			int battleEscapeType = GetPara(edata, 3);
			int battleLoseType = GetPara(edata, 4);

			bool firstAttack = (GetPara(edata, 5) != 0);

			ScrBattleBgInfo bg_info;
			bg_info.use_ab = false;
			bg_info.bg = "";

			int terrain_id;
			switch(GetPara(edata, 2)) {
			case 1:	
				{
					//GetPara(edata,7) == 0LookDown 1LookLeft
					bg_info.use_ab = false;
					bg_info.bg = edata.string;
					break;
				}
			default:
				{
					if (GetPara(edata, 2) == 0) {
						LsdObjectCommon & objhero = pGM->lsd.LocHero;
						terrain_id = ScrMapPublic::getTerrainID(&pGM->ldb, &pGM->lsd, pMapData, objhero.posX, objhero.posY);
					} else {
						terrain_id = GetPara(edata,8);

					}

					if (terrain_id > 0) {
						LdbTerrain &terr = pGM->ldb.dTerr[terrain_id-1];
						bg_info.set(terr);
					}

				}
			}

			pGM->slm->setSettingScrBattle(encounterType, groupID, bg_info, battleEscapeType!=0, battleLoseType!=0, firstAttack);
			pGM->slm->setScreen(scr_battle);
			return false;
		}

	case ecOpenShop: /*OpenShop*/
		{
			ArrayList<int> items;

			for(int i=4; i<edata.parameters.GetLength(); i++)
				items.Add(edata.parameters[i]);

			ScrShop::g_bought_something = true;

			pGM->slm->setSettingScrShop(GetPara(edata, 0), GetPara(edata, 2), items);
			pGM->slm->setScreen(scr_shop);
			edatas.eventpos++;
			return false;
		}

	case ecTransaction:
		{
			int n;
			if (!ScrShop::g_bought_something) {
				for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
					GlbEventNode &node = edatas.event.data[n];

					if (node.indent < edata.indent)
						break;

					if (node.indent == edata.indent && node.code == ecNoTransaction) {
						edatas.eventpos = n;
						break;
					}
				}
			}


			break;
		}

	case ecNoTransaction:
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
				GlbEventNode &node = edatas.event.data[n];

				if (node.indent < edata.indent)
					break;

				if (node.indent == edata.indent && node.code == ecEndShop) {
					edatas.eventpos = n;
					break;
				}
			}

			break;
		}

	case ecShowInn: /*ShowInn*/
		{
			isInnUsed = false;

			doInnEvent(GetPara(edata, 0), GetPara(edata, 1));
			break;
		}
	case ecStay:
		{
			int n;
			if (!isInnUsed) {
				for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
					GlbEventNode &node = edatas.event.data[n];

					if (node.indent < edata.indent)
						break;

					if (node.indent == edata.indent && node.code == ecNoStay) {
						edatas.eventpos = n;
						break;
					}
				}
			}


			break;
		}

	case ecNoStay:
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
				GlbEventNode &node = edatas.event.data[n];

				if (node.indent < edata.indent)
					break;

				if (node.indent == edata.indent && node.code == ecEndInn) {
					edatas.eventpos = n;
					break;
				}
			}

			break;
		}

	case ecEnterHeroName: /*EnterHeroName*/
		{
			c = getCharacter(GetPara(edata, 0));
			if (c != NULL) {
				bool show_lastname = (GetPara(edata, 2) == 1);

				if (pGM->slm->isChanging())
					return false;

				edatas.eventpos++;

				if (!show_lastname)
					c->name = "";

				pGM->slm->setSettingScrNameInput(&(c->name));
				
				pGM->slm->setScreen(scr_nameinput);

				return false;
			}
			break;
		}

	case ecTeleport: /*Teleport*/
		{

			if (pGM->slm->isChanging())
				return false;

			int mapid = GetPara(edata, 0);
			int posx = GetPara(edata, 1);
			int posy = GetPara(edata, 2);
			int chardirect = GetPara(edata, 3);//0 : direct notfix

			edatas.eventpos++;

			if (pGM->curMapId == mapid) {
				pGM->slm->setSettingScrMap(mapid, posx, posy, chardirect, false);
			} else {
				if (&(pGM->lsd.EventMap) != stack) {
					pGM->lsd.EventMap = *stack;
					stack->data.RemoveAll();
				}

				pGM->slm->setSettingScrMap(mapid, posx, posy, chardirect, true);
			}
			pGM->slm->setScreen(scr_map);
			return false;
		}

	case ecMemorizeLocation: /*MemorizeLocation*/
		{
			mst.setVar(GetPara(edata, 0), pGM->curMapId);
			LsdObjectCommon *obj;
			switch(pGM->lsd.LocHero.ride_type) {
				case 0:obj = &(pGM->lsd.LocHero);break;
				case 1:obj = &(pGM->lsd.LocRide[0]);break;
				case 2:obj = &(pGM->lsd.LocRide[1]);break;
				case 3:obj = &(pGM->lsd.LocRide[2]);break;
			}
			mst.setVar(GetPara(edata, 1), obj->posX);
			mst.setVar(GetPara(edata, 2), obj->posY);
			break;
		}

	case ecRecallToLocation: /*RecallToLocation*/
		{
			if (pGM->slm->isChanging())
				return false;

			LsdObjectCommon *obj;
			LsdObjectCommon *objhero = &(pGM->lsd.LocHero);
			switch(pGM->lsd.LocHero.ride_type) {
				case 0:obj = objhero;break;
				case 1:obj = &(pGM->lsd.LocRide[0]);break;
				case 2:obj = &(pGM->lsd.LocRide[1]);break;
				case 3:obj = &(pGM->lsd.LocRide[2]);break;
			}

			int mapid = mst.getVar(GetPara(edata, 0));
			int posx = mst.getVar(GetPara(edata, 1));
			int posy = mst.getVar(GetPara(edata, 2));

			edatas.eventpos++;

			if (pGM->curMapId != mapid) {
				if (&(pGM->lsd.EventMap) != stack) {
					pGM->lsd.EventMap = *stack;
					stack->data.RemoveAll();
				}

				pGM->slm->setSettingScrMap(mapid, posx, posy, 0, true);
			} else {
				pGM->slm->setSettingScrMap(mapid, posx, posy, 0, false);
			}
			pGM->slm->setScreen(scr_map);
			return false;
		}

	case ecEnterExitVehicle: /*EnterExitVehicle*/
		{
			ScrMap *map = (ScrMap *)pGM->slm->getCurLayer();

			map->doRide();
			break;
		}
				
	case ecSetVehicleLocation: /*SetVehicleLocation*/
		{
			int n = mst.getVar(GetPara(edata, 0));

			if (n<0 || n>=3)
				break;

			int mapID = GetPara(edata, 2);
			int mapX = GetPara(edata, 3);
			int mapY = GetPara(edata, 4);
			int direction = GetPara(edata, 5);
			if (GetPara(edata, 1) == 1) {
				mapID = mst.getVar(mapID);
				mapX = mst.getVar(mapX);
				mapY = mst.getVar(mapY);
			}

			LsdObjectCommon *o = &(pGM->lsd.LocRide[n]);
			o->map_id = mapID;
			o->posX = mapX;
			o->posY = mapY;
			if (direction > 0)
				o->direction = direction;
			o->isUsed = (mapID == pGM->curMapId);
			break; 
		}

	case ecChangeEventLocation: /*ChangeEventLocation*/
		{
			LsdObjectCommon *o = getEvent(edatas.event_id, GetPara(edata, 0));

			if (o == NULL)
				break;

			int mapX = GetPara(edata, 2);
			int mapY = GetPara(edata, 3);
			int direction = GetPara(edata, 4);
			if (GetPara(edata, 1) == 1) {
				mapX = mst.getVar(mapX);
				mapY = mst.getVar(mapY);
			}

			o->posX = mapX;
			o->posY = mapY;
			if (direction > 0)
				o->direction = direction;

			break;
		}

	case ecTradeEventLocations: /*TradeEventLocations*/
		{
			LsdObjectCommon *o1 = getEvent(edatas.event_id, GetPara(edata, 0));
			LsdObjectCommon *o2 = getEvent(edatas.event_id, GetPara(edata, 1));

			if (o1 == NULL || o2 == NULL)
				break;
				
			swap(o1->posX,o2->posX);
			swap(o1->posY,o2->posY);
			break;
		}

	case ecStoreTerrainID: /*StoreTerrainID*/
		{
			int valID = GetPara(edata, 3);
			int mapX = GetPara(edata, 1);
			int mapY = GetPara(edata, 2);

			if (GetPara(edata, 0) == 1) {
				mapX = mst.getVar(mapX);
				mapY = mst.getVar(mapY);
			}

			int val = ScrMapPublic::getTerrainID(&pGM->ldb, &pGM->lsd, pMapData, mapX, mapY);//TODO

			mst.setVar(valID, val);
			break;
		}

	case ecStoreEventID: /*StoreEventID*/
		{
			int mapX = GetPara(edata, 1);
			int mapY = GetPara(edata, 2);
			if (GetPara(edata, 0) == 1) {
				mapX = mst.getVar(mapX);
				mapY = mst.getVar(mapY);
			}

			int val = 0;
			ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
			for(int n=events.GetLength()-1; n>=0; n--) {
				if ( mapX == events[n].posX &&
					mapY == events[n].posY ) {
						val = getEventID(n);
						break;
				}
			}

			mst.setVar(GetPara(edata, 3), val );
			break;
		}
	case ecEraseScreen: /*EraseScreen*/
		{
			int inVal = GetPara(edata, 0);
			if (inVal == -1)
				inVal = sys.transition_out;

			if (!pGM->slm->Hide(inVal) || !pGM->slm->isHided())
				return false;
			
			break;
		}

	case ecShowScreen: /*ShowScreen*/
		{
			int inVal = GetPara(edata, 0);
			if (inVal == -1)
				inVal = sys.transition_in;

			if (!pGM->slm->Show(inVal) || pGM->slm->isHided())
				return false;

			break;
	}
	case ecTintScreen: /*TintScreen*/
		{
			
			screen.scr_fin_r = GetPara(edata, 0);
			screen.scr_fin_g = GetPara(edata, 1);
			screen.scr_fin_b = GetPara(edata, 2);
			screen.scr_fin_gr= GetPara(edata, 3);
			if (GetPara(edata,4) == 0)
				screen.scr_timeleft = 1;
			else
				screen.scr_timeleft = GetPara(edata, 4) * FRAME_PER_SEC / 10;

			if (GetPara(edata, 5) == 1) {
				stack->wait_left = GetPara(edata, 4) * FRAME_PER_SEC / 10;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}
			break;
		}
	case ecFlashScreen: /*FlashScreen*/
		{
			int type = 0;

			screen.flash_continuous = GetPara(edata, 6);
			screen.flash_fin_r = GetPara(edata, 0);
			screen.flash_fin_g = GetPara(edata, 1);
			screen.flash_fin_b = GetPara(edata, 2);
			screen.flash_cur_lev = GetPara(edata, 3);
			if (GetPara(edata,4) == 0)
				screen.flash_frame_left = 1;
			else
				screen.flash_frame_left = GetPara(edata, 4) * FRAME_PER_SEC / 10;

			if (GetPara(edata, 5) == 1) {
				stack->wait_left = GetPara(edata, 4) * FRAME_PER_SEC / 10;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}
			break;
		}
	case ecShakeScreen: /*ShakeScreen*/
		{
			screen.shake_power = GetPara(edata, 0);
			screen.shake_speed = GetPara(edata, 1);
			screen.shake_timeleft = GetPara(edata, 2);
			screen.shake_continuous = GetPara(edata,4);

			if (GetPara(edata, 3) == 1) {
				stack->wait_left = GetPara(edata, 2) * FRAME_PER_SEC / 10;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}
			break;
		}
	case ecPanScreen: /*PanScreen*/
		{
			switch(GetPara(edata, 0)) {
			case 0:
				{
					map.is_pan_lock = true;
					break;
				}

			case 1:
				{
					map.is_pan_lock = false;
					break;
				}

			case 2:
				{
					int direct = GetPara(edata, 1);
					int count = GetPara(edata, 2);
					int speed = GetPara(edata, 3);
					screen.pan_move_speed = speed;
					screen.pan_move_x += count*((direct==1)?1:((direct==3)?-1:0))*TILE_W;
					screen.pan_move_y += count*((direct==0)?-1:((direct==2)?1:0))*TILE_W;


					if (GetPara(edata, 4) == 1) {
						stack->wait_left = count * 300 / DELAY_MOVE[speed];
						edatas.eventpos++;
						if (msgCalledEvtStack == stack)
							isMsgEndChecking = true;
						return false;//TODO CHECK
					}
					
					break;
				}

			case 3:
				{
					int speed = GetPara(edata, 3);
					screen.pan_move_speed = speed;
					screen.pan_move_x = -screen.pan_moved_x;
					screen.pan_move_y = -screen.pan_moved_y;

					int count = max(abs((int)(screen.pan_moved_x)/TILE_W), abs((int)(screen.pan_moved_y)/TILE_W));

					if (GetPara(edata, 4) == 1) {
						stack->wait_left = count * 300 / DELAY_MOVE[speed];
						edatas.eventpos++;
						if (msgCalledEvtStack == stack)
							isMsgEndChecking = true;
						return false;//TODO CHECK
					}
					
					break;
				}

			}

			break;
		}
	case ecWeatherEffects: /*WeatherEffects*/
		{
			ScrMap *map = (ScrMap *)pGM->slm->getCurLayer();

			map->setEffect(GetPara(edata, 0), GetPara(edata, 1));
			break;
		}

	case ecShowPicture: /**/
		{

			int valX, valY;
			if (GetPara(edata, 1) == 0) {
				valX = GetPara(edata, 2);
				valY = GetPara(edata, 3);
			} else {
				valX = mst.getVar(GetPara(edata, 2));
				valY = mst.getVar(GetPara(edata, 3));
			}
			int UpAlpha = GetPara(edata, 6);
			int DownAlpha;
			if (edata.parameters.GetLength() >= 15)
				DownAlpha = GetPara(edata, 14);
			else
				DownAlpha = UpAlpha;

			GrpPicture::setPic(
				pGM,
				GetPara(edata, 0), 
				valX, valY, edata.string ,
				GetPara(edata, 4) == 1,
				GetPara(edata, 7) == 1,
				GetPara(edata, 5),
				UpAlpha,DownAlpha,
				GetPara(edata, 8),
				GetPara(edata, 9),
				GetPara(edata, 10),
				GetPara(edata, 11),
				GetPara(edata, 12),
				GetPara(edata, 13)				
			);

			break;
		}
	case ecMovePicture: /**/
		{
			int valX, valY;
			if (GetPara(edata, 1) == 0) {
				valX = GetPara(edata, 2);
				valY = GetPara(edata, 3);
			} else {
				valX = mst.getVar(GetPara(edata, 2));
				valY = mst.getVar(GetPara(edata, 3));
			}

			int UpAlpha = GetPara(edata, 6);
			int DownAlpha;
			if (edata.parameters.GetLength() >= 17)
				DownAlpha = GetPara(edata, 16);
			else
				DownAlpha = UpAlpha;

			GrpPicture::movePic(
				pGM,
				GetPara(edata, 0), 
				valX, valY, 
				GetPara(edata, 5),
				UpAlpha,DownAlpha,
				GetPara(edata, 8),
				GetPara(edata, 9),
				GetPara(edata, 10),
				GetPara(edata, 11),
				GetPara(edata, 12),
				GetPara(edata, 13),
				GetPara(edata, 14)		
			);

			if (GetPara(edata, 15)==1) {
				stack->wait_left = GetPara(edata, 14) * FRAME_PER_SEC / 10;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}
			break;
		}
	case ecErasePicture: /*ErasePicture*/
		{
			GrpPicture::removePic(
				pGM,
				GetPara(edata, 0)	
			);
			break;
		}
	case ecShowBattleAnimation: /*ShowBattleAnimation*/
		{
			int targetid = GetPara(edata, 1);
			
			screen.ani_id = GetPara(edata, 0);
			screen.ani_target = (targetid == 10005)?edatas.event_id:targetid;
			screen.ani_allscreen = (GetPara(edata, 3) == 1);
			screen.ani_frameleft = 0;
			screen.ani_curframe = 0;

			if (GetPara(edata, 2) == 1) {
				stack->wait_left = GrpAniOne::getAnimationFrame(pGM, screen.ani_id) - 4;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}

			break;
		}
	case ecSpriteTransparency: /*SpriteTransparency*/
		{
			pGM->lsd.LocHero.invisible = (GetPara(edata, 0) == 0);
			break;
		}
	case ecFlashSprite: /*FlashSprite*/
		{
			LsdObjectCommon *o = getEvent(edatas.event_id, GetPara(edata, 0));
			if (o == NULL)
				break;

			o->flash_continuous = GetPara(edata, 6);
			o->flash_fin_r = GetPara(edata, 1);
			o->flash_fin_g = GetPara(edata, 2);
			o->flash_fin_b = GetPara(edata, 3);
			o->flash_cur_lev = GetPara(edata, 4);
			if (GetPara(edata,5) == 0)
				o->flash_frame_left = 1;
			else
				o->flash_frame_left = GetPara(edata, 5) * FRAME_PER_SEC / 10;
			break;
		}
	case ecMoveEvent: /*MoveEvent*/
		{
			LsdObjectCommon *o = getEvent(edatas.event_id, GetPara(edata, 0));
			if (o == NULL)
				break;

			GlbMoveRoute &gMR = o->move_route;
				
			gMR.move_commands_count = edata.parameters.GetLength()-4;
			gMR.move_commands.ChangeMode(ArrayMode);
			gMR.move_commands.SetLength(gMR.move_commands_count);
			for(int n=0; n<gMR.move_commands_count; n++)
				gMR.move_commands[n] = GetPara(edata, n+4);
			gMR.move_freq = GetPara(edata, 1);
			gMR.repeat = (GetPara(edata, 2) != 0);
			gMR.skippable = (GetPara(edata, 3) != 0);

			o->move_route_custom = true;
			o->move_route_original_index = o->move_route_index;
			o->move_route_index = 0;
			o->wait_cur_frame = 99999;
			break;
		}
	case ecProceedWithMovement: /*ProceedWithMovement*/
		{
			bool pause = false;

			if (pGM->lsd.LocHero.move_route_custom && 
				!pGM->lsd.LocHero.move_route.repeat) {
				pause = true;
			}
			
			for(int i=0; i<3; i++)
				if (pGM->lsd.LocRide[i].isUsed && 
					pGM->lsd.LocRide[i].move_route_custom &&
					!pGM->lsd.LocRide[i].move_route.repeat) {
					pause = true;
					break;
				}

			ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
			for(int i=0; i<events.GetLength(); i++) {
				int pageid = ScrMapPublic::getEventPage(pGM, pMapData->Events[i]);
				if (pageid >= 0 && 
					events[i].move_route_custom && 
					!events[i].move_route.repeat) {
					pause = true;
					break;
				}
			}

			if (pause) {
				endMsg(stack);//TODO
				return false;
			}
			break;
		}	
	case ecHaltAllMovement: /*HaltAllMovement*/
		{
			pGM->lsd.LocHero.move_route_custom = false;

			for(int i=0; i<3; i++)
				pGM->lsd.LocRide[i].move_route_custom = false;

			ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
			for(int i=0; i<events.GetLength(); i++)
				events[i].move_route_custom = false;
			
			break;
		}
	case ecWait: /*Wait*/
		{
			if (GetPara(edata, 1) == 1) {
				ctlMain *ctl = pGM->ctl;

				if (stack->m_key_enter_last != ctl->getKey(rkc_enter)) {
					stack->m_key_enter_last = ctl->getKey(rkc_enter);
					if (stack->m_key_enter_last > 0)
						break;
				}
				return false;
			} else {
				stack->wait_left = GetPara(edata, 0)*FRAME_PER_SEC/10;
				edatas.eventpos++;
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}
			break;
		}
				
	case ecPlayBGM: /*PlayBGM*/
		{
			GlbMusic gM;
			if (edata.string.isEmpty() || edata.string == "(OFF)") {
				pGM->snd.StopMusic();
			} else {
				gM.name = edata.string;
				gM.fadein = GetPara(edata, 0);//CHECK it seems to millisec
				gM.volume = GetPara(edata, 1);
				gM.tempo = GetPara(edata, 2);
				gM.balance = GetPara(edata, 3);
				pGM->snd.PlayMusic(gM);
			}
			break;
		}
	case ecFadeOutBGM: /*FadeOutBGM*/
		{
			pGM->snd.FadeoutMusic(GetPara(edata, 0));
			break;
		}
	case ecMemorizeBGM: /*MemorizeBGM*/
		{
			pGM->lsd.System.music_saved = pGM->lsd.System.music_current;
			break;
		}
	case ecPlayMemorizedBGM: /*PlayMemorizedBGM*/
		{
			pGM->snd.PlayMusic(pGM->lsd.System.music_saved);
			break;
		}
	case ecPlaySound: /*PlaySound*/
		{
			GlbSound gS;
			gS.name = edata.string;
			gS.volume = GetPara(edata, 0);
			gS.tempo = GetPara(edata, 1);
			gS.balance = GetPara(edata, 2);

			if (edata.string == "(OFF)")
				pGM->snd.StopSound();
			else
				pGM->snd.PlaySound(gS);
			break;
		}
	case ecPlayMovie: /*PlayMovie*/
		{
			int sX = GetPara(edata, 1);
			int sY = GetPara(edata, 2);
			if (GetPara(edata, 0) == 1) {
				sX = mst.getVar(sX);
				sY = mst.getVar(sY);
			}
			
			MultiOutter::openMovie(edata.string);
			//SCRMovie.Show(edata.string);
			break;
		}
	case ecKeyInputProc: /*KeyInputProc*/
		{
			int resultVarID = GetPara(edata, 0);
			bool isWait = (GetPara(edata, 1) == 1);

			int keyLenVarID = GetPara(edata, 7);
			bool isKeyLen = (GetPara(edata, 8) == 1);
			

			if (usingMsg()) {
				if (msgCalledEvtStack == stack)
					isMsgEndChecking = true;
				return false;//TODO CHECK
			}

			ctlMain *ctl = pGM->ctl;

// 			TString s1 = "";
// 			for(int i=0; i<edata.parameters.GetLength(); i++)
// 				s1 = s1 + "[" + i + "]" + edata.parameters[i] + " ";
// 			CCLOG("%d %s", edatas.eventpos, s1.getTextUTF8());

			int inkey = 0;
			if (pGM->getVersion() == RPG2000) {
				if (stack->m_key_arrow_last != ctl->getKey(rkc_arrow) || !isWait) {
					stack->m_key_arrow_last = ctl->getKey(rkc_arrow);

					if ((GetPara(edata, 2) == 1 || GetPara(edata, 6) == 1) &&
						stack->m_key_arrow_last == rkca_down)
						inkey = 1;
					if ((GetPara(edata, 2) == 1 || GetPara(edata, 7) == 1) && 
						stack->m_key_arrow_last == rkca_left)
						inkey = 2;
					if ((GetPara(edata, 2) == 1 || GetPara(edata, 8) == 1) && 
						stack->m_key_arrow_last == rkca_right)
						inkey = 3;
					if ((GetPara(edata, 2) == 1 || GetPara(edata, 9) == 1) && 
						stack->m_key_arrow_last == rkca_up)
						inkey = 4;
				}

				if (stack->m_key_enter_last != ctl->getKey(rkc_enter) || !isWait) {
					stack->m_key_enter_last = ctl->getKey(rkc_enter);
					if (GetPara(edata, 3) == 1 && stack->m_key_enter_last > 0)
						inkey = 5;
				}
				if (stack->m_key_esc_last != ctl->getKey(rkc_esc) || !isWait) {
					stack->m_key_esc_last = ctl->getKey(rkc_esc);
					if (GetPara(edata, 4) == 1 && stack->m_key_esc_last > 0)
						inkey = 6;
				}
				if (stack->m_key_special_last != ctl->getKey(rkc_shift) || !isWait) {
					stack->m_key_special_last = ctl->getKey(rkc_shift);
					if (GetPara(edata, 5) == 1 && stack->m_key_special_last > 0)
						inkey = 7;
				}

			} else {
				if (edata.parameters.GetLength() == 14) {
					/*
					0 입력번수
					1 키대기여부

					2는 무엇인가..

					8 키 길이 입력여부
					7 키 길이를 받을 변수

					3 결정키
					4 취소키
					9 쉬프트
					5 숫자
					6 기타

					10 아래
					11 왼쪽
					12 오른쪽
					13 위
					*/
					if (stack->m_key_arrow_last != ctl->getKey(rkc_arrow) || !isWait) {
						stack->m_key_arrow_last = ctl->getKey(rkc_arrow);

						if ((GetPara(edata, 10) == 1) &&
							stack->m_key_arrow_last == rkca_down)
							inkey = 1;
						if ((GetPara(edata, 11) == 1) && 
							stack->m_key_arrow_last == rkca_left)
							inkey = 2;
						if ((GetPara(edata, 12) == 1) && 
							stack->m_key_arrow_last == rkca_right)
							inkey = 3;
						if ((GetPara(edata, 13) == 1) && 
							stack->m_key_arrow_last == rkca_up)
							inkey = 4;
					}

					if (stack->m_key_enter_last != ctl->getKey(rkc_enter) || !isWait) {
						stack->m_key_enter_last = ctl->getKey(rkc_enter);
						if (GetPara(edata, 3) == 1 && stack->m_key_enter_last > 0)
							inkey = 5;
					}
					if (stack->m_key_esc_last != ctl->getKey(rkc_esc) || !isWait) {
						stack->m_key_esc_last = ctl->getKey(rkc_esc);
						if (GetPara(edata, 4) == 1 && stack->m_key_esc_last > 0)
							inkey = 6;
					}
					if (stack->m_key_num_last != ctl->getKey(rkc_num) || !isWait) {
						stack->m_key_num_last = ctl->getKey(rkc_num);
						if (GetPara(edata, 5) == 1 && stack->m_key_num_last > 0)
							inkey = stack->m_key_num_last;
					}
					if (stack->m_key_special_last != ctl->getKey(rkc_special) || !isWait) {
						stack->m_key_special_last = ctl->getKey(rkc_special);
						if (GetPara(edata, 6) == 1 && stack->m_key_special_last > 0)
							inkey = stack->m_key_special_last;
					}
					if (stack->m_key_special_last != ctl->getKey(rkc_shift) || !isWait) {
						stack->m_key_special_last = ctl->getKey(rkc_shift);
						if (GetPara(edata, 9) == 1 && stack->m_key_special_last > 0)
							inkey = 7;
					}
				} else {
					if (stack->m_key_arrow_last != ctl->getKey(rkc_arrow) || !isWait) {
						stack->m_key_arrow_last = ctl->getKey(rkc_arrow);

						if (GetPara(edata, 2) == 1 && stack->m_key_arrow_last > 0) {
							switch(stack->m_key_arrow_last) {
							case rkca_up:		inkey = 4; break;
							case rkca_left:		inkey = 2; break;
							case rkca_down:		inkey = 1; break;
							case rkca_right:	inkey = 3; break;
							}
						}						
					}
					if (stack->m_key_enter_last != ctl->getKey(rkc_enter) || !isWait) {
						stack->m_key_enter_last = ctl->getKey(rkc_enter);
						if (GetPara(edata, 3) == 1 && stack->m_key_enter_last > 0)
							inkey = 5;
					}
					if (stack->m_key_esc_last != ctl->getKey(rkc_esc) || !isWait) {
						stack->m_key_esc_last = ctl->getKey(rkc_esc);
						if (GetPara(edata, 4) == 1 && stack->m_key_esc_last > 0)
							inkey = 6;
					}
					if (stack->m_key_num_last != ctl->getKey(rkc_num) || !isWait) {
						stack->m_key_num_last = ctl->getKey(rkc_num);
						if (GetPara(edata, 5) == 1 && stack->m_key_num_last > 0)
							inkey = stack->m_key_num_last;
					}
					if (stack->m_key_special_last != ctl->getKey(rkc_special) || !isWait) {
						stack->m_key_special_last = ctl->getKey(rkc_special);
						if (GetPara(edata, 6) == 1 && stack->m_key_special_last > 0)
							inkey = stack->m_key_special_last;
					}
				}
			}

			if (isKeyLen == 1)
				stack->m_wait_key++;

			if (inkey == 0) {
				if (isWait) {
					return false;
				} else {
					mst.setVar(resultVarID, 0);
					//Log.e("r2k", "ID:" + ID + " invalue" + 0);
				}
			} else {
				//Log.e("r2k", "ID:" + ID + " invalue" + inkey);
				mst.setVar(resultVarID, inkey);
				if (pGM->getVersion() == RPG2003) {
					//Only RPG 2003
					if (isWait && isKeyLen)
						mst.setVar(keyLenVarID, (int)(stack->m_wait_key * 10 / FRAME_PER_SEC));
					stack->m_wait_key = 0;
				}
			}
					
			break;
		}
	case ecChangeMapTileset: /*ChangeMapTileset*/
		{
			pGM->lsd.Map.chipset_id = GetPara(edata, 0);
			break;
		}
	case ecChangePBG: /*ChangePBG*/
		{
			LsdMap &map = pGM->lsd.Map;
			map.bg_name = edata.string;
			map.bg_vscroll = (GetPara(edata, 2) == 1);
			map.bg_hscroll = (GetPara(edata, 4) == 1);
			map.bg_vscrollauto_speed = GetPara(edata, 3);
			map.bg_hscrollauto_speed = GetPara(edata, 5);
			
			break;
		}
	case ecChangeEncounterRate: /*ChangeEncounterRate*/
		{
			LsdMap &map = pGM->lsd.Map;
			map.encounter_rate = GetPara(edata, 0);
			break;
		}
	case ecTileSubstitution: /*TileSubstitution*/
		{
			bool changed = false;
			LsdMap &map = pGM->lsd.Map;
			int fromID = GetPara(edata, 1);
			int toID = GetPara(edata, 2);
			if (0<=fromID && fromID<144 && 0<=toID && toID<144) {
				if (GetPara(edata, 0) == 0) {
					for(int n=0; n<144; n++)
						if (map.tile_lower[n] == fromID) {
							map.tile_lower[n] = toID;
							changed = true;
						}
				} else {
					for(int n=0; n<144; n++)
						if (map.tile_upper[n] == fromID) {
							map.tile_upper[n] = toID;
							changed = true;
						}
				}
			}
			if (changed)
				pGM->lsd.Map.need_tile_refresh = true;
			break;
		}
	case ecTeleportTargets: /*TeleportTargets*/
		{
			int switchid = 0;

			if (GetPara(edata, 4) == 1)
				switchid = GetPara(edata, 5);

			if (GetPara(edata, 0) == 0)
				mtele.addTeleport(GetPara(edata, 1), GetPara(edata, 2), GetPara(edata, 3), switchid);
			else
				mtele.removeTeleport(GetPara(edata, 1));

			break;
		}
	case ecChangeTeleportAccess: /*ChangeTeleportAccess*/
		{
			sys.able_teleport = (GetPara(edata, 0) == 1);
			break;
		}
	case ecEscapeTarget: /*EscapeTarget*/
		{
			int switchid = 0;

			if (GetPara(edata, 3) == 1)
				switchid = GetPara(edata, 4);

			mtele.setEscape(GetPara(edata, 0), GetPara(edata, 1), GetPara(edata, 2), switchid);
			break;
		}
	case ecChangeEscapeAccess: /*ChangeEscapeAccess*/
		{
			sys.able_escape = (GetPara(edata, 0) == 1);
			break;
		}
	case ecOpenSaveMenu: /*OpenSaveMenu*/
		{
			edatas.eventpos++;
			pGM->slm->setSettingScrSave(true, false);
			pGM->slm->setScreen(scr_save);
			return false;
		}
	case ecChangeSaveAccess: /*ChangeSaveAccess*/
		{
			sys.able_save = (GetPara(edata, 0) == 1);
			break;
		}
	case ecOpenMainMenu: /*OpenMainMenu*/
		{
			pGM->slm->setSettingScrMenu();
			pGM->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
			edatas.eventpos++;
			return false;
		}
	case ecChangeMainMenuAccess: /*ChangeMainMenuAccess*/
		{
			sys.able_menu = (GetPara(edata, 0) == 1);
			break;
		}
	case ecConditionalBranch: /*ConditionalBranch*/
		{
			bool result = false;
			int inID, inVal, inVal2; bool inBVal;
			switch(GetPara(edata, 0)) {
				case 0:/*Switch*/{
					inID = GetPara(edata, 1);
					inBVal = ((GetPara(edata, 2)==0)?true:false);
					if (inBVal == mst.getSw(inID))
						result = true;
							
					break;
				}
				case 1:/*Variance*/{
					inVal = mst.getVar(GetPara(edata, 1));
							
					if (GetPara(edata, 2) == 0)
						inVal2 = GetPara(edata, 3);
					else
						inVal2 = mst.getVar(GetPara(edata, 3));
							
					switch(GetPara(edata, 4)) {
						case 0:if (inVal== inVal2)result=true;break;
						case 1:if (inVal>= inVal2)result=true;break;
						case 2:if (inVal<= inVal2)result=true;break;
						case 3:if (inVal > inVal2)result=true;break;
						case 4:if (inVal < inVal2)result=true;break;
						case 5:if (inVal!= inVal2)result=true;break;
					}
					break;
				}
				case 2:/*Timer1*/{
					inVal = state.timer1_leftframe/60;
					inVal2 = GetPara(edata, 1);
					switch(GetPara(edata, 2)) {
						case 0:if (inVal>= inVal2)result=true;break;
						case 1:if (inVal<= inVal2)result=true;break;
					}
					break;
				}
				case 10:/*Timer2*/{
					inVal = state.timer2_leftframe/60;
					inVal2 = GetPara(edata, 1);
					switch(GetPara(edata, 2)) {
						case 0:if (inVal>= inVal2)result=true;break;
						case 1:if (inVal<= inVal2)result=true;break;
					}
					break;
				}
				case 3:/*Money*/{
					inVal = state.money;
					inVal2 = GetPara(edata, 1);
					switch(GetPara(edata, 2)) {
						case 0:if (inVal>= inVal2)result=true;break;
						case 1:if (inVal<= inVal2)result=true;break;
					}
					break;
				}
				case 4:/*ITEM*/{
					inVal = GetPara(edata, 1);
					switch(GetPara(edata, 2)) {
						case 0:if (mit.getItemCount(inVal)>0)result=true;break;
						case 1:if (mit.getItemCount(inVal)==0)result=true;break;
					}
					break;
				}
				case 5:/*HERO*/{
					inID = GetPara(edata, 1);
					inVal = GetPara(edata, 3);
	
					c = getCharacter(inID);

					if (c != NULL) {
						switch(GetPara(edata, 2)) {
							case 0:result = mst.isPartyIn(inID);break;
							case 1:result = (mch.getS(inID, NAME) == edata.string);break;
							case 2:result = (mch.get(inID, LEVEL) >= inVal);break;
							case 3:result = (mch.get(inID, HP) >= inVal);break;
							case 4:result = mch.getSkill(inID, inVal);break;
							case 5:result = mch.isEquiped(inID, inVal);break;
							case 6:result = mch.getState(inID, inVal);break;
						}
					}
							
					break;
				}
				case 6:{
					LsdObjectCommon *o = getEvent(edatas.event_id, GetPara(edata, 1));
					if (o == NULL)
						break;
			
					inVal = GetPara(edata, 2);
					result = (o->direction == inVal+1);	
					break;
				}
				case 7:{
					inVal = GetPara(edata, 1);
					//TODO?
					break;
				}
				case 8:{
					//TODO 결정키로 이벤트가 실행되었나? 뭐지..
					break;
				}
				case 9:{
					//TODO BGM이 끝까지 한번이라도 재생되었는가?
					break;
				}
			}
					
			if (!result) { //TODO Check this effect
				if (GetPara(edata, 5)==0) {
					int n;
					for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
						GlbEventNode &node = edatas.event.data[n];

						if (node.indent < edata.indent)
							break;

						if (node.indent == edata.indent && node.code == ecEndBranch)
							break;
					}
					edatas.eventpos = n;
				} else {
					int n;
					for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
						GlbEventNode &node = edatas.event.data[n];

						if (node.indent < edata.indent)
							break;

						if (node.indent == edata.indent && node.code == ecElseBranch)
							break;
					}
					edatas.eventpos = n;
				}
			}
					
			break;
		}
				
	case ecLabel: /*Label*/break;
				
	case ecJumpToLabel: /*JumpToLabel*/
		{
			int n;
			for(n = 0; n < edatas.event.data.GetLength() - 1; n++) {
				GlbEventNode &node = edatas.event.data[n];

				if (node.code == ecLabel && node.parameters[0] == GetPara(edata, 0))
				{
					edatas.eventpos = n;
					break;
				}
			}
			break;
		}
				
	case ecLoop: /*Loop*/break;

	case ecBreakLoop: /*BreakLoop*/
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength(); n++) {
				GlbEventNode &node = edatas.event.data[n];

				if (	node.code == ecEndLoop || node.code == ecEndLoop2)
					break;
			}
			edatas.eventpos = n;
			break;
		}
				

	case ecEndLoop: /*EndLoop*/
	case ecEndLoop2:
		{
			int n;
			for(n = edatas.eventpos; n >= 0; n--) {
				GlbEventNode &node = edatas.event.data[n];

				if (node.indent < edata.indent)
					break;

				if (node.code == ecLoop && node.indent == edata.indent)
				{
					edatas.eventpos = n;
					break;
				}
			}
			break;
		}
				
	case ecEndEventProcessing: /*EndEventProcessing*/
		{
			//stack->data.RemoveAll();
// 			for(int i=0; i<stack->data.GetLength(); i++)
// 				stack->data[i].eventpos = stack->data[i].event_length;
			//Fixed to 2.2.9
			edatas.eventpos = edatas.event_length;

			break;
		}
				
	case ecEraseEvent: /*EraseEvent*/
		{
			LsdObjectCommon *o = getEvent(edatas.event_id, edatas.event_id);
			if (o != NULL)
				o->isUsed = false;
			break;
		}
				
	case ecCallEvent: /*CallEvent*/
		{
			GlbEvent *event_data = NULL;
			int index, page;
			index = GetPara(edata, 1);
			page = GetPara(edata, 2);
			getEvent(&(pGM->lsd), 0);
			switch(GetPara(edata, 0)) {
				case 0:event_data = &(pGM->ldb.dComEvt[index-1].event_data);break;
				case 1:
					{
						if (index == 10005) {
							for(int i=0; i<pMapData->Events.GetLength(); i++) {
								if (pMapData->Events[i].getIndex() == edatas.event_id) {
									event_data = &(pMapData->Events[i].Page[page-1].event_data);
									break;
								}
							}

							index = edatas.event_id;
						} else {
							for(int i=0; i<pMapData->Events.GetLength(); i++) {
								if (pMapData->Events[i].getIndex() == index) {
									event_data = &(pMapData->Events[i].Page[page-1].event_data);
									break;
								}
							}
						}

						break;
					}
				case 2:
					{
						index = mst.getVar(index);
						page = mst.getVar(page);
						for(int i=0; i<pMapData->Events.GetLength(); i++) {
							if (pMapData->Events[i].getIndex() == index) {
								event_data = &(pMapData->Events[i].Page[page-1].event_data);
								break;
							}
						}
						break;
					}
			}
			
			if (event_data == NULL)
				break;

			LsdMapEventData &data = stack->data.Add();
			if (data.pevent != event_data)
				data.event = *(event_data);
			data.pevent = event_data;
			data.event_id = index;
			data.event_length = event_data->data.GetLength();
			data.eventpos = 0;


			break;
		}

	case ecCallCommonEvent:
		{
			GlbEvent *event_data;
			int val1;
			val1 = GetPara(edata, 0);
			event_data = &(pGM->ldb.dComEvt[val1-1].event_data);

			LsdMapEventData &data = stack->data.Add();
			if (data.pevent != event_data)
				data.event = *(event_data);
			data.pevent = event_data;
			data.event_id = edatas.event_id;
			data.event_length = event_data->data.GetLength();
			data.eventpos = 0;

			break;
		}
				
	case ecComment: /*Comment*/
		{
			break;
		}
	case ecGameOver: /*GameOver*/
		{
			pGM->slm->setScreen(scr_gameover);
			return false;
		}
				
	case ecReturntoTitleScreen: /*ReturntoTitleScreen*/
		{
			pGM->slm->setScreen(scr_title);
			return false;
		}
	/*--------------------------------------------*/
	case 30001:
	case 30002:
	case 30003:/* Riding Object*/
		{
			int type = edata.code % 30000;

			//TODO sysMapVehicle.Ride(type);
			break;
		}
				
	/*--------------------------------------------*/
	case ecChangeMonsterHP: /*ChangeMonsterHP*/
	case ecChangeMonsterMP: /*ChangeMonsterMP*/
	case ecChangeMonsterCondition: /*ChangeMonsterCondition*/
	case ecShowHiddenMonster: /*ShowHiddenMonster*/
	case ecForceFlee:
	case ecEnableCombo:
	case ecChangeBattleBG: /*ChangeBattleBG*/
	case ecShowBattleAnimation_B: /*ShowBattleAnimation_B*/
	case ecTerminateBattle: /*TerminateBattle*/
		{
			pBatrMgr->doEvent(edata);
			break;
		}

	case ecConditionalBranch_B: /*ConditionalBranch_B*/
		{
			bool result = false;
			int inID, inVal, inVal2; bool inBVal;
			//String inStr = CheckString(edata.string);
			switch(GetPara(edata, 0)) {
				case 0:/*Switch*/{
					inID = GetPara(edata, 1);
					inBVal = ((GetPara(edata, 2)==0)?true:false);
					if (inBVal == mst.getSw(inID))
						result = true;
							
					break;
				}
				case 1:/*Variance*/{
					inVal = mst.getVar(GetPara(edata, 1));
							
					if (GetPara(edata, 2) == 0)
						inVal2 = GetPara(edata, 3);
					else
						inVal2 = mst.getVar(GetPara(edata, 3));
							
					switch(GetPara(edata, 4)) {
						case 0:if (inVal== inVal2)result=true;break;
						case 1:if (inVal>= inVal2)result=true;break;
						case 2:if (inVal<= inVal2)result=true;break;
						case 3:if (inVal > inVal2)result=true;break;
						case 4:if (inVal < inVal2)result=true;break;
						case 5:if (inVal!= inVal2)result=true;break;
					}
					break;
				}
				case 2:/*HeroActive?*/{
					inVal = GetPara(edata, 1) - 1;
					if (pBatrMgr->isAlive(inVal))
						result = true;
					break;
				}
				case 3:/*MonsterActive?*/{
					inVal = GetPara(edata, 1) - 1 + SCR_BATTLE_MAX_HEROS;
					if (pBatrMgr->isAlive(inVal))
						result = true;
					break;
				}
				case 4:/*Monster Target*/{
					if (batLastActId<0 || batLastActId>=SCR_BATTLE_MAX_HEROS)
						break;

					inVal = batLastTgtId[batLastActId];
					if (inVal == GetPara(edata, 1) - 1 + SCR_BATTLE_MAX_HEROS ||
						inVal == EFFECT_ALL_ENEMIES)
						result = true;
					break;
				}
				case 5:/*Command?*/{
					if (batLastActId != GetPara(edata, 1)-1)
						break;

					if (batLastActType[GetPara(edata, 1)-1] == GetPara(edata, 2)-1)
						result = true;
					break;
				}
			}
					
			if (!result) { //TODO Check this effect
				if (GetPara(edata, 5)==0) {
					int n;
					for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
						if (edatas.event.data[n].code == ecEndBranch_B && 
							edatas.event.data[n].indent == edata.indent)
							break;
					}
					edatas.eventpos = n;
				} else {
					int n;
					for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
						if (edatas.event.data[n].code == ecElseBranch_B && 
							edatas.event.data[n].indent == edata.indent)
							break;
					}
					edatas.eventpos = n;
				}
			}

			break;
		}
	/*--------------------------------------------*/
	case 20710: /*VictoryHandler*/
	case 20711: /*EscapeHandler*/
	case 20712: /*DefeatHandler*/
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
				if ((edatas.event.data[n].code == ecEndBattle) && 
					edatas.event.data[n].indent == edata.indent)
					break;
			}
			edatas.eventpos = n;
			break;
		}
	case 20713: /*EndBattle*/break;
	case 20722: /*EndShop*/break;
	case 20732: /*EndInn*/break;
				
	case ecShowChoiceOption: /*ShowChoiceOption*/
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
				GlbEventNode &node = edatas.event.data[n];

// 				if (node.indent < edata.indent)
// 					break;

				if (node.code == ecShowChoiceEnd && node.indent == edata.indent)
					break;
			}
			edatas.eventpos = n;
			break;
		}
	case ecElseBranch: /*ElseBranch*/
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {
				GlbEventNode &node = edatas.event.data[n];

// 				if (node.indent < edata.indent)
// 					break;

				if (node.code == ecEndBranch && node.indent == edata.indent)
					break;
			}
			edatas.eventpos = n;
			break;
		}
	case ecElseBranch_B: /*ElseBranch_B*/
		{
			int n;
			for(n = edatas.eventpos; n < edatas.event.data.GetLength() - 1; n++) {

				GlbEventNode &node = edatas.event.data[n];

// 				if (node.indent < edata.indent)
// 					break;

				if (node.code == ecEndBranch_B && node.indent == edata.indent)
					break;
			}
			edatas.eventpos = n;
			break;
		}
	case ecShowChoiceEnd: /*ShowChoiceEnd*/
	case ecEndBranch_B: /*EndBranch_B*/
	case ecEndBranch: /*EndBranch*/break;
	case ecComment_2: /*Comment_2*/break;
	/*--------------------------------------------*/
	/*--------------------------------------------*/
	case 0:
	case ecEND:
		{
			break;
		}
	}
	
	if (edatas.eventpos + 1 >= edatas.event.data.GetLength()) {
		if (stack->data.GetLength() <= 1 && !endMsg(stack)) {
			sys.face_name = "";
			return false;
		}

		stack->data.Remove(stack->data.GetLength()-1);
	} else {
		edatas.eventpos++;
	}

	return true;
}

int EvtMain::GetPara(GlbEventNode &edata, int index) {
	if (index < edata.parameters.GetLength())
		return edata.parameters[index];
	else
		return 0;
}
