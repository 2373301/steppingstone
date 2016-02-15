#ifndef MANAGE_PAIR_SYSTEM_RES_HPP
#define MANAGE_PAIR_SYSTEM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PairSystemRes.h"
class GAMELOGIC_EXOPRT ManagePairSystemRes
{
public:
	bool loadContent(Document * xml_doc);
	const PairSystemInfo & getPairSystemInfo();
protected:
	bool loadInfo(Element * element);
private:
	PairSystemInfo m_pair_system_res_;
};
#endif
