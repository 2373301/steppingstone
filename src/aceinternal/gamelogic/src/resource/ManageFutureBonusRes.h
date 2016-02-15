#ifndef MANAGE_FUTURE_BONUS_RES_HPP
#define MANAGE_FUTURE_BONUS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FutureBonusRes.h"
class GAMELOGIC_EXOPRT ManageFutureBonusRes
{
public:
	bool loadContent(Document * xml_doc);
	const FutureBonusInfoVec_t & getFutureBonusInfo();
protected:
	bool loadInfo(Element * element);
private:
	FutureBonusInfoVec_t m_Future_Bonus_res_vec;
};
#endif
