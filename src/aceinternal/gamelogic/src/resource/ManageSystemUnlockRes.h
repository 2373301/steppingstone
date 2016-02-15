
#ifndef MANAGE_SYSTEM_UNLOCK_RES_HPP
#define MANAGE_SYSTEM_UNLOCK_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SystemUnlockRes.h"

class ManageSystemUnlockRes
{
public:
	bool loadContent(Document * xml_doc);

	const SystemUnlockRes & getSystemUnlockRes();
protected:
	bool loadInfo(Element * element);

private:

	SystemUnlockRes m_system_unlock_res;
};
#endif