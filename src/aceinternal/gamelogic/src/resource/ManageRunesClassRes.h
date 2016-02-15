#ifndef MANAGE_RUNES_CLASS_RES_HPP
#define MANAGE_RUNES_CLASS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RunesClassRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageRunesClassRes
{
public:
	~ManageRunesClassRes()
	{
		cleanMapSecondContent(m_runes_class_res_map);
	}
	bool loadContent(Document * xml_doc);
	const RunesClassInfoMap_t & getRunesClassInfo();
protected:
	bool loadInfo(Element * element);
private:
	RunesClassInfoMap_t m_runes_class_res_map;
};
#endif
