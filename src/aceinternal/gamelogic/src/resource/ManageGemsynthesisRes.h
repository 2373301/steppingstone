#ifndef MANAGE_GEMSYNTHESIS__RES_HPP
#define MANAGE_GEMSYNTHESIS__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GemsynthesisRes.h"
class GAMELOGIC_EXOPRT ManageGemsynthesisRes
{
public:
	bool loadContent(Document * xml_doc);
	const GemsynthesisInfoMap_t & getGemsynthesisInfo();
protected:
	bool loadInfo(Element * element);
private:
	GemsynthesisInfoMap_t m_gemsynthesis__res_map;
};
#endif
