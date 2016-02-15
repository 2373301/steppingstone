
#ifndef MANAGE_PLAYER_INIT_HPP
#define MANAGE_PLAYER_INIT_HPP

#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManagePlayerInit
{
public:
	virtual bool loadPlayerContent(Document * xml_doc);

	InitializationCfg & getPlayerInitCfg();
protected:
	bool loadConfig(Document * xml_doc);

	bool loadInitCfg(Element * root_ele);

private:
	InitializationCfg m_init_cfg;
};

#endif