#ifndef MANAGE_TAVERNSHOP__RES_HPP
#define MANAGE_TAVERNSHOP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TavernshopRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTavernshopRes
{
public:
	~ManageTavernshopRes()
	{
		cleanVectorContent(m_tavernshop_res_);
	}
	bool loadContent(Document * xml_doc);
	const TavernshopInfoVector_t & getTavernshopInfo();
protected:
	bool loadInfo(Element * element);
private:
	TavernshopInfoVector_t m_tavernshop_res_;
};
#endif
