#include "ScrMap.h"
#include "../../AppMultiTime.h"
#include "../../COMMON/MgrSetting.h"
#include "../../COMMON/GlobalLangFont.h"
#include "../../DATA/ArrayList.h"
#include "../../EVENT/EvtMain.h"

extern int g_evt_do_move_cnt[30000];

TString getString(evtcode code);

void ScrMap::initDebug() {
	m_debuglabel = CCLabelTTF::create("", GlobalLangFont::getDefFontName(), 13);
	m_debuglabel->setPosition(ccp(200/ScreenCommon::game_s+1, ScreenCommon::game_h - (220)/ScreenCommon::game_s));
	m_debuglabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	m_debuglabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_debuglabel->setDimensions(CCSize(400, 400));
	m_debuglabel->setScale(1.0f/ScreenCommon::game_s);
	m_debuglabel->enableStroke(ccc3(0, 0, 0), 1.0f);
	m_debuglabel->setVisible(false);
	this->addChild(m_debuglabel, sz_controler+100);
}

void ScrMap::drawDebug()
{
	MgrSetting *s = MgrSetting::getInstance();
	int debug_time = (int)getTime();
	if (s->getBool(RST_ETC_DEBUG) && debug_time_last<debug_time ) {
// 		TString str, strnum;
// 
// 		int empty = CLOCKS_PER_SEC - (debug_time_doevents + debug_time_checkkeys + debug_time_drawevents +
// 			debug_time_drawevents + debug_time_drawevents2 + debug_time_draweffect + 
// 			debug_time_drawbg + debug_time_chkride + debug_time_drwanimation +
// 			debug_time_drawtimer);
// 
// 		strnum.setText(debug_time_doevents		*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Do Event\n";
// 		strnum.setText(debug_time_checkkeys		*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Chk Key\n";
// 		strnum.setText(debug_time_drawevents	*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Event\n";
// 		strnum.setText(debug_time_drawchipset	*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Chipset\n";
// 		strnum.setText(debug_time_drawevents2	*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Event2\n";
// 		strnum.setText(debug_time_draweffect	*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw BackGround\n";
// 		strnum.setText(debug_time_drawbg		*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Chk Ride\n";
// 		strnum.setText(debug_time_chkride		*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Picture\n";
// 		strnum.setText(debug_time_drwanimation	*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Animation\n";
// 		strnum.setText(debug_time_drawtimer		*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Draw Timer\n";
// 		strnum.setText(empty					*1000/CLOCKS_PER_SEC, 4);
// 		str = str + strnum + " Idle(or Graphic)\n\n";
// 
// 		strnum.setText(EvtMain::g_event_run_count					, 6);
// 		str = str + strnum + " EvtRunCnt\n";
// 		EvtMain::g_event_run_count = 0;

// 		char str[1024];
// 		char *pstr = str;
// 
// 		int maxVal = 0;
// 		int minVal = 9999999;
// 		int maxID = 0;
// 
// 		pstr += sprintf(pstr, "new : %d byte\ndel : %d byte\nto_gar : %d cnt\nfr_gar : %d cnt\nmove : %d cnt\nevt : %d cnt\n",
// 							g_arraylist_byte_new, g_arraylist_byte_delete, 
// 							g_arraylist_count_to_garbage, g_arraylist_count_from_garbage,
// 							g_arraylist_count_move_node, EvtMain::g_event_run_count, g_evt_do_move_cnt);
// 
// 		g_arraylist_byte_new = g_arraylist_byte_delete = g_arraylist_count_from_garbage = 
// 			g_arraylist_count_to_garbage = g_arraylist_count_move_node = EvtMain::g_event_run_count = 0;
// 
// 		for(int i=1; i<=15; i++) {
// 			maxID = 0;
// 			maxVal = 0;
// 			for(int j=0; j<30000; j++) {
// 				if (maxVal < g_evt_do_move_cnt[j] && g_evt_do_move_cnt[j] < minVal) {
// 					maxVal = g_evt_do_move_cnt[j];
// 					maxID = j;
// 				}
// 			}
// 			minVal = maxVal;
// 
// 			if (maxVal == 0)
// 				break;
// 
// 			pstr += sprintf(pstr, "%02d : %06dcnt => %06d(%s)\n", i, maxVal, maxID, getString((evtcode)maxID).getTextUTF8());
// 		}
// 
// 		m_debuglabel->setString(str);
// 
// 		memset(g_evt_do_move_cnt, 0, sizeof(int) * 30000);

		

// 		debug_time_doevents = debug_time_checkkeys = debug_time_drawevents = debug_time_drawchipset
// 			= debug_time_drawevents2 = debug_time_draweffect = debug_time_drawbg
// 			= debug_time_chkride = debug_time_drwanimation = debug_time_drawtimer = 0;
	}

	debug_time_last = debug_time;
}
