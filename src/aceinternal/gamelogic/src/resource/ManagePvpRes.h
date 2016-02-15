#ifndef MANAGE_PVP__RES_HPP
#define MANAGE_PVP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PvpRes.h"
class GAMELOGIC_EXOPRT ManagePvpRes
{
public:
	bool loadContent(Document * xml_doc);
	const PvpInfo & getPvpInfo();
protected:
	bool loadInfo(Element * element);
private:
	PvpInfo m_pvp_res_;
};
#endif
