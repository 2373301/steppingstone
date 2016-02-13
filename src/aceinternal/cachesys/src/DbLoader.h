
#ifndef DB_LOADER_HPP
#define DB_LOADER_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Logger.h"
#include "protocol/msg_cache.pb.h"
#include "cache_auto_assistant.h"
#include "factory.h"
#include "cachesys_opcode.hpp"
#include "DataRequestInfo.h"
#include "Singleton.h"
#include "SingletonInit.h"

using namespace dbass;

namespace cached {
namespace service {



class DbLoader : public ACE_Task<ACE_NULL_SYNCH>, public SingletonInit
{
public:
	DbLoader();
	~DbLoader();
public:
	static DbLoader * instance()
	{
		return Singleton<DbLoader>::instance();
	}
public:
	virtual int init();

	virtual int stop();

	virtual int finit();

	int svc();

public:
	void process(DataRequestInfo * data_request_info);

protected:
	void handleRequest(::mysqlpp::Connection & conn, DataRequestInfo * data_request_info);

	void handleLoadRequest(::mysqlpp::Connection & conn, DataRequestInfo * data_request_info);

private:
	bool connect_to_database(::mysqlpp::Connection& conn);

	bool connect_to_database(::mysqlpp::Connection& conn, const ::std::string& host, uint16 port, const ::std::string& user, const ::std::string& password, const ::std::string& database_name);

	void ping_db(::mysqlpp::Connection& conn);
private:
	typedef vector<DataRequestInfo *> DataRequestInfoVec_t;

	DataRequestInfoVec_t m_data_req_info_vec;

	ACE_Thread_Mutex m_data_req_info_vec_mutex;

	ACE_Thread_Mutex m_sql_connection_mutex;
};

};  // namespace service
};  // namespace cached

#endif