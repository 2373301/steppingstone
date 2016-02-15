#ifndef MANAGE_SHORTCUT_BONUS_RES_HPP
#define MANAGE_SHORTCUT_BONUS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ShortcutBonusRes.h"
class GAMELOGIC_EXOPRT ManageShortcutBonusRes
{
public:
	bool loadContent(Document * xml_doc);
	const ShortcutBonusInfo & getShortcutBonusInfo();
protected:
	bool loadInfo(Element * element);
private:
	ShortcutBonusInfo m_Shortcut_Bonus_res_;
};
#endif
