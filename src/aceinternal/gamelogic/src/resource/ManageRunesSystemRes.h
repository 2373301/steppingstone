#ifndef MANAGE_RUNES_SYSTEM_RES_HPP
#define MANAGE_RUNES_SYSTEM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RunesSystemRes.h"
class GAMELOGIC_EXOPRT ManageRunesSystemRes
{
public:
	bool loadContent(Document * xml_doc);
	const RunesSystemInfo & getRunesSystemInfo();
protected:
	bool loadInfo(Element * element);
private:
	RunesSystemInfo m_runes_system_res;
};
#endif
