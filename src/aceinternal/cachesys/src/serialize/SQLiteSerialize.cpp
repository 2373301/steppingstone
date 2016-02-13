
#include <sstream>
#include <ace/OS.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "SQLiteSerialize.h"
#include "Logger.h"

namespace cached {
namespace service {

const string insert_record_sql = "insert into record(guid, object_guid, owner_guid, opcode, stream_size, stream, optype, last_modify_time, trans_index, trans_num, trans_id, line_id) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

SQLiteSerialize::SQLiteSerialize(void)
: m_db(NULL)
{
}

SQLiteSerialize::~SQLiteSerialize(void)
{
	close();
}

bool SQLiteSerialize::open(const string & file_name)
{
	boost::filesystem::path pa = boost::filesystem::path(file_name);
	boost::filesystem::path p = boost::filesystem::system_complete(pa);
	m_file_name = p.string();
	//string file_path = BINARY_DIR;
	//file_path += file_name;
	int result = sqlite3_open_v2(m_file_name.c_str() ,&m_db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (SQLITE_OK == result)
	{
		result = sqlite3_exec(m_db, "PRAGMA synchronous = OFF", NULL, NULL, NULL);
		if (SQLITE_OK == result)
			return true;
		else
			return false;
	}
	else
	{
		//DEF_LOG_ERROR("Failed to open sqlite, last error is <%d>", result);
		return false;
	}
}

bool SQLiteSerialize::exeCmd(const string & command_str)
{
	if (NULL == m_db)
	{
		return false;
	}
	char* zErrMsg = NULL;
	int result = sqlite3_exec(m_db ,command_str.c_str(),NULL,NULL,&zErrMsg);
	if (SQLITE_OK == result)
	{
		return true;
	}
	else
	{
		sqlite3_free(zErrMsg);
		DEF_LOG_ERROR("Failed to exe sqlite cmd, the cmd is <%s>, last error is <%d>", command_str.c_str(), result);
		return false;
	}
}

bool SQLiteSerialize::read(int read_number, const string & condition, RecordSet & record_set)
{
	bool result = false;
	int curr_read = 0;

	if (NULL == m_db)
	{
		DEF_LOG_ERROR("db is null while read, last error is <%d>\n", ACE_OS::last_error());
		return false;
	}

	sqlite3_stmt * stmt = NULL;
	std::ostringstream oss_;
	oss_.clear();

	oss_ << "SELECT guid, object_guid, opcode, stream_size, stream, optype, last_modify_time, trans_index, trans_num, trans_id, line_id FROM record ";

	if(condition.length())
	{
		oss_ << condition.c_str();
	}
	
	int sqlite_result = sqlite3_prepare(m_db ,oss_.str().c_str(),-1,&stmt,NULL);

	if (SQLITE_OK == sqlite_result)
	{
		Record * record = NULL;
		result = true;
		int s;

		while(curr_read < read_number)
		{
			s = sqlite3_step (stmt);
			if (s == SQLITE_ROW ) 
			{
				record = new Record();

				record->guid = sqlite3_column_int64(stmt, 0);
				record->object_guid = sqlite3_column_int64(stmt, 1);
				record->opcode = sqlite3_column_int(stmt, 2);
				record->stream_size = sqlite3_column_int(stmt, 3);
				record->stream = std::string((char*)(sqlite3_column_blob(stmt, 4)),record->stream_size);
				record->optype = sqlite3_column_int(stmt, 5);
				record->last_modify_time = sqlite3_column_int64(stmt, 6);
				record->transaction_index = sqlite3_column_int(stmt, 7);
				record->transaction_num = sqlite3_column_int(stmt, 8);
				record->transaction_id = sqlite3_column_int(stmt, 9);
				record->line_id = sqlite3_column_int(stmt, 10);

				record_set.push_back(record);
				++curr_read;
			}
			else if (s == SQLITE_DONE) 
			{
				result = true;
				break;
			}
			else {
				result = false;
				DEF_LOG_ERROR("Failed to read record, the guid is <%llu>, last error is <%d>\n", record->object_guid, sqlite_result);
				break;
			}
		}

		if ((SQLITE_ROW == s) || (SQLITE_DONE == s))
		{
			sqlite3_finalize(stmt);
		}
	}
	else
	{
		DEF_LOG_ERROR("Failed to read record, the cmd is <%s>, last error is <%d>\n", oss_.str().c_str(), sqlite_result);
	}

	return result;
}

bool SQLiteSerialize::beginWriteTransaction()
{
	sqlite3_exec(m_db, "begin transaction;", NULL, NULL, NULL);
	return true;
}

bool SQLiteSerialize::endWriteTransaction()
{
	sqlite3_exec(m_db, "commit transaction;", NULL, NULL, NULL);
	return true;
}

bool SQLiteSerialize::write(const Record & record)
{
	if (NULL == m_db)
	{
		return false;
	}

	bool result = false;
	sqlite3_stmt * stat = NULL;
	int exe_result = sqlite3_prepare(m_db, insert_record_sql.c_str(), -1, &stat, 0);
	exe_result = sqlite3_bind_int64(stat, 1, record.guid);
	exe_result = sqlite3_bind_int64(stat, 2, record.object_guid);
	exe_result = sqlite3_bind_int64(stat, 3, record.owner_guid);
	exe_result = sqlite3_bind_int(stat, 4, record.opcode);
	exe_result = sqlite3_bind_int(stat, 5, record.stream_size);
	exe_result = sqlite3_bind_blob(stat, 6, record.stream.data(), record.stream.size(), 0);
	exe_result = sqlite3_bind_int(stat, 7, record.optype);
	exe_result = sqlite3_bind_int64(stat, 8, record.last_modify_time);
	exe_result = sqlite3_bind_int(stat, 9, record.transaction_index);
	exe_result = sqlite3_bind_int(stat, 10, record.transaction_num);
	exe_result = sqlite3_bind_int(stat, 11, record.transaction_id);
	exe_result = sqlite3_bind_int(stat, 12, record.line_id);
	exe_result = sqlite3_step(stat);
	if (SQLITE_DONE == exe_result)
	{
		result = true;
	}
	else
	{
		//DEF_LOG_ERROR("Failed to write sqlite record, last error is <%d>", result);
		result = false;
	}
	sqlite3_finalize(stat);
	return result;
}

bool SQLiteSerialize::close()
{
	if (NULL != m_db)
	{
		sqlite3_close(m_db);
		m_db = NULL;
	}
	return true;
}

const string & SQLiteSerialize::getFileName()
{
	return m_file_name;
}

};
};