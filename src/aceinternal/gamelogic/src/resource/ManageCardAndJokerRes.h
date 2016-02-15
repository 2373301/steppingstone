
#ifndef MANAGE_CARD_AND_JOKER_RES_HPP
#define MANAGE_CARD_AND_JOKER_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageCardAndJokerRes
{
public:
	~ManageCardAndJokerRes()
	{
		cleanMapSecondContent(m_card_and_joker_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const CardAndJokerResInfoMap_t & getCardAndJokerInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	CardAndJokerResInfoMap_t m_card_and_joker_info_map;
};

#endif