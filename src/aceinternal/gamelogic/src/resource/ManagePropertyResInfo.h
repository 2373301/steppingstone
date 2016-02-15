
#ifndef MANAGE_PROPERTY_RES_INFO_HP
#define MANAGE_PROPERTY_RES_INFO_HP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManagePropertyResInfo
{
public:
	~ManagePropertyResInfo()
	{
		cleanMapSecondContent(m_property_info_map);
	}
public:
	bool loadPropertyContent(Document * xml_doc);

	const PropertyResInfoMap_t & getPropertyInfoMap();
protected:
	bool loadPropertyInfo(Element * property_element);

private:

	PropertyResInfoMap_t m_property_info_map;
};
#endif