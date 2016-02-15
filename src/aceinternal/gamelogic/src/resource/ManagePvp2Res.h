#ifndef MANAGE_PVP_2_RES_HPP
#define MANAGE_PVP_2_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Pvp2Res.h"
class GAMELOGIC_EXOPRT ManagePvp2Res
{
public:
	bool loadContent(Document * xml_doc);
	const Pvp2Info & getPvp2Info();
protected:
	bool loadInfo(Element * element);
private:
	Pvp2Info m_pvp_2_res_;
};
#endif
