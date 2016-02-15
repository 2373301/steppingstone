#ifndef MANAGE_LUCKY_TURNTABLE_RES_HPP
#define MANAGE_LUCKY_TURNTABLE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "LuckyTurntableRes.h"
class GAMELOGIC_EXOPRT ManageLuckyTurntableRes
{
public:
	bool loadContent(Document * xml_doc);
	const LuckyTurntableInfo & getLuckyTurntableInfo();
protected:
	bool loadInfo(Element * element);
private:
	LuckyTurntableInfo m_Lucky_Turntable_res_;
};
#endif
