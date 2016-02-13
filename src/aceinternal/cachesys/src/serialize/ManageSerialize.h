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
 * @brief ���ڹ���ǰ�����л��������������л���Ĵ�����binary�ļ��Ĵ�����
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
	/// ������صĳ�ʼ������
	bool open();

	/// д��һ����¼
	void writeRecord(DataRequestInfo * data_request_info, RecordType record_type);

	/// ��ȡ��ǰ�����л�ʵ��
	Serialize & getSerialize();

	/// ʹ�ø������ļ����Ƴ�ʼ����ǰ�����л�ʵ��
	bool initSerialize(const string & file_name);

	/// ����ķ���ʼ������
	bool shutdown();
protected:
	/// Task���̺߳������
	int svc();

	/// ��ȡ��ǰ���õ�GUID
	uint64 getGUID();

	/// ���binaryĿ¼�����ڣ��򴴽�
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