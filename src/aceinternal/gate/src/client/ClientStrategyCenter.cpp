
#include "ClientStrategyCenter.h"
#include "RouteGSPacket.h"
#include "ManageClientBeat.h"
#include "opcode.h"
#include "ManageStat.h"
#include "ManageClientValidation.h"
#include "ManageClientConnectionTimeout.h"
#include "RouteClientPackage.h"
#include "ManageClientSessionCooler.h"
#include "ManageClientValidation.h"

ClientStrategyCenter::ClientStrategyCenter()
: m_stop(false)
, m_wait(true)
{
	m_wait = false;
}
ClientStrategyCenter::~ClientStrategyCenter()
{
}

int ClientStrategyCenter::svc(void)
{
	FUNCTIONTRACE(ClientStrategyCenter::svc);
	REPORT_THREAD_INFO("ClientStrategyCenter::svc", ACE_OS::thr_self());
	while(!m_stop)
	{
		ACE_OS::sleep(10);
	}

	m_wait = false;
	return 0;
}

int ClientStrategyCenter::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to init ClientStrategyCenter, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ClientStrategyCenter\n"));
		return 0;
	}
}

int ClientStrategyCenter::fini()
{
	m_stop = true;
	return 0;
}

int ClientStrategyCenter::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ClientStrategyCenter\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ClientStrategyCenter::clientConnected(ClientSession * client_session)
{
	ManageStat::instance()->statClientConnection(1);

	ManageClientConnectionTimeout::instance()->putClientSession(client_session);
}

void ClientStrategyCenter::clientConnectionTimeout(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("client connection timeout, account is <%s>\n"), client_session->getAccountID().c_str());
	client_session->setConnectionTimeout(true);
	removeClientHandler(client_session);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
}

void ClientStrategyCenter::clientAuthSuccess(ClientSession * client_session)
{
	ManageClientConnectionTimeout::instance()->removeClientSession(client_session);
	ManageStat::instance()->statLoginSuccessAuth(1);
	ManageClientValidation::instance()->accountAuthSuccess(client_session);
	ManageClientBeat::instance()->addClient(client_session);
}

void ClientStrategyCenter::clientNormalClosed(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("client normal closed, guid is <%llu>\n"), client_session->getRoleGuid());
	clientLogoutClean(client_session);
}

void ClientStrategyCenter::clientExceptionClosed(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("client exception closed, guid is <%llu>\n"), client_session->getRoleGuid());
	clientClosedNotifyGS(client_session);
	clientLogoutClean(client_session);
}

void ClientStrategyCenter::clientBeatTimeout(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("client beat timeout, guid is <%llu>, client session pointer is <%x>\n"), client_session->getRoleGuid(), (void *)client_session);
	client_session->setBeatTimeout(true);
	//removeClientHandler(client_session);
	//clientClosedNotifyGS(client_session);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
}

void ClientStrategyCenter::clientNotFindGS(ClientSession * client_session)
{
	//client_session->setWaitLogoutFromGS(false);
	client_session->setNotFindGS(true);
	//client_session->setClientLogout(true);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
	//clientLogoutClean(client_session);
}

void ClientStrategyCenter::closeClientByMapID(uint64 map_id)
{
	ManageClientBeat::ClientSessionList client_session_list;
	ManageClientBeat::instance()->getOnlineClient(client_session_list);
	for (ManageClientBeat::ClientSessionList::iterator it = client_session_list.begin(); it != client_session_list.end(); ++it)
	{
		if ((*it)->getRoleLocation() == map_id)
		{
			GATE_LOG_ERROR(ACE_TEXT("start to remove the client session in ClientStrategyCenter::closeClientByMapID, pointer is <%x>\n"), (void *)(*it));
			clientNotFindGS(*it);
		}
	}
}

void ClientStrategyCenter::notFindGSWhileJumpMap(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::clientGetRoleListFailed(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::clientAuthFailed(ClientSession * client_session)
{
	ManageClientConnectionTimeout::instance()->removeClientSession(client_session);
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::parseProtocolFailed(ClientSession * client_session, uint32 opcode)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::checkRoleNameFailed(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::makeRoleFailed(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::reachMaxRoleNumber(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::getAccountRoleNumberFailed(ClientSession * client_session)
{
	sendClientErrorInfo(client_session, 1);
}

void ClientStrategyCenter::kickPlayer(uint64 player_guid)
{
	ClientSession * client_session = RouteClientPackage::instance()->getRoleClientSession(player_guid);
	if (NULL != client_session)
	{
		GATE_LOG_INFO(ACE_TEXT("start to kick player, guid is <%llu>\n"), player_guid);
		//client_session->setWaitLogoutFromGS(false);
		//client_session->setClientLoginGS(false);
		GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(client_session);
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("start to kick player, guid is <%llu>, but not find the player session\n"), player_guid);
	}
}

void ClientStrategyCenter::playerLogoutFromGS(uint64 player_guid, int leave_type)
{
	if (1 == leave_type)
	{
		GATE_LOG_INFO(ACE_TEXT("Getting player logout from gs, guid is <%llu>\n"), player_guid);
		if (ManageClientValidation::instance()->isInWaitQueue(player_guid))
		{
			GATE_LOG_INFO(ACE_TEXT("Getting player logout from gs, player in queue, guid is <%llu>\n"), player_guid);
			ManageClientValidation::instance()->logoutFromGS(player_guid);
		}
		else
		{
			GATE_LOG_INFO(ACE_TEXT("Getting player logout from gs, while finding the player, guid is <%llu>\n"), player_guid);
			ClientSession * client_session = RouteClientPackage::instance()->getRoleClientSession(player_guid);
			if (NULL != client_session)
			{
				ManageClientValidation::instance()->accountLogout(client_session);
				GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
				ManageClientSessionCooler::instance()->putClientSession(client_session);
			}
			else
			{
				GATE_LOG_INFO(ACE_TEXT("Getting player logout from gs, but not find the player, guid is <%llu>\n"), player_guid);
			}
		}
	}
	else if (1 == leave_type)
	{
		ClientSession * client_session = RouteClientPackage::instance()->getRoleClientSession(player_guid);
		if (NULL != client_session)
		{
			client_session->setWaitLogoutFromGS(false);
			client_session->setClientLoginGS(false);
		}
		GATE_LOG_INFO(ACE_TEXT("player leave world to role choose , guid is <%llu>\n"), player_guid);
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("failed to get leave type : <%d>, player guid is <%llu>\n"), (int)leave_type, player_guid);
	}
}

void ClientStrategyCenter::streamSpeedException(uint64 player_guid)
{
	ClientSession * cs = RouteClientPackage::instance()->getRoleClientSession(player_guid);
	if (NULL != cs)
	{
		GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(cs);
	}
}

void ClientStrategyCenter::clientClosedNotifyGS(ClientSession * client_session)
{
	//client already send rolechoose message to gs
	if (client_session->getClientLoginGS())
	{
		//client_session->setClientLogout(true);
		client_session->setNotFindGS(true);
		typed::protocol::cmsg_player_leave_map leave_map;
		leave_map.set_leave_type(1);

		PacketStream * ps = new PacketStream(CMSG_PLAYER_LEAVE_MAP, client_session->getRoleGuid(), leave_map.SerializeAsString());
		ps->setRoleLocation(client_session->getRoleLocation());
		ps->setLineNO(client_session->getLineNo());
		RouteClientPackage::instance()->putInputPackage(ps);
	}
}

void ClientStrategyCenter::closeClientHandle(ClientSession * client_session)
{
	client_session->closeHandle();
}

void ClientStrategyCenter::clientLogoutClean(ClientSession * client_session)
{
	RouteGSPacket::instance()->removeClientThreadPacketListInfo(client_session->getRoleGuid());

	client_session->cleanup();

	if (!client_session->getBeatTimeout())
	{
		ManageClientBeat::instance()->removeClient(client_session);
	}

	if (client_session->getConnected())
	{
		ManageStat::instance()->statClientDisconnection(1);
	}
}

void ClientStrategyCenter::removeClientHandler(ClientSession * client_session)
{
	if (client_session->reactor() != NULL)
	{
		client_session->reactor()->remove_handler(client_session, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
	}
}

void ClientStrategyCenter::sendClientErrorInfo(ClientSession * client_session, int error_code)
{
	typed::protocol::smsg_error msg_error;
	msg_error.set_error(error_code);
	client_session->handleOutputStream(new PacketStream(SMSG_ERROR, client_session->getRoleGuid(), msg_error.SerializeAsString()));
}