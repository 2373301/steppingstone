#ifndef MANAGE_SHENGHUO_LING_RES_HPP
#define MANAGE_SHENGHUO_LING_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ShenghuoLingRes.h"
class GAMELOGIC_EXOPRT ManageShenghuoLingRes
{
public:
	bool loadContent(Document * xml_doc);
	const ShenghuoLingInfoVec_t & getShenghuoLingInfo();
protected:
	bool loadInfo(Element * element);
private:
	ShenghuoLingInfoVec_t m_Shenghuo_ling_res_vec;
};
#endif
