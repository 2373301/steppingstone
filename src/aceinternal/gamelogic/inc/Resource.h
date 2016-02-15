
#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "typedef.h"
#include "gamelogic_def.h"
#include "coredef.h"
#include <boost/shared_ptr.hpp>

#define GET_ENEMY_RES_INFO(res_info_map, info, find_id)	\
	EnemyResInfo * info = NULL;	\
	EnemyResInfoMap_t::iterator unique_it = res_info_map->find(find_id);	\
	if (unique_it != res_info_map->end())	\
	{	\
		info = unique_it->second;	\
	}

#define GET_ROLE_RES_INFO(res_info_map, info, find_id)	\
	RoleResInfo * info = NULL;	\
	RoleResInfoMap_t::iterator unique_it = res_info_map->find(find_id);	\
	if (unique_it != res_info_map->end())	\
	{	\
	info = unique_it->second;	\
	}

#define GET_MAP_RES_INFO(res_info_map, info, find_id)	\
	MapResInfo * info = NULL;	\
	MapResInfoMap_t::iterator unique_it = res_info_map->find(find_id);	\
	if (unique_it != res_info_map->end())	\
	{	\
		info = unique_it->second;	\
	}

#define GET_LEVEL_EXP_RES_INFO(res_info_map, info, find_id)	\
	LevelExpResInfo * info = NULL;	\
	LevelExpResInfoMap_t::iterator unique_it = res_info_map->find(find_id);	\
	if (unique_it != res_info_map->end())	\
	{	\
	info = unique_it->second;	\
	}

#define GET_EXP_ADDITION_RES_INFO(res_info_map, info, find_id)	\
	ExpAdditionResInfo * info = NULL;	\
	ExpAdditionResInfoMap_t::iterator unique_it = res_info_map->find(find_id);	\
	if (unique_it != res_info_map->end())	\
{	\
	info = unique_it->second;	\
}


//////////////////////////////////////////////////////////////////////////


struct PairUint32Uint32
{
	PairUint32Uint32()
		: first(0)
		, second(0)
	{}

	PairUint32Uint32(uint32 f_v, uint32 s_v)
		: first(f_v)
		, second(s_v)
	{}

	uint32 first;
	uint32 second;
};

struct PairInt32Int32
{
	PairInt32Int32()
		: first(0)
		, second(0)
	{}

	PairInt32Int32(int32 f_v, int32 s_v)
		: first(f_v)
		, second(s_v)
	{}

	int32 first;
	int32 second;
};

struct PairInt32VectorInt32
{
	PairInt32VectorInt32()
		: first(0)
	{}

	PairInt32VectorInt32(int32 f_v, Int32Vec_t s_v)
		: first(f_v)
		, second(s_v)
	{}

	int32 first;
	Int32Vec_t second;
};

struct ThreeUint32
{
	ThreeUint32()
		: first(0)
		, second(0)
		, third(0)
	{}

	ThreeUint32(uint32 f_v, uint32 s_v, uint32 t_v)
		: first(f_v)
		, second(s_v)
		, third(t_v)
	{}

	uint32 first;
	uint32 second;
	uint32 third;
};

//////////////////////////////////////////////////////////////////////////

struct  MapResInfo 
{
	MapResInfo()
		: map_id(0)
		, type(0)
		, sub_type(0)
		, born_x(0)
		, born_y(0)
		, enter_level(0)
		, unlock_level(0)
		, enter_quest(0)
		//, exp(0)
		//, yueli(0)
		, stage_id(0)
		, talk(0)
	{}

	uint32 map_id;
	uint32 type;
	uint32 sub_type;
	uint32 born_x;
	uint32 born_y;
	uint32 enter_level;
	uint32 unlock_level;
	uint32 enter_quest;
	//uint32 exp;
	//uint32 yueli;
	uint32 stage_id;
	int32  talk;
	Uint32Set_t npc_set;
	//Uint32Set_t enemy_set;
	//Uint32Vec_t bonus_vec;
};

 typedef define_unordered_map<int, MapResInfo *> MapResInfoMap_t;

struct  EnemyResInfo 
{
	EnemyResInfo()
	: enemy_id(0)
	, type(0)
	, level(0)
	, money(0)
	, exp(0)
	, yueli(0)
	//, walk_speed(0)
	, map(0)
	//, x(0)
	//, y(0)
	//, r(0)
	, talk(0)
	{}

	uint32 enemy_id;
	int32  type;
	Uint32Vec_t sequence;
	uint32 level;
	uint32 money;
	uint32 exp;
	uint32 yueli;
	Uint32Vec_t bonus;
	//string resouce;
	Uint32Vec_t slot;
	//uint32 walk_speed;
	uint32 map;
	//uint32 x;
	//uint32 y;
	//uint32 r;
	uint32 talk;
	string name;
	Uint32Vec_t boss_type;
};

typedef define_unordered_map<int, EnemyResInfo *> EnemyResInfoMap_t;


//////////////////////////////////////////////////////////////////////////

struct  SkillFunResInfo
{
	SkillFunResInfo()
	: fun_id(0)
	, rate(0)
	, a(0)
	, b(0)
	, c(0)
	, d(0)
	{}

	int32 fun_id;
	int32 rate;
	int32 a;
	int32 b;
	int32 c;
	int32 d;
	int32 e;
};

typedef vector<SkillFunResInfo> SkillFunResInfoVec_t;

struct  SkillResInfo 
{
	SkillResInfo()
	: skill_id(0)
	, type(ST_BASE)
	, target(STT_BASE)
	, manaleft(0)
	, iscritical(0)
	, isfengding(0)
	, isblock(0)
	, isantiattack(0)
	, hit(0)
	, critical(0)
	, others(0)
	, buff_flag(0)
	, boss(0)
	{}

	uint32 skill_id;
	SkillType type;
	SkillTargetType target;
	int32 manaleft;
	int32 iscritical;
	int32 isfengding;
	int32 isblock;
	int32 isantiattack;
	int32 hit;
	int32 critical;
	int32 others;
	//string resouce;
	//string icon;
	SkillFunResInfoVec_t skill_fun_vec;
	int32 buff_flag;
	int32 boss;
};

typedef define_unordered_map<int, SkillResInfo *> SkillResInfoMap_t;

struct  NpcResInfo 
{
	NpcResInfo()
	: id(0)
	, map_id(0)
	, x(0)
	, y(0)
	{}

	uint32 id;
	int32 map_id;
	int32 x;
	int32 y;
	vector<PairInt32VectorInt32> function_with_p;
};

typedef define_unordered_map<int, NpcResInfo *> NpcResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct  FormResInfo 
{
	FormResInfo()
	: form_id(0)
	, group(0)
	, level(0)
	{}

	uint32 form_id;
	int32 group;
	int32 level;
	vector<std::pair<int, int> > attr_increase;
	Int32Vec_t slot;
};

typedef define_unordered_map<int, FormResInfo *> FormResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct  MonsterResInfo 
{
	MonsterResInfo()
	: id(0)
	, level(0)
	, sex(0)
	, job(0)
	, char_ability(0)
	, str(0)
	, agi(0)
	, int_value(0)
	, phy_att(0)
	, phy_def(0)
	, agi_att(0)
	, agi_def(0)
	, int_att(0)
	, int_def(0)
	, hp(0)
	, critical(0)
	, critical_damage(0)
	, block(0)
	, block_defence(0)
	, block_damage(0)
	, hit(0)
	, fending(0)
	, normal_skill(0)
	, extremely_skill(0)
	, strike_back(0)
	, role_id(0)
	, talk(0)
	, combo(0)
	, joint(0)
	, combo_up(0)
	, joint_up(0)
	, combo_down(0)
	, joint_down(0)
	, dodge_attack1(0)
	, dodge_attack2(0)	
	, enarge(0)
	{}

	uint32 id;
	uint32 level;
	uint32 sex;
	uint32 job;
	int32 char_ability;
	int32 str;
	int32 agi;
	int32 int_value;
	int32 phy_att;
	int32 phy_def;
	int32 agi_att;
	int32 agi_def;
	int32 int_att;
	int32 int_def;
	uint32 hp;
	int32 critical;
	int32 critical_damage;
	int32 block;
	int32 block_defence;
	int32 block_damage;
	int32 hit;
	int32 fending;
	int32 normal_skill;
	int32 extremely_skill;
	int32 strike_back;
	int32 role_id;
	int32 talk;
	int32 combo;
	int32 joint;
	int32 combo_up;
	int32 joint_up;
	int32 combo_down;
	int32 joint_down;
	uint32 dodge_attack1;
	uint32 dodge_attack2;
	int32 enarge;
	string name;
	//string avatar;
};

typedef define_unordered_map<int, MonsterResInfo *> MonsterResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct  InitializationCfg 
{
	InitializationCfg()
	: level(0)
	, exp(0)
	, money(0)
	, coin(0)
	, src_x(0)
	, src_y(0)
	, seniority(0)
	, coin_exp(0)
	, frame(0)
	, energy(0)
	, training_slot(0)
	, member_slot(0)
	, curr_battle_array(0)
	, map_id(0)
	, level_cap(0)
	, checkplint(0)
	{}

	int level;
	int exp;
	int money;
	int coin;
	int src_x;
	int src_y;
	int seniority;
	int coin_exp;
	int frame;
	int energy;
	int training_slot;
	int member_slot;
	int curr_battle_array;
	int map_id;
	Uint32Vec_t ba_role_location;
	Uint32Vec_t battle_array_list;		// player initialize law list
	Uint32Vec_t wonder_skill;
	Uint64Vec_t buffers;
	Uint64Vec_t roles;
	Uint64Vec_t bag;
	Uint64Vec_t warehouse;
	Uint32Vec_t can_recruit_roles;
	Uint32Vec_t quest;
	uint32		level_cap;
	Uint32Vec_t	checkplint;
	Uint32Vec_t gift_item_id;
	Uint32Vec_t gift_item_num;
};

//////////////////////////////////////////////////////////////////////////
struct  WarJobEnrageRes
{
	WarJobEnrageRes()
		: job_id(0)
	{}

	int32		job_id;
	int32		mana_range;
	Int32Vec_t	hit;
	Int32Vec_t	un_hit;
	Int32Vec_t	dodge;
	Int32Vec_t	un_dodge;
	Int32Vec_t	block;
	Int32Vec_t	un_block;
	Int32Vec_t	critical;
	Int32Vec_t	un_critical;
};

typedef define_unordered_map<int, WarJobEnrageRes *> WarJobEnrageResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct RoleResInfo 
{
	RoleResInfo()
	: role_id(0)
	, level(0)
	, sex(0)
	, job(0)
	, char_ability(0)
	, str(0)
	, agi(0)
	, int_value(0)
	, phy_att(0)
	, phy_def(0)
	, agi_att(0)
	, agi_def(0)
	, int_att(0)
	, int_def(0)
	, hp(0)
	, hp_growth(0)
	, critical(0)
	, critical_damage(0)
	, block(0)
	, block_defence(0)
	, block_damage(0)
	, hit(0)
	, dodge(0)
	, strike_back(0)
	, normal_skill(0)
	, extremely_skill(0)
	, faction(0)
	, color(0)
	, combo(0)
	, joint(0)
	, combo_up(0)
	, joint_up(0)
	, combo_down(0)
	, joint_down(0)
	, enarge(0)
	, phy_att_growth(0)
	, phy_def_growth(0)
	, agi_att_growth(0)
	, agi_def_growth(0)
	, int_att_growth(0)
	, int_def_growth(0)
	, hp_percent(0)
	, phy_att_percent(0)
	, phy_def_percent(0)
	, agi_att_percent(0)
	, agi_def_percent(0)
	, int_att_percent(0)
	, int_def_percent(0)
	{}

	uint32 role_id;
	int32 level;
	int32 sex;
	int32 job;
	int32 char_ability;
	int32 str;
	int32 agi;
	int32 int_value;
	int32 phy_att;
	int32 phy_def;
	int32 agi_att;
	int32 agi_def;
	int32 int_att;
	int32 int_def;
	int32 hp;
	int32 hp_growth;
	int32 critical;
	int32 critical_damage;
	int32 block;
	int32 block_defence;
	int32 block_damage;
	int32 hit;
	int32 dodge;
	int32 strike_back;
	int32 normal_skill;
	int32 extremely_skill;
	int32 faction;
	int32 color;
	int32 combo;
	int32 joint;
	int32 combo_up;
	int32 joint_up;
	int32 combo_down;
	int32 joint_down;
	int32 enarge;
	string name;

	int32 phy_att_growth;
	int32 phy_def_growth;
	int32 agi_att_growth;
	int32 agi_def_growth;
	int32 int_att_growth;
	int32 int_def_growth;

	int32 hp_percent;
	int32 phy_att_percent;
	int32 phy_def_percent;
	int32 agi_att_percent;
	int32 agi_def_percent;
	int32 int_att_percent;
	int32 int_def_percent;
};

typedef define_unordered_map<int, RoleResInfo *> RoleResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct WarInitResInfo 
{
	WarInitResInfo()
	: round(20)
	, str(0)
	, agi(0)
	, int_v(0)
	, str_att(0)
	, str_def(0)
	, agi_att(0)
	, agi_def(0)
	, int_att(0)
	, int_def(0)
	, float_value(0)
	, def_gas_effect(0)
	, min_damage(0)
	, attack_gas_effect(0)
	, attack_gas_max(0)
	, attack_att_max(0)
	, attack_att_maxrate(0)
	{}

	int round;
	int str;
	int agi;
	int int_v;
	int str_att;
	int str_def;
	int agi_att;
	int agi_def;
	int int_att;
	int int_def;
	int float_value;

	int def_gas_effect;
	int min_damage;
	int attack_gas_effect;
	int attack_gas_max;
	Int32Vec_t  attack_hp_range;
	Int32Vec_t  attack_hp_damage;
	Int32Vec_t  hp_range;
	Int32Vec_t  hp_damage;
	int attack_att_max;
	int attack_att_maxrate;
};

//////////////////////////////////////////////////////////////////////////

struct PropertyResInfo 
{
	PropertyResInfo()
	: id(0)
	, program_id(0)
	, type(0)
	, value(0)
	, subtype(0)
	{}

	int32 id;
	int32 program_id;
	int32 type;
	int32 value;
	uint32 subtype;
};

typedef define_unordered_map<int, PropertyResInfo *> PropertyResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct BufferFunResInfo 
{
	BufferFunResInfo()
	: fun_id(0)
	, rate(0)
	, a(0)
	, b(0)
	, c(0)
	, d(0)
	{}

	int32 fun_id;
	int32 rate;
	int32 a;
	int32 b;
	int32 c;
	int32 d;
	int32 e;
};

typedef vector<BufferFunResInfo> BufferFunResInfoVec_t;

struct BufferResInfo
{
	BufferResInfo()
	: buffer_id(0)
	, type(0)
	, pile_type(0)
	, pile_limit(0)
	, pile_percent(0)
	, pile_value(0)
	, group(0)
	, level(0)
	, confirm(0)
	, buffer_feature(0)
	//, normal_trigger_skill(0)
	//, unusual_trigger_skill(0)
	, damage_display(0)
	, round(0)
	, target(0)
	{}

	uint32	buffer_id;
	uint32	type;
	uint32	pile_type;
	uint32	pile_limit;
	int32	pile_percent;
	int32	pile_value;
	int32	group;
	int32	level;
	int32	confirm;
	uint32	buffer_feature;
	//int32	normal_trigger_skill;
	//int32	unusual_trigger_skill;
	int32	damage_display;
	uint32	round;
	uint32	target;
	BufferFunResInfoVec_t	buffer_fun_vec;
};

typedef define_unordered_map<int, BufferResInfo *> BufferResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct HireResInfo 
{
	HireResInfo()
	: id(0)
	, level(0)
	, limit(0)
	, role_id(0)
	, lv_req(0)
	, prestige_req(0)
	, lv_unlock(0)
	, prestige_unlock(0)
	, special_req(0)
	, money_cost(0)
	, coin_cost(0)
	, seniority_cost(0)
	, tavern_hire(0)
	, tavern_limit_frame(0)
	, tavern_limit_level(0)
	, skillcolor(0)
	, talk(0)
    ,hire_cost(0)
	{}

	int32 id;
	int32 level;
	int32 limit;
	int32 role_id;
	int32 lv_req;
	int32 prestige_req;		// 声望
	int32 lv_unlock;
	int32 prestige_unlock;	// 声望
	int32 special_req;
	int32 money_cost;		// 铜钱
	int32 coin_cost;		// 元宝
	int32 seniority_cost;	// 阅历
	uint32 tavern_hire;
	uint32 tavern_limit_frame;
	uint32 tavern_limit_level;
	uint32 skillcolor;
	uint32 talk;
    uint32 hire_cost;
};

typedef define_unordered_map<int, HireResInfo *> HireResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct LevelExpResInfo 
{
	LevelExpResInfo()
	: level(0)
	, exp(0)
	{}

	uint32 level;
	uint32 exp;
};

typedef define_unordered_map<uint32, LevelExpResInfo *> LevelExpResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct ExpAdditionResInfo
{
	ExpAdditionResInfo()
	: level_diff(0)
	, addition_percent(0)
	, limit_percent(0)
	{}

	int level_diff;
	int addition_percent;
	int limit_percent;
};

typedef define_unordered_map<int32, ExpAdditionResInfo *> ExpAdditionResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct WarScoreResInfo 
{
	WarScoreResInfo()
	: type(0)
	, subtype(0)
	, value(0)
	, value_score(0)
	{}

	int32 type;
	int32 subtype;
	int32 value;
	int32 value_score;
};

typedef vector<WarScoreResInfo *> WarScoreResInfoVec_t;

//////////////////////////////////////////////////////////////////////////

struct LawResInfo 
{
	LawResInfo()
	: heart_id(0)
	, law_id(0)
	, law_level(0)
	, need_level(0)
	, need_item(0)
	, martial(0)
	, lawpay(0)
	, function_id(0)
	, function_num(0)
	, display_level(0)
	, display_martial(0)
	, group(0)
	, delay(0)
	, first_strike(0)
	, money_cost(0)
	{}

	uint32 heart_id;
	uint32 law_id;
	uint32 law_level;
	uint32 need_level;
	uint32 need_item;
	int32 martial;
	int32 lawpay;
	int32 function_id;
	int32 function_num;
	uint32 display_level;
	int32 display_martial;
	uint32 group;
	int32 type_value;
	string name;
	uint32 delay;
	uint32 first_strike;
	uint32 money_cost;
};

typedef vector<LawResInfo *> LawResInfoVec_t;

//////////////////////////////////////////////////////////////////////////

struct ItemResInfo 
{
	ItemResInfo()
	: item_id(0)
	, level(0)
	, job(0)
	, sex(0)
	, martial(0)
	, part(0)
	, color(0)
	, equipment(0)
	, type(0)
	, subtype(0)
	, pub1(0)
	, pub2(0)
	, pub3(0)
	, pub4(0)
	, pub5(0)
	, pub6(0)
	, pub7(0)
	, link(0)
	, price(0)
	, pricenum(0)
	, stack(0)
	, talk(0)
	, coin_value(0)
	, group(0)
	, auto_sale(0)
	, holefree(0)
	, holelimit(0)
	, cost(0)
	, vip_limit(0)
	, resolve_bonus(0)
	{}

	int32 item_id;
	int32 level;
	int32 job;
	int32 sex;
	int32 martial;
	int32 part;
	int32 color;
	int32 equipment;
	int32 type;
	int32 subtype;
	int32 pub1;
	int32 pub2;
	int32 pub3;
	int32 pub4;
	int32 pub5;
	int32 pub6;
	int32 pub7;
	int32 link;
	int32 price;
	int32 pricenum;
	int32 stack;
	int32 talk;
	int32 coin_value;
	uint32 group;
	int32 auto_sale;
	uint32 holefree;
	uint32 holelimit;
	uint32 cost;
	uint32 vip_limit;
	Uint32Vec_t options;
	Uint32Vec_t options_num;
	Uint32Vec_t result;
	uint32 resolve_bonus;
};

typedef define_unordered_map<int32, ItemResInfo *> ItemResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct BonusItemResInfo 
{
	BonusItemResInfo()
	: item_id(0)
	, item_no(0)
	, probability(0)
	, curr_probability(0)
	{}

	int32 item_id;
	int32 item_no;
	int32 probability;
	int32 curr_probability;
};

typedef vector<BonusItemResInfo> BonusItemResInfoVec_t;

struct BonusResInfo 
{
	BonusResInfo()
	: id(0)
	, type_value(0)
	, produce(0)
	, total_probability(0)
	, divide(0)
	, need_pos_num(0)
	{}

	int32 id;
	int32 type_value;
	int32 produce;
	int32 total_probability;
	int32 divide;
	uint32 need_pos_num;
	BonusItemResInfoVec_t bonus_item_vec;
};

typedef define_unordered_map<int32, BonusResInfo *> BonusResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct SectResInfo 
{
	SectResInfo()
		: id(0)
		, limit(0)
	{}

	int32 id;
	int32 limit;
};

typedef define_unordered_map<int32, SectResInfo *> SectResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct PrandomResInfo 
{
	PrandomResInfo()
	: level(0)
	, low(0)
	, high(0)
	, sum(0)
	{}

	int32 level;
	int32 low;
	int32 high;
	int32 sum;
};

typedef define_unordered_map<int32, PrandomResInfo *> PrandomResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct RandomSkillResInfo
{
	RandomSkillResInfo()
	: skill_id(0)
	, unlock_lv(0)
	, weight(0)
	{}

	int32 skill_id;
	int32 unlock_lv;
	int32 weight;
};

typedef define_unordered_map<int32, RandomSkillResInfo *> RandomSkillResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct GoldParamResInfo 
{
	GoldParamResInfo()
	: fun_id(0)
	, levelunlock(0)
	, vipunlock(0)
	, free_times(0)
	, a_coin(0)
	, b_coin(0)
	, limit_coin(0)
	, limit_times(0)
	, a_effect(0)
	, b_effect(0)
	{}

	int32 fun_id;
	uint32 levelunlock;
	uint32 vipunlock;
	int32 free_times;
	int32 a_coin;
	int32 b_coin;
	int32 limit_coin;
	int32 limit_times;
	int32 a_effect;
	int32 b_effect;
};

typedef define_unordered_map<int32, GoldParamResInfo *> GoldParamResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

typedef vector<string>	ProhibitWordResVec_t;

//////////////////////////////////////////////////////////////////////////

struct TalkResInfo 
{
	TalkResInfo()
		: id(0)
		, type_value(0)
		, or_value(0)
		, enable(0)
	{}

	uint32 id;
	uint32 type_value;
	uint32 or_value;
	uint32 enable;
};

typedef define_unordered_map<uint32, TalkResInfo *>	TalkResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct ItemStrengResInfo 
{
	ItemStrengResInfo()
		: id(0)
	, strenglv(0)
	, part(0)
	, color(0)
	, consumption(0)
	, profit(0)
	, num(0)
	, money(0)
	, law(0)
	, profit1(0)
	, num1(0)
	, first_strike(0)
	, time(0)
	{}

	uint32 id;
	//string name;
	int32 strenglv;
	int32 part;
	int32 color;
	int32 consumption;
	int32 profit;
	int32 num;
	int32 money;
	int32 law;
	int32 profit1;
	int32 num1;
	int32 first_strike;
	uint32 time;
};

typedef define_unordered_map<uint32, ItemStrengResInfo *> ItemStrengResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct RecruitmentResInfo 
{
	RecruitmentResInfo()
		: id(0)
	, level(0)
	, lock(0)
	, function_value(0)
	, type_value(0)
	, num(0)
	, cti(0)
	, upgrade1(0)
	, upgrade2(0)
	, weight(0)
	, money(0)
	, gold(0)
	{}

	uint32 id;
	int32 level;
	int32 lock;
	int32 function_value;
	int32 type_value;
	int32 num;
	int32 cti;
	int32 upgrade1;
	int32 upgrade2;
	int32 weight;
	int32 money;
	uint32 gold;
};

typedef define_unordered_map<uint32, RecruitmentResInfo *> RecruitmentResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct RecruitexpResInfo 
{
	RecruitexpResInfo()
		: level(0)
		, exp(0)
	{}

	uint32 level;
	uint32 exp;
};

typedef define_unordered_map<uint32, RecruitexpResInfo *> RecruitexpResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct SystemResInfo 
{
	SystemResInfo()
	: edition(0)
	, level_cap(0)
	, quality_cap(0)
	, itemstreng_op(0)
	, illusion_op(0)
	, itemstreng_get(0)
	, strenger_cd(0)
	, strengerlevel_cap(0)
	, itemstreng_cd(0)
	, martial(0)
	, itemstreng_coin(0)
	, step_down(0)
	, listing(0)
	, returnitemnum(0)
	, returnitemtimes(0)
	, tili(0)
	, tilireturn(0)
	, streng_success(0)
	, cardku(0)
	, cardhad(0)
	, cardlun(0)
	, eggnum(0)
	, lawcd(0)
	, waittime(0)
	, intwaittime(0)
	, inspire_time(0)
	, boss_start1(0)
	, boss_start2(0)
	, copy_playernum_limit(0)
	, copy_times_free(0)
	, give_tili(0)
	, givetili_top(0)
	, chakra_caches_num(0)
	, confraternity_pay(0)
	, recruitment_special(0)
	, checkpoints_cd(0)
	, cleaning_cd(0)
	, cleaningcd_clean(0)
	, egg_cd(0)
	, eggcd_clean(0)
	, pvp_times(0)
	, rank_param(0)
	, energy_reward_value(0)
	, raise_a(0)
	, raise_b(0)
	, level_first_strike(0)
	, sanwei_first_strike_1(0)
	, sanwei_first_strike_2(0)
	, sanwei_first_strike_3(0)
	, race_num(0)
	, splendid_force_cool_time(0)
	, splendid_cool_time(0)
	, splendid_canvass_cool_time(0)
	, splendid_worship_time(0)
	, arena_time(0)
	, raise_max_a(0)
	, raise_max_b(0)
	, tresure_shop_limittime(0)
	, tresure_shop_cd(0)
	, tavern_nums2(0)
	{}

	int32 edition;
	int32 level_cap;
	int32 quality_cap;
	int32 itemstreng_op;
	int32 illusion_op;
	int32 itemstreng_get;
	int32 strenger_cd;
	int32 strengerlevel_cap;
	int32 itemstreng_cd;
    int32 itemstreng_multiple;
	Int32Vec_t strengbox1;
	Int32Vec_t strengbox2;
	Int32Vec_t strengbox3;
	int32 martial;
	int32 itemstreng_coin;
	int32 step_down;
	int32 listing;
	int32 returnitemnum;
	int32 returnitemtimes;
	int32 tili;
	int32 tilireturn;
	int32 streng_success;
	Int32Vec_t jjwin;
    Int32Vec_t specialshop_unlock;
	int32 cardku;
	int32 cardhad;
	int32 cardlun;
	int32 eggnum;
	int32 lawcd;
	int32 waittime;
	int32 intwaittime;
	int32 inspire_time;
	int32 boss_start1;
	int32 boss_start2;
    Int32Vec_t chakra_box;
	int32 copy_playernum_limit;
	int32 copy_times_free;
	uint32 give_tili;
	uint32 givetili_top;
	uint32 chakra_caches_num;
	uint32 confraternity_pay;
	uint32 recruitment_special;
	Int32Vec_t maxpcopygood_a;
	Int32Vec_t maxpcopygood_b;
	
	uint32 checkpoints_cd;	//>>自动推塔cd
	uint32 cleaning_cd;		//>>射雕轮次间隔
	uint32 cleaningcd_clean; //>>射雕清除冷却时间消费表id
	uint32 egg_cd;
	uint32 eggcd_clean;
	uint32 pvp_times;
	uint32 rank_param;
	uint32 tavern_times1;
	uint32 tavern_times2;
	uint32 tavern_cd;
	uint32 tavern_cdmax;
	uint32 tavern_nums;
	Uint32Vec_t pvp_past;
	Uint32Vec_t pvp_past_num;
	Uint32Vec_t energy_reward_time;
	uint32 energy_reward_value;
	uint32 raise_a;
	uint32 raise_b;
	uint32  level_first_strike;
	uint32 sanwei_first_strike_1;
	uint32 sanwei_first_strike_2;
	uint32 sanwei_first_strike_3;
	uint32 race_num;
	uint32 splendid_force_cool_time;
	uint32 splendid_cool_time;
	uint32 splendid_canvass_cool_time;
	uint32 splendid_worship_time;
	uint32 arena_time;
	uint32 raise_max_a;
	uint32 raise_max_b;
	uint32 tresure_shop_limittime;
	uint32 tresure_shop_cd;
	uint32 tavern_nums2;
	Uint32Vec_t tresure_shop_time;
};

//////////////////////////////////////////////////////////////////////////

struct Stage1stResInfo 
{
	Stage1stResInfo()
	: ordinate(0)
	, stage_id(0)
	, type_value(0)
	, sub_type(0)
	, unlock3(0)
	, unlock4(0)
	, level_cap(0)
	, times(0)
	, function_id(0)
	, score(0)
	, energy(0)	
	, max_stage_id(0)
	, level_limit(0)
	{}

	uint32 ordinate;
	uint32 stage_id;
	uint32 type_value;
	uint32 sub_type;
	Uint32Vec_t unlock1;
	Uint32Vec_t unlock2;
	uint32 unlock3;
	vector<uint32> unlock4;
	uint32 level_cap;
	uint32 times;
	uint32 function_id;
	uint32 score;
	uint32 energy;
	uint32 max_stage_id;
	uint32 level_limit;
	uint32 raids_unlock;
};

typedef define_unordered_map<uint32, Stage1stResInfo *> Stage1stResInfoMap_t;

//////////////////////////////////////////////////////////////////////////
struct QuestResInfo
{
	QuestResInfo()
	{}

	uint32 id;
	int32 canbecancel;
	int32 sort;
	uint32 level_unlock;
	int32 type_value;
	uint32 accept_npcid;
	Uint32Vec_t unlock_questid;
	int32 limit_playerlv_low;
	int32 limit_playerlv_high;
	int32 isitemempty;
	int32 ispanterempty;
	int32 sex;
	int32 job;
	int32 sect;
	vector<PairUint32Uint32> acceptdo_item;
	uint32 acceptdo_jump;
	uint32 acceptdo_jumpx;
	uint32 acceptdo_jumpy;
	int32 isdead;
	int32 isoffline;
	uint32 complete_npcid;
	vector<ThreeUint32> complete_req;
	uint32 complete_itemempty;
	int32 completedo_addexp;
	int32 completedo_addteamexp;
	int32 completedo_addfamilyexp;
	int32 completedo_addmoney;
	int32 completedo_addcoin;
	int32 completedo_addframe;
	int32 completedo_addseniority;
	vector<PairUint32Uint32> completedo_additem;

	vector<PairUint32Uint32> completedo_selectitem;

	uint32 completedo_unlockrole;
	Uint32Vec_t completedo_deleteitem;
	uint32 completedo_jumpmap;
	uint32 completedo_jumpx;
	uint32 completedo_jumpy;
	uint32 completedo_selectitemmodel;
	uint32 auto_accept;
};

typedef define_unordered_map<uint32, QuestResInfo *> QuestResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct SynthesisResInfo
{
	SynthesisResInfo()
	{}

	uint32 id;
	uint32 item_id;
	uint32 item_get;
	Uint32Vec_t needitem;
	Uint32Vec_t num;
};

typedef define_unordered_map<uint32, SynthesisResInfo *> SynthesisResInfoMap_t;

//////////////////////////////////////////////////////////////////////////

struct RaiseResInfo 
{
	RaiseResInfo()
	: group_id(0)
	, sequence_id(0)
	, str(0)
	, agi(0)
	, int_value(0)
	{}

	uint32 group_id;
	uint32 sequence_id;
	int32  str;
	int32  agi;
	int32  int_value;
};

typedef vector<RaiseResInfo *>	RaiseResInfoVec_t;

//////////////////////////////////////////////////////////////////////////

struct RaiseRateResInfo 
{
	RaiseRateResInfo()
	: type_value(0)
	, pool_1(0)
	, rate_1(0)
	, pool_2(0)
	, rate_2(0)
	, pool_3(0)
	, rate_3(0)
	, pool_4(0)
	, rate_4(0)
	, pool_5(0)
	, rate_5(0)
	, basecount_1(0)
	, pool_a(0)
	, basecount_2(0)
	, pool_b(0)
	, basecount_0(0)
	, pool_0(0)
	{}

	uint32 type_value;
	uint32 pool_1;
	uint32 rate_1;
	uint32 pool_2;
	uint32 rate_2;
	uint32 pool_3;
	uint32 rate_3;
	uint32 pool_4;
	uint32 rate_4;
	uint32 pool_5;
	uint32 rate_5;
	uint32 basecount_1;
	uint32 pool_a;
	uint32 basecount_2;
	uint32 pool_b;
	uint32 basecount_0;
	uint32 pool_0;
};

typedef vector<RaiseRateResInfo *> RaiseRateResInfoVec_t;

//////////////////////////////////////////////////////////////////////////

struct ShopGrid {
    uint32 shop_id;
    uint32 item_id;
    uint32 item_num;
    uint32 currency;
    uint32 pay_item;
    uint32 pricenum;
	uint32 lable_id;
	uint32 goldblessme_id;
	uint32 limitnum;
	uint32 limittype;
	uint32 vip_price;
	uint32 vip_goldblessme_id;
	uint32 talk;
};

typedef std::vector<ShopGrid*> ShopInfo;

typedef boost::unordered_map<uint32, ShopInfo*> ShopInfoMap;

//////////////////////////////////////////////////////////////////////////

struct ArenaResInfo 
{
	ArenaResInfo()
	: section(0)
	, two_renown_a(0)
	, two_renown_b(0)
	, two_money_a(0)
	, two_money_b(0)
	, two_mana_a(0)
	, two_mana_b(0)
	, week_renown_a(0)
	, week_renown_b(0)
	, week_money_a(0)
	, week_money_b(0)
	, week_mana_a(0)
	, week_mana_b(0)
	, once_renown(0)
	, once_money(0)
	, once_mana(0)
	{}

	uint32 section;
	uint32 two_renown_a;
	uint32 two_renown_b;
	uint32 two_money_a;
	uint32 two_money_b;
	uint32 two_mana_a;
	uint32 two_mana_b;
	uint32 week_renown_a;
	uint32 week_renown_b;
	uint32 week_money_a;
	uint32 week_money_b;
	uint32 week_mana_a;
	uint32 week_mana_b;
	uint32 once_renown;
	uint32 once_money;
	uint32 once_mana;
};

typedef vector<ArenaResInfo *> ArenaResInfoVec_t;

//////////////////////////////////////////////////////////////////////////

struct SpecialShopResInfo 
{
	SpecialShopResInfo()
	: id(0)
	, item_id(0)
	, item_num(0)
	, pricenum(0)
	, currency(0)
	, prompt(0)
	, _weight(0)
	, _player_level(0){}

	uint32 id;
	uint32 item_id;
    uint32 item_num;
    uint32 pricenum;
	uint32 currency;
	uint32 prompt;

	uint32 _weight;
	uint32 _player_level;
};

typedef define_unordered_map<uint32, SpecialShopResInfo *> SpecialShopResInfoMap_t;

struct DailyBonusResInfo 
{
	DailyBonusResInfo()
	: id(0)
	, bonustype(0)
	, a(0)
	, b(0)
	, lv(0)
	, limit_time(0)
	, enable_level(0)
    ,bonus_id(0)
    ,boom(0)
    ,free_a(0)
    ,free_b(0)
    ,boom_num(0)
    ,talk(0)
	{}

	uint32 id;
	uint32 bonustype;
	int32 a;
	int32 b;
	int32 lv;
	uint32 limit_time;
	uint32 enable_level;
    uint32 bonus_id;
    uint32 boom;

    uint32 free_a;
    uint32 free_b;
    Uint32Vec_t critial_num_vec;
    Uint32Vec_t critial_value_vec;
    uint32 boom_num;
    uint32 talk;
};

typedef boost::unordered_map<uint32, DailyBonusResInfo * > DailyBonusResInfoMap_t;

/////////////////////////////////////////////////////////////////////////////////

struct EggResInfo 
{
	EggResInfo()
		: id(0)
		, playerlv(0)
		, type(0)
		, a(0)
		, b(0)
		, weight(0)
	{}

	uint32 id;
	uint32 playerlv;
	uint32 type;
	int32 a;
	int32 b;
	uint32 weight;

};

typedef vector<EggResInfo *> EggResInfoVec_t;
//////////////////////////////////////////////////////////////////////////////////

struct BossGuWuResInfo 
{
	BossGuWuResInfo()
		: type(0)
		, cost(0)
		, rate(0)
		, wait_time(0)
		, wait_timemax(0)
		, buffid(0)
		, effect(0)
	{}

	uint32 type;
	uint32 cost;
	uint32 rate;
	uint32 wait_time;
	uint32 wait_timemax;
	Uint32Vec_t buffid;
	Uint32Vec_t effect;

};

typedef boost::unordered_map<uint32, BossGuWuResInfo * > BossGuWuResInfoMap_t;
//////////////////////////////////////////////////////////////////////////////////

struct BossRewardResInfo 
{
	BossRewardResInfo()
		: enemy(0)
		, money_x1(0)
		, money_a1(0)
		, money_b1(0)
		, frame_x2(0)
		, frame_a2(0)
		, frame_b2(0)
		, prestige_x3(0)
		, prestige_a3(0)
		, prestige_b3(0)
		, substitute_money(0)
		, substitute_frame(0)
		, substitute_prestige(0)
		, substitute_value(0)
		, money(0)
		, frame(0)
		, prestige(0)
	{}

	uint32 enemy;
	uint32 money_x1;
	uint32 money_a1;
	uint32 money_b1;
	uint32 frame_x2;
	uint32 frame_a2;
	uint32 frame_b2;
	uint32 prestige_x3;
	uint32 prestige_a3;
	uint32 prestige_b3;
	uint32 substitute_money;
	uint32 substitute_frame;
	uint32 substitute_prestige;
	uint32 substitute_value;
	Uint32Vec_t money;
	Uint32Vec_t frame;
	Uint32Vec_t prestige;

};

typedef boost::unordered_map<uint32, BossRewardResInfo * > BossRewardResInfoMap_t;
//////////////////////////////////////////////////////////////////////////////////

struct BossForResInfo 
{
	BossForResInfo()
		: map_id(0)
		, enemy_id(0)
		, role_id(0)
		, boss_starttime(0)
		, boss_endtime(0)
	{}

	uint32 map_id;
	uint32 enemy_id;
	uint32 role_id;
	Uint32Vec_t boss_starttime;
	uint32 boss_endtime;
	Uint32Vec_t buff;
};

typedef boost::unordered_map<uint32, BossForResInfo * > BossForResInfoMap_t;
//////////////////////////////////////////////////////////////////////////////
struct BossExpResInfo 
{
	BossExpResInfo()
		: times(0)
		, level(0)
		, hp1(0)
		, bonus1(0)
		, hp2(0)
		, bonus2(0)
		, hp3(0)
		, bonus3(0)
	{}

	uint32 times;
	uint32 level;
	uint32 hp1;
	Uint32Vec_t bonus1;
	uint32 hp2;
	Uint32Vec_t bonus2;
	uint32 hp3;
	Uint32Vec_t bonus3;
};

typedef vector<BossExpResInfo *> BossExpResInfoVec_t;

//////////////////////////////////////////////////////////////////////////////
struct HammerResInfo 
{
	HammerResInfo()
		: id(0)
		, probability(0)
		, critical(0)
		, pay(0)
		, num(0)
	{}

	uint32 id;
	uint32 probability;
	uint32 critical;
	uint32 pay;
	uint32 num;
};

typedef boost::unordered_map<uint32, HammerResInfo * > HammerResInfoMap_t;

//////////////////////////////////////////////////////////////////////////////////
struct CardAndJokerResInfo 
{
	 CardAndJokerResInfo()
		: id(0)
		, type(0)
		, subtype(0)
		, a(0)
		, b(0)
		, probability(0)
		, critical(0)
		, weight(0)
	{}

	uint32 id;
	uint32 type;
	uint32 subtype;
	int32 a;
	int32 b;
	uint32 probability;
	uint32 critical;
	uint32 weight;
};

typedef boost::unordered_map<uint32,  CardAndJokerResInfo * >  CardAndJokerResInfoMap_t;

//////////////////////////////////////////////////////////////////////////////////
// Chakra {{

struct ChakrpLevel {
    uint32 level;
    uint32 exp;         // Exp to next level
    uint32 value;
    uint32 first_strike;
};

struct ChakrpInfo {
    uint32 id;
    uint32 seq;
    uint32 property;
    std::vector<ChakrpLevel> levels;
};

typedef boost::unordered_map<uint32, ChakrpInfo*> ChakrpInfoMap;
typedef std::vector<ChakrpInfo*> ChakrpSortedVec;

struct ChakreInfo {
    uint32 id;
    uint32 point;
    uint32 color;
    uint32 exp;
    uint32 money;
};

typedef boost::unordered_map<uint32, ChakreInfo*> ChakreInfoMap;

struct ChakreKey {
    uint32 point;
    uint32 color;
};

#include <boost/functional/hash.hpp>
static inline bool
operator==(ChakreKey const& a, ChakreKey const& b) {
    return a.point == b.point && a.color == b.color;
}

static inline size_t
hash_value(ChakreKey const& k) {
    size_t seed = 0;
    boost::hash_combine(seed, k.point);
    boost::hash_combine(seed, k.color);
    return seed;
}

typedef boost::unordered_map<ChakreKey, ChakreInfo*> ChakreSelectMap;

struct ChakroWeight {
    uint32 total;
    std::vector<uint32> weights;
};

struct ChakroInfo {
    uint32 id;
    ChakroInfo *next;
    uint32 seq;
    uint32 next_per100;
    uint32 consumption;
    uint32 act_coins;
    uint32 act_max;
    uint32 acc_top;
    struct random_l {
        std::vector<uint32> colors;
        std::vector<uint32> points;
    } rand;
};

typedef boost::unordered_map<uint32, ChakroInfo *> ChakroInfoMap;
typedef std::vector<ChakroInfo*> ChakroSortedVec;

struct VipexpInfo 
{
	VipexpInfo()
		: vip_level(0)
		, vip_exp(0)
	{}

	int vip_level;
	int vip_exp;
};

typedef define_unordered_map<uint32, VipexpInfo *>	VipexpResInfoMap_t;

// }}

#include "ColorStructMap.h"
#include "CondorStructMap.h"
struct AnswerResource;

//////////////////////////////////////////////////////////////////////////
struct VipInfo; 

typedef define_unordered_map<uint32, VipInfo *>	VipInfoMap_t;

//////////////////////////////////////////////////////////////////////////
struct DailyQuestInfo; 

typedef define_unordered_map<uint32, DailyQuestInfo *>	DailyQuestInfoMap_t;

struct DailyQuestLibraryInfo; 

typedef vector<DailyQuestLibraryInfo *> DailyQuestLibraryInfoVec_t;
//////////////////////////////////////////////////////////////////////////
struct SystemUnlockRes;
struct HangUpRes;
struct GiftResource;
struct MultiCopyRes;
typedef define_unordered_map<uint32, MultiCopyRes *>	MultiCopyResMap_t;
//////////////////////////////////////////////////////////////////////////
struct AchievementUnlock;
struct AchievementInfo;
struct AchievementReward;

typedef define_unordered_map<uint32, AchievementUnlock *> AchievementUnlockMap_t;
typedef define_unordered_map<uint32, AchievementInfo *> AchievementInfoMap_t;
typedef define_unordered_map<uint32, AchievementReward *> AchievementRewardMap_t;
//////////////////////////////////////////////////////////////////////////////////
struct DailyPointsEvents;
struct DailyPointsReward;

typedef define_unordered_map<uint32, DailyPointsEvents *> DailyPointsEventsMap_t;
typedef define_unordered_map<uint32, DailyPointsReward *> DailyPointsRewardMap_t;
//////////////////////////////////////////////////////////////////////////////////
struct Guild_Nity;

typedef define_unordered_map<uint32, Guild_Nity *> Guild_NityMap_t;
typedef std::vector<uint32>  Guild_ActivityVector_t;
//////////////////////////////////////////////////////////////////////////////////

struct SweepStageResInfo;

//////////////////////////////////////////////////////////////////////////
struct FlowerRes;

typedef define_unordered_map<uint32, FlowerRes *> FlowerResMap_t;

/***********伙伴双修(江湖一杯酒)**********************/

struct WineTankResInfo;
typedef define_unordered_map<uint32, WineTankResInfo *> WineTankResInfoMap_t;

struct GourdResInfo;
struct StomachResInfo;
struct WineIncreaseResInfo;
struct WineTankUpgradeResInfo;
typedef vector<WineTankResInfo *> WineTankResInfoVec_t;

/**********伙伴强化(训练馆)********************/

struct TrainingPartnersInfo;
typedef define_unordered_map<uint32, TrainingPartnersInfo*> TrainingPartnersInfoMap_t;

struct TrainingLevelTable;
typedef define_unordered_map<uint32, TrainingLevelTable*> TrainingLevelMap_t;
typedef define_unordered_map<uint32, TrainingLevelMap_t*> TrainingQualityMap_t;


/**********仗剑江湖(推图)********************/
struct TowerStageInfo;
typedef define_unordered_map<uint32, TowerStageInfo *>	TowerStageInfoMap_t;
struct TowerBuffInfo;
typedef define_unordered_map<uint32, TowerBuffInfo *>	TowerBuffInfoMap_t;

struct TowerRewardsInfo;
typedef define_unordered_map<uint32, TowerRewardsInfo *>	TowerRewardsInfoMap_t;


/**************************************/

/**************************************/

//押镖
struct EscortTerminalInfo;
typedef define_unordered_map<uint32, EscortTerminalInfo *>	EscortTerminalInfoMap_t;
struct EscortCarInfo;
typedef define_unordered_map<uint32, EscortCarInfo *>	EscortCarInfoMap_t;
struct EscortSystemInfo;

struct EscortAgencyInfo;
typedef define_unordered_map<uint32, EscortAgencyInfo *>	EscortAgencyInfoMap_t;

// free time giving bag
struct FreeTimeBag;
typedef vector<FreeTimeBag *> FreeTimeBagVec_t;

/**********酒馆（begin）********************/
struct TavernInfo;
typedef std::map<uint32, TavernInfo*> TavernInfoMap_t;

struct TavernhireInfo;
typedef std::map<uint32, TavernhireInfo*> TavernhireInfoMap_t;

struct TavernprobabilityInfo;
typedef std::map<uint32, TavernprobabilityInfo*> TavernprobabilityInfoMap_t;

struct TavernshopInfo;
typedef std::vector<TavernshopInfo*> TavernshopInfoVector_t;

struct TavernspebilityInfo;
typedef define_unordered_map<uint32, TavernspebilityInfo*> TavernspebilityInfoMap_t;

struct Tavern_groupInfo;
typedef define_unordered_map<uint32, Tavern_groupInfo *>	Tavern_groupInfoMap_t;
/**********酒馆（end）********************/

/**********帮会战（begin）********************/
struct GvgInfo;
struct GvgrewardsInfo;
typedef define_unordered_map<uint32, GvgrewardsInfo *>	GvgrewardsInfoMap_t;
/**********帮会战（end）********************/

//钓鱼
struct FishingInfo;
typedef define_unordered_map<uint32, FishingInfo *>	FishingInfoMap_t;

//环任务库
struct RingQuestInfo;
typedef vector<RingQuestInfo *> RingQuestInfoVec_t;
//环任务随机奖励
struct RingRandomInfo;
typedef vector<RingRandomInfo *> RingRandomInfoVec_t;
//帮派环任务奖励
struct RingGuildInfo;
typedef define_unordered_map<uint32, RingGuildInfo *>	RingGuildInfoMap_t;
//环任务每环奖励
struct RingRewardInfo;
typedef define_unordered_map<uint32, RingRewardInfo *>	RingRewardInfoMap_t;

// 离线挂机
struct OfflinehangupInfo;

//轻功
struct DodgeInfo;
typedef define_unordered_map<uint32, DodgeInfo *>	DodgeInfoMap_t;
struct PlumInfo;
typedef define_unordered_map<uint32, PlumInfo *>	PlumInfoMap_t;
struct DodgeMiniInfo;
typedef define_unordered_map<uint32, DodgeMiniInfo *>	DodgeMiniInfoMap_t;
struct DodgeRevokeInfo;

struct PlumSystemInfo;
struct PlumRewardInfo;
typedef vector<PlumRewardInfo *> PlumRewardInfoVec_t;
struct PlumJarInfo;
typedef define_unordered_map<uint32, PlumJarInfo *>	PlumJarInfoMap_t;



/****阵营战****/
struct BattlegroundDatetimeInfo;
typedef define_unordered_map<uint32, BattlegroundDatetimeInfo *>	BattlegroundDatetimeInfoMap_t;

struct BattlegroundRewardInfo;
typedef define_unordered_map<uint32, BattlegroundRewardInfo *>	BattlegroundRewardInfoMap_t;

struct BattlegroundBetInfo;
typedef define_unordered_map<uint32, BattlegroundBetInfo *>	BattlegroundBetInfoMap_t;

struct BattlegroundBuffInfo;
typedef define_unordered_map<uint32, BattlegroundBuffInfo *>	BattlegroundBuffInfoMap_t;


// 道具套装 
struct SuitInfo;
typedef define_unordered_map<uint32, SuitInfo *>	SuitInfoMap_t;

struct MateupInfo;
typedef define_unordered_map<uint32, MateupInfo *>	MateupInfoMap_t;

struct MateinheritInfo;
typedef define_unordered_map<uint32, MateinheritInfo *>	MateinheritInfoMap_t;

struct MatecolorInfo;
typedef define_unordered_map<uint32, MatecolorInfo *>	MatecolorInfoMap_t;

struct MategateInfo;
typedef define_unordered_map<uint32, MategateInfo *>	MategateInfoMap_t;

struct MateInfo;
typedef define_unordered_map<uint32, MateInfo *>	MateInfoMap_t;

struct MailInfo;
typedef define_unordered_map<uint32, MailInfo *>	MailInfoMap_t;

struct MateskillInfo;
typedef define_unordered_map<uint32, MateskillInfo *>	MateskillInfoMap_t;

// 武林大会相关
struct PvpInfo;
struct GamblingInfo;
struct PvprewardsInfo;
typedef vector<PvprewardsInfo *> PvprewardsInfoVec_t;
struct TalklinkInfo;
typedef define_unordered_map<uint32, TalklinkInfo *>	TalklinkInfoMap_t;
struct Pvp2Info;

// 宝石镶嵌
struct GemgroupInfo;
typedef vector<GemgroupInfo *> GemgroupInfoVec_t;
struct GemsynthesisInfo;
typedef define_unordered_map<uint32, GemsynthesisInfo *>	GemsynthesisInfoMap_t;

struct Mail_formatInfo;
typedef define_unordered_map<uint32, Mail_formatInfo *>	Mail_formatInfoMap_t;

// 海捕文书
struct ArrestListInfo;
typedef define_unordered_map<uint32, ArrestListInfo *>	ArrestListInfoMap_t;
struct ArrestControllerInfo;
typedef vector<ArrestControllerInfo *> ArrestControllerInfoVec_t;

// 迷宫信息
struct MazeInfo;
typedef define_unordered_map<uint32, MazeInfo *>	MazeInfoMap_t;

struct MazeexplorationInfo;

struct MazerewardsInfo;
typedef vector<MazerewardsInfo *>	MazerewardsInfoVec_t;

struct MazeroomInfo;
typedef vector<MazeroomInfo *>	MazeroomInfoVec_t;

// 送装备
struct Equip_gettingInfo;
typedef define_unordered_map<uint32, Equip_gettingInfo *>	Equip_gettingInfoMap_t;
//define end

#define PELLET_SCALER_NUMBER	1000
#define PELLET_MAKE_SCALER_ID(type_id, grade)	(type_id * PELLET_SCALER_NUMBER + grade)
struct PelletInfoRes;
typedef define_unordered_map<uint32, PelletInfoRes *> PelletInfoResMap_t;


struct AmahInfo;
typedef define_unordered_map<uint32, AmahInfo *>	AmahInfoMap_t;
struct InheritInfoRes;

// 开服活动
struct AccOnlineInfo;
struct SectionBonusInfo;
typedef vector<SectionBonusInfo *> SectionBonusInfoVec_t;
struct FirstBuyInfo;
typedef vector<FirstBuyInfo *> FirstBuyInfoVec_t;
struct AccBuyInfo;
typedef vector<AccBuyInfo *> AccBuyInfoVec_t;
struct NextdayInterestInfo;
typedef vector<NextdayInterestInfo *> NextdayInterestInfoVec_t;
struct FutureBonusInfo;
typedef vector<FutureBonusInfo *> FutureBonusInfoVec_t;
struct ShortcutBonusInfo;

struct BillListInfo;
typedef define_unordered_map<uint32, BillListInfo *>	BillListInfoMap_t;

// 开服活动
struct OpenactInfo;
typedef define_unordered_map<uint32, OpenactInfo *>	OpenactInfoMap_t;

struct FriendAddInfoRes;

//战败原因评分
struct FailScoreInfo;
typedef define_unordered_map<uint32, FailScoreInfo *>	FailScoreInfoMap_t;

struct TitleInfo;
typedef define_unordered_map<uint32, TitleInfo *>	TitleInfoMap_t;

//剑冢
struct JianZhongInfo;
typedef define_unordered_map<uint32, JianZhongInfo *>	JianZhongInfoMap_t;

// 五绝系统
struct SplendidVoteTime;
typedef define_unordered_map<uint32, SplendidVoteTime *> SplendidVoteTimeMap_t;

struct SplendidRankReward;
typedef define_unordered_map<uint32, SplendidRankReward *> SplendidRankRewardMap_t;

struct SplendidWorshipReward;
typedef vector<SplendidWorshipReward *> SplendidWorshipRewardMap_t;


// 排行榜
struct RankingRuleInfo;
typedef define_unordered_map<uint32, RankingRuleInfo *>	RankingRuleInfoMap_t;

struct RankingRewardInfo;
typedef vector<RankingRewardInfo *> RankingRewardVec;
typedef boost::shared_ptr< RankingRewardVec > RankingRewardPtr;
typedef define_unordered_map<uint32, RankingRewardPtr>	RankingRewardInfoMap_t;

struct RankingTalkInfo;
typedef define_unordered_map<uint32, RankingTalkInfo *>	RankingTalkInfoMap_t;

// 切磋
struct RaceInfo;
typedef vector<RaceInfo *> RaceInfoVec_t;

// 推送系统
struct PushInfo;
typedef define_unordered_map<uint32, PushInfo *>	PushInfoMap_t;

// 充值任务
struct ChargequestInfo;

// 开服vip活动
struct Openact1Info;
// 开服 冲级王活动
struct Openact2Info;
typedef vector<Openact1Info *> Openact1InfoVec_t;
typedef vector<Openact2Info *> Openact2InfoVec_t;

// 符文
struct RunesClassInfo;
typedef define_unordered_map<uint32, RunesClassInfo *>	RunesClassInfoMap_t;
struct RunesExpInfo;
typedef define_unordered_map<uint32, RunesExpInfo *>	RunesExpInfoMap_t;
struct RunesLevelInfo;
typedef define_unordered_map<uint32, RunesLevelInfo *>	RunesLevelInfoMap_t;
struct RunesSystemInfo;

// 明教试炼
struct FiresInfo;

struct FiresattackInfo;
typedef define_unordered_map<uint32, FiresattackInfo *>	FiresattackInfoMap_t;


// 圣火令
struct ShenghuoLingInfo;
typedef vector<ShenghuoLingInfo *> ShenghuoLingInfoVec_t;

struct ShlUnlockInfo;
typedef vector<ShlUnlockInfo *> ShlUnlockInfoVec_t;

struct StageExpInfo;
typedef define_unordered_map<uint32, StageExpInfo *>	StageExpInfoMap_t;

struct DoubleexpRes; 
typedef define_unordered_map<uint32, DoubleexpRes *>	DoubleexpResMap_t;

// 武魂 begin
struct SoulSoulInfo;
typedef define_unordered_map<uint32, SoulSoulInfo *>	SoulSoulInfoMap_t;

struct SoulSetInfo;
typedef define_unordered_map<uint32, SoulSetInfo *>	SoulSetInfoMap_t;

struct SoulSuperInfo;
typedef define_unordered_map<uint32, SoulSuperInfo *>	SoulSuperInfoMap_t;

struct SoulChangeInfo;
// 武魂 end

// 圣火异宝商店
struct TreasureShopInfo;
typedef define_unordered_map<uint32, TreasureShopInfo *>	TreasureShopInfoMap_t;

// 武魂 PVE
struct SoulStageInfo;
typedef define_unordered_map<uint32, SoulStageInfo *>	SoulStageInfoMap_t;
struct SoulCfgInfo;

// 丹药合成
struct DanyaoHechengInfo;
typedef define_unordered_map<uint32, DanyaoHechengInfo *>	DanyaoHechengInfoMap_t;

//2v2本服赛系统表
struct PairSystemInfo;

//2v2押注
struct PairGamblingInfo;
typedef vector<PairGamblingInfo *> PairGamblingInfoVec_t;

//2v2称号
struct PairNameInfo;
typedef vector<PairNameInfo *> PairNameInfoVec_t;

//2v2奖励
struct PairRewardsInfo;
typedef vector<PairRewardsInfo *> PairRewardsInfoVec_t;

// 三天登陆送礼
struct LogingiftInfo;

struct GetmoneyInfo; // 百万银两
typedef vector<GetmoneyInfo *> GetmoneyInfoVec_t;

// 目标系统
struct TargetInfo;
typedef define_unordered_map<uint32, TargetInfo *>	TargetInfoMap_t;
struct TargetquestInfo;
typedef vector<TargetquestInfo *> TargetquestInfoVec_t;

// 特权
struct GoldCardInfo;
typedef define_unordered_map<uint32, GoldCardInfo *>	GoldCardInfoMap_t;

// 幸运大转盘
struct LuckyRewardsInfo;
typedef vector<LuckyRewardsInfo *> LuckyRewardsInfoVec_t;
struct LuckyTurntableInfo;


// 资质系统
struct TalentInfo;
typedef define_unordered_map<uint32, TalentInfo *>	TalentInfoMap_t;
struct TalentgroupInfo;
typedef define_unordered_map<uint32, TalentgroupInfo *>	TalentgroupInfoMap_t;
struct TalentsystemInfo;

//平台vip
struct WebVipInfo;

// 骑乘
struct HorseRoomInfo;
typedef define_unordered_map<uint32, HorseRoomInfo *>	HorseRoomInfoMap_t;

struct HorseAttrInfo;
typedef vector<HorseAttrInfo *> HorseAttrInfoVec_t;

struct RanchInfoInfo;
typedef vector<RanchInfoInfo *> RanchInfoInfoVec_t;

struct RanchCfgInfo;
typedef vector<RanchCfgInfo *> RanchCfgInfoVec_t;

struct HorsemanLevelupInfo;
typedef vector<HorsemanLevelupInfo *> HorsemanLevelupInfoVec_t;

struct HelperInfoInfo;
typedef vector<HelperInfoInfo *> HelperInfoInfoVec_t;

struct QqCfgInfo;
typedef define_unordered_map<uint32, QqCfgInfo *>	QqCfgInfoMap_t;

struct QqinviteInfo;
typedef define_unordered_map<uint32, QqinviteInfo *>	QqinviteInfoMap_t;

struct QqshareInfo;

struct QqbragInfo;
typedef define_unordered_map<uint32, QqbragInfo *>	QqbragInfoMap_t;

class Document;
class ManageResource
{
public:
	~ManageResource()
	{}

public:
	virtual bool loadResource(const string & resource_name, Document * * xml_doc) = 0;

	virtual const MapResInfoMap_t & getMapResInfo() = 0;

	virtual const EnemyResInfoMap_t & getEnemyResInfo() = 0;

	virtual const SkillResInfoMap_t & getSkillResInfo() = 0;

	virtual const NpcResInfoMap_t & getNpcResInfo() = 0;

	virtual const FormResInfoMap_t & getFormResInfo() = 0;

	virtual const MonsterResInfoMap_t & getMonsterResInfo() = 0;

	virtual const InitializationCfg & getInitializationCfg() = 0;

	virtual const WarJobEnrageResInfoMap_t & getWarJobEnrageResInfo() = 0;

	virtual const WarInitResInfo & getWarInitResInfo() = 0;

	virtual const PropertyResInfoMap_t & getPropertyResInfo() = 0;

	virtual const RoleResInfoMap_t & getRoleResInfo() = 0;

	virtual const BufferResInfoMap_t & getBufferResInfo() = 0;

	virtual const HireResInfoMap_t & getHireResInfo() = 0;

	virtual const LevelExpResInfoMap_t & getLevelExpResInfo() = 0;

	virtual const ExpAdditionResInfoMap_t & getExpAdditionResInfo() = 0;
	
	virtual const WarScoreResInfoVec_t & getWarScoreResInfo() = 0;

	virtual const LawResInfoVec_t & getLawInfoVec() = 0;

	virtual const ItemResInfoMap_t & getItemInfoMap() = 0;

	virtual const BonusResInfoMap_t & getBonusInfoMap() = 0;

	virtual const SectResInfoMap_t & getSectInfoMap() = 0;

	virtual const PrandomResInfoMap_t & getPrandomInfoMap() = 0;

	virtual const RandomSkillResInfoMap_t & getRandomSkillInfoMap() = 0;

	virtual const GoldParamResInfoMap_t & getGoldParamResInfoMap() = 0;

	virtual const ProhibitWordResVec_t & getProhibitWordResVec() = 0;

	virtual const ProhibitWordResVec_t & getProhibitNameInfoMap() = 0;

	virtual const TalkResInfoMap_t & getTalkResInfoMap() = 0;

	virtual const ItemStrengResInfoMap_t & getItemStrengResInfoMap() = 0;

	virtual const RecruitmentResInfoMap_t & getRecruitmentResInfoMap() = 0;

	virtual const RecruitexpResInfoMap_t & getRecruitexpResInfoMap() = 0;

	virtual const SystemResInfo & getSystemResInfo() = 0;

	virtual const Stage1stResInfoMap_t & getStage1stResInfoMap() = 0;

	virtual const QuestResInfoMap_t & getQuestResInfoMap() = 0;

	virtual const SynthesisResInfoMap_t & getSynthesisResInfo() = 0;

	virtual const RaiseResInfoVec_t & getRaiseResInfo() = 0;

	virtual const RaiseRateResInfoVec_t & getRaiseRateResInfo() = 0;

	virtual const ShopInfoMap & getShopInfoMap() = 0;

	virtual const ArenaResInfoVec_t & getArenaResInfo() = 0;

	virtual const SpecialShopResInfoMap_t & getSpecialShopInfoMap() = 0;

    virtual const ColorInfoMap& getColorInfoMap() = 0;

    virtual const ChakroInfoMap& getChakroInfoMap() = 0;
    virtual const ChakroSortedVec& getChakroSortedInfos() = 0;

    virtual const ChakrpInfoMap& getChakrpInfoMap() = 0;
    virtual const ChakrpSortedVec& getChakrpSortedInfos() = 0;

    virtual const ChakreInfoMap& getChakreInfoMap() = 0;
    virtual const ChakreSelectMap& getChakreSelectMap() = 0;

	virtual const DailyBonusResInfoMap_t & getDailyBonusResInfoMap() = 0;

    virtual const CondorPreyMap& getCondorPreyMap() = 0;
    virtual const CondorWeaponMap& getCondorWeaponMap() = 0;
    virtual const CondorFormula& getCondorFormula() = 0;


	virtual const EggResInfoVec_t & getEggResInfo() = 0;
	virtual const HammerResInfoMap_t & getHammerResInfoMap() = 0;
	virtual const CardAndJokerResInfoMap_t & getCardAndJokerResInfoMap() = 0;

	virtual const VipexpResInfoMap_t & getVipexpResInfoMap() = 0;

	virtual const BossGuWuResInfoMap_t & getBossGuWuResInfoMap() = 0;
	virtual const BossRewardResInfoMap_t & getBossRewardResInfoMap() = 0;
	virtual const BossForResInfoMap_t & getBossForResInfoMap() = 0;
	virtual const BossExpResInfoVec_t & getBossExpResInfo() = 0;

    virtual const AnswerResource& getAnswerResource() = 0;

	virtual const VipInfoMap_t & getVipInfoMap() = 0;

	virtual const SystemUnlockRes & getSystemUnlockInfoMap() = 0;

	virtual const HangUpRes & getHangUpRes() = 0;
    virtual const GiftResource & getGiftResource() = 0;

	virtual const AchievementInfoMap_t & getAchevementInfoMap() = 0;
	virtual const AchievementRewardMap_t & getAchievementRewardMap() = 0;
	virtual const AchievementUnlockMap_t & getAchievementUnlockMap() = 0;

	virtual const DailyPointsEventsMap_t & getDailyPointsEventsMap() = 0;
	virtual const DailyPointsRewardMap_t & getDailyPointsRewardMap() = 0;
	virtual const TavernInfoMap_t& getTavernInfo()	= 0;
	virtual const TavernhireInfoMap_t& getTavernhireInfo() = 0;
	virtual const TavernprobabilityInfoMap_t & getTavernprobabilityInfo() = 0;
	virtual const TavernshopInfoVector_t& getTavernshopInfo() = 0;
	virtual const GvgInfo& getGvgInfo() = 0;
	virtual const GvgrewardsInfoMap_t & getGvgrewardsInfoMap() = 0;
	
	virtual const Guild_NityMap_t & getGuildNityMap() = 0;
	virtual const Guild_ActivityVector_t & getGuildActivity() = 0;

	virtual const SweepStageResInfo & getSweepStageResInfo() = 0;

	virtual const MultiCopyResMap_t & getMultiCopyResMap() = 0;

	virtual const FlowerResMap_t & getFlowerResMap() = 0;

	virtual const WineTankResInfoMap_t & getWineTankResInfoMap() = 0;

	virtual const GourdResInfo & getGourdResInfo() = 0;

	virtual const StomachResInfo & getStomachResInfo() = 0;

	virtual const WineIncreaseResInfo & getWineIncreaseResInfo() = 0;

	virtual const WineTankUpgradeResInfo & getWineTankUpgradeResInfo() = 0;
	
	virtual const TrainingPartnersInfoMap_t& getTrainingPartnersInfoMap() = 0;
	virtual const TrainingQualityMap_t& getTrainingQualityMap() = 0;

	virtual const DailyQuestInfoMap_t & getDailyQuestInfoMap() = 0;
	virtual const DailyQuestLibraryInfoVec_t & getDailyQuestLibraryInfo() = 0;

	virtual const TowerStageInfoMap_t & getTowerStageInfoMap() = 0;
	virtual const TowerBuffInfoMap_t & getTowerBuffInfoMap() = 0;
	virtual const TowerRewardsInfoMap_t & getTowerRewardsInfoMap() = 0;

	virtual const EscortTerminalInfoMap_t & getEscortTerminalInfoMap() = 0;
	virtual const EscortCarInfoMap_t & getEscortCarInfoMap() = 0;
	virtual const EscortSystemInfo & getEscortSystemInfo() = 0;
	virtual const EscortAgencyInfoMap_t & getEscortAgencyInfoMap() = 0;

	virtual const FreeTimeBagVec_t & getFreeTimeBagVec() = 0;

	virtual const FishingInfoMap_t & getFishingInfoMap() = 0;
	
	virtual const RingGuildInfoMap_t & getRingGuildInfoMap() = 0;
	virtual const RingQuestInfoVec_t & getRingQuestInfoVec() = 0;
	virtual const RingRandomInfoVec_t & getRingRandomInfoVec() = 0;
	virtual const RingRewardInfoMap_t & getRingRewardInfoMap() = 0;

	virtual const OfflinehangupInfo & getOfflinehangupInfo() = 0;

	virtual const DodgeInfoMap_t & getDodgeInfoMap() = 0;
	virtual const PlumInfoMap_t & getPlumInfoMap() = 0;
	virtual const DodgeMiniInfoMap_t & getDodgeMiniInfoMap() = 0;
	virtual const DodgeRevokeInfo& getDodgeRevokeInfo() = 0;
	virtual const PlumRewardInfoVec_t & getPlumRewardInfoVec() = 0;

	virtual const PlumJarInfoMap_t & getPlumJarInfoMap() = 0;
	virtual const PlumSystemInfo& getPlumSystemInfo() = 0;

	virtual const BattlegroundDatetimeInfoMap_t & getBattlegroundDatetimeInfoMap() = 0;
	virtual const BattlegroundRewardInfoMap_t & getBattlegroundRewardInfoMap() = 0;
	virtual const BattlegroundBetInfoMap_t & getBattlegroundBetInfoMap() = 0;
	virtual const BattlegroundBuffInfoMap_t & getBattlegroundBuffInfoMap() = 0;
	virtual const TavernspebilityInfoMap_t & getTavernspebilityInfoMap() = 0;
	virtual const SuitInfoMap_t & getSuitInfoMap() = 0;
	virtual const MateupInfoMap_t & getMateupInfoMap() = 0;
	virtual const MateinheritInfoMap_t & getMateinheritInfoMap() = 0;
	virtual const MatecolorInfoMap_t & getMatecolorInfoMap() = 0;
	virtual const MategateInfoMap_t & getMategateInfoMap() = 0;
	virtual const MateInfoMap_t & getMateInfoMap() = 0;
	virtual const MateskillInfoMap_t & getMateskillInfoMap() = 0;
	virtual const MailInfoMap_t & getMailInfoMap() = 0;
	virtual const PvpInfo& getPvpInfo() = 0;
	virtual const Pvp2Info & getPvp2Info() = 0;
	virtual const GamblingInfo& getGamblingInfo() = 0;
	virtual const PvprewardsInfoVec_t & getPvprewardsInfoVec() = 0;
	virtual const Mail_formatInfoMap_t & getMail_formatInfoMap() = 0;
	virtual const Tavern_groupInfoMap_t & getTavern_groupInfoMap() = 0;
	virtual const GemgroupInfoVec_t & getGemgroupInfoVec() = 0;
    virtual const ArrestListInfoMap_t & getArrestListInfoMap() = 0;
    virtual const ArrestControllerInfoVec_t & getArrestControllerInfoVec() = 0;
	virtual const PelletInfoResMap_t & getPelletInfoResMap() = 0;
	virtual const MazeInfoMap_t & getMazeInfoMap() = 0;
	virtual const MazeexplorationInfo& getMazeexplorationInfo() = 0;
	virtual const MazerewardsInfoVec_t & getMazerewardsInfoMap() = 0;
	virtual const MazeroomInfoVec_t & getMazeroomInfoMap() = 0;
	virtual const TalklinkInfoMap_t & getTalklinkInfoMap() = 0;

	virtual const InheritInfoRes & getInheritInfoRes() = 0;
	virtual const AmahInfoMap_t & getAmahInfoMap() = 0;
	virtual const GemsynthesisInfoMap_t & getGemsynthesisInfoMap() = 0;
	virtual const AccOnlineInfo & getAccOnlineInfo() = 0;
    virtual const SectionBonusInfoVec_t & getSectionBonusInfoVec() = 0;
    virtual const FirstBuyInfoVec_t & getFirstBuyInfoVec() = 0;
    virtual const AccBuyInfoVec_t & getAccBuyInfoVec() = 0;
    virtual const NextdayInterestInfoVec_t & getNextdayInterestInfoVec() = 0;
    virtual const FutureBonusInfoVec_t & getFutureBonusInfoVec() = 0;
    virtual const ShortcutBonusInfo & getShortcutBonusInfo() = 0;

	virtual const FriendAddInfoRes & getFriendAddInfoRes() = 0;
	virtual const OpenactInfoMap_t & getOpenactInfoMap() = 0;

	virtual const FailScoreInfoMap_t & getFailScoreInfoMap() = 0;
	virtual const Equip_gettingInfoMap_t & getEquip_gettingInfoMap() = 0;

	virtual const TitleInfoMap_t & getTitleInfoMap() = 0;

	virtual const JianZhongInfoMap_t & getJianZhongInfoMap() = 0;
	virtual const SplendidVoteTimeMap_t & getSplendidVoteTimeMap() = 0;

	virtual const SplendidRankRewardMap_t & getSplendidRankRewardMap() = 0;

	virtual const SplendidWorshipRewardMap_t & getSplendidWorshipRewardMap() = 0;

    virtual const RankingRuleInfoMap_t & getRankingRuleInfoMap() = 0;
    virtual const RankingRewardInfoMap_t & getRankingRewardInfoMap() = 0;

	virtual const RaceInfoVec_t & getRaceInfoVec() = 0;

	virtual const PushInfoMap_t & getPushInfoMap() = 0;

	virtual const ChargequestInfo & getChargequestInfo() = 0;
	virtual const Openact1InfoVec_t & getOpenact1InfoVec() = 0;
	virtual const Openact2InfoVec_t & getOpenact2InfoVec() = 0;
	virtual const RunesClassInfoMap_t & getRunesClassInfoMap() = 0;
	virtual const RunesExpInfoMap_t & getRunesExpInfoMap() = 0;
	virtual const RunesLevelInfoMap_t & getRunesLevelInfoMap() = 0;
	virtual const RunesSystemInfo & getRunesSystemInfo() = 0;
	virtual const FiresInfo & getFiresInfo() = 0;
	virtual const FiresattackInfoMap_t & getFiresattackInfoMap() = 0;
    virtual const ShenghuoLingInfoVec_t & getShenghuoLingInfoVec() = 0;
    virtual const ShlUnlockInfoVec_t & getShlUnlockInfoVec() = 0;
    virtual const StageExpInfoMap_t & getStageExpInfoMap() = 0;

	virtual const DoubleexpResMap_t & getDoubleexpResMap() = 0;
    virtual const RankingTalkInfoMap_t & getRankingTalkInfoMap() = 0;
    virtual const BillListInfoMap_t & getBillListInfoMap() = 0;
	virtual const SoulSoulInfoMap_t & getSoulSoulInfoMap() = 0;
	virtual const SoulSetInfoMap_t & getSoulSetInfoMap() = 0;
	virtual const SoulChangeInfo & getSoulChangeInfo() = 0;
	virtual const TreasureShopInfoMap_t & getTreasureShopInfoMap() = 0;
    virtual const SoulStageInfoMap_t & getSoulStageInfoMap() = 0;
    virtual const SoulCfgInfo & getSoulCfgInfo() = 0;
	virtual const DanyaoHechengInfoMap_t & getDanyaoHechengInfoMap() = 0;
	virtual const PairSystemInfo & getPairSystemInfo() = 0;
	virtual const PairGamblingInfoVec_t & getPairGamblingInfoVec() = 0;
	virtual const PairNameInfoVec_t & getPairNameInfoVec() = 0;
	virtual const PairRewardsInfoVec_t & getPairRewardsInfoVec() = 0;

	virtual const LogingiftInfo & getLogingiftInfo() = 0;
	virtual const SoulSuperInfoMap_t & getSoulSuperInfoMap() = 0;
	virtual const GetmoneyInfoVec_t & getGetmoneyInfoVec() = 0;
	virtual const TargetInfoMap_t & getTargetInfoMap() = 0;
	virtual const TargetquestInfoVec_t & getTargetquestInfoVec() = 0;
	virtual const GoldCardInfoMap_t & getGoldCardInfoMap() = 0;
    virtual const LuckyRewardsInfoVec_t & getLuckyRewardsInfoVec() = 0;
    virtual const LuckyTurntableInfo & getLuckyTurntableInfo() = 0;
	virtual const TalentInfoMap_t & getTalentInfoMap() = 0;
	virtual const TalentgroupInfoMap_t & getTalentgroupInfoMap() = 0;
	virtual const TalentsystemInfo & getTalentsystemInfo() = 0;

	virtual const WebVipInfo & getWebVipInfo() = 0;
    virtual const HorseRoomInfoMap_t & getHorseRoomInfoMap() = 0;
    virtual const HorseAttrInfoVec_t & getHorseAttrInfoVec() = 0;
    virtual const RanchInfoInfoVec_t & getRanchInfoInfoVec() = 0;
    virtual const RanchCfgInfoVec_t & getRanchCfgInfoVec() = 0;
    virtual const HorsemanLevelupInfoVec_t & getHorsemanLevelupInfoVec() = 0;
    virtual const HelperInfoInfoVec_t & getHelperInfoInfoVec() = 0;
    virtual const QqCfgInfoMap_t & getQqCfgInfoMap() = 0;
	virtual const QqinviteInfoMap_t & getQqinviteInfoMap() = 0;
	virtual const QqshareInfo & getQqshareInfo() = 0;
	virtual const QqbragInfoMap_t & getQqbragInfoMap() = 0;
};

#endif
