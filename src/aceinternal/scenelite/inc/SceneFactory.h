
#ifndef SCENE_FACTORY_HPP
#define SCENE_FACTORY_HPP

#include "Scend_def.h"
#include "Scene.h"
#include "Plugin.h"
#include "PluginDepot.h"
#include "SceneDepot.h"
#include "Pool.h"

class SCENE_EXOPRT SceneFactory
{
public:
	static Scene * createScene();

	static PluginDepot *	createPluginDepot();

	static SceneDepot *		createSceneDepot();

	static Pool *			createPool();
};

#endif