
#ifndef LOAD_DLL_HPP
#define LOAD_DLL_HPP

#include <ace/DLL.h>
#include <ace/DLL_Manager.h>
#include "typedef.h"

struct DllInfo 
{
	DllInfo()
	: dll_handle(NULL)
	, shlib_handle(NULL)
	{}

	~DllInfo()
	{
		delete dll_handle;
	}

	string				dll_path;
	ACE_DLL_Handle *	dll_handle;
	ACE_SHLIB_HANDLE	shlib_handle;
};

typedef vector<DllInfo *> DllInfoVec_t;

class LoadDll
{
public:
	~LoadDll();

	bool loadByDir(const string & dir);

	DllInfoVec_t & getDllInfoVec();
protected:
	void searchDllFile(const string & dir);

	bool loadDll();
private:

	DllInfoVec_t  m_dll_info_vec;
};

#endif