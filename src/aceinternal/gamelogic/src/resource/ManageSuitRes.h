#ifndef MANAGE_SUIT__RES_HPP
#define MANAGE_SUIT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SuitRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageSuitRes
{
public:
	~ManageSuitRes()
	{
		cleanMapSecondContent(m_suit__res_map);
	}
public:
	bool loadContent(Document * xml_doc);
	const SuitInfoMap_t & getSuitInfo();
protected:
	bool loadInfo(Element * element);
private:
	SuitInfoMap_t m_suit__res_map;
};
#endif
