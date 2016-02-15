
#ifndef MANAGE_SYSTEM_RES_HPP
#define MANAGE_SYSTEM_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageSystemRes
{
public:
	bool loadContent(Document * xml_doc);

	const SystemResInfo & getSystemInfo();
protected:
	bool loadInfo(Element * element);

private:

	SystemResInfo m_system_info;
};

#endif