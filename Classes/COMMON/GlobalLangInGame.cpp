#include "GlobalLangInGame.h"

void GlobalLangInGame::setLang( Encoding encoding )
{
	curEncoding = encoding;
}

TString GlobalLangInGame::getLang( InGameLang lang )
{
	if (curEncoding == LKOREAN) {
		switch(lang) {
		case Battle_EnemyTakeDamage:
		case Battle_HeroTakeDamage:
		case Battle_UseItem:
		case Battle_AttDefUp:
		case Battle_AttDefDown:
		case Game_LvUp:
			return "은";

		case Battle_Status:
		case Battle_AbsorbHeroStatus:
		case Battle_AbsorbEnemyStatus:
			return "의";
		}
	} else if (curEncoding == LJAPANESE) {
		switch(lang) {
		case Battle_EnemyTakeDamage:
			return "に";

		case Battle_HeroTakeDamage:
		case Battle_UseItem:
		case Battle_AbsorbEnemyStatus:
		case Battle_AttDefUp:
		case Battle_AttDefDown:
		case Game_LvUp:
			return "は";
			
		case Battle_Status:
		case Battle_AbsorbHeroStatus:
			return "の";

		case Battle_Status2:
			return "が";

		case Battle_AbsorbHeroStatus2:
		case Battle_AbsorbEnemyStatus2:
			return "を";
		}
	} else if (curEncoding == LCHINESE) {
		switch(lang) {
		case Battle_GetExp:
		case Battle_GetItem:
			return "獲得";

		case Battle_EnemyTakeDamage:
		case Battle_HeroTakeDamage:
			return "受到";

		case Battle_UseItem:
			return "使用";

		case Battle_Status:
		case Battle_AbsorbHeroStatus:
		case Battle_AbsorbEnemyStatus:
		case Game_LvUp:
			return "的";

		case Game_LvUp2:
			return "提升為";
		}
	} else {// if (m_cur_encoding == ENGLISH ) {
		switch(lang) {
		case Battle_EnemyTakeDamage:
		case Battle_HeroTakeDamage:
		case Battle_UseItem:
		case Battle_AttDefUp:
		case Battle_AttDefDown:
		case Game_LvUp:
			return "is";

		case Battle_Status:
		case Battle_AbsorbHeroStatus:
		case Battle_AbsorbEnemyStatus:
			return "'s";
		}
	}
	return "";
}