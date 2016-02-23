
#ifndef	_CLIENT_STRATEGY_CENTER_HPP
#define _CLIENT_STRATEGY_CENTER_HPP

#include <list>
#include <map>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "AppNotifyInterface.h"
#include "ClientSession.h"
#include "protocol/msg_error.pb.h"
#include "protocol/msg_player.pb.h"

class ClientStrategyCenter : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ClientStrategyCenter();
	~ClientStrategyCenter();
public:
	static ClientStrategyCenter * instance()
	{
		return Singleton<ClientStrategyCenter>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	virtual int svc(void);

public:
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	void clientConnected(ClientSession * client_session);

	void clientConnectionTimeout(ClientSession * client_session);

	void clientAuthSuccess(ClientSession * client_session);

	void clientNormalClosed(ClientSession * client_session);

	void clientExceptionClosed(ClientSession * client_session);

	void clientBeatTimeout(ClientSession * client_session);

	void clientNotFindGS(ClientSession * client_session);

	void closeClientByMapID(uint64 map_id);
public:
	void notFindGSWhileJumpMap(ClientSession * client_session);

	void clientGetRoleListFailed(ClientSession * client_session);

	void clientAuthFailed(ClientSession * client_session);

	void parseProtocolFailed(ClientSession * client_session, uint32 opcode);

	void checkRoleNameFailed(ClientSession * client_session);

	void makeRoleFailed(ClientSession * client_session);

	void reachMaxRoleNumber(ClientSession * client_session);

	void getAccountRoleNumberFailed(ClientSession * client_session);

	void kickPlayer(uint64 player_guid);

	void playerLogoutFromGS(uint64 player_guid, int leave_type);

	void streamSpeedException(uint64 player_guid);
private:
	void clientClosedNotifyGS(ClientSession * client_session);

	void closeClientHandle(ClientSession * client_session);

	void clientLogoutClean(ClientSession * client_session);

	void removeClientHandler(ClientSession * client_session);

	void sendClientErrorInfo(ClientSession * client_session, int error_code);
private:
	bool m_stop;

	bool m_wait;
};

#endif