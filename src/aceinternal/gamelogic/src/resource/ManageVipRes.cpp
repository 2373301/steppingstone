
#include "Logger.h"
#include "ManageVipRes.h"

bool ManageVipRes::loadContent(Document * xml_doc)
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

const VipInfoMap_t & ManageVipRes::getVipInfo()
{
	return m_vip_res_map;
}

bool ManageVipRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	VipInfo * info = new VipInfo();

	result = getAttrValue(element, "functionid", info->function_id) && result;
	result = getAttrValue(element, "unlock_lv", info->unlock_lv) && result;
	result = getAttrValue(element, "isfree", info->is_free) && result;
	result = getAttrValue(element, "cost", info->cost) && result;
	result = getAttrValue(element, "option_a", info->option_a) && result;
	result = getAttrValue(element, "option_b", info->option_b) && result;
	result = getAttrValue(element, "type", info->utype) && result;


	VipInfoMap_t::iterator it = m_vip_res_map.find(info->function_id);
	if (it != m_vip_res_map.end())
	{
		DEF_LOG_ERROR("failed to load vip, get reduplicate vip function id <%d>\n", info->function_id);
		return false;
	}

	m_vip_res_map.insert(std::make_pair(info->function_id, info));

	return result;
}
