#ifndef MANAGE_PUSH__RES_HPP
#define MANAGE_PUSH__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PushRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManagePushRes
{
public:
	~ManagePushRes()
	{
		cleanMapSecondContent(m_push__res_map);
	}
	bool loadContent(Document * xml_doc);
	const PushInfoMap_t & getPushInfo();
protected:
	bool loadInfo(Element * element);
private:
	PushInfoMap_t m_push__res_map;
};
#endif
