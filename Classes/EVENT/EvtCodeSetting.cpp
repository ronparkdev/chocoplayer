#include "EvtMain.h"

bool isRunableSerial(GlbEventNode &node) {
	switch(node.code) {
	case ecShowMessage:
	case ecShowMessage_2:
	case ecShowChoice:
	case ecInputNumber:
	case ecSimulatedAttack://?
	case ecEnemyEncounter:
	case ecOpenShop:
	case ecShowInn:
	case ecEnterHeroName:
	case ecTeleport:
	case ecRecallToLocation:
	case ecEnterExitVehicle:
	case ecOpenSaveMenu:
	case ecOpenMainMenu:

		return false;

	default:
		return true;
	}
}

TString getString(evtcode code) {
	switch(code) {
	case ecEND:					return "END";
	case ecChangeClass:			return "ChangeClass";
	case ecChangeBattleCommands:	return "ChangeBattleCommands";
	case ecMessageOptions:		return "MessageOptions";
	case ecShowMessage:			return "ShowMessage";
	case ecShowMessage_2:		return "ShowMessage_2";
	case ecShowChoice:			return "ShowChoice";
	case ecInputNumber:			return "InputNumber";
	case ecChangeFaceGraphic:	return "ChangeFaceGraphic";
	case ecControlSwitches:		return "ControlSwitches";
	case ecControlVars:			return "ControlVars";
	case ecTimerOperation:		return "TimerOperation";
	case ecChangeGold:			return "ChangeGold";
	case ecChangeItems:			return "ChangeItems";
	case ecChangePartyMembers:	return "ChangePartyMembers";
	case ecChangeExp:			return "ChangeExp";
	case ecChangeLevel:			return "ChangeLevel";
	case ecChangeParameters:	return "ChangeParameters";
	case ecChangeSkills:		return "ChangeSkills";
	case ecChangeEquipment:		return "ChangeEquipment";
	case ecChangeHP:			return "ChangeHP";
	case ecChangeMP:			return "ChangeMP";
	case ecChangeCondition:		return "ChangeCondition";
	case ecFullHeal:			return "FullHeal";
	case ecSimulatedAttack:		return "SimulatedAttack";
	case ecChangeHeroName:		return "ChangeHeroName";
	case ecChangeHeroTitle:		return "ChangeHeroTitle";
	case ecChangeSpriteAssociation:	return "ChangeSpriteAssociation";
	case ecChangeActorFace:		return "ChangeActorFace";
	case ecChangeVehicleGraphic:	return "ChangeVehicleGraphic";
	case ecChangeSystemBGM:		return "ChangeSystemBGM";
	case ecChangeSystemSFX:		return "ChangeSystemSFX";
	case ecChangeSystemGraphics:	return "ChangeSystemGraphics";
	case ecChangeScreenTransitions:	return "ChangeScreenTransitions";
	case ecEnemyEncounter:		return "EnemyEncounter";
	case ecOpenShop:			return "OpenShop";
	case ecShowInn:				return "ShowInn";
	case ecEnterHeroName:		return "EnterHeroName";
	case ecTeleport:			return "Teleport";
	case ecMemorizeLocation:	return "MemorizeLocation";
	case ecRecallToLocation:	return "RecallToLocation";
	case ecEnterExitVehicle:	return "EnterExitVehicle";
	case ecSetVehicleLocation:	return "SetVehicleLocation";
	case ecChangeEventLocation:	return "ChangeEventLocation";
	case ecTradeEventLocations:	return "TradeEventLocations";
	case ecStoreTerrainID:		return "StoreTerrainID";
	case ecStoreEventID:		return "StoreEventID";
	case ecEraseScreen:			return "EraseScreen";
	case ecShowScreen:			return "ShowScreen";
	case ecTintScreen:			return "TintScreen";
	case ecFlashScreen:			return "FlashScreen";
	case ecShakeScreen:			return "ShakeScreen";
	case ecPanScreen:			return "PanScreen";
	case ecWeatherEffects:		return "WeatherEffects";
	case ecShowPicture:			return "ShowPicture";
	case ecMovePicture:			return "MovePicture";
	case ecErasePicture:		return "ErasePicture";
	case ecShowBattleAnimation:	return "ShowBattleAnimation";
	case ecSpriteTransparency:	return "SpriteTransparency";
	case ecFlashSprite:			return "FlashSprite";
	case ecMoveEvent:			return "MoveEvent";
	case ecProceedWithMovement:	return "ProceedWithMovement";
	case ecHaltAllMovement:		return "HaltAllMovement";
	case ecWait:				return "Wait";
	case ecPlayBGM:				return "PlayBGM";
	case ecFadeOutBGM:			return "FadeOutBGM";
	case ecMemorizeBGM:			return "MemorizeBGM";
	case ecPlayMemorizedBGM:	return "PlayMemorizedBGM";
	case ecPlaySound:			return "PlaySound";
	case ecPlayMovie:			return "PlayMovie";
	case ecKeyInputProc:		return "KeyInputProc";
	case ecChangeMapTileset:	return "ChangeMapTileset";
	case ecChangePBG:			return "ChangePBG";
	case ecChangeEncounterRate:	return "ChangeEncounterRate";
	case ecTileSubstitution:	return "TileSubstitution";
	case ecTeleportTargets:		return "TeleportTargets";
	case ecChangeTeleportAccess:	return "ChangeTeleportAccess";
	case ecEscapeTarget:		return "EscapeTarget";
	case ecChangeEscapeAccess:	return "ChangeEscapeAccess";
	case ecOpenSaveMenu:		return "OpenSaveMenu";
	case ecChangeSaveAccess:	return "ChangeSaveAccess";
	case ecOpenMainMenu:		return "OpenMainMenu";
	case ecChangeMainMenuAccess:	return "ChangeMainMenuAccess";
	case ecConditionalBranch:	return "ConditionalBranch";
	case ecLabel:				return "Label";
	case ecJumpToLabel:			return "JumpToLabel";
	case ecLoop:				return "Loop";
	case ecBreakLoop:			return "BreakLoop";
	case ecEndLoop:				return "EndLoop";
	case ecEndEventProcessing:	return "EndEventProcessing";
	case ecEraseEvent:			return "EraseEvent";
	case ecCallEvent:			return "CallEvent";
	case ecComment:				return "Comment";
	case ecGameOver:			return "GameOver";
	case ecReturntoTitleScreen:	return "ReturntoTitleScreen";
	case ecRidingObject1:		return "RidingObject1";
	case ecRidingObject2:		return "RidingObject2";
	case ecRidingObject3:		return "RidingObject3";
	case ecChangeMonsterHP:		return "ChangeMonsterHP";
	case ecChangeMonsterMP:		return "ChangeMonsterMP";
	case ecChangeMonsterCondition:	return "ChangeMonsterCondition";
	case ecShowHiddenMonster:	return "ShowHiddenMonster";
	case ecChangeBattleBG:		return "ChangeBattleBG";
	case ecShowBattleAnimation_B:	return "ShowBattleAnimation_B";
	case ecConditionalBranch_B:	return "ConditionalBranch_B";
	case ecTerminateBattle:		return "TerminateBattle";
	case ecVictoryHandler:		return "VictoryHandler";
	case ecEscapeHandler:		return "EscapeHandler";
	case ecDefeatHandler:		return "DefeatHandler";
	case ecEndBattle:			return "EndBattle";
	case ecTransaction:			return "Transaction";
	case ecNoTransaction:		return "NoTransaction";
	case ecEndShop:				return "EndShop";
	case ecStay:				return "Stay";
	case ecNoStay:				return "NoStay";
	case ecEndInn:				return "EndInn";
	case ecShowChoiceOption:	return "ShowChoiceOption";
	case ecElseBranch:			return "ElseBranch";
	case ecElseBranch_B:		return "ElseBranch_B";
	case ecShowChoiceEnd:		return "ShowChoiceEnd";
	case ecEndBranch_B:			return "EndBranch_B";
	case ecEndBranch:			return "EndBranch";
	case ecComment_2:			return "Comment_2";
	default: return "";
	}
}
