#include "ScrMapPublic.h"

int ScrMapPublic::getEventPage(GameMain *main, LmuEvent &ev) {
	for( int i = ev.Page.GetLength() - 1 ; i>=0 ; i--)
	{
		if (chkEventPage(main, ev.Page[i])) {
			return i;
		}
	}
	return -1;
}

inline bool ScrMapPublic::chkEventPage(GameMain *main, LmuEventPage &page) {
	LmuEventTrigger &trigger = page.trigger;
	int flag = trigger.flag;

	bool notmatching = false;

	MgrState mstate(main);
	MgrItem mitem(main);

	int n=1;
	if (flag == 0) {
		return true;
	} else {
		while((n <= flag && !notmatching))
		{
			if ( (flag & n) != 0)
			{
				switch(n)
				{
				case 1:if (!mstate.getSw(trigger.switch1id))notmatching=true;break;
				case 2:if (!mstate.getSw(trigger.switch2id))notmatching=true;break;
				case 4:
					{
						int var = mstate.getVar(trigger.variableid);
						int trivar = trigger.variabledata;
						int tricon = trigger.variableif;
						switch(tricon)
						{
						case 0:if ( ! (var == trivar))notmatching=true;break;
						case 1:if ( ! (var >= trivar))notmatching=true;break;
						case 2:if ( ! (var <= trivar))notmatching=true;break;
						case 3:if ( ! (var > trivar))notmatching=true;break;
						case 4:if ( ! (var < trivar))notmatching=true;break;
						case 5:if ( ! (var != trivar))notmatching=true;break;
						}
						break;
					}
				case 8:if (mitem.getItemCount(trigger.itemid)==0)notmatching=true;break;//TODO fix this
				case 16:if (!mstate.isPartyIn(trigger.characterid))notmatching=true;break;
				case 32:if (main->lsd.State.timer1_leftframe/FRAME_PER_SEC>trigger.timer1data)notmatching=true;break;
				case 64:if (main->lsd.State.timer2_leftframe/FRAME_PER_SEC>trigger.timer2data)notmatching=true;break;
				}
			}
			//flag -= n;
			n *=2;
		}
		if (!notmatching)
			return true;
	}
	return false;
}

int ScrMapPublic::getChipsetID(int chipID)
{
	if (chipID<2000)//0,1
		return chipID/1000;
	else if (chipID<3000)//2,3
		return (chipID-2000)/500 + 2;
	else if (chipID<4000)//4,5
		return (chipID - 3000)/50+3;//TODO check
	else if (chipID<5000)//6,7
		return (chipID-4000)/50 + 6;
	else if (chipID<6000)
		return (chipID-5000)+18;
	else if (chipID>=10000)
		return (chipID-10000);
	else
		return 0;
}	

int ScrMapPublic::getTerrainID(LdbMain *ldb, LsdMain *lsd, LmuMain *lmu, int x, int y)
{
	int posid = x + y*lmu->width;

	if ( posid < 0 || posid >= lmu->lowerMap.GetLength() )
		return 1;

	int chipsetID = lsd->Map.chipset_id;

	if (chipsetID <= 0)
		return 1;

	int cID = getChipsetID(lmu->lowerMap[posid]);
	if (cID > 17)cID = lsd->Map.tile_lower[cID-18] + 18;

	if (cID < ldb->dChip[chipsetID-1].TerrainData.GetLength())
		return ldb->dChip[chipsetID-1].TerrainData[cID];//TODO?
	else
		return 1;
}

void ScrMapPublic::getEventScreenPos( LsdMain * m_pSave, int eventid, int &posx, int &posy)
{
	LsdObjectCommon *obj = EvtMain::getEvent(m_pSave, eventid);

	if (obj == NULL)
		return;

	posx = obj->screen_pos_x;
	posy = obj->screen_pos_y;
}
