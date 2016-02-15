#ifndef MANAGE_QQBRAG__RES_HPP
#define MANAGE_QQBRAG__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "QqbragRes.h"
class GAMELOGIC_EXOPRT ManageQqbragRes
{
public:
	bool loadContent(Document * xml_doc);
	const QqbragInfoMap_t & getQqbragInfo();
protected:
	bool loadInfo(Element * element);
private:
	QqbragInfoMap_t m_QQBrag__res_map;
};
#endif
