#ifndef MANAGE_ESCORT_AGENCY_RES_HPP
#define MANAGE_ESCORT_AGENCY_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "EscortAgencyRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageEscortAgencyRes
{
public:
	~ManageEscortAgencyRes()
	{
		cleanMapSecondContent(m_escort_agency_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
	const EscortAgencyInfoMap_t & getEscortAgencyInfo();
protected:
	bool loadInfo(Element * element);
private:
	EscortAgencyInfoMap_t m_escort_agency_res_map;
};
#endif
