#ifndef MANAGE_NEXTDAY_INTEREST_RES_HPP
#define MANAGE_NEXTDAY_INTEREST_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "NextdayInterestRes.h"
class GAMELOGIC_EXOPRT ManageNextdayInterestRes
{
public:
	bool loadContent(Document * xml_doc);
	const NextdayInterestInfoVec_t & getNextdayInterestInfo();
protected:
	bool loadInfo(Element * element);
private:
	NextdayInterestInfoVec_t m_Nextday_Interest_res_vec;
};
#endif
