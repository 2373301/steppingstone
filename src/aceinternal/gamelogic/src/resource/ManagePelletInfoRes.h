
#ifndef MANAGE_PELLET_INFO_RES_HPP
#define MANAGE_PELLET_INFO_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PelletInfoRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManagePelletInfoRes
{
public:
	~ManagePelletInfoRes()
	{
		cleanMapSecondContent(m_pellet_info_res_map);
	}
	bool loadContent(Document * xml_doc);

	const PelletInfoResMap_t & getPelletInfoResMap();
protected:
	bool loadInfo(Element * element);
private:
	PelletInfoResMap_t m_pellet_info_res_map;
};
#endif