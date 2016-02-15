
#include "ManageVipexpRes.h"
#include "Logger.h"

bool ManageVipexpRes::loadContent(Document * xml_doc)
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

const VipexpResInfoMap_t & ManageVipexpRes::getVipexpInfo()
{
	return m_vipexp_res_map;
}

bool ManageVipexpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	VipexpInfo * info = new VipexpInfo();

	result = getAttrValue(element, "level", info->vip_level) && result;

	result = getAttrValue(element, "exp", info->vip_exp) && result;

	VipexpResInfoMap_t::iterator it = m_vipexp_res_map.find(info->vip_level);
	if (it != m_vipexp_res_map.end())
	{
		DEF_LOG_ERROR("failed to load vip exp, get reduplicate vip level <%d>\n", info->vip_level);
		return false;
	}

	m_vipexp_res_map.insert(std::make_pair(info->vip_level, info));

	return result;
}
