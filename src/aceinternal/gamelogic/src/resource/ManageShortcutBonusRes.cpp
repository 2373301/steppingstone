#include "Logger.h"
#include "ManageShortcutBonusRes.h"
bool ManageShortcutBonusRes::loadContent(Document * xml_doc)
{
	if (NULL == xml_doc)
	{
		return false;
	}
	Element * root_ele = xml_doc->get_root();
	if (NULL == root_ele)
	{
		return false;
	}
	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadInfo(*it) && result;
	}
	return result;
}
const ShortcutBonusInfo & ManageShortcutBonusRes::getShortcutBonusInfo()
{
	return m_Shortcut_Bonus_res_;
}
bool ManageShortcutBonusRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"bonus1", m_Shortcut_Bonus_res_.bonus1) && result;
	result = getAttrValue(element,"end_date", m_Shortcut_Bonus_res_.end_date) && result;
	result = getAttrValue(element,"id", m_Shortcut_Bonus_res_.id) && result;
	result = getAttrValue(element,"star_date", m_Shortcut_Bonus_res_.star_date) && result;

	return result;
}
