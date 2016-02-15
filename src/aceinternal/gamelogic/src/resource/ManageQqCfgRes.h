#ifndef MANAGE_QQ_CFG_RES_HPP
#define MANAGE_QQ_CFG_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "QqCfgRes.h"
class GAMELOGIC_EXOPRT ManageQqCfgRes
{
public:
	bool loadContent(Document * xml_doc);
	const QqCfgInfoMap_t & getQqCfgInfo();
protected:
	bool loadInfo(Element * element);
private:
	QqCfgInfoMap_t m_QQ_Cfg_res_map;
};
#endif
