
#include "Share_def.h"
#include "share_fun.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void searchFileByExt(const string & search_dir, const string & file_ext, vector<string> & searched_file)
{
	fs::path parent_path(search_dir);
	if (!fs::is_directory(parent_path))
	{
		return ;
	}

	fs::directory_iterator end_it;
	for (fs::directory_iterator it(parent_path); it != end_it; ++it)
	{
		if (!fs::is_directory(*it))
		{
			if (it->path().extension() == file_ext)
			{
				// find one
				searched_file.push_back(it->path().string());
			}
		}
	}
}