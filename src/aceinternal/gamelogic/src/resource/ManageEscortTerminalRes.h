#ifndef MANAGE_ESCORT_TERMINAL_RES_HPP
#define MANAGE_ESCORT_TERMINAL_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "EscortTerminalRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageEscortTerminalRes
{
public:
	~ManageEscortTerminalRes()
	{
		cleanMapSecondContent(m_escort_terminal_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
const EscortTerminalInfoMap_t & getEscortTerminalInfo();
protected:
	bool loadInfo(Element * element);
private:
	EscortTerminalInfoMap_t m_escort_terminal_res_map;
};
#endif
