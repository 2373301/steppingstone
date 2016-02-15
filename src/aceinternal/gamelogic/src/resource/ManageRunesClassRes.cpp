#include "Logger.h"
#include "ManageRunesClassRes.h"
bool ManageRunesClassRes::loadContent(Document * xml_doc)
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
const RunesClassInfoMap_t & ManageRunesClassRes::getRunesClassInfo()
{
	return m_runes_class_res_map;
}
bool ManageRunesClassRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RunesClassInfo * info = new RunesClassInfo();
	result = getAttrValue(element,"class", info->classs) && result;
	result = getAttrValue(element,"max_level", info->max_level) && result;
	result = getAttrValue(element,"money", info->money) && result;
	result = getAttrValue(element,"need_runes_fire", info->need_runes_fire) && result;
	result = getAttrValue(element,"player_level", info->player_level) && result;
	RunesClassInfoMap_t::iterator it = m_runes_class_res_map.find(info->classs);
	if (it != m_runes_class_res_map.end())
	{
		DEF_LOG_ERROR("failed to load RUNESCLASS, get reduplicate id <%d>\n",info->classs);
		return false;
	}
	m_runes_class_res_map.insert(std::make_pair(info->classs, info));
	return result;
}
