#ifndef MANAGE_CHARGEQUEST__RES_HPP
#define MANAGE_CHARGEQUEST__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ChargequestRes.h"
class GAMELOGIC_EXOPRT ManageChargequestRes
{
public:
	bool loadContent(Document * xml_doc);
	const ChargequestInfo & getChargequestInfo();
protected:
	bool loadInfo(Element * element);
private:
	ChargequestInfo m_ChargeQuest__res_;
};
#endif
