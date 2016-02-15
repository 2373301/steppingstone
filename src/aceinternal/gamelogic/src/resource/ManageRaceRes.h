#ifndef MANAGE_RACE__RES_HPP
#define MANAGE_RACE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RaceRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageRaceRes
{
public:
	~ManageRaceRes()
	{
		cleanVectorContent(m_race__res_vec);
	}
	bool loadContent(Document * xml_doc);
	const RaceInfoVec_t & getRaceInfo();
protected:
	bool loadInfo(Element * element);
private:
	RaceInfoVec_t m_race__res_vec;
};
#endif
