
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/message.h>
#include "ManageSerialize.h"
#include "SQLiteSerialize.h"
#include "SerializeFactory.h"
#include "config.h"
//#include "msg.h"
#include "Packet.h"
#include "FileNameGenerator.h"
//#include "ManageFileSerialize.h"
#include "cachesys_opcode.hpp"
#include "ManageConvertSession.h"
#include "protocol/msg_cache.pb.h"
#include "DataRequestInfo.h"

namespace cached {
namespace service {

#define CREATE_RECORD_TABLE "create table record (guid INTEGER, object_guid INTEGER, owner_guid INTEGER, opcode INTEGER, stream_size INTEGER, stream BLOB, optype INTEGER, last_modify_time INTEGER, trans_index INTEGER, trans_num INTEGER, trans_id INTEGER, line_id INTEGER)"
#define CREATE_RECORD_INDEX "create index index_guid on record(guid)"

ManageSerialize::ManageSerialize()
: m_serialize(NULL)
, m_guid_index(0)
, m_stop(false)
, m_wait(true)
{
}

ManageSerialize::~ManageSerialize()
{
	for (RecordSet::iterator it = m_record_set.begin(); it != m_record_set.end(); ++it)
	{
		delete *it;
	}
	m_record_set.clear();

	if (NULL != m_serialize)
	{
		delete m_serialize;
	}
}
int ManageSerialize::stop()
{
	m_stop = true;
	return 0;
}

int ManageSerialize::finit()
{
	while (m_wait)
	{
		std::cout << "wait ManageSerialize" << std::endl;
		ACE_OS::sleep(1);
	}

	std::cout << "exit ManageSerialize" << std::endl;

	return 0;
}

bool ManageSerialize::open()
{
	m_serialize = SerializeFactory::makeSerialize(CONTAINER->lookup(SERIALIZE_TYPE));

	if (NULL == m_serialize)
	{
		return false;
	}

	//if (!ManageFileSerialize::instance()->open())
	//{
	//	return false;
	//}

	if (!createBinaryDir(BINARY_DIR))
		return false;

	if (!initSerialize(m_file_name_generator.getFileName()))
	{
		return false;
	}

	bool result = this->activate() == -1;
	ACE_OS::sleep(3);
	return !result;
}

void ManageSerialize::writeRecord(DataRequestInfo * data_request_info, RecordType record_type)
{
	Record * record = new Record();
	record->guid = getGUID();
	record->object_guid = data_request_info->guid;
	record->owner_guid = data_request_info->owner_guid;
	record->opcode = data_request_info->opcode;
	record->stream = data_request_info->data_request->data_stream();
	record->stream_size = record->stream.size();
	record->optype = record_type;
	record->last_modify_time = data_request_info->last_modify_time;

	record->transaction_index = data_request_info->data_request->transaction_index();
	record->transaction_num = data_request_info->data_request->transaction_num();
	record->transaction_id = data_request_info->data_request->transaction_id();
	record->line_id = data_request_info->data_request->line_id();

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_record_set_mutex, );
		m_record_set.push_back(record);
	}

}

Serialize & ManageSerialize::getSerialize()
{
	return *m_serialize;
}

int ManageSerialize::svc()
{
	ACE_Time_Value sleep_time(0, 1 * 1000);
	string file_name;
	//Record last_write_record;
	RecordSet record_set;
	RecordSet record_set_left;
	while (!m_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_record_set_mutex, -1);
			std::copy(m_record_set.begin(), m_record_set.end(), std::back_inserter(record_set));
			m_record_set.clear();
		}

		if (m_file_name_generator.expire())
		{
			string curr_file_name = m_serialize->getFileName();

			file_name = m_file_name_generator.getFileName();
			bool init_res = initSerialize(file_name);
			DEF_LOG_INFO("binary file time expire, the pre file is <%s>, curr file is <%s>, init result is <%d>\n", curr_file_name.c_str(), file_name.c_str(), init_res);

			ManageConvertSession::instance()->handleFile(curr_file_name);
		}

		if (record_set.empty())
		{
			ACE_OS::sleep(sleep_time);

			if (m_stop)
			{
				break;
			}

			continue;
		}

		//last_write_record = **record_set.rbegin();

		Record * curr_record = NULL;

		uint64 start_time = 0;
		uint64 end_time = 0;
		ACE_OS::gettimeofday().to_usec(start_time);
		DEF_LOG_INFO("start to write record, record size is <%d>\n", record_set.size());

		m_serialize->beginWriteTransaction();
		for (RecordSet::iterator it = record_set.begin(); it != record_set.end(); ++it)
		{
			curr_record = *it;

			//if ((curr_record->transaction_id == last_write_record.transaction_id) 
			//	&& (last_write_record.transaction_index < last_write_record.transaction_num))
			//{
			//	record_set_left.push_back(curr_record);
			//}
			//else
			{
				doWritingRecord(curr_record);
			}
		}
		m_serialize->endWriteTransaction();

		ACE_OS::gettimeofday().to_usec(end_time);
		DEF_LOG_INFO("end to write record, record size is <%d>, spend time is <%llu>\n", record_set.size(), end_time - start_time);

		record_set.clear();
		std::copy(record_set_left.begin(), record_set_left.end(), std::back_inserter(record_set));
		record_set_left.clear();
	}

	m_wait = false;
	return 0;
}

uint64 ManageSerialize::getGUID()
{
	return ++m_guid_index;
}

bool ManageSerialize::createBinaryDir(const string & binary_dir)
{
	fs::path path(binary_dir);
	if (!fs::exists(binary_dir))
	{
		return fs::create_directory(binary_dir);
	}

	return true;
}

void ManageSerialize::doWritingRecord(Record * record)
{
	if (NULL != m_serialize)
	{
		m_serialize->write(*record);
	}
	else
	{
		// error
	}

	delete record;
}

bool ManageSerialize::initSerialize(const string & file_name)
{
	string file_path = BINARY_DIR;
	file_path += file_name;
	if (NULL != m_serialize)
	{
		m_serialize->close();

		if (m_serialize->open(file_path))
		{
			bool result = m_serialize->exeCmd(CREATE_RECORD_TABLE);
			result = m_serialize->exeCmd(CREATE_RECORD_INDEX) && result;
			return result;
		}
	}
	return false;
}

bool ManageSerialize::shutdown()
{
	m_stop = true;
	if (this->wait() == -1)
	{
		return false;
	}
	return true;
}

};
};