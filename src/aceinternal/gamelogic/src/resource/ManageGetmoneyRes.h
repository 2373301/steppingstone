#ifndef MANAGE_GETMONEY__RES_HPP
#define MANAGE_GETMONEY__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GetmoneyRes.h"
class GAMELOGIC_EXOPRT ManageGetmoneyRes
{
public:
	bool loadContent(Document * xml_doc);
	const GetmoneyInfoVec_t & getGetmoneyInfo();
protected:
	bool loadInfo(Element * element);
private:
	GetmoneyInfoVec_t m_GetMoney__res_vec;
};
#endif
