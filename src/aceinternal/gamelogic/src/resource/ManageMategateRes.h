#ifndef MANAGE_MATEGATE__RES_HPP
#define MANAGE_MATEGATE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MategateRes.h"
class GAMELOGIC_EXOPRT ManageMategateRes
{
public:
	bool loadContent(Document * xml_doc);
	const MategateInfoMap_t & getMategateInfo();
protected:
	bool loadInfo(Element * element);
private:
	MategateInfoMap_t m_Mategate__res_map;
};
#endif
