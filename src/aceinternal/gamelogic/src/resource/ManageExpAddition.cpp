
#include "ManageExpAddition.h"
#include "Logger.h"

bool ManageExpAddition::loadContent(Document * xml_doc)
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

const ExpAdditionResInfoMap_t & ManageExpAddition::getExpAdditionInfo()
{
	return m_exp_addition_info_map;
}

bool ManageExpAddition::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	ExpAdditionResInfo * info = new ExpAdditionResInfo();

	result = getAttrValue(element, "lv", info->level_diff) && result;
	result = getAttrValue(element, "exprate", info->addition_percent) && result;
	result = getAttrValue(element, "last", info->limit_percent) && result;


	ExpAdditionResInfoMap_t::iterator it = m_exp_addition_info_map.find(info->level_diff);
	if (m_exp_addition_info_map.end() != it)
	{
		DEF_LOG_ERROR("get reduplicate exp addition id : <%d>\n", info->level_diff);
		delete info;
		return false;
	}

	m_exp_addition_info_map.insert(std::make_pair(info->level_diff, info));

	return result;
}