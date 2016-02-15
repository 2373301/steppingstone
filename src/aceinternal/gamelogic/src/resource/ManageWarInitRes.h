
#ifndef MANAGE_WAR_INIT_RES
#define MANAGE_WAR_INIT_RES

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageWarInitRes
{
public:
	bool loadWarInitContent(Document * xml_doc);

	const WarInitResInfo & getWarInitResInfo();
protected:
	bool loadWarInitInfo(Element * war_init_element);

private:
	WarInitResInfo m_war_init_res_info;
};
#endif