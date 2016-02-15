
#ifndef MANAGE_TALK_RES_HPP
#define MANAGE_TALK_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTalkRes
{
public:
	~ManageTalkRes()
	{
		cleanMapSecondContent(m_talk_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const TalkResInfoMap_t & getTalkInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	TalkResInfoMap_t m_talk_info_map;
};

#endif