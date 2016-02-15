#ifndef MANAGE_HORSE_ROOM_RES_HPP
#define MANAGE_HORSE_ROOM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "HorseRoomRes.h"
class GAMELOGIC_EXOPRT ManageHorseRoomRes
{
public:
	bool loadContent(Document * xml_doc);
	const HorseRoomInfoMap_t & getHorseRoomInfo();
protected:
	bool loadInfo(Element * element);
private:
	HorseRoomInfoMap_t m_Horse_Room_res_map;
};
#endif
