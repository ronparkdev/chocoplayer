#include "ScrBattleHueManager.h"

ScrBattleHueMgr::ScrBattleHueMgr(GameMain *main)
{
	m_main = main;

	LsdScreen &scr = main->lsd.Screen;

	scr_fin_r = scr_cur_r = scr.scr_cur_r;
	scr_fin_g = scr_cur_g = scr.scr_cur_g;
	scr_fin_b = scr_cur_b = scr.scr_cur_b;
	scr_fin_gr = scr_cur_gr = scr.scr_cur_gr;
	scr_left_frame = scr_total_frame = 0;

	for(int i=0; i<SCR_BATTLE_MAX_HUE; i++) {
		fl_left_frame[i] = fl_total_frame[i] = 0;
		fl_continuous[i] = 0;
		fl_cur_lev[i] = 0.0f;
	}

}

ScrBattleHueMgr::~ScrBattleHueMgr()
{

}

void ScrBattleHueMgr::init( int id, CCSprite *node )
{
	int i;
	if (id < 0 || id >= SCR_BATTLE_MAX_ENEMIES + SCR_BATTLE_MAX_HEROS)
		i = SCR_BATTLE_MAX_HUE-1;
	else
		i = id;

	m_shaders[i].setShader(node);
}

void ScrBattleHueMgr::init( int id, CCSpriteBatchNode *node )
{
	int i;
	if (id < 0 || id >= SCR_BATTLE_MAX_ENEMIES + SCR_BATTLE_MAX_HEROS)
		i = SCR_BATTLE_MAX_HUE-1;
	else
		i = id;

	m_shaders[i].setShader(node);
}

void ScrBattleHueMgr::setColor( int r, int g, int b, int gr, int frame )
{
	scr_fin_r = r;
	scr_fin_g = g;
	scr_fin_b = b;
	scr_fin_gr = gr;

	if (frame <= 0)
		frame = 1;

	scr_left_frame = scr_total_frame = frame;
}

void ScrBattleHueMgr::setFlash( int id, int type, int r, int g, int b, int lev, int frame )
{
	fl_continuous[id] = type;
	fl_cur_lev[id] = lev;
	fl_fin_r[id] = r;
	fl_fin_g[id] = g;
	fl_fin_b[id] = b;
	fl_left_frame[id] = fl_total_frame[id] = frame;
}

void ScrBattleHueMgr::Draw(int framedelta)
{
	bool force = false;
	if (scr_left_frame > 0) {
		if (scr_left_frame - framedelta <= 0) {
			scr_cur_r = scr_fin_r;
			scr_cur_g = scr_fin_g;
			scr_cur_b = scr_fin_b;
			scr_cur_gr= scr_fin_gr;
			scr_left_frame = scr_total_frame = 0;
		} else {
			scr_cur_r += (scr_fin_r - scr_cur_r) / scr_left_frame;
			scr_cur_g += (scr_fin_g - scr_cur_g) / scr_left_frame;
			scr_cur_b += (scr_fin_b - scr_cur_b) / scr_left_frame;
			scr_cur_gr+= (scr_fin_gr - scr_cur_gr) / scr_left_frame;
			scr_left_frame -= framedelta;
		}
		force = true;
	}

	int scrID = SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES;
	if (fl_continuous[scrID] != 2) {
		if (fl_left_frame[scrID] > 0) {
			if (fl_left_frame[scrID]-framedelta <= 0) {
				fl_cur_lev[scrID] = 0.0f;
				fl_left_frame[scrID] = fl_total_frame[scrID] = 0;
			} else {
				fl_cur_lev[scrID] -= fl_cur_lev[scrID] / fl_left_frame[scrID];
				fl_left_frame[scrID] -= framedelta;
			}
			force = true;
		}
	} else {
		fl_continuous[scrID] = 0;
		fl_cur_lev[scrID] = 0.0f;
		if (fl_left_frame[scrID] > 0) {
			fl_left_frame[scrID] = 0;
			force = true;
		}
	}

	if (force) {
		float fl_per = fl_cur_lev[scrID] / 31.0f;
		m_shaders[scrID].setColor(
			(scr_cur_r-100.0f)/100.0f + fl_fin_r[scrID] * fl_per / 31.0f,
			(scr_cur_g-100.0f)/100.0f + fl_fin_g[scrID] * fl_per / 31.0f,
			(scr_cur_b-100.0f)/100.0f + fl_fin_b[scrID] * fl_per / 31.0f,
			(scr_cur_gr-100.0f)/100.0f + fl_per);
	}

	bool force2;
	for(int id=0; id<SCR_BATTLE_MAX_ENEMIES+SCR_BATTLE_MAX_HEROS; id++) {
		force2 = false;
		if (fl_continuous[id] != 2) {
			if (fl_left_frame[id] > 0) {
				if (fl_left_frame[id]-framedelta <= 0) {
					fl_cur_lev[id] = 0.0f;
					fl_left_frame[id] = fl_total_frame[id] = 0;
				} else {
					fl_cur_lev[id] -= fl_cur_lev[id] / fl_left_frame[id];
					fl_left_frame[id] -= framedelta;
				}
				force2 = true;
			}
		} else {
			fl_continuous[id] = 0;
			fl_cur_lev[id] = 0.0f;
			if (fl_left_frame[id] > 0) {
				fl_left_frame[id] = 0;
				force2 = true;
			}
		}

		if (force || force2) {
			float flash_event_per = fl_cur_lev[id] / 31.0f;
			float flash_per = fl_cur_lev[scrID] / 31.0f;
			m_shaders[id].setColor(
				(scr_cur_r-100.0f)/100.0f + fl_fin_r[scrID] * flash_per / 31.0f + fl_fin_r[id] * flash_event_per / 31.0f,
				(scr_cur_g-100.0f)/100.0f + fl_fin_g[scrID] * flash_per / 31.0f + fl_fin_g[id] * flash_event_per / 31.0f,
				(scr_cur_b-100.0f)/100.0f + fl_fin_b[scrID] * flash_per / 31.0f + fl_fin_b[id] * flash_event_per / 31.0f,
				(scr_cur_gr-100.0f)/100.0f + flash_per + flash_event_per);
		}
	}
}

void ScrBattleHueMgr::setAlpha( int id, float val )
{
	m_shaders[id].setAlpha(val);
}

void ScrBattleHueMgr::reloadShaders()
{
	for(int i=0; i<SCR_BATTLE_MAX_HUE; i++)
		m_shaders[i].reload();
}

void ScrBattleHueMgr::doSaveToLSD() {
	LsdScreen &scr = m_main->lsd.Screen;

	scr.scr_cur_r = scr_cur_r;
	scr.scr_cur_g = scr_cur_g;
	scr.scr_cur_b = scr_cur_b;
	scr.scr_cur_gr = scr_cur_gr;
	scr.scr_timeleft = scr_left_frame;
}