#ifndef MANAGE_TITLE__RES_HPP
#define MANAGE_TITLE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TitleRes.h"
class GAMELOGIC_EXOPRT ManageTitleRes
{
public:
	bool loadContent(Document * xml_doc);
	const TitleInfoMap_t & getTitleInfo();
protected:
	bool loadInfo(Element * element);
private:
	TitleInfoMap_t m_Title__res_map;
};
#endif
