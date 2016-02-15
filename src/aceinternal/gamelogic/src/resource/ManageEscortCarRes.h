#ifndef MANAGE_ESCORT_CAR_RES_HPP
#define MANAGE_ESCORT_CAR_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "EscortCarRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageEscortCarRes
{
public:
	~ManageEscortCarRes()
	{
		cleanMapSecondContent(m_escort_car_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
const EscortCarInfoMap_t & getEscortCarInfo();
protected:
	bool loadInfo(Element * element);
private:
	EscortCarInfoMap_t m_escort_car_res_map;
};
#endif
