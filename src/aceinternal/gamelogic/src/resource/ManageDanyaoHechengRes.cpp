#include "Logger.h"
#include "ManageDanyaoHechengRes.h"
bool ManageDanyaoHechengRes::loadContent(Document * xml_doc)
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
const DanyaoHechengInfoMap_t & ManageDanyaoHechengRes::getDanyaoHechengInfo()
{
	return m_danyao_hecheng_res_map;
}
bool ManageDanyaoHechengRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	DanyaoHechengInfo * info = new DanyaoHechengInfo();
	result = getAttrValue(element,"gold", info->gold) && result;
	result = getAttrValue(element,"goldblessme_id", info->goldblessme_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"needitem", info->needitem) && result;
	result = getAttrValue(element,"neednum", info->neednum) && result;
	DanyaoHechengInfoMap_t::iterator it = m_danyao_hecheng_res_map.find(info->needitem);
	if (it != m_danyao_hecheng_res_map.end())
	{
		DEF_LOG_ERROR("failed to load DANYAOHECHENG, get reduplicate id <%d>\n",info->needitem);
		return false;
	}
	m_danyao_hecheng_res_map.insert(std::make_pair(info->needitem, info));
	return result;
}
