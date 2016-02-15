

#include "ManageLawRes.h"
#include "Logger.h"

bool ManageLawRes::loadContent(Document * xml_doc)
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

const LawResInfoVec_t & ManageLawRes::getLawInfoVec()
{
	return m_law_info_vec;
}

bool ManageLawRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	LawResInfo * info = new LawResInfo();

	result = getAttrValue(element, "heart_id", info->heart_id) && result;
	result = getAttrValue(element, "law_id", info->law_id) && result;
	result = getAttrValue(element, "law_level", info->law_level) && result;
	result = getAttrValue(element, "need_level", info->need_level) && result;
	result = getAttrValue(element, "need_item", info->need_item) && result;
	result = getAttrValue(element, "martial", info->martial) && result;
	result = getAttrValue(element, "lawpay", info->lawpay) && result;
	result = getAttrValue(element, "function_id", info->function_id) && result;
	result = getAttrValue(element, "function_num", info->function_num) && result;
	result = getAttrValue(element, "display_level", info->display_level) && result;
	result = getAttrValue(element, "display_martial", info->display_martial) && result;
	result = getAttrValue(element, "group", info->group) && result;
	result = getAttrValue(element, "lawname", info->name) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "delay", info->delay) && result;
	result = getAttrValue(element, "first_strike", info->first_strike) && result;
	result = getAttrValue(element, "costmoney", info->money_cost) && result;

	m_law_info_vec.push_back(info);

	return result;
}