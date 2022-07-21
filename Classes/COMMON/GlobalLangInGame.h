#ifndef __R2K_COMMON_GLOBAL_LANGUAGE_IN_GAME__
#define __R2K_COMMON_GLOBAL_LANGUAGE_IN_GAME__
#include "DebugingCode.h"

#include "GlobalLang.h"

enum InGameLang{
	Battle_GetExp,
	Battle_GetItem,

	Battle_EnemyTakeDamage,
	Battle_HeroTakeDamage,

	Battle_UseItem,
	Battle_Status,
	Battle_Status2,
	Battle_AbsorbHeroStatus,
	Battle_AbsorbHeroStatus2,
	Battle_AbsorbEnemyStatus,
	Battle_AbsorbEnemyStatus2,

	Battle_AttDefUp,
	Battle_AttDefDown,

	Game_LvUp,
	Game_LvUp2,
	Game_getSkill,
};

class GlobalLangInGame{
	Encoding curEncoding;

public:
	void setLang(Encoding encoding);
	TString getLang(InGameLang lang);
};

#endif