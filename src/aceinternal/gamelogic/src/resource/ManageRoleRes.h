
#ifndef MANAGE_ROLE_RES_HPP
#define MANAGE_ROLE_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRoleRes
{
public:
	~ManageRoleRes()
	{
		cleanMapSecondContent(m_role_info_map);
	}
public:
	bool loadRoleContent(Document * xml_doc);

	const RoleResInfoMap_t & getRoleInfoMap();
protected:
	bool loadRoleInfo(Element * role_element);
private:
	RoleResInfoMap_t m_role_info_map;
};
#endif