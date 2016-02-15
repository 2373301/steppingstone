
#ifndef WORLD_HPP
#define WORLD_HPP

#include "typedef.h"
#include "Scend_def.h"

int SCENE_EXOPRT startWorld(int argc, char * argv[]);

struct SCENE_EXOPRT WorldCfg 
{
	WorldCfg()
	: is_demo(false)
	, stop_gs(false)
	{}

	bool is_demo;
	bool stop_gs;
	string config_file;
};

struct WorldInfo;


class World
{
public:
	World();

	int start(const WorldCfg & world_cfg);

	int run();

	int stop();

	int finit();
protected:
	int createWorld();

	int initSingleManager();
	int finitSingleManager();

	int initLogger();
	int stopLogger();
	int finitLogger();

	int initConfig();
	int stopConfig();
	int finitConfig();

	int initTerminal();
	int stopTerminal();
	int finitTerminal();

	int initCache();
	int stopCache();
	int finitCache();

	int initSceneDepot();
	int stopSceneDepot();
	int finitSceneDepot();

private:
	bool checkStopCmd();

	int stopWorld();

	bool finitFinish();

private:
	bool m_sucess_startup;

	WorldCfg m_world_cfg;

	bool m_is_stop;
};

#endif