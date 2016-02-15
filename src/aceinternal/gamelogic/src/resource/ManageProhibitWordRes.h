
#ifndef MANAGE_PROHIBIT_WORLD_RES_HPP
#define MANAGE_PROHIBIT_WORLD_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageProhibitWordRes
{
public:
	bool loadContent(Document * xml_doc);

	const ProhibitWordResVec_t & getProhibitWordInfoMap();

	const ProhibitWordResVec_t & getProhibitNameInfoMap();
protected:
	bool loadInfoChar(Element * map_element);

	bool loadInfoName(Element * map_element);

private:

	ProhibitWordResVec_t m_prohibit_char_info_map;
	ProhibitWordResVec_t m_prohibit_name_info_map;
};
#endif