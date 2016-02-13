// convert.cpp : 定义控制台应用程序的入口点。
//

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 
#endif

#include <iostream>

class MemoryLeak
{
public:
	~MemoryLeak()
	{
#ifdef WIN32
		_CrtDumpMemoryLeaks();
#endif
	}
};

int startConvert(int argc, char * argv[]);

MemoryLeak g_memory_leak;

int main(int argc, char * argv[])
{
	try
	{
		return startConvert(argc, argv);
	}
	catch(...)
	{
		std::cout << "Raise unknown exception\n";
	}
	return 0;
}

