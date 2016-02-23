#ifndef _MANAGE_STAT_HPP
#define _MANAGE_STAT_HPP

#include <ace/Task.h>
#include <ace/Date_Time.h>
#include <ace/Time_Value.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "AppNotifyInterface.h"
#include "GateLogger.h"
#include "typedef.h"

struct NetWorkTrafficStat
{
	NetWorkTrafficStat()
	: packet_number(0)
	, byte_number(0)
	, packet_loss(0)
	, current_wobble_network(0)
	, max_wobble_network(0)
	, role_make_total_time(0)
	, role_make_total_number(0)
	{
		start_time = ACE_OS::gettimeofday();
	}

	int64				packet_number;
	int64				byte_number;
	int64				packet_loss;
	int64				current_wobble_network;
	int64				max_wobble_network;
	int64				role_make_total_time;
	int64				role_make_total_number;
	ACE_Time_Value		start_time;
	ACE_Thread_Mutex	thread_mutex;
};

struct ConnectionStat
{
	ConnectionStat()
	: connection_number(0)
	, disconnection_number(0)
	, validation_number(0)
	, max_online_connection(0)
	, active_close(0)
	, passive_close(0)
	{}

	int64				connection_number;
	int64				disconnection_number;
	int64				validation_number;
	int64				max_online_connection;
	int64				active_close;
	int64				passive_close;
	ACE_Thread_Mutex	thread_mutex;
};

struct LoginStat
{
	LoginStat()
	: recv_auth_number(0)
	, auth_success_number(0)
	, auth_failed_number(0)
	{}

	int64	recv_auth_number;
	int64	auth_success_number;
	int64	auth_failed_number;
	ACE_Thread_Mutex	thread_mutex;
};

class ManageStat : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageStat();
	~ManageStat();
public:
	static ManageStat * instance()
	{
		return Singleton<ManageStat>::instance();
	}
public:
	virtual int svc(void);

public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	/// init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	uint32 getOnLineClientNuber();

public:
	void statClientInputTraffic(int byte_number, int packet_number = 1);

	void statClientOutputTraffic(int byte_number, int packet_number = 1);

	void statClientConnection(int connection_number);

	void statClientDisconnection(int disconnection_number);

	void statClientPacketLoss(int loss_number);

	void statClientWobbleNetwork(int wobble_network);

	void statRoleMakeTime(int role_make_time, int role_make_number = 1);

	void statClientActiveClose(int close_number);

	void statClientPassiveClose(int close_number);

	// gs
	void statGSInputTraffic(int byte_number, int packet_number = 1);

	void statGSOutputTraffic(int byte_number, int packet_number = 1);

	void statGSConnection(int connection_number);

	void statGSDisconnection(int disconnection_number);

	void statGSPacketLoss(int loss_number);

	// login
	void statLoginRecvAuth(int auth_number);

	void statLoginSuccessAuth(int success_auth);

	void statLoginFailedAuth(int failed_auth);

protected:
private:
	bool m_stop;

	bool m_wait;

	// client
	NetWorkTrafficStat	m_client_input_traffic;

	NetWorkTrafficStat	m_client_output_traffic;

	ConnectionStat		m_client_connection;
	
	// gs
	NetWorkTrafficStat	m_gs_input_traffic;

	NetWorkTrafficStat	m_gs_output_traffic;

	ConnectionStat		m_gs_login_connection;

	// login 
	LoginStat			m_login_stat;

	int					m_report_interval;
};

#endif