#ifndef MANAGE_RUNES_EXP_RES_HPP
#define MANAGE_RUNES_EXP_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RunesExpRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageRunesExpRes
{
public:
	~ManageRunesExpRes()
	{
		cleanMapSecondContent(m_runes_exp_res_map);
	}
	bool loadContent(Document * xml_doc);
	const RunesExpInfoMap_t & getRunesExpInfo();
protected:
	bool loadInfo(Element * element);
private:
	RunesExpInfoMap_t m_runes_exp_res_map;
};
#endif
