#include "Logger.h"
#include "ManagePlumJarRes.h"
bool ManagePlumJarRes::loadContent(Document * xml_doc)
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
const PlumJarInfoMap_t & ManagePlumJarRes::getPlumJarInfo()
{
	return m_plum_jar_res_map;
}
bool ManagePlumJarRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PlumJarInfo * info = new PlumJarInfo();
	result = getAttrValue(element,"id", info->item_id) && result;
	result = getAttrValue(element,"level_group", info->level_group) && result;
	result = getAttrValue(element,"need_capacity", info->need_capacity) && result;
	result = getAttrValue(element,"probability", info->probability) && result;
	result = getAttrValue(element,"type", info->_type) && result;
	PlumJarInfoMap_t::iterator it = m_plum_jar_res_map.find(info->item_id);
	if (it != m_plum_jar_res_map.end())
	{
		DEF_LOG_ERROR("failed to load PLUMJAR, get reduplicate id <%d>\n",info->item_id);
		return false;
	}
	m_plum_jar_res_map.insert(std::make_pair(info->item_id, info));
	return result;
}
