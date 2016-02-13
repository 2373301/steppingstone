#ifndef MANAGE_SERIALIZE_HPP
#define MANAGE_SERIALIZE_HPP

#include <ace/Singleton.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <ace/Task.h>
#include <boost/filesystem.hpp>
#include "Serialize.h"
#include "FileNameGenerator.h"
#include "Singleton.h"
#include "DataRequestInfo.h"

namespace cached {
namespace service {

namespace fs=boost::filesystem;

/***
 * @class Serialize
 *
 * @brief 用于管理当前的序列化操作，包括序列化类的创建，binary文件的创建等
 */
class ManageSerialize : ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageSerialize();

	~ManageSerialize();
public:
	static ManageSerialize * instance()
	{
		return Singleton<ManageSerialize>::instance();
	}
public:
	int stop();

	int finit();
public:
	/// 进行相关的初始化操作
	bool open();

	/// 写入一条记录
	void writeRecord(DataRequestInfo * data_request_info, RecordType record_type);

	/// 获取当前的序列化实例
	Serialize & getSerialize();

	/// 使用给定的文件名称初始化当前的序列化实例
	bool initSerialize(const string & file_name);

	/// 该类的反初始化操作
	bool shutdown();
protected:
	/// Task的线程函数入口
	int svc();

	/// 获取当前可用的GUID
	uint64 getGUID();

	/// 如果binary目录不存在，则创建
	bool createBinaryDir(const string & binary_dir);

	void doWritingRecord(Record * record);

private:
	Serialize * m_serialize;

	uint64 m_guid_index;

	bool m_stop;

	bool m_wait;

	FileNameGenerator m_file_name_generator;

	RecordSet m_record_set;

	ACE_Thread_Mutex m_record_set_mutex;
};

//typedef ACE_Singleton<ManageSerialize, ACE_Recursive_Thread_Mutex> ManageSerializeSingleton;

#define sManageSerialize ManageSerialize::instance()

};
};
#endif