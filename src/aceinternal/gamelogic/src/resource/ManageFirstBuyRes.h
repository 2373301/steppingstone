#ifndef MANAGE_FIRST_BUY_RES_HPP
#define MANAGE_FIRST_BUY_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FirstBuyRes.h"
class GAMELOGIC_EXOPRT ManageFirstBuyRes
{
public:
	bool loadContent(Document * xml_doc);
	const FirstBuyInfoVec_t & getFirstBuyInfo();
protected:
	bool loadInfo(Element * element);
private:
	FirstBuyInfoVec_t m_First_Buy_res_vec;
};
#endif
