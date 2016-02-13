
#include "ManageSQLiteFileToDb.h"
#include "share_fun.h"
#include "Logger.h"
#include "ace/OS_NS_unistd.h"

ManageSQLiteFileToDb::ManageSQLiteFileToDb()
{

}

ManageSQLiteFileToDb::~ManageSQLiteFileToDb()
{

}

int ManageSQLiteFileToDb::init(vector<string> & sqlite_path)
{
	vector<string> & cache_vec = ManageConfig::instance()->getCacheCfg().cache_vec;
	if (cache_vec.size() <= 0)
	{
		return -1;
	}

	ManageSqliteFile::instance()->setHandleOutput(this);

	string cache_addr = cache_vec[0];
	vector<string> sqlite_file_vec;
	for (int i = 0; i < sqlite_path.size(); ++i)
	{
		sqlite_file_vec.clear();
		searchFileByExt(sqlite_path[i], ".sqlite", sqlite_file_vec);
		std::sort(sqlite_file_vec.begin(), sqlite_file_vec.end());
		DEF_LOG_INFO("path is <%s>, file size is <%d>\n", sqlite_path[i].c_str(), sqlite_file_vec.size());
		for (vector<string>::iterator it = sqlite_file_vec.begin(); it != sqlite_file_vec.end(); ++it)
		{
			ManageSqliteFile::instance()->handleFile(cache_addr, *it);
		}
	}

	while (true)
	{
		ACE_OS::sleep(1);
		int left_file_size = ManageSqliteFile::instance()->getLeftFileSize(cache_addr);
		
		DEF_LOG_INFO("check the left file size <%d>\n", left_file_size);

		if (0 == left_file_size)
		{
			if (ManageSqliteFile::instance()->lastFilePorcessFinish(cache_addr))
			{
				DEF_LOG_INFO("finish to process file, start to exit\n");
				ACE_OS::sleep(3);
			}
		}
	}

	return 0;
}

void ManageSQLiteFileToDb::output(const string & addr, Packet * ps)
{
	delete ps;
}