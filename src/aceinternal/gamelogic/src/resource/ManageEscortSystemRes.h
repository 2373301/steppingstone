#ifndef MANAGE_ESCORT_SYSTEM_RES_HPP
#define MANAGE_ESCORT_SYSTEM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "EscortSystemRes.h"
class GAMELOGIC_EXOPRT ManageEscortSystemRes
{
public:
	bool loadContent(Document * xml_doc);
const EscortSystemInfo & getEscortSystemInfo();
protected:
	bool loadInfo(Element * element);
private:
	EscortSystemInfo m_escort_system_res;
};
#endif
