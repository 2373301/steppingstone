#ifndef _MANAGE_CLIENT_VALIDATION_HPP
#define _MANAGE_CLIENT_VALIDATION_HPP

#include <map>
#include <ace/Task.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include "GateLogger.h"
#include "AppNotifyInterface.h"
#include "ManageConfig.h"
#include "Singleton.h"
#include "LoginServerSession.h"
//#include "protocol/msg_login.pb.h"
#include "PacketStream.h"
#include "gate.h"
#include "protocol/msg_member.pb.h"

struct ClientAuth
{
	ClientAuth()
	: request_index(0)
	, addiction_flag(0)
	, network_type(0)
	, line_id(0)
	{
		register_time = ACE_OS::gettimeofday();
	}

	ClientAuth(uint32 request_i, const string & a_n, uint32 addiction_f, uint32 network_t, const string & s_id, uint32 line_i)
	: request_index(request_i)
	, account_id(a_n)
	, addiction_flag(addiction_f)
	, network_type(network_t)
	, session_id(s_id)
	, line_id(line_i)
	{
		register_time = ACE_OS::gettimeofday();
	}

	uint32 request_index;
	string account_id;
	uint32 addiction_flag;
	uint32 network_type;
	string session_id;
	uint32 line_id;
	ACE_Time_Value register_time;
	
	string open_id;
	string open_key;
	string pf;
	string pfkey;
};

class LogoutAccount 
{
public:
	LogoutAccount()
	: line_no(0)
	, player_guid(0)
	, map_id(0)
	{}

	LogoutAccount(const string & ai, int ln, uint64 pg, uint64 md)
	: account_id(ai)
	, line_no(ln)
	, player_guid(pg)
	, map_id(md)
	{}

	//LogoutAccount(LogoutAccount & rsh)
	//{
	//	rsh.account_id = account_id;
	//	rsh.line_no = line_no;
	//	rsh.player_guid = player_guid;
	//}

	string account_id;
	int   line_no;
	uint64 player_guid;
	uint64 map_id;
};

class ManageClientValidation : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientValidation();
	~ManageClientValidation();
public:
	static ManageClientValidation * instance()
	{
		return Singleton<ManageClientValidation>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::map<string, ClientAuth>	ClientAuthMap;

	virtual int svc(void);

	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);

public:
	/// init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();

	void handlePackage(PacketStream * package_stream);

	void accountAuthSuccess(ClientSession * client_session);

	void accountLogout(ClientSession * client_session);

	void accountLogout(const string & account_id, int line_no);

	bool validateClientSession(const string & session_id, ClientAuth *& client_auth, const string & account_id);

	void waitFromGSLogoutMsg(const string & account_id, int line_no, uint64 player_guid, uint64 map_id);

	bool isInWaitQueue(uint64 player_guid);

	void logoutFromGS(uint64 player_guid);

	void getOnlineClient(std::list<LogoutAccount> & online_client);

	void gsClosed(uint64 map_id);

	void roleChooseLine(int account_id, int line_no);

	void handleMemberChargeConfirm(PacketStream * packet_stream);

	int handleMemberGiveGiftReturn(PacketStream * packet_stream);

protected:
	int handleMemberLogin(PacketStream * packet_stream);

	int handleMemberCharge(PacketStream * packet_stream);

	int handleMemberGiveGift(PacketStream * packet_stream);

	void registerClientAuth(typed::protocol::mmsg_member_login & member_login);

	void respondMemberLogin(typed::protocol::mmsg_member_login & member_login);

private:
	bool m_stop;

	bool m_wait;

	long m_timer_id;

	long m_report_traffic_timer_id;

	long m_beat_to_login_timer_id;

	bool m_check_client_auth;

	ACE_Thread_Mutex m_client_auth_mutex;

	ClientAuthMap m_client_auth_map;

	typedef map<uint64, LogoutAccount>	LogoutAccountMap_t;

	LogoutAccountMap_t	m_logout_account_map;

	ACE_Thread_Mutex	m_logout_account_mutex;

	ACE_Thread_Mutex	m_login_server_register_mutex;
};

#endif