
#include "Singleton.h"
#include "ManageSingleton.h"

Cleanup::~Cleanup()
{
}

void Cleanup::cleanup()
{
	delete this;
}

void Cleanup::registerSingleton()
{
	ManageSingleton::instance()->addSingleton(this);
}