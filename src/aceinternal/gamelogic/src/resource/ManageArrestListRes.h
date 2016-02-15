#ifndef MANAGE_ARREST_LIST_RES_HPP
#define MANAGE_ARREST_LIST_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ArrestListRes.h"
class GAMELOGIC_EXOPRT ManageArrestListRes
{
public:
	bool loadContent(Document * xml_doc);
	const ArrestListInfoMap_t & getArrestListInfo();
protected:
	bool loadInfo(Element * element);
private:
	ArrestListInfoMap_t m_Arrest_List_res_map;
};
#endif
