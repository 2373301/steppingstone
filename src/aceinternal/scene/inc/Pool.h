
#ifndef POOL_HPP
#define POOL_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <google/protobuf/message.h>
#include "typedef.h"
#include "Scend_def.h"
#include "CacheInterface.h"
#include "coredef.h"
#include "Cache.h"
//#include "PlayerFunction.h"

////////////////  request operator         //////////////////////////////
#define DECLARE_REQUEST_LIST(pool) RequestList * request_list_ins = pool->createRequestList()

#define QUERY_REQUEST(guid, owner_guid)						request_list_ins->query(guid, owner_guid)

#define ADD_REQUEST(guid, message, owner_guid)				request_list_ins->add(guid, message, owner_guid)

#define ADD_WITH_FLUSH_REQUEST(guid, message, owner_guid)	request_list_ins->addWithFlush(guid, message, owner_guid)

#define UPDATE_REQUEST(guid, message, owner_guid)			request_list_ins->update(guid, message, owner_guid)

#define LOAD_REQUEST(guid, owner_guid)						request_list_ins->load(guid, owner_guid)

#define LOAD_ONCE_REQUEST(guid, owner_guid)					request_list_ins->loadOnce(guid, owner_guid)

#define FLUSH_REQUEST(guid, message, owner_guid)			request_list_ins->flush(guid, message, owner_guid)

#define REMOVE_REQUEST(guid, owner_guid)					request_list_ins->remove(guid, owner_guid)

#define REQUEST_IS_EMPTY()				request_list_ins->getRequestList().size() == 0

#define DELETE_REQUEST()				delete request_list_ins
//////////////////////////////////////////////////////////////////////////

////////////////  pool operator         /////////////////////////////////

#define GET_ENTITY_FROM_POOL(pool, ENTITY_TYPE, guid, entity_ins)	\
	ENTITY_TYPE * entity_ins = NULL;	\
	{	\
		MSG_TYPE * msg = pool->get(guid);	\
		if (NULL != msg)	\
		{	\
			entity_ins = dynamic_cast<ENTITY_TYPE *>(msg);	\
		}	\
	}	\
	if (NULL == entity_ins)	\
	{	\
	}	

#define GET_PLAYER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Player, guid, entity_ins)

#define GET_ROLE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Role, guid, entity_ins)

#define GET_WIFE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Wife, guid, entity_ins)

#define GET_PLAYERWIFE_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::PlayerWife, guid, entity_ins)

#define GET_FORM_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Form, guid, entity_ins)

#define GET_BUFFER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Buffer, guid, entity_ins)

#define GET_PLAYER_BUFFER_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::PlayerBuffer, guid, entity_ins)

#define GET_LAW_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Law, guid, entity_ins)

#define GET_ITEM_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Item, guid, entity_ins)

#define GET_SECT_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Sect, guid, entity_ins)

#define GET_STAGE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Stage, guid, entity_ins)

#define GET_STHENO_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Stheno, guid, entity_ins)

#define GET_ANNOUNCE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Announce, guid, entity_ins)

#define GET_TASK_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Task, guid, entity_ins)

#define GET_TASKBRIEF_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::TasksBrief, guid, entity_ins)

#define GET_TOWNSTAGE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::TownStage, guid, entity_ins)

#define GET_EDUCATE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Educate, guid, entity_ins)

#define GET_EDUCATETIMES_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::EducateTimes, guid, entity_ins)

#define GET_ATTACH_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Attach, guid, entity_ins)

#define GET_PVP_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Pvp, guid, entity_ins)

#define GET_PVPCONTAINER_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::PvpContainer, guid, entity_ins)

#define GET_SPECSHOP_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Specshop, guid, entity_ins)

#define GET_SHORTPLAYERS_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::ShortPlayers, guid, entity_ins)

#define GET_SHORT_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Short, guid, entity_ins)

#define GET_CHAKRA_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Chakra, guid, entity_ins)

#define GET_CHAKRE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Chakre, guid, entity_ins)

#define GET_WORSHIP_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Worship, guid, entity_ins)

#define GET_EGGS_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Eggs, guid, entity_ins)

#define GET_CONDOR_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Condor, guid, entity_ins)

#define GET_MAILCONTAINER_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::SystemMailContainer, guid, entity_ins)

#define GET_MAILBOX_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::MailBox, guid, entity_ins)

#define GET_MAIL_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Mail, guid, entity_ins)

#define GET_BOSS_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Boss, guid, entity_ins)

#define GET_ANSWER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Answer, guid, entity_ins)

#define GET_ADDICTION_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Addiction, guid, entity_ins)

#define GET_GIFT_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Gift, guid, entity_ins)

#define GET_VIP_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Vip, guid, entity_ins)

#define GET_MCOPY_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Multicopy, guid, entity_ins)

#define GET_FRIEND_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Friends, guid, entity_ins)

#define GET_ACHIEVEMENT_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::Achievement, guid, entity_ins)

#define GET_PRACTICE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Practice, guid, entity_ins)

#define GET_DAILYPOINTS_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::DailyPoints, guid, entity_ins)

#define GET_GUILD_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Guild, guid, entity_ins)	

#define GET_GUILDS_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Guilds, guid, entity_ins)

#define GET_TOWER_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Tower, guid, entity_ins)	

#define GET_ESCORT_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Escort, guid, entity_ins)

#define GET_TAVERN_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Tavern, guid, entity_ins)

#define GET_DODGE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Dodge, guid, entity_ins)

#define GET_GUILDBATTLE_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::GuildBattle, guid, entity_ins)	

#define GET_FISH_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Fish, guid, entity_ins)

#define GET_BATTLEGROUND_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::Battleground, guid, entity_ins)

#define GET_QUALIFYBATTLE_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::QualifyBattle, guid, entity_ins)

#define GET_ACTIVES_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Activities, guid, entity_ins)

#define GET_PELLET_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Pellet, guid, entity_ins)

#define GET_MAZE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::PlayerMaze, guid, entity_ins)

#define GET_QUALIFYBETS_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::QualifyBets, guid, entity_ins)

#define GET_QUALIFYLOGS_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::QualifyLogs, guid, entity_ins)
#define GET_OPENACTIVITY_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::OpenActivity, guid, entity_ins)

#define GET_OPENACT_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::OpenAct, guid, entity_ins)

#define GET_COIN_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Coin, guid, entity_ins)
#define GET_RANKING_FROM_POOL(pool, guid, entity_ins)		    GET_ENTITY_FROM_POOL(pool, typed::entity::Rankings, guid, entity_ins)

#define GET_JIANZHONG_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Jianzhong, guid, entity_ins)
#define GET_EXCHANGECONTAINER_FROM_POOL(pool, guid, entity_ins)	GET_ENTITY_FROM_POOL(pool, typed::entity::ExchangeContainer, guid, entity_ins)
#define GET_EXCHANGES_FROM_POOL(pool, guid, entity_ins)		    GET_ENTITY_FROM_POOL(pool, typed::entity::Exchanges, guid, entity_ins)

#define GET_SPLENDID_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Splendid, guid, entity_ins)
#define GET_SPLENDID_INFO_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::SplendidInfo, guid, entity_ins)
#define GET_OPENACT1_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::OpenAct1, guid, entity_ins)

#define GET_RUNES_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Runes, guid, entity_ins)

#define GET_SHLSWITCH_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::ShenghuolingSwitch, guid, entity_ins)
#define GET_SHLOWNER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::ShenghuolingOwner, guid, entity_ins)

#define GET_CONTAINER_CON_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::ContainerContainer, guid, entity_ins)
#define GET_CONTAINER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Container, guid, entity_ins)

#define GET_DOUBLE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Doubleexp, guid, entity_ins)
#define GET_SOULPVE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::soulpve_scene, guid, entity_ins)

#define GET_PAIRSELF_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::PairSelf, guid, entity_ins)

#define GET_PAIRTEAM_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::PairTeam, guid, entity_ins)

#define GET_PAIRBATTLE_FROM_POOL(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL(pool, typed::entity::PairBattle, guid, entity_ins)

#define GET_PAIRLOGS_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::PairLogs, guid, entity_ins)

#define GET_PAIRBETS_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::PairBets, guid, entity_ins)

#define GET_SOUL_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Soul, guid, entity_ins)

#define GET_GOLDCARD_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::GoldCard, guid, entity_ins)

#define GET_SHOPLINE_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::ShopLine, guid, entity_ins)
#define GET_HORSE_FROM_POOL(pool, guid, entity_ins)			    GET_ENTITY_FROM_POOL(pool, typed::entity::Horse, guid, entity_ins)
//////////////////////////////////////////////////////////////////////////

#define POOL_OP_ADD(pool, guid, message, add_to_db, woner_guid)		pool->add(guid, message, add_to_db, woner_guid)

#define POOL_OP_REMOVE(pool, guid, remove_from_db)					pool->remove(guid, remove_from_db)

#define POOL_OP_UPDATE(pool, guid)									pool->update(guid)

#define POOL_OP_COMMIT(pool, call_back)								pool->commit(request_list_ins, call_back)

#define POOL_OP_REMOVE_PENDING_UPDATE(pool)							pool->removePendingUpdate(request_list_ins)

#define POOL_OP_CLEAR_FLUSH_STATE(pool, guid)						pool->clearFlushState(guid);

//////////////////////////////////////////////////////////////////////////

typedef define_unordered_set<uint64>	PlayerGuidSet_t;

enum RequestType
{
	RT_ADD		= 1,
	RT_LOAD,
	RT_QUERY,
	RT_UPDATE,
	RT_FLUSH,
	RT_DELETE,
	RT_LOADONCE,
	RT_ADD_WITH_FLUSH,
};

struct TransactionInfo 
{
	TransactionInfo()
		: index(0)
		, num(0)
		, id(0)
	{}

	TransactionInfo(uint32 t_index, uint32 t_num, uint32 t_id)
		: index(t_index)
		, num(t_num)
		, id(t_id)
	{}

	uint32 index;
	uint32 num;
	uint32 id;
};

class SCENE_EXOPRT Request 
{
public:
	//Request(RequestType rt, uint64 entity_guid, MSG_TYPE * msg);
	virtual ~Request()
	{
		// do nothing , message should be deleted by caller
	}

	virtual int handlePacket(Packet * packet) = 0;

	//virtual bool extractEntity(const string & stream) = 0;

	RequestType request_type;
	uint64		guid;
	bool		success;
	bool		complated;
	uint64		owner_guid;
	MSG_TYPE *	message;
	//TransactionInfo	transaction_info;
};

SCENE_EXOPRT bool extractEntity(uint64 guid, const string & stream, MSG_TYPE * * entity_msg);

class SCENE_EXOPRT RequestList 
{
public:
	//RequestList()
	//	: m_complated(false)
	//	, m_complated_count(0)
	//{}
	virtual ~RequestList()
	{
		for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
		{
			delete *it;
		}
	}

public:
	typedef vector<Request *> RequestVec_t;

	virtual const RequestVec_t & getRequestList() = 0;

	virtual void query(uint64 guid, uint64 owner_guid) = 0;

	virtual void add(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void addWithFlush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void update(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void loadOnce(uint64 guid, uint64 owner_guid) = 0;

	virtual void load(uint64 guid, uint64 owner_guid) = 0;

	virtual void flush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void remove(uint64 guid, uint64 owner_guid) = 0;

	virtual bool is_complated() = 0;

	virtual bool is_success() = 0;

	virtual bool is_all_failed() = 0;

	virtual void setRequestID(uint64 request_id) = 0;

	virtual uint64 getRequestID() = 0;

	virtual int handlePacket(Packet * packet) = 0;

protected:
	bool	m_complated;
	int		m_complated_count;
	uint64	m_request_id;
	RequestVec_t m_request_vec;
};

class SCENE_EXOPRT Pool : public CacheHandleInput
{
public:
	virtual ~Pool()
	{}

	virtual int init(const PoolCfg pool_cfg) = 0;

	virtual MSG_TYPE * get(uint64 guid) = 0;

	// get entity and set flush state
	virtual MSG_TYPE * getWhileFlush(uint64 guid) = 0;

	virtual void clearFlushState(uint64 guid) = 0;

public:
	typedef boost::function<void(RequestList *)>	RequestCallBack;

	virtual void beginTransaction() = 0;

	virtual void endtransaction() = 0;

	/// add_to_db add to pool and database
	virtual bool add(uint64 guid, MSG_TYPE * message, bool add_to_db, uint64 owner_guid) = 0;

	/// remove_from_db remove from pool and database
	virtual bool remove(uint64 guid, bool remove_from_db, bool remove_entity_msg = true) = 0;

	// update guid to database
	virtual bool update(uint64 guid) = 0;

	virtual void onlyRemoveEntity(uint64 guid, uint64 owner_guid) = 0;

	// for frame calling
	virtual bool update() = 0;

	virtual bool isLastUpdateFinish() = 0;

	virtual int updateEntityNumber() = 0;

	virtual void clearUpdateEntity(uint64 guid_owner, Uint64Set_t & insert_entity_set, Uint64Set_t & remove_entity_set) = 0;

	/// commit data to cache system, action : update, query, load, flush
	virtual bool commit(RequestList * request_list, RequestCallBack call_back) = 0;

	virtual Request * createRequset(RequestType rt, uint64 entity_guid, MSG_TYPE * msg) = 0;

	virtual RequestList * createRequestList() = 0;

	virtual int getEntityNumber() = 0;

	virtual void playerIsOnline(uint64 player_guid) = 0;

	virtual void playerIsOffline(uint64 player_guid) = 0;

	virtual bool isPlayerOnlineInThisMap(uint64 player_guid) = 0;

	virtual const PlayerGuidSet_t & getOnlinePlayer() = 0;

protected:
	PoolCfg m_pool_cfg;
private:
};
#endif
