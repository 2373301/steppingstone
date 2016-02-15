#ifndef MANAGE_OFFLINEHANGUP__RES_HPP
#define MANAGE_OFFLINEHANGUP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "OfflinehangupRes.h"
class GAMELOGIC_EXOPRT ManageOfflinehangupRes
{
public:
	bool loadContent(Document * xml_doc);
	const OfflinehangupInfo & getOfflinehangupInfo();
protected:
	bool loadInfo(Element * element);
private:
	OfflinehangupInfo m_offlinehangup_res;
};
#endif
