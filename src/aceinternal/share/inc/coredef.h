#ifndef COREDEF_HPP
#define COREDEF_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include "typedef.h"
//#include "entity/player.pb.h"
//#include "entity_ins.h"
//#include "protocol_ins.h"

#define MAKE_GUID(type, seed, id)			(((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(seed) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

#define SWITCH_GUID_TYPE(new_type, guid)	(((uint64(new_type) << 56) & 0xFF00000000000000) | uint64(guid) & 0X00FFFFFFFFFFFFFF)

///////////////////     map guid          ////////////////////////////////
#define MAKE_MAP_GUID(line_no, repduplicate_index, template_id)			MAKE_GUID(0xC, 15, (uint64)line_no << 40 | repduplicate_index << 24 | (0x0000000000FFFFFF & (uint64)template_id))

///////////////////     entity guid       ////////////////////////////////
#define MAKE_ENTITY_GUID(entity_type, seed, index_value)				MAKE_GUID(entity_type, seed, index_value)

#define EXTRACT_ENTITY_TYPE(guid) int((guid & 0xFF00000000000000) >> 56)

/////////////////       entity instance, entity dynamic cast///////////////
#define MAKE_ENTITY_INSTANCE(entity_type, entity_ins) entity_type * entity_ins = new entity_type()

#define ENTITY_DYNAMIC_CAST(entity_type, entity_dst, entity_src)	entity_type * entity_dst = dynamic_cast<entity_type *>(entity_src)

//////////////////////////////////////////////////////////////////////////
#define GUID_TYPE(guid)	(uint8)((uint64(guid) >> 56) & 0x00000000000000FF)

#define IS_PLAYER_GUID(guid)	(GUID_TYPE(guid) == ET_PLAYER)

#define IS_ROLE_GUID(guid)		(GUID_TYPE(guid) == ET_ROLE)

#define IS_MONSTOR_GUID(guid)	(GUID_TYPE(guid) == ET_MONSTER)

//////////////////////////////////////////////////////////////////////////
#define TEN_THOUSAND_PERCENT_BASE	10000.0

#define HUNDRED_PERCENT_BASE	100.0

#define QUALIFY_MAP_ID 99900
#define MAZE_SCENE_ID		99901			// maze场景id.
//////////////////////////////////////////////////////////////////////////
#define MAX_BAG_NUMBER			108

#define INIT_BAG_NUMBER			18

#define MAX_STOREHOUSE_NUMBER	96

#define INIT_STOREHOUSE_NUMBER	8

enum EntityType
{
	ET_NULL = 0,
	ET_PLAYER,
	ET_ROLE,
	ET_ATTACH,
	ET_FORM,
	ET_BUFFER,
	ET_MONSTER,
	ET_NPC,
	ET_ITEM,
	ET_TASK,
	ET_TEAM,
	ET_LAW,
	ET_SECT,
	ET_STHENO,
	ET_STAGE,
	ET_ANNOUNCE,
	ET_TASKBRIEF,
	ET_TOWN_STAGE,
	ET_EDUCATE,
	ET_EDUCATETIMES,
	ET_PVP,
	ET_PVPCONTAINER,
	ET_SPECSHOP,
	ET_SHORT,
	ET_SHORTPLAYERS,
	ET_CHAKRA,
	ET_CHAKRE,
	ET_WORSHIP,
	ET_INFO,
	ET_MAILBOX,
	ET_MAIL,			
	ET_MAILCONTAINER,
	ET_EGGS,
	ET_CONDOR,
	ET_BOSS,
	ET_VIP,
	ET_ANSWER,
	ET_ADDICTION,
	ET_PLAYERBUFFER,
	ET_GIFT,
	ET_HANG,
	ET_MULTICOPY,
	ET_FRIEND,
	ET_ACHIEVEMENT,
	ET_PRACTICE,
	ET_DAILYPOINTS,
	ET_GUILD,
	ET_TOWER,
	ET_GUILDS,
	ET_ESCORT,
	ET_TAVERN,
	ET_GUILDBATTLE,
	ET_FISH,
	ET_DODGE,
	ET_BATTLEGROUND,
	ET_WIFE,				// 侠侣
	ET_PLAYERWIFE,			// 玩家侠侣数据
	ET_ACTIVIES,			// 玩家侠侣数据
	ET_ACTIVE,				// 全局活动数据
	ET_ACTIVE_CONTAINER,	// 全局活动容器
	ET_QUALIFYBETS,			// 武林大会投注
	ET_QUALIFYLOGS,			// 舞林大会日志
	ET_QUALIFYBATTLE,		// 武林大会数据
	ET_PELLET,				// 丹药
	ET_MAZE,				// 秘境奇缘
    ET_ARREST,              // 海捕文书 全局相关
    ET_ARRESTSHORTINFO,     // 海捕文书 player相关
    ET_OPENACTIVITY,        // 开服活动
	ET_COIN,
	ET_OPENACT,				// 开服活动
	ET_OPENACTCONTAINER,	// 开服活动
	ET_TITLEBRIEF,			// 称号
	ET_TITLE_CONTAINER,		// 称号容器
    ET_RANKINGS,
	ET_EXCHANGECONTAINER,	// 切磋容器
	ET_EXCHANGES,			// 切磋
	ET_JIANZHONG,			// 剑冢
	ET_SPLENDID,
	ET_SPLENDIDINFO,
	ET_OPENACT1,			// 开服活动1（vip活动，冲级王活动）
	ET_TRIALCONTAINER,
	ET_TRIAL,
	ET_RUNES,				// 符文系统
	ET_CONTAINER_CONTAINER,
	ET_CONTAINER,
    ET_SHL_SWITCH,
    ET_SHL_OWNER,
	ET_DOUBLEEXP,
    ET_SOULPVE,
	ET_LOGINGIFT,			// 三日登录领奖
	ET_SOUL,
	ET_SUPERPOWER,			// 超级战力
	ET_GOLDCARD,			// 特权系统
	ET_PAIR_TEAM,			//2v2战队实体
	ET_PAIR_SELF,			//2v2个人实体
	ET_PAIR_BATTLE,			//2v2主体
	ET_PAIR_BETS,			//下注
	ET_PAIR_LOGS,			//日志
    ET_TURNTABLE,
	ET_TALENT,				// 资质
	ET_LINESHOP,			// line shop
	ET_LINETAVERN,			// line tavern(酒馆)
    ET_HORSE,
	ET_QQSPREADLINE,		// qq推广Line
	ET_QQSPREADPLAYER,		// player挂载实体
};

enum PlayerAttrType
{
	PAT_BEGIN = 0,
	PAT_EXP_PERCENT,
	PAT_ENDING,
};

enum RoleAttrType
{
	RAT_BEGIN = 0,
	RAT_HP,						// 生命值 : 初始值、装备、等级提升、命格、buffer、被动技能
	RAT_CURR_HP,				// 当前生命值
	RAT_STR,					// 外功	  : 初始值、装备、命格、培养、丹药、buffer、被动技能
	RAT_AGI,					// 内功	  : 初始值、装备、命格、培养、丹药、buffer、被动技能
	RAT_INT,					// 奇术	  : 初始值、装备、命格、培养、丹药、buffer、被动技能
	RAT_STR_ATT_BASE,			// 外功攻击基础值，所有加法的加法计算
	RAT_STR_ATT_PERCENT,		// 外功攻击百分比，所有百分比的加法计算
	RAT_STR_ATTACK,				// 外功攻击 : 内功攻击基础值 * 内功攻击百分比 ： 初始值、装备、命格、基础属性、buffer、被动技能
	RAT_AGI_ATT_BASE,
	RAT_AGI_ATT_PERCENT,
	RAT_AGI_ATTACK,				// 内功攻击
	RAT_INT_ATT_BASE,			
	RAT_INT_ATT_PERCENT,
	RAT_INT_ATTACK,				// 奇术攻击
	RAT_STR_DEF_BASE,
	RAT_STR_DEF_PERCENT,
	RAT_STR_DEFENSE,			// 外功防御
	RAT_AGI_DEF_BASE,
	RAT_AGI_DEF_PERCENT,
	RAT_AGI_DEFENSE,			// 内功防御
	RAT_INT_DEF_BASE,
	RAT_INT_DEF_PERCENT,
	RAT_INT_DEFENSE,			// 奇术防御

	RAT_CRITICAL,				// 暴击			：　初始值、装备、命格、buffer、被动技能
	RAT_CRITICAL_MULTIPLE,		// 暴击倍数
	RAT_BLOCK,					// 格挡
	RAT_BLOCK_DEFENCE_MULTIPLE,	// 格挡减免伤害倍率
	RAT_BLOCK_DAMAGE_MULTIPLE,	// 格挡伤害倍数
	RAT_DODGE,					// 闪避
	RAT_HIT,					// 命中
	RAT_FORCE_VALUE,			// 武力值
	RAT_ENRAGE,					// 气势
	RAT_STRIKE_BACK,			// 反击
	RAT_HP_BASE,				// 生命基础值
	RAT_HP_PERCENT,				// 生命百分比
	RAT_FIGHT_POWER,			// 角色战斗力

	RAT_ATTACK_BACK,			//攻击反弹
	RAT_ATTACK_TRANSFER,		//攻击转移
	RAT_ATTACK_RAT_MISS,		//概率无视攻击
	RAT_HURT_INC_WARRIOR,		//对刀客伤害增加
	RAT_HURT_INC_SWORDMAN,		//对剑侠伤害增加
	RAT_HURT_INC_JT_JEBE,		//对翎羽伤害增加
	RAT_HURT_INC_HERCULES,		//对力士伤害增加
	RAT_HURT_INC_MUSICIAN,		//对乐师伤害增加
	RAT_HURT_REDUCE_WARRIOR,	//对刀客防御增加
	RAT_HURT_REDUCE_SWORDMAN,	//对剑侠防御增加
	RAT_HURT_REDUCE_JT_JEBE,	//对翎羽防御增加
	RAT_HURT_REDUCE_HERCULES,	//对力士防御增加
	RAT_HURT_REDUCE_MUSICIAN,	//对乐师防御增加

	RAT_HURT_INC_STR,			//外功攻击对所有人造成伤害
	RAT_HURT_INC_AGI,			//内功攻击对所有人造成伤害
	RAT_HURT_INC_INT,			//奇术攻击对所有人造成伤害
	RAT_HURT_REDUCE_STR,		//受到所有人外公攻击伤害减少
	RAT_HURT_REDUCE_AGI,		//受到所有人内功攻击伤害减少
	RAT_HURT_REDUCE_INT,		//受到所有人奇术攻击伤害减少

	RAT_CURE_ABSOLUTE,			//治疗效果绝对值
	RAT_CURE_PERCENT,			//治疗效果百分比
	RAT_BE_CURE_ABSOLUTE,		//受治疗效果绝对值
	RAT_BE_CURE_PERCENT,		//受治疗效果百分比
	RAT_BE_SUCK_HP,				//被吸取生命
	RAT_SUCK_HP,				//吸取生命
	RAT_MULTI_HIT,				//连击概率
	RAT_UNION_HIT,				//合击概率
	RAT_MULTI_HIT_ADDI,			//连击伤害加成
	RAT_UNION_HIT_ADDI,			//合击伤害加成
	RAT_MULTI_HIT_REDUCE,		//降低连击伤害
	RAT_UNION_HIT_REDUCE,		//降低合击伤害

	RAT_CRITICAL_RANK,			// 暴击等级
	RAT_CRITICAL_MULTIPLE_RANK,	// 暴击伤害等级
	RAT_BLOCK_RANK,				// 格挡等级
	RAT_BLOCK_DEFENCE_MULTIPLE_RANK,	// 格挡减伤等级
	RAT_STRIKE_BACK_RANK,		// 格挡反击等级
	RAT_HIT_RANK,				// 命中等级
	RAT_DODGE_RANK,				// 闪避等级
	RAT_MULTI_HIT_RANK,			// 连击等级
	RAT_UNION_HIT_RANK,			// 合击等级
	RAT_MULTI_HIT_ADDI_RANK,	// 连击伤害等级
	RAT_UNION_HIT_ADDI_RANK,	// 合击伤害等级

	RAT_INITIATIVE_ALL,			// 总先攻值
	RAT_INITIATIVE_STHEN,		// 强化先攻值
	RAT_INITIATIVE_COLOR,		// 幻化先攻值
	RAT_INITIATIVE_LAW,			// 心法先攻值
	RAT_INITIATIVE_EDUCATE,		// 三围培养先攻值
	RAT_INITIATIVE_DODGE,		// 轻功先攻值
	RAT_INITIATIVE_CHAKRE,		// 脉轮先攻值
	RAT_INITIATIVE_TRAINING,	// 武馆训练先攻值
	RAT_INITIATIVE_LEVEL,		// 等级先攻值

	RAT_UNCRITICAL_RANK,		// 韧性等级
	RAT_UNBLOCK_RANK,			// 破击等级
	RAT_UNCRITICAL,				// 韧性几率，暴击的反属性
	RAT_UNBLOCK,				// 破击几率，格挡的反属性

	RAT_ENDING,
};

enum JobType
{
	JT_BASE,
	JT_WARRIOR,					// 刀客
	JT_SWORDMAN,				// 剑侠
	JT_JEBE,					// 翎羽
	JT_HERCULES,				// 力士
	JT_MUSICIAN,				// 乐师
};

//////////////////////////////////////////////////////////////////////////

enum SkillType
{
	ST_BASE = 0,
	ST_NORMAL_ATTACK,
	ST_EXTREMELY_ATTACK,
	ST_WONDER_ATTACK,
	ST_PASSIVE_ATTACK,
};

enum SkillTargetType
{
	STT_BASE = 0,					
	STT_SAME_ROW_NEAR,				//
	STT_SAME_ROW_FARTHER,			//
	STT_SAME_ROW_FARTHEST,			//
	STT_NEAR_ENEMY_SAME_ROW,		//
	STT_NEAR_ENEMY_SAME_COLUMNA,	//
	STT_WHOLE_ENEMY,				//
	STT_WHOLE_SELF,					//
	STT_WHOLE_ENEMY_WITH_ENRAGE,	//
	STT_WHOLE_SELF_WITH_ENRAGE,		//
	STT_WHOLE_ENEMY_NO_ENRAGE,		//
	STT_ONLY_SELF,					//
	STT_LOWEST_HP_SELF,				//
	STT_WHOLE_SELF_EXCEPT_ME,		//
	STT_SELF_RANDOM_NOT_MUSICIAN,	//
	STT_TARGET_AROUND,				//
	STT_ENEMY_RANDOM_ONE,			//16 敌方随机1人
	STT_ENEMY_RANDOM_TWO,			//17 敌方随机2人
	STT_ENEMY_RANDOM_THREE,			//18 敌方随机3人
	STT_ENEMY_RANDOM_FOUR,			//19 敌方随机4人
	STT_ENEMY_HP_LOWEST,			//20 敌方最低血量
	STT_ENEMY_HP_HIGHEST,			//21 敌方最高血量
	STT_ENEMY_HP_LOWEST_COLUMN,		//22 攻击血量最低所在列
	STT_ENEMY_COLUMNA_FARTHEST,		//23 攻击最后一排
	STT_SELF_FIRST_COLUMN,			//24 己方第一排

};

enum SkillOtherFlag
{
	SOF_HURT_SELF = 0x1,
};

#define IS_SOF_HURT_SELF(other_flag)	(other_flag & SOF_HURT_SELF)

//////////////////////////////////////////////////////////////////////////

enum WarRoleSkillStatus
{
	WSS_IS_HIT			= 0x1,
	WSS_IS_CRIT			= 0x2,
	WSS_IS_BLOCK		= 0x4,
	WSS_IS_BEATBACK		= 0x8,
	WSS_IS_EXTREMELY	= 0x10,
};

//////////////////////////////////////////////////////////////////////////

enum BufferForbidState
{
	BFS_ACTION					= 0x1,
	BFS_NORMAL_SKILL			= 0x2,
	BFS_EXTREMELY_SKILL			= 0x4,
	BFS_SKILL_INCREASE_ENRAGE	= 0x8,
	BFS_BUFFER_INCREASE_ENRAGE	= 0x10,
	BFS_WAR_JOB_INCREASE_ENRAGE	= 0x20,
	BFS_SKILL_REDUCE_ENRAGE		= 0x40,
	BFS_BUFFER_REDUCE_ENRAGE	= 0x80,
	BFS_WAR_JOB_REDUCE_ENRAGE	= 0x100,
	BFS_SKILL_CURE				= 0x200,
	BFS_BUFFER_CURE				= 0x400,
	BFS_SKILL_DAMAGE_1			= 0x800,
	BFS_BUFFER_DAMAGE_1			= 0x1000,
	BFS_SKILL_DAMAGE_2			= 0x2000,
	BFS_BUFFER_DAMAGE_2			= 0x4000,
	BFS_ADDITIONAL_DAMAGE		= 0x8000,
	BFS_BUFFER_CONFUSION		= 0x10000,
	BFS_BUFFER_SNEER			= 0x20000,
};

#define IS_BFS_ACTION(buffer_feature)					(buffer_feature & BFS_ACTION)
#define IS_BFS_NORMAL_SKILL(buffer_feature)				(buffer_feature & BFS_NORMAL_SKILL)
#define IS_BFS_EXTREMELY_SKILL(buffer_feature)			(buffer_feature & BFS_EXTREMELY_SKILL)
#define IS_BFS_SKILL_INCREASE_ENRAGE(buffer_feature)	(buffer_feature & BFS_SKILL_INCREASE_ENRAGE)
#define IS_BFS_BUFFER_INCREASE_ENRAGE(buffer_feature)	(buffer_feature & BFS_BUFFER_INCREASE_ENRAGE)
#define IS_BFS_WAR_JOB_INCREASE_ENRAGE(buffer_feature)	(buffer_feature & BFS_WAR_JOB_INCREASE_ENRAGE)
#define IS_BFS_SKILL_REDUCE_ENRAGE(buffer_feature)		(buffer_feature & BFS_SKILL_REDUCE_ENRAGE)
#define IS_BFS_BUFFER_REDUCE_ENRAGE(buffer_feature)		(buffer_feature & BFS_BUFFER_REDUCE_ENRAGE)
#define IS_BFS_WAR_JOB_REDUCE_ENRAGE(buffer_feature)	(buffer_feature & BFS_WAR_JOB_REDUCE_ENRAGE)
#define IS_BFS_SKILL_CURE(buffer_feature)				(buffer_feature & BFS_SKILL_CURE)
#define IS_BFS_BUFFER_CURE(buffer_feature)				(buffer_feature & BFS_BUFFER_CURE)
#define IS_BFS_SKILL_DAMAGE_1(buffer_feature)			(buffer_feature & BFS_SKILL_DAMAGE_1)
#define IS_BFS_BUFFER_DAMAGE_1(buffer_feature)			(buffer_feature & BFS_BUFFER_DAMAGE_1)
#define IS_BFS_SKILL_DAMAGE_2(buffer_feature)			(buffer_feature & BFS_SKILL_DAMAGE_2)
#define IS_BFS_BUFFER_DAMAGE_2(buffer_feature)			(buffer_feature & BFS_BUFFER_DAMAGE_2)
#define IS_BFS_BUFFER_CONFUSION(buffer_feature)			(buffer_feature & BFS_BUFFER_CONFUSION)
#define IS_BFS_BUFFER_SNEER(buffer_feature)				(buffer_feature & BFS_BUFFER_SNEER)

//////////////////////////////////////////////////////////////////////////
enum BufferDisappearFeature
{
	BDF_BASE			= 0,
	BDF_JUMP_MAP		= 0x1,
	BDF_AFTER_BATTLE	= 0x2,
	BDF_LOGOFF			= 0x4,
	BDF_DIE				= 0x8,
	BDF_ENABLE_CRITICAL = 0x10,
	BDF_AFTER_ATTACKED	= 0x20,
};

#define IS_BDF_JUMP_MAP(disappear_feature)			(disappear_feature & BDF_JUMP_MAP)
#define IS_BDF_AFTER_BATTLE(disappear_feature)		(disappear_feature & BDF_AFTER_BATTLE)
#define IS_BDF_LOGOFF(disappear_feature)			(disappear_feature & BDF_LOGOFF)
#define IS_BDF_DIE(disappear_feature)				(disappear_feature & BDF_DIE)
#define IS_BDF_ENABLE_CRITICAL(disappear_feature)	(disappear_feature & BDF_ENABLE_CRITICAL)
#define IS_BDF_AFTER_ATTACKED(disappear_feature)	(disappear_feature & BDF_AFTER_ATTACKED)

//////////////////////////////////////////////////////////////////////////

class Packet;
typedef boost::function<void()> TimerCallBack;
typedef std::set<uint64>	GuidSet_t;

struct MapEntity
{
	MapEntity()
	: map_type(0)
	{}

	int map_type;
	GuidSet_t	players;
	GuidSet_t	monsters;
	GuidSet_t	npcs;
};

class Timer
{
	virtual long schemeTimer(int interval_value, TimerCallBack timer_callback) = 0;

	virtual long cancelTimer(long timer_id) = 0;
};

// 前置声明
namespace typed
{
namespace entity
{
	class Player;
};
};

class ManageGrid
{
public:
	virtual void add(typed::entity::Player * player) = 0;

	virtual void readd(typed::entity::Player * player) = 0;

	virtual void move(typed::entity::Player * player) = 0;

	virtual void remove(typed::entity::Player * player) = 0;

	virtual void levelup(typed::entity::Player * player) = 0;

	virtual void flowerRank(typed::entity::Player * player) = 0;

	virtual void updateGuildName(typed::entity::Player * player) = 0;

	virtual void updateFollowRoleID(typed::entity::Player * player) = 0;

	virtual void updateTitle(typed::entity::Player * player) = 0;

	virtual void updateSpecialRight(typed::entity::Player * player) = 0;

	virtual void updateHorse(typed::entity::Player * player)  = 0;

	virtual void updateWifeFollow(typed::entity::Player * player)  = 0;

	virtual void updateYellowVipLevel(typed::entity::Player * player) = 0;

	virtual void broadcastPacket(Packet * packet) = 0;
};

class DataRecord
{
public:
	virtual void record(int scene_id, int module_id, int src_from, uint64 guid, const string & name, int level, uint64 param_1 = 0, uint64 param_2 = 0, uint64 param_3 = 0, uint64 param_4 = 0, uint64 param_5 = 0, uint64 param_6 = 0, uint64 param_7 = 0, uint64 param_8 = 0, uint64 param_9 = 0, uint64 param_10 = 0) = 0;

	virtual void record(int scene_id, int module_id, int src_from, uint64 guid, uint64 param_1 = 0, uint64 param_2 = 0, uint64 param_3 = 0, uint64 param_4 = 0, uint64 param_5 = 0, uint64 param_6 = 0, uint64 param_7 = 0, uint64 param_8 = 0, uint64 param_9 = 0, uint64 param_10 = 0) = 0;
};

#endif