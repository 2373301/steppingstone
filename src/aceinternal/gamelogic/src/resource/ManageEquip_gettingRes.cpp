#include "Logger.h"
#include "ManageEquip_gettingRes.h"
bool ManageEquip_gettingRes::loadContent(Document * xml_doc)
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
const Equip_gettingInfoMap_t & ManageEquip_gettingRes::getEquip_gettingInfo()
{
	return m_equip_getting__res_map;
}
bool ManageEquip_gettingRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	Equip_gettingInfo * info = new Equip_gettingInfo();
	result = getAttrValue(element,"equip", info->equip) && result;
	result = getAttrValue(element,"job", info->job) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"part", info->part) && result;
	result = getAttrValue(element,"part_num", info->part_num) && result;
	result = getAttrValue(element,"rewards", info->rewards) && result;
	Equip_gettingInfoMap_t::iterator it = m_equip_getting__res_map.find(info->job);
	if (it != m_equip_getting__res_map.end())
	{
		DEF_LOG_ERROR("failed to load EQUIP_GETTING, get reduplicate id <%d>\n",info->job);
		return false;
	}
	m_equip_getting__res_map.insert(std::make_pair(info->job, info));
	return result;
}
