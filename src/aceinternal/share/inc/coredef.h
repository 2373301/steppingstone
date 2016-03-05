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
#define MAZE_SCENE_ID		99901			// maze����id.
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
	ET_WIFE,				// ����
	ET_PLAYERWIFE,			// �����������
	ET_ACTIVIES,			// �����������
	ET_ACTIVE,				// ȫ�ֻ����
	ET_ACTIVE_CONTAINER,	// ȫ�ֻ����
	ET_QUALIFYBETS,			// ���ִ��Ͷע
	ET_QUALIFYLOGS,			// ���ִ����־
	ET_QUALIFYBATTLE,		// ���ִ������
	ET_PELLET,				// ��ҩ
	ET_MAZE,				// �ؾ���Ե
    ET_ARREST,              // �������� ȫ�����
    ET_ARRESTSHORTINFO,     // �������� player���
    ET_OPENACTIVITY,        // �����
	ET_COIN,
	ET_OPENACT,				// �����
	ET_OPENACTCONTAINER,	// �����
	ET_TITLEBRIEF,			// �ƺ�
	ET_TITLE_CONTAINER,		// �ƺ�����
    ET_RANKINGS,
	ET_EXCHANGECONTAINER,	// �д�����
	ET_EXCHANGES,			// �д�
	ET_JIANZHONG,			// ��ڣ
	ET_SPLENDID,
	ET_SPLENDIDINFO,
	ET_OPENACT1,			// �����1��vip����弶�����
	ET_TRIALCONTAINER,
	ET_TRIAL,
	ET_RUNES,				// ����ϵͳ
	ET_CONTAINER_CONTAINER,
	ET_CONTAINER,
    ET_SHL_SWITCH,
    ET_SHL_OWNER,
	ET_DOUBLEEXP,
    ET_SOULPVE,
	ET_LOGINGIFT,			// ���յ�¼�콱
	ET_SOUL,
	ET_SUPERPOWER,			// ����ս��
	ET_GOLDCARD,			// ��Ȩϵͳ
	ET_PAIR_TEAM,			//2v2ս��ʵ��
	ET_PAIR_SELF,			//2v2����ʵ��
	ET_PAIR_BATTLE,			//2v2����
	ET_PAIR_BETS,			//��ע
	ET_PAIR_LOGS,			//��־
    ET_TURNTABLE,
	ET_TALENT,				// ����
	ET_LINESHOP,			// line shop
	ET_LINETAVERN,			// line tavern(�ƹ�)
    ET_HORSE,
	ET_QQSPREADLINE,		// qq�ƹ�Line
	ET_QQSPREADPLAYER,		// player����ʵ��
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
	RAT_HP,						// ����ֵ : ��ʼֵ��װ�����ȼ�����������buffer����������
	RAT_CURR_HP,				// ��ǰ����ֵ
	RAT_STR,					// �⹦	  : ��ʼֵ��װ����������������ҩ��buffer����������
	RAT_AGI,					// �ڹ�	  : ��ʼֵ��װ����������������ҩ��buffer����������
	RAT_INT,					// ����	  : ��ʼֵ��װ����������������ҩ��buffer����������
	RAT_STR_ATT_BASE,			// �⹦��������ֵ�����мӷ��ļӷ�����
	RAT_STR_ATT_PERCENT,		// �⹦�����ٷֱȣ����аٷֱȵļӷ�����
	RAT_STR_ATTACK,				// �⹦���� : �ڹ���������ֵ * �ڹ������ٷֱ� �� ��ʼֵ��װ�������񡢻������ԡ�buffer����������
	RAT_AGI_ATT_BASE,
	RAT_AGI_ATT_PERCENT,
	RAT_AGI_ATTACK,				// �ڹ�����
	RAT_INT_ATT_BASE,			
	RAT_INT_ATT_PERCENT,
	RAT_INT_ATTACK,				// ��������
	RAT_STR_DEF_BASE,
	RAT_STR_DEF_PERCENT,
	RAT_STR_DEFENSE,			// �⹦����
	RAT_AGI_DEF_BASE,
	RAT_AGI_DEF_PERCENT,
	RAT_AGI_DEFENSE,			// �ڹ�����
	RAT_INT_DEF_BASE,
	RAT_INT_DEF_PERCENT,
	RAT_INT_DEFENSE,			// ��������

	RAT_CRITICAL,				// ����			������ʼֵ��װ��������buffer����������
	RAT_CRITICAL_MULTIPLE,		// ��������
	RAT_BLOCK,					// ��
	RAT_BLOCK_DEFENCE_MULTIPLE,	// �񵲼����˺�����
	RAT_BLOCK_DAMAGE_MULTIPLE,	// ���˺�����
	RAT_DODGE,					// ����
	RAT_HIT,					// ����
	RAT_FORCE_VALUE,			// ����ֵ
	RAT_ENRAGE,					// ����
	RAT_STRIKE_BACK,			// ����
	RAT_HP_BASE,				// ��������ֵ
	RAT_HP_PERCENT,				// �����ٷֱ�
	RAT_FIGHT_POWER,			// ��ɫս����

	RAT_ATTACK_BACK,			//��������
	RAT_ATTACK_TRANSFER,		//����ת��
	RAT_ATTACK_RAT_MISS,		//�������ӹ���
	RAT_HURT_INC_WARRIOR,		//�Ե����˺�����
	RAT_HURT_INC_SWORDMAN,		//�Խ����˺�����
	RAT_HURT_INC_JT_JEBE,		//�������˺�����
	RAT_HURT_INC_HERCULES,		//����ʿ�˺�����
	RAT_HURT_INC_MUSICIAN,		//����ʦ�˺�����
	RAT_HURT_REDUCE_WARRIOR,	//�Ե��ͷ�������
	RAT_HURT_REDUCE_SWORDMAN,	//�Խ�����������
	RAT_HURT_REDUCE_JT_JEBE,	//�������������
	RAT_HURT_REDUCE_HERCULES,	//����ʿ��������
	RAT_HURT_REDUCE_MUSICIAN,	//����ʦ��������

	RAT_HURT_INC_STR,			//�⹦����������������˺�
	RAT_HURT_INC_AGI,			//�ڹ�����������������˺�
	RAT_HURT_INC_INT,			//��������������������˺�
	RAT_HURT_REDUCE_STR,		//�ܵ��������⹫�����˺�����
	RAT_HURT_REDUCE_AGI,		//�ܵ��������ڹ������˺�����
	RAT_HURT_REDUCE_INT,		//�ܵ����������������˺�����

	RAT_CURE_ABSOLUTE,			//����Ч������ֵ
	RAT_CURE_PERCENT,			//����Ч���ٷֱ�
	RAT_BE_CURE_ABSOLUTE,		//������Ч������ֵ
	RAT_BE_CURE_PERCENT,		//������Ч���ٷֱ�
	RAT_BE_SUCK_HP,				//����ȡ����
	RAT_SUCK_HP,				//��ȡ����
	RAT_MULTI_HIT,				//��������
	RAT_UNION_HIT,				//�ϻ�����
	RAT_MULTI_HIT_ADDI,			//�����˺��ӳ�
	RAT_UNION_HIT_ADDI,			//�ϻ��˺��ӳ�
	RAT_MULTI_HIT_REDUCE,		//���������˺�
	RAT_UNION_HIT_REDUCE,		//���ͺϻ��˺�

	RAT_CRITICAL_RANK,			// �����ȼ�
	RAT_CRITICAL_MULTIPLE_RANK,	// �����˺��ȼ�
	RAT_BLOCK_RANK,				// �񵲵ȼ�
	RAT_BLOCK_DEFENCE_MULTIPLE_RANK,	// �񵲼��˵ȼ�
	RAT_STRIKE_BACK_RANK,		// �񵲷����ȼ�
	RAT_HIT_RANK,				// ���еȼ�
	RAT_DODGE_RANK,				// ���ܵȼ�
	RAT_MULTI_HIT_RANK,			// �����ȼ�
	RAT_UNION_HIT_RANK,			// �ϻ��ȼ�
	RAT_MULTI_HIT_ADDI_RANK,	// �����˺��ȼ�
	RAT_UNION_HIT_ADDI_RANK,	// �ϻ��˺��ȼ�

	RAT_INITIATIVE_ALL,			// ���ȹ�ֵ
	RAT_INITIATIVE_STHEN,		// ǿ���ȹ�ֵ
	RAT_INITIATIVE_COLOR,		// �û��ȹ�ֵ
	RAT_INITIATIVE_LAW,			// �ķ��ȹ�ֵ
	RAT_INITIATIVE_EDUCATE,		// ��Χ�����ȹ�ֵ
	RAT_INITIATIVE_DODGE,		// �Ṧ�ȹ�ֵ
	RAT_INITIATIVE_CHAKRE,		// �����ȹ�ֵ
	RAT_INITIATIVE_TRAINING,	// ���ѵ���ȹ�ֵ
	RAT_INITIATIVE_LEVEL,		// �ȼ��ȹ�ֵ

	RAT_UNCRITICAL_RANK,		// ���Եȼ�
	RAT_UNBLOCK_RANK,			// �ƻ��ȼ�
	RAT_UNCRITICAL,				// ���Լ��ʣ������ķ�����
	RAT_UNBLOCK,				// �ƻ����ʣ��񵲵ķ�����

	RAT_ENDING,
};

enum JobType
{
	JT_BASE,
	JT_WARRIOR,					// ����
	JT_SWORDMAN,				// ����
	JT_JEBE,					// ����
	JT_HERCULES,				// ��ʿ
	JT_MUSICIAN,				// ��ʦ
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
	STT_ENEMY_RANDOM_ONE,			//16 �з����1��
	STT_ENEMY_RANDOM_TWO,			//17 �з����2��
	STT_ENEMY_RANDOM_THREE,			//18 �з����3��
	STT_ENEMY_RANDOM_FOUR,			//19 �з����4��
	STT_ENEMY_HP_LOWEST,			//20 �з����Ѫ��
	STT_ENEMY_HP_HIGHEST,			//21 �з����Ѫ��
	STT_ENEMY_HP_LOWEST_COLUMN,		//22 ����Ѫ�����������
	STT_ENEMY_COLUMNA_FARTHEST,		//23 �������һ��
	STT_SELF_FIRST_COLUMN,			//24 ������һ��

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

// ǰ������
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