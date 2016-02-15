#ifndef MANAGE_TARGETQUEST__RES_HPP
#define MANAGE_TARGETQUEST__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TargetquestRes.h"
class GAMELOGIC_EXOPRT ManageTargetquestRes
{
public:
	bool loadContent(Document * xml_doc);
	const TargetquestInfoVec_t & getTargetquestInfo();
protected:
	bool loadInfo(Element * element);
private:
	TargetquestInfoVec_t m_TargetQuest__res_vec;
};
#endif
