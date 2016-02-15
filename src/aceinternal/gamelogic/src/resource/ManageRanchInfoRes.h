#ifndef MANAGE_RANCH_INFO_RES_HPP
#define MANAGE_RANCH_INFO_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RanchInfoRes.h"
class GAMELOGIC_EXOPRT ManageRanchInfoRes
{
public:
	bool loadContent(Document * xml_doc);
	const RanchInfoInfoVec_t & getRanchInfoInfo();
protected:
	bool loadInfo(Element * element);
private:
	RanchInfoInfoVec_t m_Ranch_Info_res_vec;
};
#endif
