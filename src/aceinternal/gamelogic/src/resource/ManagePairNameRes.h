#ifndef MANAGE_PAIR_NAME_RES_HPP
#define MANAGE_PAIR_NAME_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PairNameRes.h"
class GAMELOGIC_EXOPRT ManagePairNameRes
{
public:
	bool loadContent(Document * xml_doc);
	const PairNameInfoVec_t & getPairNameInfo();
protected:
	bool loadInfo(Element * element);
private:
	PairNameInfoVec_t m_pair_name_res_vec;
};
#endif
