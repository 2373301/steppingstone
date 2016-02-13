
#ifndef MANAGE_SQLITE_FILE_HPP
#define MANAGE_SQLITE_FILE_HPP

#include <ace/Reactor.h>
#include <ace/Thread_Mutex.h>
#include <ace/Task.h>
#include <mysql++.h>
#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "ManageConfig.h"
#include "SQLiteSerialize.h"
#include "Serialize.h"
#include "Packet.h"

using namespace cached::service;

typedef map<uint32, map<uint32, RecordSet> > LineTransRecord_t;

struct SqliteQueueInfo 
{
	SqliteQueueInfo()
		: last_file_process_finish(true)
	{}

	bool getFile(string & sqlite_file)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, file_queue_mutex, false);
		if (file_queue.size() == 0)
		{
			last_file_process_finish = true;
			return false;
		}
		else
		{
			last_file_process_finish = false;
			sqlite_file = file_queue.front();
			file_queue.pop_front();
			return true;
		}
	}

	void handleSqliteFile(const string & sqlite_file)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, file_queue_mutex, );
		file_queue.push_back(sqlite_file);
	}

	int getFileSize()
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, file_queue_mutex, 1);
		return file_queue.size();
	}

	bool			last_file_process_finish;

	deque<string>	file_queue;

	ACE_Thread_Mutex file_queue_mutex;
};

class SqliteOutput
{
public:
	virtual void output(const string & addr, Packet * ps) = 0;
};

class ManageSqliteFile : public SingletonInit, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageSqliteFile();
	~ManageSqliteFile();

public:
	static ManageSqliteFile * instance()
	{
		return Singleton<ManageSqliteFile>::instance();
	}
public:
	int svc();

public:
	virtual int init();

	virtual int stop();

	virtual int finit();
public:
	void setHandleOutput(SqliteOutput * output);

	void handleFile(const string & cache_addr, const string & file_path);

	int getLeftFileSize(const string & addr);

	bool lastFilePorcessFinish(const string & addr);
protected:
	bool connect_to_database(::mysqlpp::Connection& conn);

	void registerQueue(const string & cache_addr, SqliteQueueInfo * sqlite_queue);

	bool processSqliteFile(const string & cache_addr, const string & sqlite_file, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record);

	bool processRecordSet(const string & cache_addr, RecordSet & record_set, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record);

	bool writeRecordToDb(Record * record, ::mysqlpp::Connection & conn);

	bool ping_db(::mysqlpp::Connection & conn);

	bool collectTransaction(const string & cache_addr, Record * record, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record);

	bool processRecord(const string & cache_addr, Record * record, ::mysqlpp::Connection & conn);

	void mergeRecord(const string & cache_addr, Record * record);

	void checkMergeRecord(::mysqlpp::Connection & conn, bool file_process_finish);

	void backupFile(const string & file_path);
private:
	bool m_need_ping_db;

	uint64 m_process_record_no;

	uint64 m_merge_record_no;

	bool m_is_active;

	ACE_Time_Value m_start_time;

	SqliteOutput * m_handle_output;

	deque<string> m_cache_addr_que;

	ACE_Thread_Mutex m_cache_addr_que_mutex;

	map<string, SqliteQueueInfo *> m_sqlite_file_queue;

	ACE_Thread_Mutex m_sqlite_file_queue_mutex;

	ACE_Time_Value m_last_ping_db_time;

	typedef define_unordered_map<uint64, Record *> MergeRecordMap_t;

	MergeRecordMap_t m_merge_record_map;
};
#endif