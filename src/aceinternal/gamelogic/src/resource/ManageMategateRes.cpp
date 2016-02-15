#include "Logger.h"
#include "ManageMategateRes.h"
bool ManageMategateRes::loadContent(Document * xml_doc)
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
const MategateInfoMap_t & ManageMategateRes::getMategateInfo()
{
	return m_Mategate__res_map;
}
bool ManageMategateRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MategateInfo * info = new MategateInfo();
	result = getAttrValue(element,"blue_love", info->blue_love) && result;
	result = getAttrValue(element,"blue_pro1", info->blue_pro1) && result;
	result = getAttrValue(element,"blue_pro2", info->blue_pro2) && result;
	result = getAttrValue(element,"cost", info->cost) && result;
	result = getAttrValue(element,"free_num", info->free_num) && result;
	result = getAttrValue(element,"green_love", info->green_love) && result;
	result = getAttrValue(element,"green_pro1", info->green_pro1) && result;
	result = getAttrValue(element,"green_pro2", info->green_pro2) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"orange_love", info->orange_love) && result;
	result = getAttrValue(element,"orange_pro1", info->orange_pro1) && result;
	result = getAttrValue(element,"orange_pro2", info->orange_pro2) && result;
	result = getAttrValue(element,"purple_love", info->purple_love) && result;
	result = getAttrValue(element,"purple_pro1", info->purple_pro1) && result;
	result = getAttrValue(element,"purple_pro2", info->purple_pro2) && result;
	result = getAttrValue(element,"must_get", info->must_get) && result;
	result = getAttrValue(element,"orange_limit", info->orange_limit) && result;
	MategateInfoMap_t::iterator it = m_Mategate__res_map.find(info->id);
	if (it != m_Mategate__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATEGATE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Mategate__res_map.insert(std::make_pair(info->id, info));
	return result;
}
