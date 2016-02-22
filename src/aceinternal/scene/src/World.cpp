
#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

#include <iostream>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include "typedef.h"
#include "World.h"
#include "Scend_def.h"
#include "opcode.h"
#include "ManageSingleton.h"
#include "ManageLogger.h"
#include "ManageConfig.h"
#include "SceneDepot.h"
#include "SceneFactory.h"
#include "ManageTerminal.h"
#include "TerminalFactory.h"
#include "CacheInterface.h"
#include "CacheFactory.h"
#include "Redirect.h"

const string stop_str = ACE_TEXT("stop gs");

struct WorldInfo 
{
	WorldInfo()
		: scene_depot(NULL)
		, manage_terminal(NULL)
		, manage_cache(NULL)
	{}

	SceneDepot * scene_depot;
	SceneDepotCfg scene_depot_cfg;

	ManageTerminal * manage_terminal;
	TerminalCfg terminal_cfg;

	ManageCache * manage_cache;
	CacheCfg cache_cfg;
};

WorldInfo m_world_info;

#ifndef WIN32

void singleFlush(int single_no)
{
	DEF_LOG_INFO("recv single <%d>\n", single_no);
	if (SIGUSR1 == single_no)
	{
		// flush all player
		if (NULL != m_world_info.scene_depot)
		{
			Packet * ps = new Packet(SMSG_RECEIVE_SIGUSR_1, 0, "");
			m_world_info.scene_depot->input(ps);
		}
	}
}

#endif

World::World()
: m_sucess_startup(false)
, m_is_stop(false)
{
}

int startWorld(int argc, char * argv[])
{

#ifndef WIN32
	IGNORE_LINUX_SINGLE();
	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, singleFlush);
#endif

	if (argc < 2)
	{
		std::cout << "please input the config name as parameter" << std::endl;
		return -1;
	}

	World world;
	WorldCfg world_cfg;
	world_cfg.config_file = argv[1];
	if (argc >= 3)
	{
		world_cfg.is_demo = true;
	}
	else
	{
		world_cfg.is_demo = false;
	}

	world.start(world_cfg);

	world.run();

	ACE_OS::sleep(5);
	return 1;
}

int World::start(const WorldCfg & world_cfg)
{
	m_world_cfg = world_cfg;

	//ACE::init();

	if (createWorld() == -1)
	{
		return -1;
	}

	if (initTerminal() == -1)
	{
		DEF_LOG_ERROR("error to call initGate in World::start\n");
		return -1;
	}

	if (initCache() == -1)
	{
		DEF_LOG_ERROR("error to call initCache in World::start\n");
		return -1;
	}

	if (initSceneDepot() == -1)
	{
		DEF_LOG_ERROR("error to call initSceneDepot in World::start\n");
		return -1;
	}

	m_sucess_startup = true;
	return 0;
}

int World::run()
{
	ACE_Time_Value start_time = ACE_OS::gettimeofday();
	bool is_first_launch = m_world_info.scene_depot_cfg.is_first_launch;
	if (m_sucess_startup)
	{
		while (!m_is_stop)
		{
			ACE_OS::sleep(3);
			if (is_first_launch)
			{
				ACE_Time_Value diff_time = ACE_OS::gettimeofday() - start_time;
				uint64 diff_value = 0;
				diff_time.to_usec(diff_value);
				if (diff_value > 3 * 60 * 1000 * 1000)
				{
					ManageConfig::instance()->setFirstLunchFalse();
					is_first_launch = false;
				}
			}
			else
			{
				if (!m_world_cfg.is_demo)
				{
					checkStopCmd();
				}
			}
//			int r = ACE_Reactor::instance()->run_reactor_event_loop();
//
//#ifdef WIN32
//			break;
//#else
//			if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
//			{
//				continue;
//			}
//			else
//			{
//				break;
//			}
//#endif
		}

		//ACE::fini();
	}

	return 0;
}

int World::stop()
{
	return 0;
}

int World::createWorld()
{
	ManageSingleton::instance();
	ManageConfig::instance();
	ManageLogger::instance();

	if (initSingleManager() == -1)
	{
		std::cout << "error to call initSingleManager in World::start\n";
		return -1;
	}

	std::cout << "****** start to init logger" << std::endl;
	if (initLogger() == -1)
	{
		std::cout << "error to call initLogger in World::start\n";
		return -1;
	}

	std::cout << "****** start to init config" << std::endl;
	if (initConfig() == -1)
	{
		DEF_LOG_ERROR("error to call initConfig in World::start\n");
		return -1;
	}

	m_world_info.terminal_cfg.gate_cfg = ManageConfig::instance()->getGateCfg();
	m_world_info.terminal_cfg.local_addr = ManageConfig::instance()->getLocalAddr();
	m_world_info.manage_terminal = TerminalFactory::createManageTerminal(m_world_info.terminal_cfg.gate_cfg.type);
	if (NULL == m_world_info.manage_terminal)
	{
		DEF_LOG_ERROR("failed to create ManageTerminal\n");
		return -1;
	}

	m_world_info.cache_cfg = ManageConfig::instance()->getCacheCfg();
	m_world_info.manage_cache = CacheFactory::createManageCache(m_world_info.cache_cfg.remote_cfg.type);
	if (NULL == m_world_info.manage_cache)
	{
		DEF_LOG_ERROR("failed to create manage cache\n");
		return -1;
	}

	m_world_info.scene_depot_cfg = ManageConfig::instance()->getScenesCfg();
	m_world_info.scene_depot = SceneFactory::createSceneDepot();
	if (NULL == m_world_info.scene_depot)
	{
		DEF_LOG_ERROR("failed to create scene deopt\n");
		return -1;
	}

	m_world_info.scene_depot_cfg.cache_type = m_world_info.cache_cfg.remote_cfg.type;
	m_world_info.scene_depot_cfg.manage_terminal = m_world_info.manage_terminal;
	m_world_info.scene_depot_cfg.cache_handle_output = m_world_info.manage_cache;
	
	m_world_info.terminal_cfg.handle_input = m_world_info.scene_depot;

	return 0;
}

int World::finit()
{
	return 0;
}

int World::initSingleManager()
{
	ManageSingleton::instance();
	return 0;
}

int World::finitSingleManager()
{
	ManageSingleton::instance()->finit();
	return 0;
}

int World::initLogger()
{
	return ManageLogger::instance()->init();
}

int World::stopLogger()
{
	return ManageLogger::instance()->stop();
}

int World::finitLogger()
{
	return ManageLogger::instance()->finit();
}

int World::initConfig()
{
	return ManageConfig::instance()->init(m_world_cfg.config_file);
}

int World::stopConfig()
{
	return ManageConfig::instance()->stop();
}

int World::finitConfig()
{
	return ManageConfig::instance()->finit();
}

int World::initTerminal()
{
	std::cout << "****** start to init terminal" << std::endl;
	m_world_info.terminal_cfg.handle_input = m_world_info.scene_depot;
	int result = m_world_info.manage_terminal->init(m_world_info.terminal_cfg);
	if (-1 == result)
	{
		DEF_LOG_ERROR("failed to init manage teriminal\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int World::stopTerminal()
{
	return m_world_info.manage_terminal->stop();
}

int World::finitTerminal()
{
	return m_world_info.manage_terminal->finit();
}

int World::initCache()
{
	std::cout << "****** start to init cache" << std::endl;
	m_world_info.cache_cfg.handle_input = m_world_info.scene_depot;
	int result = m_world_info.manage_cache->init(m_world_info.cache_cfg);
	if (-1 == result)
	{
		DEF_LOG_ERROR("failed to init manage cache\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int World::stopCache()
{
	return m_world_info.manage_cache->stop();
}

int World::finitCache()
{
	return m_world_info.manage_cache->finit();
}

int World::initSceneDepot()
{
	std::cout << "****** start to init scene depot" << std::endl;
	int result = m_world_info.scene_depot->init(m_world_info.scene_depot_cfg);
	if (-1 == result)
	{
		DEF_LOG_ERROR("failed to init scene depot\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int World::stopSceneDepot()
{
	return m_world_info.scene_depot->stop();
}

int World::finitSceneDepot()
{
	return m_world_info.scene_depot->finit();
}

bool World::checkStopCmd()
{
	if (!m_world_cfg.stop_gs)
	{
		char buf[256] = {0};
		std::cin.getline(buf, 127);
		if (stop_str.compare(buf) == 0)
		{
			// stop gs
			m_world_cfg.stop_gs = true;
			stopWorld();
		}
		return false;
	}
	else
	{
		return finitFinish();
	}
}

int World::stopWorld()
{
	if (stopSceneDepot() == -1)
	{
		DEF_LOG_ERROR("error to call stopSceneDepot in World::start\n");
		return -1;
	}

	if (stopCache() == -1)
	{
		DEF_LOG_ERROR("error to call stopCache in World::start\n");
		return -1;
	}

	if (stopTerminal() == -1)
	{
		DEF_LOG_ERROR("error to call stopGate in World::start\n");
		return -1;
	}

	return 0;
}

bool World::finitFinish()
{
	ACE_OS::sleep(10);

	delete m_world_info.manage_terminal;
	delete m_world_info.manage_cache;
	delete m_world_info.scene_depot;

	// close logger
	ManageLogger::instance()->stop();

	ManageSingleton::instance()->finit();

	ACE_OS::sleep(3);

	m_is_stop = true;
	return true;
}