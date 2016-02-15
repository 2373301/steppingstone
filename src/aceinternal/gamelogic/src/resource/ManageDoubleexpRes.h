
#ifndef MANAGE_DOUBLEEXP_RES_HPP
#define MANAGE_DOUBLEEXP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DoubleexpRes.h"

class GAMELOGIC_EXOPRT ManageDoubleexpRes
{
public:
	bool loadContent(Document * xml_doc);
	const DoubleexpResMap_t & getDoubleexpMap();
protected:
	bool loadInfo(Element * element);
private:
	DoubleexpResMap_t m_doubleexp_res_map;
};
#endif