
#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <string>
#include <vector>
#include "typedef.h"
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>

#define STAT_NUM_DEC \
	static uint64 m_new_count;	\
	static uint64 m_delete_count;	\
	static ACE_Thread_Mutex m_new_mutex;	\
	static ACE_Thread_Mutex m_delete_mutex;	

#define STAT_NUM_IMP(class_name) \
	uint64 class_name::m_new_count = 0;	\
	uint64 class_name::m_delete_count = 0;	\
	ACE_Thread_Mutex class_name::m_new_mutex;	\
	ACE_Thread_Mutex class_name::m_delete_mutex;	

#define STAT_NUM_INC_NEW	\
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_new_mutex, ) \
	++m_new_count;	\
	}

#define STAT_NUM_INC_DEL \
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_delete_mutex, ) \
	++m_delete_count;	\
	}

namespace cached {
namespace service {

#define SQLITE_TYPE "SQLite"
#define BINARY_TYPE "Binary"
#define MYSQL_TYPE  "MySQL"

#define BINARY_DIR  "./binary/"

using namespace std;

enum RecordType
{
	RT_BASE = 0,
	RT_INSERT,
	RT_LOAD,
	RT_QUERY,
	RT_UPDATE,
	RT_FLUSH,
	RT_REMOVE,
	RT_INSERT_WITH_FLUSH,
};

struct Record 
{
	Record()
	: guid(0)
	, object_guid(0)
	, owner_guid(0)
	, opcode(0)
	, stream_size(0)
	, optype(0)
	, last_modify_time(0)
	, transaction_index(0)
	, transaction_num(0)
	, transaction_id(0)
	, line_id(0)
	, last_is_flush(false)
	{
		STAT_NUM_INC_NEW;
	}

	~Record()
	{
		STAT_NUM_INC_DEL;
	}

	uint64	guid;
	uint64	object_guid;
	uint64	owner_guid;
	int		opcode;
	int		stream_size;
	string	stream;
	int		optype;
	uint64	last_modify_time;
	uint32  transaction_index;
	uint32  transaction_num;
	uint32  transaction_id;
	uint32	line_id;
	bool    last_is_flush;
	string  cache_addr;
	STAT_NUM_DEC;
};

typedef vector<Record *> RecordSet;

/***
 * @class Serialize
 *
 * @brief ���л�binary�ļ��ĳ���ӿ��࣬����������ʵ�ַ�ʽ��sqlite���ݿ⡢�������ļ���Ŀǰֻ��sqlite��ʵ��
 */
class Serialize
{
public:
	/// ��һ���ļ���������صĳ�ʼ������
	virtual bool open(const string & file_name) = 0;

	/// ִ��һ�����Ŀǰ��Ҫ����sqlite�����ʹ�ö������ļ����������ʽ�Լ�����
	virtual bool exeCmd(const string & command_str) = 0;

	/// ��ȡ���������ļ�¼
	virtual bool read(int read_number, const string & condition, RecordSet & record_set) = 0;

	virtual bool beginWriteTransaction() = 0;

	virtual bool endWriteTransaction() = 0;

	/// д��һ����¼
	virtual bool write(const Record & record) = 0;

	/// �رմ򿪵��ļ�����������صķ���ʼ������
	virtual bool close() = 0;

	/// ��ȡ��ǰ����д����ļ�������
	virtual const string & getFileName() = 0;
};

};
};
#endif