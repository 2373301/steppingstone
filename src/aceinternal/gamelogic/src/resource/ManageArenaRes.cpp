

#include "ManageArenaRes.h"
#include "Logger.h"

bool ManageArenaRes::loadContent(Document * xml_doc)
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

const ArenaResInfoVec_t & ManageArenaRes::getArenaInfoVec()
{
	return m_arena_info_vec;
}

bool ManageArenaRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	ArenaResInfo * info = new ArenaResInfo();

	result = getAttrValue(element, "section", info->section) && result;
	result = getAttrValue(element, "two_renown_a", info->two_renown_a) && result;
	result = getAttrValue(element, "two_renown_b", info->two_renown_b) && result;
	result = getAttrValue(element, "two_money_a", info->two_money_a) && result;
	result = getAttrValue(element, "two_money_b", info->two_money_b) && result;
	result = getAttrValue(element, "two_mana_a", info->two_mana_a) && result;
	result = getAttrValue(element, "two_mana_b", info->two_mana_b) && result;
	result = getAttrValue(element, "week_renown_a", info->week_renown_a) && result;
	result = getAttrValue(element, "week_renown_b", info->week_renown_b) && result;
	result = getAttrValue(element, "week_money_a", info->week_money_a) && result;
	result = getAttrValue(element, "week_money_b", info->week_money_b) && result;
	result = getAttrValue(element, "week_mana_a", info->week_mana_a) && result;
	result = getAttrValue(element, "week_mana_b", info->week_mana_b) && result;
	result = getAttrValue(element, "once_renown", info->once_renown) && result;
	result = getAttrValue(element, "once_money", info->once_money) && result;
	result = getAttrValue(element, "once_mana", info->once_mana) && result;

	m_arena_info_vec.push_back(info);

	return result;
}