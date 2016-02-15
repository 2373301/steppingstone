
#include "ManageHireRes.h"
#include "Logger.h"

bool ManageHireRes::loadContent(Document * xml_doc)
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

const HireResInfoMap_t & ManageHireRes::getHireInfoMap()
{
	return m_hire_info_map;
}

bool ManageHireRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	HireResInfo * info = new HireResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "limit", info->limit) && result;
	result = getAttrValue(element, "lv", info->level) && result;
	result = getAttrValue(element, "roleid", info->role_id) && result;
	result = getAttrValue(element, "lv_req", info->lv_req) && result;
	result = getAttrValue(element, "frame_req", info->prestige_req) && result;
	result = getAttrValue(element, "lv_unlock", info->lv_unlock) && result;
	result = getAttrValue(element, "frame_unlock", info->prestige_unlock) && result;
	result = getAttrValue(element, "sepcial_req", info->special_req) && result;
	result = getAttrValue(element, "money_cost", info->money_cost) && result;
	result = getAttrValue(element, "coin_cost", info->coin_cost) && result;
	result = getAttrValue(element, "yueli_cost", info->seniority_cost) && result;
	result = getAttrValue(element, "tavern_hire", info->tavern_hire) && result;
	result = getAttrValue(element, "tavern_limit_frame", info->tavern_limit_frame) && result;
	result = getAttrValue(element, "tavern_limit_level", info->tavern_limit_level) && result;
	result = getAttrValue(element, "skillcolor", info->skillcolor) && result;
	result = getAttrValue(element, "talk", info->talk) && result;
    result = getAttrValue(element, "hire_cost", info->hire_cost) && result;

	HireResInfoMap_t::iterator it = m_hire_info_map.find(info->id);
	if (m_hire_info_map.end() != it)
	{
		DEF_LOG_ERROR("get reduplicate hire id : <%d>\n", info->id);
		delete info;
		return false;
	}

	m_hire_info_map.insert(std::make_pair(info->id, info));

	return result;
}