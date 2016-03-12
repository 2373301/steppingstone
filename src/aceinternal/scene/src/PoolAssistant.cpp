
#include <boost/lexical_cast.hpp>
#include "PoolAssistant.h"
#include "protocol/msg_cache.pb.h"
#include "entity/player.pb.h"
#include "protocol/msg_pool.pb.h"
#include "coredef.h"
#include "Logger.h"
#include "entity_ins.h"
#include "protocol_ins.h"
#include "entity/friend.pb.h"
#include "entity/multicopy.pb.h"
#include "entity/practice.pb.h"
#include "entity/tower.pb.h"
#include "entity/guild.pb.h"
#include "entity/tavern.pb.h"
#include "entity/fish.pb.h"
#include "entity/guildbattle.pb.h"
#include "entity/dodge.pb.h"
#include "entity/battleground.pb.h"
#include "entity/wife.pb.h"
#include "entity/activity.pb.h"
#include "entity/qualifybattle.pb.h"
#include "entity/pellet.pb.h"
#include "entity/arrest.pb.h"
#include "entity/maze.pb.h"
#include "entity/openactivity.pb.h"
#include "entity/openact.pb.h"
#include "entity/title.pb.h"
#include "entity/splendid.pb.h"
#include "entity/exchanges.pb.h"
#include "entity/jianzhong.pb.h"
#include "entity/ranking.pb.h"
#include "entity/openact1.pb.h"
#include "entity/runes.pb.h"
#include "entity/trial.pb.h"
#include "entity/container.pb.h"
#include "entity/shenghuoling.pb.h"
#include "entity/doubleexp.pb.h"
#include "entity/logingift.pb.h"
#include "entity/superpower.pb.h"
#include "entity/goldcard.pb.h"
#include "entity/soul.pb.h"
#include "entity/soulpve.pb.h"
#include "entity/talent.pb.h"
#include "entity/shopline.pb.h"
#include "entity/tavernline.pb.h"
#include "entity/horse.pb.h"
#include "entity/qqspread.pb.h"
#define MK_POOL "[MK_POOL] "

#define FUNCTION_LINE_TRACE		" fun<%s> line<%d>\n"

#define POOL_LOG_DEBUG(log_info, ...)		m_pool_cfg.logger->log(LL_DEBUG, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_INFO(log_info, ...)		m_pool_cfg.logger->log(LL_INFO, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_WARNING(log_info, ...)		m_pool_cfg.logger->log(LL_WARNING, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_TRACE(log_info, ...)		m_pool_cfg.logger->log(LL_TRACE, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_ERROR(log_info, ...)		m_pool_cfg.logger->log(LL_ERROR, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_FATAL(log_info, ...)		m_pool_cfg.logger->log(LL_FATAL, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

STAT_PACKET_IMP(RequestImp);

STAT_PACKET_IMP(RequestListImp);

bool extractEntity(uint64 guid, const string & stream, MSG_TYPE * * entity_msg)
{
	bool result = false;
	MSG_TYPE * message = NULL;
	switch(EXTRACT_ENTITY_TYPE(guid))
	{
	case ET_PLAYER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Player, ent);
			message = ent;
		}
		break;
	case ET_ROLE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Role, ent);
			message = ent;
		}
		break;
	case ET_FORM:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Form, ent);
			message = ent;
		}
		break;
	case ET_ITEM:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Item, ent);
			message = ent;
		}
		break;
	case ET_TASK:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Task, ent);
			message = ent;
		}
		break;
	case ET_LAW:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Law, ent);
			message = ent;
		}
		break;
	case ET_SECT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Sect, ent);
			message = ent;
		}
		break;
	case ET_STHENO:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Stheno, ent);
			message = ent;
		}
		break;
	case ET_STAGE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Stage, ent);
			message = ent;
		}
		break;
	case ET_ANNOUNCE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Announce, ent);
			message = ent;
		}
		break;
	case ET_TASKBRIEF:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TasksBrief, ent);
			message = ent;
		}
		break;
	case ET_TOWN_STAGE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TownStage, ent);
			message = ent;
		}
		break;
	case ET_EDUCATE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Educate, ent);
			message = ent;
		}
		break;
	case ET_EDUCATETIMES:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::EducateTimes, ent);
			message = ent;
		}
		break;
	case ET_PVP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Pvp, ent);
			message = ent;
		}
		break;
	case ET_PVPCONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::PvpContainer, ent);
			message = ent;
		}
		break;
	case ET_SPECSHOP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Specshop, ent);
			message = ent;
		}
		break;
	case ET_SHORT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Short, ent);
			message = ent;
		}
		break;
	case ET_SHORTPLAYERS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ShortPlayers, ent);
			message = ent;
		}
		break;
	case ET_CHAKRA:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Chakra, ent);
			message = ent;
		}
		break;
	case ET_CHAKRE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Chakre, ent);
			message = ent;
		}
		break;
	case ET_WORSHIP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Worship, ent);
			message = ent;
		}
		break;
	case ET_INFO:
		{
			//MAKE_ENTITY_INSTANCE(typed::entity::Info, ent);
			//message = ent;
		}
		break;
	case ET_MAILBOX:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::MailBox, ent);
			message = ent;
		}
		break;
	case ET_MAIL:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Mail, ent);
			message = ent;
		}
		break;
	case ET_MAILCONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::SystemMailContainer, ent);
			message = ent;
		}
		break;
	case ET_EGGS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Eggs, ent);
			message = ent;
		}
		break;
	case ET_CONDOR:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Condor, ent);
			message = ent;
		}
		break;
	case ET_BOSS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Boss, ent);
			message = ent;
		}
		break;
	case ET_ANSWER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Answer, ent);
			message = ent;
		}
		break;
	case ET_ADDICTION:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Addiction, ent);
			message = ent;
		}
		break;
	case ET_PLAYERBUFFER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::PlayerBuffer, ent);
			message = ent;
		}
		break;
	case ET_GIFT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Gift, ent);
			message = ent;
		}
		break;
	case ET_VIP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Vip, ent);
			message = ent;
		}
		break;
	case ET_MULTICOPY:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Multicopy, ent);
			message = ent;
		}
		break;
	case ET_FRIEND:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Friends, ent);
			message = ent;
		}
		break;
	case ET_ACHIEVEMENT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Achievement, ent);
			message = ent;
		}
		break;
	case ET_PRACTICE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Practice, ent);
			message = ent;
		}
		break;
	case ET_DAILYPOINTS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::DailyPoints, ent);
			message = ent;
		}
		break;
	case ET_GUILD:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Guild, ent);
			message = ent;
		}
		break;
	case ET_TOWER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Tower, ent);
			message = ent;
		}
		break;
	case ET_GUILDS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Guilds, ent);
			message = ent;
		}
		break;
	case ET_ESCORT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Escort, ent);
			message = ent;
		}
		break;
	case ET_TAVERN:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Tavern, ent);
			message = ent;
		}
		break;
	case ET_FISH:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Fish, ent);
			message = ent;
		}
		break;
	case ET_GUILDBATTLE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::GuildBattle, ent);
			message = ent;
		}		
		break;
	case ET_DODGE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Dodge, ent);
			message = ent;
		}		
		break;	
	case ET_BATTLEGROUND:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Battleground, ent);
			message = ent;
		}		
		break;	
	case ET_WIFE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Wife, ent);
			message = ent;
		}		
		break;
	case ET_PLAYERWIFE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::PlayerWife, ent);
			message = ent;
		}		
		break;	
	case ET_ACTIVIES:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Activities, ent);
			message = ent;
		}		
		break;	
	case ET_ACTIVE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Activite, ent);
			message = ent;
		}		
		break;	
	case ET_ACTIVE_CONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ActiviteGuidContainer, ent);
			message = ent;
		}		
		break;	
	case ET_QUALIFYBETS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::QualifyBets, ent);
			message = ent;
		}		
		break;	
	case ET_QUALIFYLOGS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::QualifyLogs, ent);
			message = ent;
		}		
		break;	
	case ET_QUALIFYBATTLE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::QualifyBattle, ent);
			message = ent;
		}		
		break;	
	case ET_PELLET:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Pellet, ent);
			message = ent;
		}		
		break;
	case ET_MAZE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::PlayerMaze, ent);
			message = ent;
		}		
		break;
	case ET_ARREST:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Arrest, ent);
			message = ent;
		}		
		break;
	case ET_ARRESTSHORTINFO:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ArrestShortInfo, ent);
			message = ent;
		}		
		break;
	case ET_OPENACTIVITY:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::OpenActivity, ent);
			message = ent;
			break;
		}
	case ET_COIN:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Coin, ent);
			message = ent;
		}		
		break;
	case ET_OPENACT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::OpenAct, ent);
			message = ent;
		}		
		break;
	case ET_OPENACTCONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::OpenActContainer, ent);
			message = ent;
		}		
		break;
	case ET_TITLEBRIEF:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TitleBrief, ent);
			message = ent;
		}		
		break;
	case ET_TITLE_CONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TitleBriefContainer, ent);
			message = ent;
		}		
		break;
	case ET_RANKINGS:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Rankings, ent);
			message = ent;
		}		
		break;
	case ET_EXCHANGECONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ExchangeContainer, ent);
			message = ent;
		}		
		break;
	case ET_EXCHANGES:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Exchanges, ent);
			message = ent;
		}		
		break;
	case ET_JIANZHONG:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Jianzhong, ent);
			message = ent;
		}		
		break;
	case ET_SPLENDID:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Splendid, ent);
			message = ent;
		}		
		break;
	case ET_SPLENDIDINFO:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::SplendidInfo, ent);
			message = ent;
		}		
		break;
	case ET_OPENACT1:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::OpenAct1, ent);
			message = ent;
		}
		break;
	case ET_TRIALCONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TrialContainer, ent);
			message = ent;
		}		
		break;
	case ET_TRIAL:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Trial, ent);
			message = ent;
		}
		break;
	case ET_RUNES:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Runes, ent);
			message = ent;
		}
		break;
	case ET_CONTAINER_CONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ContainerContainer, ent);
			message = ent;
		}
		break;
	case ET_CONTAINER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Container, ent);
			message = ent;
		}
		break;
	case ET_SHL_SWITCH:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ShenghuolingSwitch, ent);
			message = ent;
		}
		break;
	case ET_SHL_OWNER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ShenghuolingOwner, ent);
			message = ent;
		}
		break;
	case ET_DOUBLEEXP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Doubleexp, ent);
			message = ent;
		}
		break;
	case ET_LOGINGIFT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::LoginGift, ent);
			message = ent;
		}
		break;
	case ET_SUPERPOWER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::SuperPower, ent);
			message = ent;
		}
		break;
	case ET_GOLDCARD:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::GoldCard, ent);
			message = ent;
		}
		break;
	case ET_SOUL:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Soul, ent);
			message = ent;
		}
		break;
	case ET_SOULPVE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::soulpve_scene, ent);
			message = ent;
		}
		break;
	case ET_TALENT:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Talent, ent);
			message = ent;
		}
		break;
	case ET_LINESHOP:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::ShopLine, ent);
			message = ent;
		}
		break;
	case ET_LINETAVERN:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::TavernLine, ent);
			message = ent;
		}
		break;
	case ET_HORSE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::Horse, ent);
			message = ent;
		}
		break;
	case ET_QQSPREADPLAYER:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::QQSpreadPlayer, ent);
			message = ent;
		}
		break;
	case ET_QQSPREADLINE:
		{
			MAKE_ENTITY_INSTANCE(typed::entity::QQSpreadShort, ent);
			message = ent;
		}
		break;
	default:
		// error
		break;
	}

	if (NULL != message)
	{
		if (parsePacket(stream, message))
		{
			*entity_msg = message;
			result = true;
		}
		else
		{
			delete message;
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

RequestImp::RequestImp(RequestType rt, uint64 entity_guid, MSG_TYPE * msg, const PoolCfg pool_cfg, uint64 owner_g)
{
	request_type = rt;
	guid = entity_guid;
	success = false;
	complated = false;
	message = msg;
	owner_guid = owner_g;
	m_pool_cfg = pool_cfg;
	//transaction_info = trans_info;

	STAT_PACKET_INC_NEW();
}

RequestImp::~RequestImp()
{
	STAT_PACKET_INC_DEL();
}

int RequestImp::handlePacket(Packet * packet)
{
	int result = -1;
	typed::cached::protocol::Result op_result;
	if (PARSE_PACKET(packet, &op_result))
	{
		success = op_result.status() == typed::cached::protocol::Result_status_t_STATE_NORMAL;
		if (success)
		{
			result = 0;
			switch(request_type)
			{
			case RT_ADD:
			case RT_ADD_WITH_FLUSH:
				// ok , do nothing
				break;
			case RT_LOAD:
			case RT_LOADONCE:
				if (op_result.has_message())
				{
					if (!extractEntity(this->guid, op_result.message(), &this->message))
					{
						POOL_LOG_ERROR("failed to extract entity from cache packet, guid is <%s>", boost::lexical_cast<string>(packet->guid()).c_str());
						result = 1;
					}
				}
				else
				{
					// error
					POOL_LOG_ERROR("load entity while get empty entity message from cache, guid is <%s>", boost::lexical_cast<string>(packet->guid()).c_str());
					result = 1;
				}
				break;
			case RT_QUERY:
				if (op_result.has_message())
				{
					if (!extractEntity(this->guid, op_result.message(), &this->message))
					{
						result = -1;
					}
				}
				else
				{
					// error
					result = -1;
				}
				break;
			case RT_UPDATE:		// ok , do nothing
				break;
			case RT_FLUSH:		// ok , do nothing
				break;
			case RT_DELETE:		// ok , do nothing
				break;
			default:
				break;
			}
		}
		else
		{
			// error
			POOL_LOG_ERROR("get unnormal state packet from cache, op result is <%d>, guid is <%llu>", op_result.status(), packet->guid());
		}
	}
	else
	{
		POOL_LOG_ERROR("failed to parse packet from cache, guid is <%llu>", packet->guid());
		// error
	}

	return 0;
}

//bool RequestImp::extractEntity(const string & stream)
//{
//	bool result = false;
//	switch(EXTRACT_ENTITY_TYPE(guid))
//	{
//	case ET_PLAYER:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Player, ent);
//			message = ent;
//		}
//		break;
//	case ET_ROLE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Role, ent);
//			message = ent;
//		}
//		break;
//	case ET_FORM:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Form, ent);
//			message = ent;
//		}
//		break;
//	case ET_ITEM:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Item, ent);
//			message = ent;
//		}
//		break;
//	case ET_TASK:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Task, ent);
//			message = ent;
//		}
//		break;
//	case ET_LAW:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Law, ent);
//			message = ent;
//		}
//		break;
//	case ET_SECT:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Sect, ent);
//			message = ent;
//		}
//		break;
//	case ET_STHENO:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Stheno, ent);
//			message = ent;
//		}
//		break;
//	case ET_STAGE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Stage, ent);
//			message = ent;
//		}
//		break;
//	case ET_ANNOUNCE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Announce, ent);
//			message = ent;
//		}
//		break;
//	case ET_TASKBRIEF:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::TasksBrief, ent);
//			message = ent;
//		}
//		break;
//	case ET_TOWN_STAGE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::TownStage, ent);
//			message = ent;
//		}
//		break;
//	case ET_EDUCATE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Educate, ent);
//			message = ent;
//		}
//		break;
//	case ET_EDUCATETIMES:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::EducateTimes, ent);
//			message = ent;
//		}
//		break;
//	case ET_PVP:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Pvp, ent);
//			message = ent;
//		}
//		break;
//	case ET_PVPCONTAINER:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::PvpContainer, ent);
//			message = ent;
//		}
//		break;
//	case ET_SPECSHOP:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Specshop, ent);
//			message = ent;
//		}
//		break;
//	case ET_SHORT:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Short, ent);
//			message = ent;
//		}
//		break;
//	case ET_SHORTPLAYERS:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::ShortPlayers, ent);
//			message = ent;
//		}
//		break;
//	case ET_CHAKRA:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Chakra, ent);
//			message = ent;
//		}
//		break;
//	case ET_CHAKRE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Chakre, ent);
//			message = ent;
//		}
//		break;
//	case ET_WORSHIP:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Worship, ent);
//			message = ent;
//		}
//		break;
//	case ET_INFO:
//		{
//			//MAKE_ENTITY_INSTANCE(typed::entity::Info, ent);
//			//message = ent;
//		}
//		break;
//	case ET_MAILBOX:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::MailBox, ent);
//			message = ent;
//		}
//		break;
//	case ET_MAIL:
//		{
//			//MAKE_ENTITY_INSTANCE(typed::entity::Mail, ent);
//			//message = ent;
//		}
//		break;
//	case ET_MAILCONTAINER:
//		{
//			//MAKE_ENTITY_INSTANCE(typed::entity::SystemMailContainer, ent);
//			//message = ent;
//		}
//		break;
//	case ET_EGGS:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Eggs, ent);
//			message = ent;
//		}
//		break;
//	case ET_CONDOR:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Condor, ent);
//			message = ent;
//		}
//		break;
//	case ET_BOSS:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Boss, ent);
//			message = ent;
//		}
//		break;
//	case ET_ANSWER:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Answer, ent);
//			message = ent;
//		}
//		break;
//	case ET_ADDICTION:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Addiction, ent);
//			message = ent;
//		}
//		break;
//	case ET_PLAYERBUFFER:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::PlayerBuffer, ent);
//			message = ent;
//		}
//		break;
//	case ET_GIFT:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Gift, ent);
//			message = ent;
//		}
//		break;
//	case ET_VIP:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Vip, ent);
//			message = ent;
//		}
//		break;
//	case ET_MULTICOPY:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Multicopy, ent);
//			message = ent;
//		}
//		break;
//	case ET_FRIEND:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Friends, ent);
//			message = ent;
//		}
//		break;
//	case ET_ACHIEVEMENT:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Achievement, ent);
//			message = ent;
//		}
//		break;
//	case ET_PRACTICE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Practice, ent);
//			message = ent;
//		}
//		break;
//	case ET_DAILYPOINTS:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::DailyPoints, ent);
//			message = ent;
//		}
//		break;
//	case ET_GUILD:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Guild, ent);
//			message = ent;
//		}
//		break;
//	case ET_TOWER:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Tower, ent);
//			message = ent;
//		}
//		break;
//	case ET_GUILDS:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Guilds, ent);
//			message = ent;
//		}
//		break;
//	case ET_ESCORT:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Escort, ent);
//			message = ent;
//		}
//		break;
//	case ET_TAVERN:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Tavern, ent);
//			message = ent;
//		}
//		break;
//	case ET_FISH:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Fish, ent);
//			message = ent;
//		}
//		break;
//	case ET_GUILDBATTLE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::GuildBattle, ent);
//			message = ent;
//		}		
//		break;
//	case ET_DODGE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Dodge, ent);
//			message = ent;
//		}		
//		break;	
//	case ET_BATTLEGROUND:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Battleground, ent);
//			message = ent;
//		}		
//		break;	
//	case ET_WIFE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::Wife, ent);
//			message = ent;
//		}		
//		break;
//	case ET_PLAYERWIFE:
//		{
//			MAKE_ENTITY_INSTANCE(typed::entity::PlayerWife, ent);
//			message = ent;
//		}		
//		break;	default:
//		// error
//		break;
//	}
//
//	if (NULL != message)
//	{
//		if (parsePacket(stream, message))
//		{
//			result = true;
//		}
//	}
//	else
//	{
//		result = false;
//	}
//
//	return result;
//}
//
void RequestImp::setOwnerGuid(uint64 owner_g)
{
	owner_guid = owner_g;
}

RequestListImp::RequestListImp(const PoolCfg pool_cfg)
{
	m_complated = false;
	m_complated_count = 0;
	m_request_id = 0;
	m_pool_cfg = pool_cfg;

	STAT_PACKET_INC_NEW();
}

RequestListImp::~RequestListImp()
{
	STAT_PACKET_INC_DEL();
}

const RequestListImp::RequestVec_t & RequestListImp::getRequestList()
{
	return m_request_vec;
}

void RequestListImp::query(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_QUERY, guid, NULL, m_pool_cfg, owner_guid));
}

void RequestListImp::add(uint64 guid, MSG_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_ADD, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::addWithFlush(uint64 guid, MSG_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_ADD_WITH_FLUSH, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::update(uint64 guid, MSG_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_UPDATE, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::load(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_LOAD, guid, NULL, m_pool_cfg, owner_guid));
}

void RequestListImp::loadOnce(uint64 guid, uint64 owner_guid, const MSG_TYPE * message)
{
	m_request_vec.push_back(new RequestImp(RT_LOADONCE, guid, const_cast<MSG_TYPE *>(message), m_pool_cfg, owner_guid));
}

void RequestListImp::flush(uint64 guid, MSG_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_FLUSH, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::remove(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_DELETE, guid, NULL, m_pool_cfg, owner_guid));
}

bool RequestListImp::is_complated()
{
	return m_complated;
}

bool RequestListImp::is_success()
{
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		if (!(*it)->success)
		{
			return false;
		}
	}

	return true;
}

bool RequestListImp::is_all_failed()
{
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		if ((*it)->success)
		{
			return false;
		}
	}

	return true;
}

void RequestListImp::setRequestID(uint64 request_id)
{
	m_request_id = request_id;
}

uint64 RequestListImp::getRequestID()
{
	return m_request_id;
}

int RequestListImp::handlePacket(Packet * packet)
{
	int result = -1;
	int packet_process_result = -1;
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		Request * request = *it;
		if (request->guid == packet->guid())
		{
			result = 0;

			packet_process_result = request->handlePacket(packet);
			if (++m_complated_count == m_request_vec.size())
			{
				m_complated = true;
			}

			break;
		}
	}

	return result;
}