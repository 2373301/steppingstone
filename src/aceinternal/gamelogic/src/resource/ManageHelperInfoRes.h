#ifndef MANAGE_HELPER_INFO_RES_HPP
#define MANAGE_HELPER_INFO_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "HelperInfoRes.h"
class GAMELOGIC_EXOPRT ManageHelperInfoRes
{
public:
	bool loadContent(Document * xml_doc);
	const HelperInfoInfoVec_t & getHelperInfoInfo();
protected:
	bool loadInfo(Element * element);
private:
	HelperInfoInfoVec_t m_Helper_Info_res_vec;
};
#endif
