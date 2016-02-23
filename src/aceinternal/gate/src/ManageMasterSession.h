
#ifndef MANAGE_MASTER_SESSION_HPP
#define MANAGE_MASTER_SESSION_HPP

#include <ace/Thread_Mutex.h>
#include "Singleton.h"
#include "SingleConnection.h"
#include "AppNotifyInterface.h"

struct ForbidIPInfo
{
	ForbidIPInfo()
	: end_time(0)
	{}

	ForbidIPInfo(const string & ip, uint64 end_t)
	: end_time(end_t)
	, ip_addr(ip)
	{}

	bool outOfTime();

	uint64 end_time;	// Ω· ¯∑‚Õ£ ±º‰£¨Œ™0”¿æ√∑‚Õ£
	string ip_addr;
};

struct ForbidAccountInfo 
{
	ForbidAccountInfo()
	: end_time(0)
	{}

	ForbidAccountInfo(const string & account_v, uint64 end_t)
	: end_time(end_t)
	, account_id(account_v)
	{}

	bool outOfTime();

	uint64 end_time;	// Ω· ¯∑‚Õ£ ±º‰£¨Œ™0”¿æ√∑‚Õ£
	string account_id;
};

class ManageMasterSession : public netstream::SingleConnection, public AppNotifyInterface
{
public:
	ManageMasterSession();
	~ManageMasterSession();
public:
	static ManageMasterSession * instance()
	{
		return Singleton<ManageMasterSession>::instance();
	}
public:
	/// init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	virtual void handlePacket(Packet * packet);

	bool isValidatedAccountAndIP(const string & account_id, const string & ip_addr);
protected:
	void handleAllowIP(Packet * packet);
	void handleAllowIPRemove(Packet * packet);
	void handleForbidIP(Packet * packet);
	void handleForbidIPRemove(Packet * packet);
	void handleQueryIPSet(Packet * packet);
	void handleForbidAccount(Packet * packet);
	void handleForbidAccountRemove(Packet * packet);
	void handleForbidAccountQuery(Packet * packet);
	void handleSetGateLogLevel(Packet * packet);
	void handleCleanGateUnnormalPlayer(Packet * packet);
protected:
	void saveIPSetInfo();

	bool loadIPSetInfo();

	void saveForbidAccount();

	bool loadForbidAccount();

	bool validatedAccount(const string & account_id);

	bool validatedIpAddr(const string & ip_addr);

	void collectForbidIpFlag();
private:
	typedef map<string, ForbidIPInfo>	ForbidIPInfoMap_t;
	typedef set<string>					AllowIPInfoSet_t;
	typedef map<string, ForbidAccountInfo>	ForbidAccountInfoMap_t;

	bool m_forbid_all_ip;

	ForbidIPInfoMap_t m_forbid_ip_map;
	AllowIPInfoSet_t m_allow_ip_set;
	ForbidAccountInfoMap_t m_forbid_account_map;

	ACE_Thread_Mutex m_ip_query_mutex;
};

#endif