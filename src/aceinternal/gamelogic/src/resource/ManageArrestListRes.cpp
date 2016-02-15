#include "Logger.h"
#include "ManageArrestListRes.h"
bool ManageArrestListRes::loadContent(Document * xml_doc)
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
const ArrestListInfoMap_t & ManageArrestListRes::getArrestListInfo()
{
	return m_Arrest_List_res_map;
}
bool ManageArrestListRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	ArrestListInfo * info = new ArrestListInfo();
	result = getAttrValue(element,"enemy", info->enemy) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level_group", info->level_group) && result;
	result = getAttrValue(element,"probability", info->probability) && result;
	result = getAttrValue(element,"type", info->type) && result;
    result = getAttrValue(element, "items", info->bonus) && result;
    result = getAttrValue(element, "color", info->color) && result;

	ArrestListInfoMap_t::iterator it = m_Arrest_List_res_map.find(info->id);
	if (it != m_Arrest_List_res_map.end())
	{
		DEF_LOG_ERROR("failed to load ARRESTLIST, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Arrest_List_res_map.insert(std::make_pair(info->id, info));
	return result;
}
