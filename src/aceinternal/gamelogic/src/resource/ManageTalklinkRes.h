#ifndef MANAGE_TALKLINK__RES_HPP
#define MANAGE_TALKLINK__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TalklinkRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTalklinkRes
{
public:
	~ManageTalklinkRes()
	{
		cleanMapSecondContent(m_talklink__res_map);
	}
	bool loadContent(Document * xml_doc);
	const TalklinkInfoMap_t & getTalklinkInfo();
protected:
	bool loadInfo(Element * element);
private:
	TalklinkInfoMap_t m_talklink__res_map;
};
#endif
