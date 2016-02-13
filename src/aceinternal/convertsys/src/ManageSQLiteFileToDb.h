
#ifndef MANAGE_SQLITE_FILE_TO_DB_HPP
#define MANAGE_SQLITE_FILE_TO_DB_HPP

#include "Singleton.h"
#include "ManageSQLiteFile.h"

class ManageSQLiteFileToDb : public SqliteOutput
{
public:
	ManageSQLiteFileToDb();
	~ManageSQLiteFileToDb();
public:
	static ManageSQLiteFileToDb * instance()
	{
		return Singleton<ManageSQLiteFileToDb>::instance();
	}
public:
	int init(vector<string> & sqlite_path);

	void output(const string & addr, Packet * ps);
protected:
private:
};
#endif