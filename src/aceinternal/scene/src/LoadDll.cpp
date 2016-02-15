
#include <boost/filesystem.hpp>
#include "LoadDll.h"
#include "Logger.h"
#include "share_fun.h"

namespace fs = boost::filesystem;

LoadDll::~LoadDll()
{
	cleanVectorContent(m_dll_info_vec);
}

bool LoadDll::loadByDir(const string & dir)
{
	searchDllFile(dir);
	if (m_dll_info_vec.size() == 0)
	{
		DEF_LOG_ERROR("get empey dll info by dir <%s>\n", dir.c_str());
		return false;
	}

	if (loadDll())
	{
		return true;
	}
	else
	{
		DEF_LOG_ERROR("failed to load dll by dir : <%s>\n", dir.c_str());
		return false;
	}
}

DllInfoVec_t & LoadDll::getDllInfoVec()
{
	return m_dll_info_vec;
}

void LoadDll::searchDllFile(const string & dir)
{
	string ex_name;
#ifdef WIN32
	ex_name = ".dll";
#else
	ex_name = ".so";
#endif // WIN32

	vector<string> dll_vec;
	searchFileByExt(dir, ex_name, dll_vec);

	DllInfo * dll_info = NULL;
	for (vector<string>::iterator it = dll_vec.begin(); it != dll_vec.end(); ++it)
	{
		dll_info = new DllInfo();
		dll_info->dll_path = *it;
		m_dll_info_vec.push_back(dll_info);
	}
}

bool LoadDll::loadDll()
{
	DllInfo * dll_info = NULL;
	for (DllInfoVec_t::iterator it = m_dll_info_vec.begin(); it != m_dll_info_vec.end(); ++it)
	{
		dll_info = *it;
		dll_info->dll_handle = new ACE_DLL_Handle();
		if (dll_info->dll_handle->open(dll_info->dll_path.c_str(), RTLD_NOW, dll_info->shlib_handle) == -1)
		{
			DEF_LOG_ERROR("failed to load dll by path : <%s>, last error is <%d>\n", dll_info->dll_path.c_str(), ACE_OS::last_error());
			return false;
		}
	}

	return true;
}