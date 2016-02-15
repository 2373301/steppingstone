#ifndef MANAGE_TAVERN_GROUP__RES_HPP
#define MANAGE_TAVERN_GROUP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Tavern_groupRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTavern_groupRes
{
public:
	~ManageTavern_groupRes()
	{
		cleanMapSecondContent(m_tavern_group__res_map);
	}
	bool loadContent(Document * xml_doc);
	const Tavern_groupInfoMap_t & getTavern_groupInfo();
protected:
	bool loadInfo(Element * element);
private:
	Tavern_groupInfoMap_t m_tavern_group__res_map;
};
#endif
