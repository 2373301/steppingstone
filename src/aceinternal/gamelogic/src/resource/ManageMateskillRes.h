#ifndef MANAGE_MATESKILL__RES_HPP
#define MANAGE_MATESKILL__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MateskillRes.h"
class GAMELOGIC_EXOPRT ManageMateskillRes
{
public:
	bool loadContent(Document * xml_doc);
	const MateskillInfoMap_t & getMateskillInfo();
protected:
	bool loadInfo(Element * element);
private:
	MateskillInfoMap_t m_Mateskill__res_map;
};
#endif
