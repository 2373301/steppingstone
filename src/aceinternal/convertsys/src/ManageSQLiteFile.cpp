
#include <ace/OS.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "ManageSQLiteFile.h"
#include "Logger.h"
#include "SQLiteSerialize.h"
#include "Packet.h"
#include "protocol/msg_binary.pb.h"
#include "cachesys_opcode.hpp"
#include "ManageConfig.h"

#define PING_MYSQL_INTERVAL 30 * 60 * 1000 * 1000

#define MAX_MERGE_RECORD_NUMBER 100000

ManageSqliteFile::ManageSqliteFile()
: m_need_ping_db(true)
, m_process_record_no(0)
, m_merge_record_no(0)
, m_is_active(false)
, m_handle_output(NULL)
{}

ManageSqliteFile::~ManageSqliteFile()
{

}

int ManageSqliteFile::svc()
{
	LineTransRecord_t line_trans_record;

	::mysqlpp::Connection conn;
	conn.set_option(new mysqlpp::SetCharsetNameOption("utf8"));
	if (!this->connect_to_database(conn))
	{
		DEF_LOG_ERROR("failed to connect the database in ManageSqliteFile::svc, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	m_last_ping_db_time = ACE_OS::gettimeofday();

	string cache_addr;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_addr_que_mutex, -1);
		if (m_cache_addr_que.size() == 0)
		{
			// neven happen
			DEF_LOG_ERROR("error to get cache addr, empty size, last error is <%d>\n", ACE_OS::last_error());
			return -1;
		}
		cache_addr = m_cache_addr_que.front();
		m_cache_addr_que.pop_front();
	}

	SqliteQueueInfo sqlite_queue_info;

	this->registerQueue(cache_addr, &sqlite_queue_info);

	ACE_Time_Value sleep_time(0, 5 * 1000);

	string should_process_file;

	bool process_file_result = false;

	uint64 last_ping_msyq_time = 0;
	uint64 curr_time = 0;
	
	m_is_active = true;

	while (true)
	{
		if (!sqlite_queue_info.getFile(should_process_file))
		{
			ACE_OS::gettimeofday().to_usec(curr_time);
			if (curr_time - last_ping_msyq_time >= PING_MYSQL_INTERVAL)
			{
				if (!this->ping_db(conn))
				{
					DEF_LOG_ERROR("failed to ping mysql\n");
					return -1;
				}
				else
				{
					ACE_OS::gettimeofday().to_usec(last_ping_msyq_time);
				}
			}

			ACE_OS::sleep(sleep_time);
			continue;
		}

		process_file_result = processSqliteFile(cache_addr, should_process_file, conn, line_trans_record);

		if (process_file_result)
		{
			backupFile(should_process_file);
		}
	}
	return 0;
}

int ManageSqliteFile::init()
{
	if (ManageConfig::instance()->getCacheCfg().cache_vec.size() == 0)
	{
		return -1;
	}

	std::copy(ManageConfig::instance()->getCacheCfg().cache_vec.begin(), ManageConfig::instance()->getCacheCfg().cache_vec.end(), std::back_inserter(m_cache_addr_que));

	if (this->activate(THR_NEW_LWP, m_cache_addr_que.size()) == -1)
	{
		return -1;
	}

	boost::filesystem::path backup_dir(ManageConfig::instance()->getSerializeCfg().backup_directory);
	if ((!boost::filesystem::exists(backup_dir))
		&& (!boost::filesystem::create_directory(backup_dir)))
	{
		std::cout << "failed to create backup dir : " << ManageConfig::instance()->getSerializeCfg().backup_directory.c_str() << std::endl;
		return -1;
	}

	while(!m_is_active)
	{
		ACE_OS::sleep(1);
	}

	return 0;
}

int ManageSqliteFile::stop()
{
	return 0;
}

int ManageSqliteFile::finit()
{
	return 0;
}

void ManageSqliteFile::setHandleOutput(SqliteOutput * output)
{
	m_handle_output = output;
}

void ManageSqliteFile::handleFile(const string & cache_addr, const string & file_path)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sqlite_file_queue_mutex, );
	map<string, SqliteQueueInfo *>::iterator it = m_sqlite_file_queue.find(cache_addr);
	if (it != m_sqlite_file_queue.end())
	{
		it->second->handleSqliteFile(file_path);
	}
	else
	{
		DEF_LOG_ERROR("error to handle cache sqlite file, cache addr is <%s>, file path is <%s>\n", cache_addr.c_str(), file_path.c_str());
	}
}

int ManageSqliteFile::getLeftFileSize(const string & addr)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sqlite_file_queue_mutex, 1);
	map<string, SqliteQueueInfo *>::iterator it = m_sqlite_file_queue.find(addr);
	if (it != m_sqlite_file_queue.end())
	{
		return it->second->getFileSize();
	}
	else
	{
		DEF_LOG_ERROR("error to handle cache sqlite file, cache addr is <%s>\n", addr.c_str());
	}

	return 1;
}

bool ManageSqliteFile::lastFilePorcessFinish(const string & addr)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sqlite_file_queue_mutex, false);
	map<string, SqliteQueueInfo *>::iterator it = m_sqlite_file_queue.find(addr);
	if (it != m_sqlite_file_queue.end())
	{
		return it->second->last_file_process_finish;
	}
	else
	{
		DEF_LOG_ERROR("error to handle cache sqlite file, cache addr is <%s>\n", addr.c_str());
	}

	return false; 
}

bool ManageSqliteFile::connect_to_database(::mysqlpp::Connection& conn)
{
	SQLCfg & sql_cfg = ManageConfig::instance()->getSQLCfg();
	return conn.connect(sql_cfg.database_name.c_str(), sql_cfg.server_ip.c_str(), sql_cfg.user_name.c_str(), sql_cfg.password.c_str(), sql_cfg.port);
}

void ManageSqliteFile::registerQueue(const string & cache_addr, SqliteQueueInfo * sqlite_queue)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sqlite_file_queue_mutex, );
	m_sqlite_file_queue.insert(std::make_pair(cache_addr, sqlite_queue));
}

bool ManageSqliteFile::processSqliteFile(const string & cache_addr, const string & sqlite_file, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record)
{
	SQLiteSerialize sqlite;
	if (!sqlite.open(sqlite_file))
	{
		DEF_LOG_ERROR("error to open sqlite file : <%s>\n", sqlite_file.c_str());
		return false;
	}

	int once_read_no = 10000;
	int already_read_no = 0;
	uint64 guid = 0;
	RecordSet record_set;
	string condition_str;
	bool opr = true;
	uint32 curr_read_number = 0;

	uint64 start_read_time = 0;
	uint64 end_read_time = 0;
	do
	{
		record_set.clear();

		DEF_LOG_INFO("start to read record\n");
		ACE_OS::gettimeofday().to_usec(start_read_time);

		condition_str = " where guid > " + boost::lexical_cast<string>(guid) + " order by guid";
		if (!sqlite.read(once_read_no, condition_str, record_set))
		{
			DEF_LOG_ERROR("failed to read record set from file : <%s>\n", sqlite_file.c_str());
			return false;
		}

		ACE_OS::gettimeofday().to_usec(end_read_time);
		DEF_LOG_INFO("end to read record, record number is <%d>, spend time is <%llu>\n", record_set.size(), end_read_time - start_read_time);

		already_read_no = record_set.size();
		if (already_read_no > 0)
		{
			Record * last_record = *record_set.rbegin();
			guid = last_record->guid;
		}

		m_need_ping_db = record_set.size() == 0;

		m_process_record_no += record_set.size();

		curr_read_number += record_set.size();

		opr = processRecordSet(cache_addr, record_set, conn, line_trans_record) || opr;

	} 
	while (already_read_no >= once_read_no);

	checkMergeRecord(conn, true);

	if (opr)
	{
		DEF_LOG_INFO("success to process file, record num is <%d>, total number is <%llu>, file is <%s>\n", curr_read_number, m_process_record_no, sqlite_file.c_str());
	}
	else
	{
		DEF_LOG_ERROR("error to process sqlite file to database, record num is <%d>, total number is <%llu>, file is <%s>\n", curr_read_number, m_process_record_no, sqlite_file.c_str());
	}

	return opr;
}

bool ManageSqliteFile::processRecordSet(const string & cache_addr, RecordSet & record_set, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record)
{
	bool result = true;
	Record * record = NULL;

	for (RecordSet::iterator it = record_set.begin(); it != record_set.end(); ++it)
	{
		record = *it;

		bool opr = collectTransaction(cache_addr, record, conn, line_trans_record);

		result = opr || result;

		//delete record;
	}

	return result;
}

bool ManageSqliteFile::writeRecordToDb(Record * record, ::mysqlpp::Connection & conn)
{
	std::unique_ptr<CacheAssistantx> ca(sMsgCfg->getParser()->create(record->object_guid, record->entity_name));

	if (!ca.get())
	{
		DEF_LOG_ERROR("error to create cache assistant, guid is <%s>", boost::lexical_cast<string>(record->object_guid).c_str());
		return false;
	}

	if (RT_REMOVE != record->optype)
	{
		if (!parsePacket(record->stream, ca->data()))
		{
			DEF_LOG_ERROR("error to parse data from record, guid is <%s>\n", boost::lexical_cast<string>(record->object_guid).c_str());
			return false;
		}
	}

	bool op_result = false;
	mysqlpp::Query q = conn.query();
	switch (record->optype)
	{
	case RT_INSERT:
		op_result = ca->insert(q);
		DEF_LOG_DEBUG("insert entity to db, guid is <%llu>, write record to db result <%d>\n", record->object_guid, op_result);
		break;
	case RT_INSERT_WITH_FLUSH:
		op_result = ca->insert(q);
		DEF_LOG_DEBUG("insert and flush entity to db, guid is <%llu>, write record to db result <%d>\n", record->object_guid, op_result);
		break;
	//case RT_LOAD:
	//	break;
	//case RT_QUERY:
	//	break;
	case RT_UPDATE:
		op_result = ca->update(q);
		DEF_LOG_DEBUG("update entity to db, guid is <%llu>, write record to db result <%d>\n", record->object_guid, op_result);
		break;
	case RT_FLUSH:
		op_result = ca->update(q);
		DEF_LOG_DEBUG("flush entity to db, guid is <%llu>, write record to db result <%d>\n", record->object_guid, op_result);
		break;
	case RT_REMOVE:
		op_result = ca->remove(q);
		DEF_LOG_DEBUG("remove entity to db, guid is <%llu>, write record to db result <%d>\n", record->object_guid, op_result);
		break;
	default:
		DEF_LOG_ERROR("unknown op type <%d>\n", record->optype);
		break;
	}

	if (!op_result)
	{
		DEF_LOG_ERROR("errot to operate entity to db, guid is <%llu>, op type is <%d>, error code is <%d>, error msg is <%s>\n", record->object_guid, record->optype, ca->last_error(), ca->what().c_str());
	}

	return op_result;
}

bool ManageSqliteFile::ping_db(::mysqlpp::Connection & conn)
{
	if (!m_need_ping_db)
	{
		return true;
	}

	ACE_Time_Value diff_time = ACE_OS::gettimeofday() - m_last_ping_db_time;
	if (diff_time.sec() <= 30 * 60)
	{
		return true;
	}

	try {
		::mysqlpp::Query q = conn.query();
		q << "SELECT pt FROM ping";
		q.store();
		return true;
	}
	catch (const ::mysqlpp::BadQuery& er) {
		DEF_LOG_ERROR("Ping mysql failed: %s", er.what());
		return false;
	}
	catch (const ::mysqlpp::Exception& er) {
		DEF_LOG_ERROR("Ping mysql failed: %s", er.what());
		return false;
	}

	return true;
}

bool ManageSqliteFile::collectTransaction(const string & cache_addr, Record * record, ::mysqlpp::Connection & conn, LineTransRecord_t & line_trans_record)
{
	if (1 == record->transaction_num)
	{
		DEF_LOG_INFO("process transaction, line is <%d>, transaction id is <%d>, transaction num is <%d>\n", record->line_id, record->transaction_id, record->transaction_num);
		return processRecord(cache_addr, record, conn);
	}

	bool result = true;
	LineTransRecord_t::iterator it = line_trans_record.find(record->line_id);
	if (line_trans_record.end() == it)
	{
		line_trans_record[record->line_id][record->transaction_id].push_back(record);
	}
	else
	{
		map<uint32, RecordSet>::iterator sub_it = it->second.find(record->transaction_id);
		if (sub_it == it->second.end())
		{
			it->second[record->transaction_id].push_back(record);
		}
		else
		{
			sub_it->second.push_back(record);
			if (record->transaction_num == record->transaction_index)
			{
				// 
				DEF_LOG_INFO("process transaction, line is <%d>, transaction id is <%d>, transaction num is <%d>\n", record->line_id, record->transaction_id, record->transaction_num);
				for (RecordSet::iterator rec_it = sub_it->second.begin(); rec_it != sub_it->second.end(); ++rec_it)
				{
					result = processRecord(cache_addr, *rec_it, conn) && result;
				}

				it->second.erase(sub_it);
			}
		}
	}

	return result;
}

bool ManageSqliteFile::processRecord(const string & cache_addr, Record * record, ::mysqlpp::Connection & conn)
{
	DEF_LOG_DEBUG("start to process record, index is <%llu>, guid is <%llu>, optype is <%d>, transaction info <%d, %d, %d>\n", record->guid, record->object_guid, record->opcode, record->transaction_id, record->transaction_index, record->transaction_num);

	// merger record, for less db writing
	mergeRecord(cache_addr, record);

	checkMergeRecord(conn, false);

	return true;
}

void ManageSqliteFile::mergeRecord(const string & cache_addr, Record * record)
{
	MergeRecordMap_t::iterator it = m_merge_record_map.find(record->object_guid);
	if (it != m_merge_record_map.end())
	{
		Record * first_record = it->second;
		Record * second_record = record;

		first_record->stream = second_record->stream;
		first_record->stream_size = second_record->stream_size;

		DEF_LOG_DEBUG("merge record, guid is <%llu>, first optype is <%d>, second optype is <%d>\n", first_record->guid, first_record->optype, second_record->optype);

		switch (first_record->optype)
		{
			case RT_INSERT:
				if (RT_UPDATE == second_record->optype)
				{
				}
				else if (RT_FLUSH == second_record->optype)
				{
					first_record->last_modify_time = second_record->last_modify_time;
					first_record->last_is_flush = true;
				}
				else if (RT_REMOVE == second_record->optype)
				{
					// no need to db
					m_merge_record_map.erase(it);
					delete first_record;
				}
				break;
			case RT_UPDATE:
				if (RT_UPDATE == second_record->optype)
				{
				}
				else if (RT_FLUSH == second_record->optype)
				{
					first_record->optype = second_record->optype;
					first_record->last_modify_time = second_record->last_modify_time;
					first_record->last_is_flush = true;
				}
				else if (RT_REMOVE == second_record->optype)
				{
					first_record->optype = second_record->optype;
				}
				break;
			case RT_FLUSH:
				if (RT_UPDATE == second_record->optype)
				{
					first_record->last_is_flush = true;
				}
				else if (RT_FLUSH == second_record->optype)
				{
					first_record->last_modify_time = second_record->last_modify_time;
					first_record->last_is_flush = true;
				}
				else if (RT_REMOVE == second_record->optype)
				{
					first_record->optype = second_record->optype;
					first_record->last_is_flush = true;
				}
				break;
			case RT_REMOVE:
				// do nothing
				break;
			case RT_INSERT_WITH_FLUSH:
				first_record->last_is_flush = true;
				if (RT_UPDATE == second_record->optype)
				{
				}
				else if (RT_FLUSH == second_record->optype)
				{
					first_record->last_modify_time = second_record->last_modify_time;
				}
				else if (RT_REMOVE == second_record->optype)
				{
					// no need to db
					m_merge_record_map.erase(it);
					delete first_record;
				}
				break;
			default:
				DEF_LOG_ERROR("get unknow optye while merge <%d>\n", first_record->optype);
				break;
		}

		delete record;
	}
	else
	{
		record->cache_addr = cache_addr;
		if (RT_FLUSH== record->optype)
		{
			record->last_is_flush = true;
		}
		m_merge_record_map.insert(std::make_pair(record->object_guid, record));
	}
}

void ManageSqliteFile::checkMergeRecord(::mysqlpp::Connection & conn, bool file_process_finish)
{
	if (!file_process_finish)
	{
		// check the merge number
		if (m_merge_record_map.size() < MAX_MERGE_RECORD_NUMBER)
		{
			return ;
		}
	}

	DEF_LOG_INFO("start to process merge record to db, record is <%d>, totoal number is <%llu>\n", m_merge_record_map.size(), m_merge_record_no);

	uint64 start_time = 0;
	uint64 end_time = 0;
	ACE_OS::gettimeofday().to_usec(start_time);

	Record * record = NULL;
	for (MergeRecordMap_t::iterator it = m_merge_record_map.begin(); it != m_merge_record_map.end(); ++it)
	{
		record = it->second;
		bool opr = writeRecordToDb(record, conn);
		if (opr)
		{
			DEF_LOG_DEBUG("success to write record to db, guid is <%llu>, last is flush is <%d>\n", record->object_guid, record->last_is_flush ? 1 : 0);
			if (record->last_is_flush)
			{
				protocol::msg_binary_record sqlite_record;
				sqlite_record.set_guid(record->object_guid);
				sqlite_record.set_last_modify_time(record->last_modify_time);
				Packet * ps = new Packet(DCMSG_BINARY_RECORD, record->guid, sqlite_record.SerializeAsString());
				//ManageCacheSession::instance()->output(cache_addr, ps);
				m_handle_output->output(record->cache_addr, ps);
			}
		}
		else
		{
			DEF_LOG_ERROR("failed to write record to db, guid is <%llu>\n", record->object_guid);
		}

		delete record;
		record = NULL;
	}

	ACE_OS::gettimeofday().to_usec(end_time);

	m_merge_record_no += m_merge_record_map.size();

	DEF_LOG_INFO("end to process merge record to db, record is <%d>, total number is <%llu>, spend time is <%llu>\n", m_merge_record_map.size(), m_merge_record_no, end_time - start_time);

	m_merge_record_map.clear();
}

void ManageSqliteFile::backupFile(const string & file_path)
{
	boost::filesystem::path src_path(file_path);
	boost::filesystem::path dst_path(ManageConfig::instance()->getSerializeCfg().backup_directory);
	dst_path /= src_path.filename();

	boost::filesystem::rename(src_path, dst_path);
}