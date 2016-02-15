#ifndef MANAGE_SOUL_CFG_RES_HPP
#define MANAGE_SOUL_CFG_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulCfgRes.h"
class GAMELOGIC_EXOPRT ManageSoulCfgRes
{
public:
	bool loadContent(Document * xml_doc);
	const SoulCfgInfo & getSoulCfgInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulCfgInfo m_Soul_Cfg_res_;
};
#endif
