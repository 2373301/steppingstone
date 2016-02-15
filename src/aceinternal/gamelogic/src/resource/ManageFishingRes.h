#ifndef MANAGE_FISHING__RES_HPP
#define MANAGE_FISHING__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FishingRes.h"
class GAMELOGIC_EXOPRT ManageFishingRes
{
public:
	bool loadContent(Document * xml_doc);
	const FishingInfoMap_t & getFishingInfo();
protected:
	bool loadInfo(Element * element);
private:
	FishingInfoMap_t m_fishing__res_map;
};
#endif
