#ifndef MANAGE_HORSEMAN_LEVELUP_RES_HPP
#define MANAGE_HORSEMAN_LEVELUP_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "HorsemanLevelupRes.h"
class GAMELOGIC_EXOPRT ManageHorsemanLevelupRes
{
public:
	bool loadContent(Document * xml_doc);
	const HorsemanLevelupInfoVec_t & getHorsemanLevelupInfo();
protected:
	bool loadInfo(Element * element);
private:
	HorsemanLevelupInfoVec_t m_Horseman_Levelup_res_vec;
};
#endif
