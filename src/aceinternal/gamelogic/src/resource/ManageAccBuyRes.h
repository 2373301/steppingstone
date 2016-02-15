#ifndef MANAGE_ACC_BUY_RES_HPP
#define MANAGE_ACC_BUY_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "AccBuyRes.h"
class GAMELOGIC_EXOPRT ManageAccBuyRes
{
public:
	bool loadContent(Document * xml_doc);
	const AccBuyInfoVec_t & getAccBuyInfo();
protected:
	bool loadInfo(Element * element);
private:
	AccBuyInfoVec_t m_Acc_Buy_res_vec;
};
#endif
