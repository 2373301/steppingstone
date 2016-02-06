// logger_test.cpp : Defines the entry point for the console application.
//
#include <ace/OS.h>
#include "ManageSingleton.h"
#include "ManageLogger.h"
#include "EffectTest.h"
#include "ManageSingletonInit.h"

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

int main(int argc, char * argv[])
{
	startApplication();
	return 0;
}

