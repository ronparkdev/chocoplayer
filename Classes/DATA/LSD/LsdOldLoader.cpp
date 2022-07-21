#include "LsdOldLoader.h"

bool LsdOldLoader::ReadData( const TString &path, LdbMain *ldb, LsdMain *lsd )
{
	m_ldb = ldb;
	m_lsd = lsd;
	if (FileLoadOpen(path)) {
		ReadData();
		FileLoadClose();
		return true;
	}
	return false;
}

void LsdOldLoader::ReadData()
{
	TString header = ReadString();
	while(finid<finc)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
			{
				case 0x64:ReadPreview(finc);break;

				case 0x65:ReadSystem(finc);break;
				case 0x66:ReadScreen(finc);break;
				case 0x67:ReadPicture(finc);break;
				case 0x68:ReadPublicEvent(finc,0);break;
				case 0x69:ReadPublicEvent(finc,1);break;
				case 0x6A:ReadPublicEvent(finc,2);break;
				case 0x6B:ReadPublicEvent(finc,3);break;
				case 0x6C:ReadCharacter(finc);break;
				case 0x6D:ReadStatus(finc);break;
				case 0x6E:ReadTeleport(finc);break;
				case 0x6F:ReadMap(finc);break;
				case 0x71:ReadMapEvent(finc);break;
				default:ReadGarb();break;
			}
	}

	int id = 0;
	ObjectInit(id++, &(m_lsd->LocHero));
	for(int i=0; i<3; i++)
		ObjectInit(id++, &(m_lsd->LocRide[i]));

	for(int i=0; i<m_lsd->Map.events.GetLength(); i++)
		ObjectInit(id++, &(m_lsd->Map.events[i]));

	m_lsd->Map.events.SetLength(m_lsd->Map.events.GetLength()-4);
}

void LsdOldLoader::ReadPreview( int StructIdLimit )
{
	LsdPreview &pv = m_lsd->Preview;
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
		{
			case 0x01:pv.Time		= ReadUInt();break;
			case 0x0B:pv.Hero1Name	= ReadString();break;
			case 0x0C:pv.Hero1Level	= ReadUInt();break;
			case 0x0D:pv.Hero1HP	= ReadUInt();break;

			case 0x15:
			case 0x17:
			case 0x19:
			case 0x1B:
				pv.HeroFaceName[(type-0x15)/2] = ReadString();
				break;

			case 0x16:
			case 0x18:
			case 0x1A:
			case 0x1C:
				pv.HeroFaceID[(type-0x16)/2] = ReadUInt();
				break;
			
			default:
				ReadGarb();
				break;
		}
	}
}

void LsdOldLoader::ReadSystem( int StructIdLimit )
{
	LsdSystem &sys = m_lsd->System;

	char NULL_STR_CODE[] = {1,0};
	TString NULL_STR = NULL_STR_CODE;

	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;

	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		int type = (int) ReadAInt();
		if (type == 0)
			break;
		else
			switch(type)
			{

				case 0x01:sys.screen = ReadUInt();break;
				case 0x0B:sys.frame_count = ReadUInt();break;

				case 0x1F:sys.switch_length = ReadUInt();break;
				case 0x20:
				{
					long VectorSize = ReadAInt();
					sys.switches.SetLength(sys.switch_length);
					for(int i=0; i<VectorSize && i<sys.switch_length; i++)
						sys.switches[i] = ReadBoolFix();
					break;
				}

				case 0x21:sys.variable_length = ReadUInt();break;
				case 0x22:
				{
					long VectorSize = ReadAInt();
					sys.variables.SetLength(sys.variable_length);
					for(int i=0; i<VectorSize && i<sys.variables.GetLength(); i++)
						sys.variables[i] = ReadIntFixInv();
					break;
				}

				case 0x29:sys.msg_visiable = (ReadUInt() == 1);break;
				case 0x2A:sys.msg_pos = ReadUInt();break;
				case 0x2B:sys.msg_automove = (ReadUInt() == 1);break;
				case 0x2C:sys.msg_moveevent = (ReadUInt() == 1);break;

				case 0x33:sys.face_name = ReadString();break;
				case 0x34:sys.face_index = ReadUInt();break;
				case 0x35:sys.face_right = ReadBool();break;
				case 0x36:sys.face_flip = ReadBool();break;

				case 0x6F:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.transition_out = i;
					break;
						  }
				case 0x70:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.transition_in = i;
					break;
						  }
				case 0x71:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.battle_start_fadeout = i;
					break;
						  }
				case 0x72:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.battle_start_fadein = i;
					break;
						  }
				case 0x73:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.battle_end_fadeout = i;
					break;
						  }
				case 0x74:{
					int i = ReadUInt(); 
					if (i != 127)
						sys.battle_end_fadein = i;
					break;
						  }

				case 0x79:sys.able_teleport = ReadBool();break;
				case 0x7A:sys.able_escape = ReadBool();break;
				case 0x7B:sys.able_save = ReadBool();break;
				case 0x7C:sys.able_menu = ReadBool();break;

				case 0x83:sys.save_count = ReadUInt();break;
				case 0x84:sys.save_slot = ReadUInt();break;

				default:
					ReadGarb();break;
			}
	}
}

void LsdOldLoader::ReadScreen( int StructIdLimit )
{
	LsdScreen &scr = m_lsd->Screen;
	//int pan_x
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
		{
			case 0x01:scr.scr_cur_r = ReadUInt();break;
			case 0x02:scr.scr_cur_g = ReadUInt();break;
			case 0x03:scr.scr_cur_b = ReadUInt();break;
			case 0x04:scr.scr_cur_gr= ReadUInt();break;
			case 0x0B:scr.scr_fin_r = ReadUInt();break;
			case 0x0C:scr.scr_fin_g = ReadUInt();break;
			case 0x0D:scr.scr_fin_b = ReadUInt();break;
			case 0x0E:scr.scr_fin_gr= ReadUInt();break;
			case 0x0F:scr.scr_timeleft = ReadUInt();break;

			default:
				ReadGarb();break;
		}
	}
}

void LsdOldLoader::ReadPicture( int StructIdLimit )
{
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	long VectorLen = ReadAInt();
	m_lsd->Picture.SetLength(50);
	for(int i=0;i<VectorLen && finid < StructLimit; i++)
	{
		if (finid < 0)
			return;

		int VectorID = (int)ReadAInt() - 1;
		if (VectorID >= 0 && VectorID < 50)
			ReadPictureOne(m_lsd->Picture[VectorID], StructLimit);
	}
}

void LsdOldLoader::ReadPictureOne( LsdPicture &pic, long StructLimit )
{
	while(finid < StructLimit) {
		if (finid < 0)
			return;

		byte type = ReadByte();
		
		if (type == 0)
			break;
		else
			switch(type)
			{
				case 0x01:	pic.picName = ReadString();	break;

				case 0x1F:	pic.staX = pic.curX = pic.finX	= (int)ReadDouble();	break;
				case 0x20:	pic.staY = pic.curY = pic.finY	= (int)ReadDouble();	break;	

				case 0x09:	pic.is_use_backcolor = ReadBool();		break;

				case 0x21:	pic.curS = pic.finS = (float)ReadDouble();	break; 
				case 0x23:	pic.finTA = pic.curTA = (float)ReadDouble();	break; 
				case 0x22:	pic.finBA = pic.curBA = (float)ReadDouble();	break; 

				case 0x29:	pic.finR = pic.curR = (int)ReadDouble();	break; 
				case 0x2A:	pic.finG = pic.curG = (int)ReadDouble();	break; 
				case 0x2B:	pic.finB = pic.curB = (int)ReadDouble();	break; 
				case 0x2C:	pic.finGr = pic.curGr = (int)ReadDouble();break; 

				case 0x0F:	pic.effect_type = ReadUInt();	break; 
				case 0x41:	pic.effect_rotate_speed = pic.effect_wave_speed = (float) ReadDouble();break;

				case 0x06:	pic.is_fixed_to_map = ReadBool();	break;

				default:
					ReadGarb();break;
			}
	}
}

void LsdOldLoader::ReadPublicEvent( int StructIdLimit, int id )
{
	LsdObjectCommon *obj;
	if (id == 0)
		obj = &(m_lsd->LocHero);
	else
		obj = &(m_lsd->LocRide[id-1]);
	

	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
			{		
				case 0x0B:obj->map_id = ReadUInt();break;
				case 0x0C:obj->posX = ReadUInt();break;
				case 0x0D:obj->posY = ReadUInt();break;
				case 0x0E:obj->direction = ReadUInt();break;

				default:
					ReadGarb();break;
			}
	}
}

void LsdOldLoader::ReadCharacter( int StructIdLimit )
{
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	long VectorLen = ReadAInt();
	for(int i=0;i<VectorLen && finid < StructLimit; i++)
	{
		if (finid < 0)
			return;

		long VectorID = ReadAInt() - 1;
		ReadCharacterOne(i, m_lsd->Character[i], StructIdLimit);
	}
}

void LsdOldLoader::ReadCharacterOne(int index, LsdCharacter &lsdCh, int structIdLimit )
{
	char NULL_STR_CODE[] = {1,0};
	TString NULL_STR = NULL_STR_CODE;

	while(finid < structIdLimit) {
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
			{
				case 0x01:
				{
					TString str = ReadString();
					if (str != NULL_STR)lsdCh.name = str;break;
				}
				case 0x02:
				{
					TString str = ReadString();
					if (str != NULL_STR)lsdCh.title = str;break;
				}

				case 0x0B:lsdCh.pic_name = ReadString();break;
				case 0x0C:lsdCh.pic_id = ReadUInt();break;
				case 0x0D:/*SpriteFlag??*/ReadGarb();break;

				case 0x15:lsdCh.face_name = ReadString();break;
				case 0x16:lsdCh.face_index = ReadUInt();break;

				case 0x1D:lsdCh.chp = ReadUInt();break;
				case 0x1E:lsdCh.cmp = ReadUInt();break;
				case 0x1F:lsdCh.clevel = ReadUInt();break;
				case 0x20:lsdCh.cexp = ReadUInt();break;
				case 0x21:lsdCh.mhp = ReadUInt();break;
				case 0x22:lsdCh.mmp = ReadUInt();break;
				case 0x29:lsdCh.catk = ReadUInt();break;
				case 0x2A:lsdCh.cdef = ReadUInt();break;
				case 0x2B:lsdCh.cspt = ReadUInt();break;
				case 0x2C:lsdCh.cagi = ReadUInt();break;
				case 0x34:
				{
					long ArrSize = ReadAInt();
					long ArrLimit = finid + ArrSize;
					while(finid < ArrLimit) {
						int skillID = ReadShortFixInv();
						if (0<=skillID && skillID < 10000)
							lsdCh.skills.Add(skillID);
					}

					break;
				}
				case 0x52:
				{
					long ArrSize = ReadAInt();
					long ArrLimit = finid + ArrSize;
					while(finid < ArrLimit) {
						int stateVal = ReadShortFix();
						if (0<=stateVal && stateVal < lsdCh.ccond.GetLength())
							lsdCh.ccond[stateVal] = true;
					}
					break;
				}
				case 0x5A:lsdCh.cjob = ReadUInt()-1;break;
				case 0x5B:lsdCh.is_back_attack = (ReadUInt()==1?false:true);break;

				case 0x5C:lsdCh.is_two_hand_sword = ReadBool();break;
				case 0x5D:lsdCh.is_equip_fixed = ReadBool();break;
				case 0x5E:lsdCh.is_auto_battle = ReadBool();break;
				case 0x5F:lsdCh.is_super_gard = ReadBool();break;

				case 0x60:lsdCh.cequip.weapon_id = ReadUInt();break;
				case 0x61:lsdCh.cequip.shield_id = ReadUInt();break;
				case 0x62:lsdCh.cequip.helmet_id = ReadUInt();break;
				case 0x63:lsdCh.cequip.armor_id = ReadUInt();break;
				case 0x64:lsdCh.cequip.accessory_id = ReadUInt();break;

				default:
					ReadGarb();break;
			}
	}
	lsdCh.pic_default_trans = (m_ldb->dChar[index].transparent?3:0);
}

void LsdOldLoader::ReadStatus( int StructIdLimit )
{
	LsdState &sta = m_lsd->State;
	int itemLen = 0;

	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
			{
				case 0x01:sta.party_length = ReadUInt();break;
				case 0x02:
					{
						long VectorSize = ReadAInt();

						sta.party.SetLength(VectorSize);
						for(int i=0; i<VectorSize; i++)
							sta.party[i] = (int)ReadAInt();
						ReadByte();
						break;
					}
				case 0x0B:
					{
						itemLen = ReadUInt();
						sta.item_length = itemLen;
						sta.item_count.SetLength(itemLen);
						sta.item_index.SetLength(itemLen);
						sta.item_used.SetLength(itemLen);
						for(int i=0; i<itemLen; i++) {
							sta.item_used[i] = 0;
						}
						break;
					}
				case 0x0C:
					{
						long VectorSize = ReadAInt();
						for(int i=0; i<VectorSize; i++)
							sta.item_index[i] = ReadShortFixInv();
						break;
					}
				case 0x0D:
					{
						long VectorSize = ReadAInt();
						for(int i=0; i<VectorSize; i++)
							sta.item_count[i] = ReadByte();
						break;
					}
				case 0x15:sta.money = ReadUInt();break;

				default:
					ReadGarb();break;
			}
	}
}

void LsdOldLoader::ReadTeleport( int StructIdLimit )
{
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			ReadGarb();break;
	}
}

void LsdOldLoader::ReadMap( int StructIdLimit )
{
	LsdMap &map = m_lsd->Map;
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;

	while(finid < StructLimit)
	{
		if (finid < 0)
			return;

		byte type = ReadByte();
		if (type == 0)
			break;
		else
			switch(type)
			{
				case 0x01://PanX
					map.pan_x = ReadUInt();
					break;
					  
				case 0x02://PanY
					map.pan_y = ReadUInt();
					break;

				case 0x03://EncounterRate
					map.encounter_rate = ReadUInt();
					break;
					  
				case 0x0B:
					finid += ReadAInt();
					break;
					  
				case 0x10:
					map.is_pan_lock = ReadBool();
					break;
					  
				case 0x15://LowerTile [00 01 02 ... 8E 8F]
					for(int i=0; i<144; i++)
						map.tile_lower[i] = ReadShortFixInv();
					break;
					  
				case 0x16://UpperTile
					for(int i=0; i<144; i++)
						map.tile_upper[i] = ReadShortFixInv();
					break;
					  
				case 0x20://Parallax name
					map.bg_name = ReadString();
					break;
					  
				case 0x21://Horz bool
					map.bg_hscroll = ReadBool();
					break;
					  
				case 0x22://Vert bool
					map.bg_vscroll = ReadBool();
					break;
					  
				case 0x23://Horz auto?
					map.bg_hscrollauto = ReadBool();
					break;
					  
				case 0x24://Horz speed
					map.bg_hscrollauto_speed = ReadUInt();
					break;
					  
				case 0x25://Vert auto?
					map.bg_vscrollauto = ReadBool();;
					break;
					  
				case 0x26://Vert speed
					map.bg_vscrollauto_speed = ReadUInt();
					break;

				default:
					ReadGarb();break;
			}
	}
}

void LsdOldLoader::ReadMapEvent( int StructIdLimit )
{
	ArrayList<LsdMapEvent> &objs = m_lsd->Map.events;
	long StructSize = ReadAInt();
	long StructLimit = finid + StructSize;
	long VectorLen = ReadAInt();
	objs.SetLength(VectorLen); 

	StructLoader sl_init;
	sl_init.setInitMode();

	for(int i=0;i<VectorLen && finid < StructLimit; i++)
	{
		if (finid < 0)
			return;

		int VectorID = (int)ReadAInt() - 1;

		LsdMapEvent &obj = objs[i];

		obj.serialize(sl_init);

		obj.isUsed = true;
		obj.map_id = m_lsd->LocHero.map_id;

		while(finid < StructLimit) {
			byte type = ReadByte();
			if (type == 0)
				break;
			else{
				switch(type)
				{
					case 0x0C:obj.posX = ReadUInt();break;
					case 0x0D:obj.posY = ReadUInt();break;
					case 0x15:obj.direction_moving = obj.direction = ReadUInt();break;
					case 0x71:obj.move_freq = ReadUInt();break;
					case 0x72:obj.move_speed = ReadUInt();break;
					case 0x49:obj.pic_name = ReadString();break;
					case 0x4A:obj.pic_index = ReadUInt();break;
					default:
						ReadGarb();break;
				}

			}
		}
	}
}

void LsdOldLoader::ObjectInit(int index, LsdObjectCommon *obj) {
	obj->ani_next_left_frame = 0;
	obj->ani_paused = false;
	obj->ani_type = 0;
	obj->ani_walk = 0;
	obj->direction_lock = false;
	obj->draw_dontover = false;
	obj->draw_priority = 1;
	obj->flying = false;
	obj->invisible = false;
	obj->move_route_custom = false;
	obj->transparent = 0;
	obj->flash_frame_left = 0;
	obj->wait_cur_frame = 0;
	obj->wait_total_frame = 0;
	
	if (index < 4) {
		ArrayList<LsdMapEvent> &objs = m_lsd->Map.events;
		LsdMapEvent *tgtobj = &(objs[objs.GetLength()-4+index]);
		obj->direction_moving = obj->direction = tgtobj->direction;
		obj->move_freq	= tgtobj->move_freq;
		obj->move_speed = tgtobj->move_speed;
		obj->pic_name	= tgtobj->pic_name;
		obj->pic_index	= tgtobj->pic_index;
	}
}
