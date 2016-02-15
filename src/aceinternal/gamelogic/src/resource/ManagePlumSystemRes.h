#ifndef MANAGE_PLUM_SYSTEM_RES_HPP
#define MANAGE_PLUM_SYSTEM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PlumSystemRes.h"
class GAMELOGIC_EXOPRT ManagePlumSystemRes
{
public:
	bool loadContent(Document * xml_doc);
	const PlumSystemInfo & getPlumSystemInfo();
protected:
	bool loadInfo(Element * element);
private:
	PlumSystemInfo m_plum_system_res;
};
#endif
