#include "ScreenLayerManager.h"

#include "../AppMulti.h"

#if (SR_WIN == 1)
_CrtMemState s1;
#endif

#if (SR_WIN == 1)
void chkKeyStateDebug(bool &tgt, int chkKey) {
	bool val = (GetAsyncKeyState(chkKey)	& 0x8000);
	if (tgt != val) {
		tgt = val;

		if (val)
			CCLOG("KEY %d DOWN", chkKey);
		else
			CCLOG("KEY %d UP", chkKey);
	}
}
#endif

void ScreenLayerManager::windowsKeyInput()
{
#if (SR_WIN == 1)
	chkKeyStateDebug(m_pushed[AND_KEYCODE_DPAD_UP],		VK_UP);
	chkKeyStateDebug(m_pushed[AND_KEYCODE_DPAD_DOWN],	VK_DOWN);
	chkKeyStateDebug(m_pushed[AND_KEYCODE_DPAD_RIGHT],	VK_RIGHT);
	chkKeyStateDebug(m_pushed[AND_KEYCODE_DPAD_LEFT],	VK_LEFT);

	chkKeyStateDebug(m_pushed[AND_KEYCODE_BUTTON_A],	'Z');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_BUTTON_B],	'X');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_BUTTON_C],	'C');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_V],	'V');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_A],	'A');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_S],	'S');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_D],	'D');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_F],	'F');
	chkKeyStateDebug(m_pushed[AND_KEYCODE_SHIFT_LEFT],	VK_LSHIFT);
	chkKeyStateDebug(m_pushed[AND_KEYCODE_SPACE],		VK_SPACE);

	int last_q, last_w;
	last_q = m_pushed[AND_KEYCODE_Q];
	last_w = m_pushed[AND_KEYCODE_W];
	m_pushed[AND_KEYCODE_Q]				= (GetAsyncKeyState('Q')		& 0x8000)?1:0;
	m_pushed[AND_KEYCODE_W]				= (GetAsyncKeyState('W')		& 0x8000)?1:0;
	/*
	if (last_q != m_pushed[AND_KEYCODE_Q] && m_pushed[AND_KEYCODE_Q] > 0)
		_CrtMemCheckpoint( &s1 );

	if (last_w != m_pushed[AND_KEYCODE_W] && m_pushed[AND_KEYCODE_W] > 0)
		_CrtMemDumpAllObjectsSince(  &s1 );
	*/
	for(int i=0; i<256; i++)
		if (m_pushed[i])
			m_last_pushed = i;

#endif
}
