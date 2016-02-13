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
 * @brief Serialize抽象接口的SQlite的实现
 */
class SQLiteSerialize : public Serialize
{
public:
	SQLiteSerialize(void);
	~SQLiteSerialize(void);
public:
	/// 打开一个文件，进行相关的初始化操作
	virtual bool open(const string & file_name);

	/// 执行一个命令，目前主要用于sqlite，如果使用二进制文件，则命令格式自己定义
	virtual bool exeCmd(const string & command_str);

	/// 读取满足条件的记录
	virtual bool read(int read_number, const string & condition, RecordSet & record_set);

	virtual bool beginWriteTransaction();

	virtual bool endWriteTransaction();

	/// 写入一条记录
	virtual bool write(const Record & record);

	/// 关闭打开的文件，并进行相关的反初始化操作
	virtual bool close();

	/// 获取当前正在写入的文件名名称
	virtual const string & getFileName();
private:
	string m_file_name;

	sqlite3 * m_db;
};

};
};

#endif