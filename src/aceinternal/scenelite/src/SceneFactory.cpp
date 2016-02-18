
#include "SceneFactory.h"
#include "SceneImp.h"
#include "PluginDepotImp.h"
#include "SceneDepotImp.h"
#include "PoolImp.h"

Scene * SceneFactory::createScene()
{
	return new SceneImp();
}

PluginDepot * SceneFactory::createPluginDepot()
{
	return new PluginDepotImp();
}

SceneDepot * SceneFactory::createSceneDepot()
{
	return new SceneDepotImp();
}

Pool * SceneFactory::createPool()
{
	return new PoolImp();
}