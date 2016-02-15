#ifndef MANAGE_SOUL_CHANGE_RES_HPP
#define MANAGE_SOUL_CHANGE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulChangeRes.h"
class GAMELOGIC_EXOPRT ManageSoulChangeRes
{
public:
	bool loadContent(Document * xml_doc);
	const SoulChangeInfo & getSoulChangeInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulChangeInfo m_soul_change_res_;
};
#endif
