#ifndef MANAGE_RANCH_CFG_RES_HPP
#define MANAGE_RANCH_CFG_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RanchCfgRes.h"
class GAMELOGIC_EXOPRT ManageRanchCfgRes
{
public:
	bool loadContent(Document * xml_doc);
	const RanchCfgInfoVec_t & getRanchCfgInfo();
protected:
	bool loadInfo(Element * element);
private:
	RanchCfgInfoVec_t m_Ranch_Cfg_res_vec;
};
#endif
