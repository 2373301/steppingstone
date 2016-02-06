
#ifndef EFFECT_TEST_HPP
#define EFFECT_TEST_HPP

#include <ace/Task.h>
#include "Singleton.h"
#include "SingletonInit.h"

class EffectTest : SingletonInit, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	EffectTest();

public:
	static EffectTest * instance()
	{
		return Singleton<EffectTest>::instance();
	}
public:
	virtual int svc();

public:
	// SingletionInit functions
	virtual int init();

	virtual int stop();

	virtual int finit();

	bool isStoped()
	{
		return started_  && stoped_;
	}
protected:
private:
	int m_test_count;
	bool started_;
	bool stoped_;
};

#endif