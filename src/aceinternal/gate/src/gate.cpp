// gate.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#define ACE_NTRACE 0

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

int startGate(int argc, char * argv[]);

MemoryLeak g_memory_leak;

int main(int argc, char * argv[])
{
	//try
	{
		startGate(argc, argv);
	}
	//catch(...)
	//{
	//	std::cout << "Raise unknown exception\n";
	//}
}

