#ifndef __R2K_DATA__LDB_VOCA__
#define __R2K_DATA__LDB_VOCA__

#include <iostream>
#include "../BaseStruct.h"

#define VOCA_SHOP_LEN 3
#define VOCA_INN_LEN 2

class LdbVoca : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/TString special_combat;
	/*03*/TString escape_success;
	/*04*/TString escape_failure;
	/*05*/TString victory;
	/*06*/TString defeat;
	/*07*/TString exp_received;
	/*08*/TString money_recieved_a;
	/*09*/TString money_recieved_b;
	/*0A*/TString item_recieved;
	/*0B*/TString attacking;
	/*0C*/TString actor_critical;
	/*0D*/TString enemy_critical;
	/*0E*/TString defending;
	/*0F*/TString observing;
	/*10*/TString gather_power;
	/*11*/TString self_dest;
	/*12*/TString enemy_escape;
	/*13*/TString enemy_transform;
	/*14*/TString enemy_damaged;
	/*15*/TString enemy_undamaged;
	/*16*/TString actor_damaged;
	/*17*/TString actor_undamaged;
	/*18*/TString skill_failure_a;
	/*19*/TString skill_failure_b;
	/*1A*/TString skill_failure_c;
	/*1B*/TString dodge;
	/*1C*/TString use_item;
	/*1D*/TString hp_recovery;
	/*1E*/TString parameter_increase;
	/*1F*/TString parameter_decrease;
	/*20*/TString actor_hp_absorbed;
	/*21*/TString enemy_hp_absorbed;
	/*22*/TString resistance_increase;
	/*23*/TString resistance_decrease;
	/*24*/TString level_up;
	/*25*/TString skill_learned;
	/*26*/TString battle_start;
	/*27*/TString miss;
	
	/*29*/TString shop_greeting		[VOCA_SHOP_LEN];/*36 43*/
	/*2A*/TString shop_regreeting	[VOCA_SHOP_LEN];
	/*2B*/TString shop_buy			[VOCA_SHOP_LEN];
	/*2C*/TString shop_sell			[VOCA_SHOP_LEN];
	/*2D*/TString shop_leave		[VOCA_SHOP_LEN];
	/*2E*/TString shop_buy_select	[VOCA_SHOP_LEN];
	/*2F*/TString shop_buy_number	[VOCA_SHOP_LEN];
	/*30*/TString shop_purchased	[VOCA_SHOP_LEN];
	/*31*/TString shop_sell_select	[VOCA_SHOP_LEN];
	/*32*/TString shop_sell_number	[VOCA_SHOP_LEN];
	/*33*/TString shop_sold			[VOCA_SHOP_LEN];/*40 4D*/
	
	/*50*/TString inn_a_greeting1	[VOCA_INN_LEN];//55
	/*51*/TString inn_a_greeting2	[VOCA_INN_LEN];
	/*52*/TString inn_a_greeting3	[VOCA_INN_LEN];
	/*53*/TString inn_a_accept		[VOCA_INN_LEN];
	/*54*/TString inn_a_cancel		[VOCA_INN_LEN];

	/*5C*/TString possessed_items;
	/*5D*/TString equipped_items;
	/*5F*/TString gold;
	/*65*/TString battle_fight;
	/*66*/TString battle_auto;
	/*67*/TString battle_escape;
	/*68*/TString command_attack;
	/*69*/TString command_defend;
	/*6A*/TString command_item;
	/*6B*/TString command_skill;
	/*6C*/TString menu_equipment;
	/*6E*/TString menu_save;
	/*70*/TString menu_quit;
	/*72*/TString new_game;
	/*73*/TString load_game;
	/*75*/TString exit_game;
	/*76*/TString status;
	/*77*/TString row;
	/*78*/TString order;
	/*79*/TString wait_on;
	/*7A*/TString wait_off;
	/*7B*/TString level;
	/*7C*/TString health_points;
	/*7D*/TString magic_points;
	/*7E*/TString normal_status;
	/*7F*/TString exp_short;
	/*80*/TString lvl_short;
	/*81*/TString hp_short;
	/*82*/TString mp_short;
	/*83*/TString mp_cost;
	/*84*/TString attack;
	/*85*/TString defense;
	/*86*/TString spirit;
	/*87*/TString agility;
	/*88*/TString weapon;
	/*89*/TString shield;
	/*8A*/TString armor;
	/*8B*/TString helmet;
	/*8C*/TString accessory;
	/*92*/TString save_game_message;
	/*93*/TString load_game_message;
	/*94*/TString file;
	/*97*/TString exit_game_message;
	/*98*/TString yes;
	/*99*/TString no;

public:
	void init() {

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, special_combat);
		BaseStruct::serialize (sl, 0x03, escape_success);
		BaseStruct::serialize (sl, 0x04, escape_failure);
		BaseStruct::serialize (sl, 0x05, victory);
		BaseStruct::serialize (sl, 0x06, defeat);
		BaseStruct::serialize (sl, 0x07, exp_received);
		BaseStruct::serialize (sl, 0x08, money_recieved_a);
		BaseStruct::serialize (sl, 0x09, money_recieved_b);
		BaseStruct::serialize (sl, 0x0A, item_recieved);
		BaseStruct::serialize (sl, 0x0B, attacking);
		BaseStruct::serialize (sl, 0x0C, actor_critical);
		BaseStruct::serialize (sl, 0x0D, enemy_critical);
		BaseStruct::serialize (sl, 0x0E, defending);
		BaseStruct::serialize (sl, 0x0F, observing);
		BaseStruct::serialize (sl, 0x10, gather_power);
		BaseStruct::serialize (sl, 0x11, self_dest);
		BaseStruct::serialize (sl, 0x12, enemy_escape);
		BaseStruct::serialize (sl, 0x13, enemy_transform);
		BaseStruct::serialize (sl, 0x14, enemy_damaged);
		BaseStruct::serialize (sl, 0x15, enemy_undamaged);
		BaseStruct::serialize (sl, 0x16, actor_damaged);
		BaseStruct::serialize (sl, 0x17, actor_undamaged);
		BaseStruct::serialize (sl, 0x18, skill_failure_a);
		BaseStruct::serialize (sl, 0x19, skill_failure_b);
		BaseStruct::serialize (sl, 0x1A, skill_failure_c);
		BaseStruct::serialize (sl, 0x1B, dodge);
		BaseStruct::serialize (sl, 0x1C, use_item);
		BaseStruct::serialize (sl, 0x1D, hp_recovery);
		BaseStruct::serialize (sl, 0x1E, parameter_increase);
		BaseStruct::serialize (sl, 0x1F, parameter_decrease);
		BaseStruct::serialize (sl, 0x20, actor_hp_absorbed);
		BaseStruct::serialize (sl, 0x21, enemy_hp_absorbed);
		BaseStruct::serialize (sl, 0x22, resistance_decrease);
		BaseStruct::serialize (sl, 0x23, resistance_decrease);
		BaseStruct::serialize (sl, 0x24, level_up);
		BaseStruct::serialize (sl, 0x25, skill_learned);
		BaseStruct::serialize (sl, 0x26, battle_start);
		BaseStruct::serialize (sl, 0x27, miss);

		for(int i=0; i<VOCA_SHOP_LEN; i++) {
			int id = 0x29 + 0xD * i;
			BaseStruct::serialize (sl, id + 0x00, shop_greeting[i]);
			BaseStruct::serialize (sl, id + 0x01, shop_regreeting[i]);
			BaseStruct::serialize (sl, id + 0x02, shop_buy[i]);
			BaseStruct::serialize (sl, id + 0x03, shop_sell[i]);
			BaseStruct::serialize (sl, id + 0x04, shop_leave[i]);
			BaseStruct::serialize (sl, id + 0x05, shop_buy_select[i]);
			BaseStruct::serialize (sl, id + 0x06, shop_buy_number[i]);
			BaseStruct::serialize (sl, id + 0x07, shop_purchased[i]);
			BaseStruct::serialize (sl, id + 0x08, shop_sell_select[i]);
			BaseStruct::serialize (sl, id + 0x09, shop_sell_number[i]);
			BaseStruct::serialize (sl, id + 0x0A, shop_sold[i]);
		}

		for(int i=0; i<VOCA_INN_LEN; i++) {
			int id = 0x50 + 0x5 * i;
			BaseStruct::serialize (sl, id + 0x00, inn_a_greeting1[i]);
			BaseStruct::serialize (sl, id + 0x01, inn_a_greeting2[i]);
			BaseStruct::serialize (sl, id + 0x02, inn_a_greeting3[i]);
			BaseStruct::serialize (sl, id + 0x03, inn_a_accept[i]);
			BaseStruct::serialize (sl, id + 0x04, inn_a_cancel[i]);
		}

		BaseStruct::serialize (sl, 0x5C, possessed_items);
		BaseStruct::serialize (sl, 0x5D, equipped_items);
		BaseStruct::serialize (sl, 0x5F, gold);
		BaseStruct::serialize (sl, 0x65, battle_fight);
		BaseStruct::serialize (sl, 0x66, battle_auto);
		BaseStruct::serialize (sl, 0x67, battle_escape);
		BaseStruct::serialize (sl, 0x68, command_attack);
		BaseStruct::serialize (sl, 0x69, command_defend);
		BaseStruct::serialize (sl, 0x6A, command_item);
		BaseStruct::serialize (sl, 0x6B, command_skill);
		BaseStruct::serialize (sl, 0x6C, menu_equipment);
		BaseStruct::serialize (sl, 0x6E, menu_save);
		BaseStruct::serialize (sl, 0x70, menu_quit);
		BaseStruct::serialize (sl, 0x72, new_game);
		BaseStruct::serialize (sl, 0x73, load_game);
		BaseStruct::serialize (sl, 0x75, exit_game);
		BaseStruct::serialize (sl, 0x76, status);
		BaseStruct::serialize (sl, 0x77, row);
		BaseStruct::serialize (sl, 0x78, order);
		BaseStruct::serialize (sl, 0x79, wait_on);
		BaseStruct::serialize (sl, 0x7A, wait_off);
		BaseStruct::serialize (sl, 0x7B, level);
		BaseStruct::serialize (sl, 0x7C, health_points);
		BaseStruct::serialize (sl, 0x7D, magic_points);
		BaseStruct::serialize (sl, 0x7E, normal_status);
		BaseStruct::serialize (sl, 0x7F, exp_short);
		BaseStruct::serialize (sl, 0x80, lvl_short);
		BaseStruct::serialize (sl, 0x81, hp_short);
		BaseStruct::serialize (sl, 0x82, mp_short);
		BaseStruct::serialize (sl, 0x83, mp_cost);
		BaseStruct::serialize (sl, 0x84, attack);
		BaseStruct::serialize (sl, 0x85, defense);
		BaseStruct::serialize (sl, 0x86, spirit);
		BaseStruct::serialize (sl, 0x87, agility);
		BaseStruct::serialize (sl, 0x88, weapon);
		BaseStruct::serialize (sl, 0x89, shield);
		BaseStruct::serialize (sl, 0x8A, armor);
		BaseStruct::serialize (sl, 0x8B, helmet);
		BaseStruct::serialize (sl, 0x8C, accessory);
		BaseStruct::serialize (sl, 0x92, save_game_message);
		BaseStruct::serialize (sl, 0x93, load_game_message);
		BaseStruct::serialize (sl, 0x94, file);
		BaseStruct::serialize (sl, 0x97, exit_game_message);
		BaseStruct::serialize (sl, 0x98, yes);
		BaseStruct::serialize (sl, 0x99, no);
		sl.finishStruct();
	}
};

#endif
