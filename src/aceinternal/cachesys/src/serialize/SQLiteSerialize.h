#ifndef SQLITE_SERIALIZE_HPP
#define SQLITE_SERIALIZE_HPP

#include <string>
#include "sqlite3.h"
#include "Serialize.h"

namespace cached {
namespace service {

using namespace std;

/***
 * @class SQLiteSerialize
 *
 * @brief Serialize����ӿڵ�SQlite��ʵ��
 */
class SQLiteSerialize : public Serialize
{
public:
	SQLiteSerialize(void);
	~SQLiteSerialize(void);
public:
	/// ��һ���ļ���������صĳ�ʼ������
	virtual bool open(const string & file_name);

	/// ִ��һ�����Ŀǰ��Ҫ����sqlite�����ʹ�ö������ļ����������ʽ�Լ�����
	virtual bool exeCmd(const string & command_str);

	/// ��ȡ���������ļ�¼
	virtual bool read(int read_number, const string & condition, RecordSet & record_set);

	virtual bool beginWriteTransaction();

	virtual bool endWriteTransaction();

	/// д��һ����¼
	virtual bool write(const Record & record);

	/// �رմ򿪵��ļ�����������صķ���ʼ������
	virtual bool close();

	/// ��ȡ��ǰ����д����ļ�������
	virtual const string & getFileName();
private:
	string m_file_name;

	sqlite3 * m_db;
};

};
};

#endif