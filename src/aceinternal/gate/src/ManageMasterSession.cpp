
#include <boost/lexical_cast.hpp>
#include "ManageMasterSession.h"
#include "ManageConfig.h"
#include "opcode.h"
#include "msg_query.pb.h"
#include "GateLogger.h"
#include "SimpleXML.h"
#include "RouteClientPackage.h"
#include "ManageRoleGsState.h"

const string all_ip_str = "*";

uint64 getCurrentTimeInSec()
{
	uint64 curr_time = 0;
	ACE_OS::gettimeofday().to_usec(curr_time);
	curr_time /= 1000000;
	return curr_time;
}

bool ForbidIPInfo::outOfTime()
{
	uint64 curr_time = getCurrentTimeInSec();
	if ((end_time != 0) && (curr_time > end_time))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ForbidAccountInfo::outOfTime()
{
	uint64 curr_time = getCurrentTimeInSec();
	if ((end_time != 0) && (curr_time > end_time))
	{
		return true;
	}
	else
	{
		return false;
	}
}

ManageMasterSession::ManageMasterSession()
: m_forbid_all_ip(false)
{

}

ManageMasterSession::~ManageMasterSession()
{

}

int ManageMasterSession::init(int argc, ACE_TCHAR * argv[])
{
	if (this->SingleConnection::init(ManageConfig::instance()->getQueryAddr()) == -1)
	{
		return -1;
	}

	if (!loadIPSetInfo())
	{
		GATE_LOG_ERROR("failed to load ip set info\n");
		return -1;
	}

	loadForbidAccount();

	return 0;
}

int ManageMasterSession::fini()
{
	return 0;
}

int ManageMasterSession::wait()
{
	return 0;
}


void ManageMasterSession::handlePacket(Packet * packet)
{
	int op_code = packet->opcode();
	switch (op_code)
	{
	case QMSG_ALLOW_IP:
		handleAllowIP(packet);
		break;
	case QMSG_ALLOW_IP_REMOVE:
		handleAllowIPRemove(packet);
		break;
	case QMSG_FORBID_IP:
		handleForbidIP(packet);
		break;
	case QMSG_FORBID_IP_REMOVE:
		handleForbidIPRemove(packet);
		break;
	case QMSG_QUERY_IP_SET:
		handleQueryIPSet(packet);
		break;
	case QMSG_FORBID_ACCOUNT:
		handleForbidAccount(packet);
		break;
	case QMSG_FORBID_ACCOUNT_REMOVE:
		handleForbidAccountRemove(packet);
		break;
	case QMSG_FORBID_ACCOUNT_QUERY:
		handleForbidAccountQuery(packet);
		break;
	case QMSG_SET_GATE_LOG_LEVEL:
		handleSetGateLogLevel(packet);
		break;
	case QMSG_CLEAN_GATE_UNNORMAL_PLAYER:
		handleCleanGateUnnormalPlayer(packet);
		break;
	default:
		break;
	}

	delete packet;
}

bool ManageMasterSession::isValidatedAccountAndIP(const string & account_id, const string & ip_addr)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, true);
	if (!validatedAccount(account_id))
	{
		DEF_LOG_INFO("the account is login, but the account is not valid, account is <%s>\n", account_id.c_str());
		return false;
	}
	
	if (!validatedIpAddr(ip_addr))
	{
		DEF_LOG_INFO("the account is login, but the ip is not valid, account is <%s>, ip is <%s>\n", account_id.c_str(), ip_addr.c_str());
		return false;
	}

	return true;
}

void ManageMasterSession::handleAllowIP(Packet * packet)
{
	typed::protocol::qmsg_allow_ip qa_ip;
	if (parsePacket(packet, &qa_ip))
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
		for (int i = 0; i < qa_ip.ip_addr_size(); ++i)
		{
			m_allow_ip_set.insert(qa_ip.ip_addr(i));
		}
		saveIPSetInfo();
	}

	MAKE_NEW_PACKET(ps, GMSG_ALLOW_IP, packet->guid(), "");

	this->asyncWrite(ps);
}

void ManageMasterSession::handleAllowIPRemove(Packet * packet)
{
	typed::protocol::qmsg_allow_ip_remove qai_remove;
	if (parsePacket(packet, &qai_remove))
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
		for (int i = 0; i < qai_remove.ip_addr_size(); ++i)
		{
			m_allow_ip_set.erase(qai_remove.ip_addr(i));
		}
		saveIPSetInfo();
	}

	MAKE_NEW_PACKET(ps, GMSG_ALLOW_IP_REMOVE, packet->guid(), "");

	this->asyncWrite(ps);
}

void ManageMasterSession::handleForbidIP(Packet * packet)
{
	typed::protocol::qmsg_forbid_ip qf_ip;
	if (parsePacket(packet, &qf_ip))
	{
		if (qf_ip.ip_addr_size() == qf_ip.end_time_size())
		{
			uint64 curr_time = getCurrentTimeInSec();
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
			for (int i = 0; i < qf_ip.ip_addr_size(); ++i)
			{
				string ip_addr = qf_ip.ip_addr(i);
				int end_time = qf_ip.end_time(i);
				m_forbid_ip_map[ip_addr] = ForbidIPInfo(ip_addr, end_time);
			}

			collectForbidIpFlag();

			saveIPSetInfo();
		}
	}

	MAKE_NEW_PACKET(ps, GMSG_FORBID_IP, packet->guid(), "");

	this->asyncWrite(ps);

}

void ManageMasterSession::handleForbidIPRemove(Packet * packet)
{
	typed::protocol::qmsg_forbid_ip_remove qfi_remove;
	if (parsePacket(packet, &qfi_remove))
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
		for (int i = 0; i < qfi_remove.ip_addr_size(); ++i)
		{
			ForbidIPInfoMap_t::iterator it = m_forbid_ip_map.find(qfi_remove.ip_addr(i));
			if (it != m_forbid_ip_map.end())
			{
				m_forbid_ip_map.erase(it);
			}
		}

		collectForbidIpFlag();

		saveIPSetInfo();
	}

	MAKE_NEW_PACKET(ps, GMSG_FORBID_IP_REMOVE, packet->guid(), "");

	this->asyncWrite(ps);

}

void ManageMasterSession::handleQueryIPSet(Packet * packet)
{
	typed::protocol::gmsg_query_ip_set gqi_set;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
	for (ForbidIPInfoMap_t::iterator it = m_forbid_ip_map.begin(); it != m_forbid_ip_map.end(); ++it)
	{
		typed::protocol::gmsg_query_ip_set_result_info * result_info = gqi_set.add_info();
		result_info->set_ip_addr(it->first);
		result_info->set_ip_type(1);
		result_info->set_forbid_ip_end_time(it->second.end_time);
	}

	for (AllowIPInfoSet_t::iterator it = m_allow_ip_set.begin(); it != m_allow_ip_set.end(); ++it)
	{
		typed::protocol::gmsg_query_ip_set_result_info * result_info = gqi_set.add_info();
		result_info->set_ip_addr(*it);
		result_info->set_ip_type(2);
		result_info->set_forbid_ip_end_time(0);
	}

	MAKE_NEW_PACKET(ps, GMSG_QUERY_IP_SET, packet->guid(), gqi_set.SerializeAsString());

	this->asyncWrite(ps);
}

void ManageMasterSession::handleForbidAccount(Packet * packet)
{
	typed::protocol::qmsg_forbid_account qf_account;
	if (parsePacket(packet, &qf_account))
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
		for (int i = 0; i < qf_account.info_size(); ++i)
		{
			typed::protocol::qmsg_forbid_account_account_info * forbid_account_info = qf_account.mutable_info(i);

			DEF_LOG_INFO("forbid account <%s>, end time is <%llu>\n", forbid_account_info->account_id().c_str(), forbid_account_info->end_time());

			uint64 end_time = forbid_account_info->end_time();

			ForbidAccountInfoMap_t::iterator it = m_forbid_account_map.find(forbid_account_info->account_id());
			if (it != m_forbid_account_map.end())
			{
				ForbidAccountInfo & account_info = it->second;
				account_info.end_time = end_time;
			}
			else
			{
				m_forbid_account_map[forbid_account_info->account_id()] = ForbidAccountInfo(forbid_account_info->account_id(), end_time);
			}
		}

		saveForbidAccount();
	}

	MAKE_NEW_PACKET(ps, GMSG_FORBID_ACCOUNT, packet->guid(), "");
	this->asyncWrite(ps);
}

void ManageMasterSession::handleForbidAccountRemove(Packet * packet)
{
	typed::protocol::qmsg_forbid_account_remove qfa_remove;
	typed::protocol::gmsg_forbid_account_remove gfa_remove;

	if (parsePacket(packet, &qfa_remove))
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
		for (int i = 0; i < qfa_remove.account_id_size(); ++i)
		{
			typed::protocol::gmsg_forbid_account_remove_result_info * info = gfa_remove.add_info();
			info->set_account_id(qfa_remove.account_id(i));
			ForbidAccountInfoMap_t::iterator it = m_forbid_account_map.find(info->account_id());
			if (it != m_forbid_account_map.end())
			{
				m_forbid_account_map.erase(it);
				info->set_result(1);
			}
			else
			{
				// error
				info->set_result(2);
			}
		}

		saveForbidAccount();

	}

	MAKE_NEW_PACKET(ps, GMSG_FORBID_ACCOUNT_REMOVE, packet->guid(), gfa_remove.SerializeAsString());
	this->asyncWrite(ps);
}

void ManageMasterSession::handleForbidAccountQuery(Packet * packet)
{
	typed::protocol::gmsg_forbid_account_query gfa_query;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_ip_query_mutex, );
	for (ForbidAccountInfoMap_t::iterator it = m_forbid_account_map.begin(); it != m_forbid_account_map.end(); ++it)
	{
		ForbidAccountInfo & account_inof = it->second;

		typed::protocol::gmsg_forbid_account_query_result_info * result_info = gfa_query.add_info();
		result_info->set_account_id(account_inof.account_id);
		result_info->set_end_time(account_inof.end_time);
	}

	MAKE_NEW_PACKET(ps, GMSG_FORBID_ACCOUNT_QUERY, packet->guid(), gfa_query.SerializeAsString());
	this->asyncWrite(ps);
}

void ManageMasterSession::handleSetGateLogLevel(Packet * packet)
{
	typed::protocol::qmsg_set_gate_log_level qsgl_level;
	if (parsePacket(packet, &qsgl_level))
	{
		switch (qsgl_level.level())
		{
		case 1:
			Logger::setFilter(LL_DEBUG);
			break;
		case 2:
			Logger::setFilter(LL_INFO);
			break;
		case 3:
			Logger::setFilter(LL_WARNING);
			break;
		case 4:
			Logger::setFilter(LL_TRACE);
			break;
		case 5:
			Logger::setFilter(LL_ERROR);
			break;
		case 6:
			Logger::setFilter(LL_FATAL);
			break;
		default :
			Logger::setFilter(LL_INFO);
			break;
		}
	}
	else
	{
		GATE_LOG_ERROR("failed to parse QMSG_SET_GATE_LOG_LEVEL\n");
	}

}

void ManageMasterSession::handleCleanGateUnnormalPlayer(Packet * packet)
{
	typed::protocol::qmsg_clean_gate_unnormal_player qcgu_player;
	if (parsePacket(packet, &qcgu_player))
	{
		GATE_LOG_INFO("start to clean player info, account is <%s>, guid is <%llu>\n", qcgu_player.player_account().c_str(), qcgu_player.player_guid());
		RouteClientPackage::instance()->cleanPlayerInfo(qcgu_player.player_account(), qcgu_player.player_guid());
		ManageRoleGsState::instance()->cleanUnnormalPlayer(qcgu_player.player_guid());
	}
	else
	{
		GATE_LOG_ERROR("failed to parse qmsg_clean_gate_unnormal_player\n");
	}
}

void ManageMasterSession::saveIPSetInfo()
{
	// todo
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	Element * root_ele = auto_doc->make_element("ipset");
	auto_doc->set_root(root_ele);

	Element * allows_ele = auto_doc->make_element("allows", root_ele);
	for (AllowIPInfoSet_t::iterator it = m_allow_ip_set.begin(); it != m_allow_ip_set.end(); ++it)
	{
		Element * ele = auto_doc->make_element("allow", root_ele);
		ele->set_attribute("ip", *it);
	}

	Element * forbids_ele = auto_doc->make_element("forbids", root_ele);
	for (ForbidIPInfoMap_t::iterator it = m_forbid_ip_map.begin(); it != m_forbid_ip_map.end(); ++it)
	{
		Element * ele = auto_doc->make_element("forbid", root_ele);
		ele->set_attribute("ip", it->first);
		ele->set_attribute("end_time", boost::lexical_cast<string>(it->second.end_time));
	}

	auto_doc->save_file("ipset_config.xml");
}

bool ManageMasterSession::loadIPSetInfo()
{
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call XMLFactory::create_document()\n"));
		return false;
	}

	if (auto_doc->load_file("ipset_config.xml"))
	{
		Element * allows_ele = auto_doc->get_root()->get_element("allows");
		if (NULL == allows_ele)
		{
			return false;
		}

		Elements allow_list = allows_ele->get_elements("allow");
		for (Elements::iterator it = allow_list.begin(); it != allow_list.end(); ++it)
		{
			Attribute * attr = (*it)->get_attribute("ip");
			if (NULL == attr)
			{
				return false;
			}
			m_allow_ip_set.insert(attr->as_cstr());
		}

		Element * forbids_ele = auto_doc->get_root()->get_element("forbids");
		if (NULL == forbids_ele)
		{
			return false;
		}

		Elements forbid_list = forbids_ele->get_elements("forbid");
		for (Elements::iterator it = forbid_list.begin(); it != forbid_list.end(); ++it)
		{
			Attribute * ip_attr = (*it)->get_attribute("ip");
			Attribute * end_time_attr = (*it)->get_attribute("end_time");
			if ((NULL == ip_attr) || (NULL == end_time_attr))
			{
				return false;
			}

			m_forbid_ip_map[ip_attr->as_cstr()] = ForbidIPInfo(ip_attr->as_cstr(), end_time_attr->as_int());
		}

		return true;
	}
	else
	{
		// maybe it does not exist
		return true;
	}
}

void ManageMasterSession::saveForbidAccount()
{
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	Element * root_ele = auto_doc->make_element("forbid_account");
	auto_doc->set_root(root_ele);

	for (ForbidAccountInfoMap_t::iterator it = m_forbid_account_map.begin(); it != m_forbid_account_map.end(); ++it)
	{
		ForbidAccountInfo & forbid_account = it->second;
		Element * ele = auto_doc->make_element("forbid", root_ele);
		ele->set_attribute("account", forbid_account.account_id);
		ele->set_attribute("end_time", boost::lexical_cast<string>(forbid_account.end_time));
	}

	auto_doc->save_file("forbid_account.xml");
}

bool ManageMasterSession::loadForbidAccount()
{
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call XMLFactory::create_document()\n"));
		return false;
	}

	if (auto_doc->load_file("forbid_account.xml"))
	{
		Elements forbid_account_eles = auto_doc->get_root()->get_elements("forbid");
		for (Elements::iterator it = forbid_account_eles.begin(); it != forbid_account_eles.end(); ++it)
		{
			Element * ele = *it;
			Attribute * account_attr = ele->get_attribute("account");
			Attribute * end_time_attr = ele->get_attribute("end_time");
			if ((NULL != account_attr) && (NULL != end_time_attr))
			{
				ForbidAccountInfo forbid_account_info;
				forbid_account_info.account_id = account_attr->get_value();
				try
				{
					forbid_account_info.end_time = boost::lexical_cast<uint64>(end_time_attr->get_value().c_str());
				}
				catch (...)
				{
				}
				m_forbid_account_map.insert(std::make_pair(forbid_account_info.account_id, forbid_account_info));
			}
		}
		return true;
	}
	else
	{
		return true;
	}
}

bool ManageMasterSession::validatedAccount(const string & account_id)
{
	ForbidAccountInfoMap_t::iterator it = m_forbid_account_map.find(account_id);
	if (it != m_forbid_account_map.end())
	{
		ForbidAccountInfo & account_info = it->second;
		if (account_info.outOfTime())
		{
			m_forbid_account_map.erase(it);
			saveForbidAccount();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

bool ManageMasterSession::validatedIpAddr(const string & ip_addr)
{
	{
		ForbidIPInfoMap_t::iterator it = m_forbid_ip_map.find(ip_addr);
		if (it != m_forbid_ip_map.end())
		{
			// judge the time
			ForbidIPInfo & ip_info = it->second;
			if (!ip_info.outOfTime())
			{
				return false;
			}
		}
	}

	if (!m_forbid_all_ip)
	{
		return true;
	}

	{
		AllowIPInfoSet_t::iterator it = m_allow_ip_set.find(ip_addr);
		if (it != m_allow_ip_set.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void ManageMasterSession::collectForbidIpFlag()
{
	for (ForbidIPInfoMap_t::iterator it = m_forbid_ip_map.begin(); it != m_forbid_ip_map.end(); ++it)
	{
		const string & ip_addr = it->first;
		if (all_ip_str== ip_addr)
		{
			m_forbid_all_ip = true;
			return ;
		}
	}

	m_forbid_all_ip = false;
}