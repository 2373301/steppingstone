
#include "ManageGoldParamRes.h"
#include "Logger.h"

bool ManageGoldParamRes::loadContent(Document * xml_doc)
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

const GoldParamResInfoMap_t & ManageGoldParamRes::getGoldParamInfoMap()
{
	return m_gold_param_info_map;
}

bool ManageGoldParamRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	GoldParamResInfo * info = new GoldParamResInfo();

	result = getAttrValue(element, "functionid", info->fun_id) && result;
	result = getAttrValue(element, "levelunlock", info->levelunlock) && result;
	result = getAttrValue(element, "vipunlock", info->vipunlock) && result;
	result = getAttrValue(element, "free_times", info->free_times) && result;
	result = getAttrValue(element, "a_coin", info->a_coin) && result;
	result = getAttrValue(element, "b_coin", info->b_coin) && result;
	result = getAttrValue(element, "limit_coin", info->limit_coin) && result;
	result = getAttrValue(element, "limit_times", info->limit_times) && result;
	result = getAttrValue(element, "a_effect", info->a_effect) && result;
	result = getAttrValue(element, "b_effect", info->b_effect) && result;

	GoldParamResInfoMap_t::iterator it = m_gold_param_info_map.find(info->fun_id);
	if (it != m_gold_param_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate gold param id : <%d>\n", info->fun_id);
		return false;
	}

	m_gold_param_info_map.insert(std::make_pair(info->fun_id, info));

	return result;
}