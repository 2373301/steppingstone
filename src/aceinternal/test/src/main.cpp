// logger_test.cpp : Defines the entry point for the console application.
//
#include <ace/OS.h>
#include <ace/Init_ACE.h>
#include "ManageSingleton.h"
#include "ManageLogger.h"
#include "testlogger.h"
#include "ManageSingletonInit.h"
#include "dynamicparse.h"
#include "typedef.h"
#include "testpoo.hpp"
#include "testnetcore.hpp"
#include "testscenex.hpp"
#include "actor.hpp"

void startApplication()
{
	ManageSingleton::instance();

	// register singleton
	ManageLogger::instance();
	EffectTest::instance();

	ManageSingletonInit::instance()->init();
	ACE_OS::sleep(10000); // µÈ´ý´òÓ¡Íê±Ï

	ManageSingletonInit::instance()->stop();

	ACE_OS::sleep(1);

	ManageSingletonInit::instance()->finit();

	ACE_OS::sleep(1);

	ManageSingleton::instance()->finit();
}

void TestProto()
{
	IDynamicParser *parser = createDynamicParser();
	if (!parser->init("../../common/material/entity"))
		std::cout << "failed to init entity" << std::endl;

	MsgVec msg;
	parser->getMsgDesc("test", msg);

	for (auto it = msg.begin(); it != msg.end(); ++it)
	{	
		auto f = *it;
		std::cout << f->key_name
			<< "    " << f->type1 << "    " << f->type2
			<< "\n";
	}

// 	std::map<std::string, std::set<std::string> > out;
// 	parser.querydbDesc("127.0.0.1", 3306, "root", "root", "game", out);
// 	
// 	std::vector<std::string> changed;
// 	parser.checkEntity(out, "game", changed);
// 	for (auto it = changed.begin(); it != changed.end(); ++it)
// 	{
// 		std::cout <<"newline:" <<  *it << "\n";
// 	}
// 
// 	if (changed.size())
// 	{
// 		parser.updatedbChanged("127.0.0.1", 3306, "root", "root", "game", changed);
// 	}
}

int main(int argc, char * argv[])
{	
	ACE::init();
	//ManageSingleton::instance();
	//ManageLogger::instance();

	//ManageSingletonInit::instance()->init();

	//TestProto();
	//startApplication();
	//pool_ut::run();
	//netcore::netcore_ut::run();
	//scenex_ut::run(argc, argv);
	//ThreadPool t;
	Actor<char> a;
	a.Init(4);
	getchar();
	a.Stop();
	return 0;
}

