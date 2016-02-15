#ifndef MANAGE_PAIR_GAMBLING_RES_HPP
#define MANAGE_PAIR_GAMBLING_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PairGamblingRes.h"
class GAMELOGIC_EXOPRT ManagePairGamblingRes
{
public:
	bool loadContent(Document * xml_doc);
	const PairGamblingInfoVec_t & getPairGamblingInfo();
protected:
	bool loadInfo(Element * element);
private:
	PairGamblingInfoVec_t m_pair_gambling_res_vec;
};
#endif
