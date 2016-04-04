
#ifndef MANAGE_CONFIG_HPP
#define MANAGE_CONFIG_HPP

#include "typedef.h"
#include "Singleton.h"
#include "SimpleXML.h"
#include "Logger.h"

struct GateAddr 
{
	string gate_member;
	string gate_client;
};

typedef vector<GateAddr> GateAddrVec_t;

struct DbConfig 
{
	DbConfig()
		: port(0)
	{}

	int	   port;
	string ip;
	string db_name;
	string user_name;
	string password;
};

class ManageConfig
{
public:
	ManageConfig();
	~ManageConfig();
public:
	static ManageConfig * instance()
	{
		return Singleton<ManageConfig>::instance();
	}
public:
	int init(const string & config_file);

	const string & getMemberAddr();

	const GateAddrVec_t & getTelecomGateAddrs();

	const GateAddrVec_t & getUnitedGatesAddrs();

	uint32 getLineId();

	const string & getMemberSecurityKey();

	const string getGateAddrByType(int network_type);

	const DbConfig & getAccountDb();

	const DbConfig & getGameDb();

	const DbConfig & getGmlogDb();

	int getUpdateInterval();

protected:
	bool loadConfig(const string & config_file);

	bool loadMemberCfg(Element * root_element);

	bool loadGateCfg(Element * root_element);

	bool loadLineCfg(Element * root_element);

	bool loadMemberSecurityKeyCfg(Element * root_element);

	bool loadAccountDb(Element * root_element);

	bool loadGameDb(Element * root_element);

	bool loadGmlogDb(Element * root_element);

	bool loadUpdateInterval(Element * root_element);

	const string getTelecomGateAddr();
	
	const string getUnitedGatesAddr();
private:
	string m_member_addr;

	// µÁ–≈
	GateAddrVec_t m_telecom_gate;

	// Õ¯Õ®
	GateAddrVec_t m_united_gate;

	uint32 m_line_id;

	string m_member_security_key;

	uint32 m_telecom_index;

	uint32 m_united_index;

	DbConfig m_account_db;

	DbConfig m_game_db;

	string m_client_addr_1;

	string m_client_addr_2;

	DbConfig m_gmlog_db;

	int m_update_interval;
};

#endif
