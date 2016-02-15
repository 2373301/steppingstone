#include "Logger.h"
#include "ManageTitleRes.h"
bool ManageTitleRes::loadContent(Document * xml_doc)
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
const TitleInfoMap_t & ManageTitleRes::getTitleInfo()
{
	return m_Title__res_map;
}
bool ManageTitleRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TitleInfo * info = new TitleInfo();
	result = getAttrValue(element,"buff", info->buff) && result;
//	result = getAttrValue(element,"color", info->color) && result;
//	result = getAttrValue(element,"fps", info->fps) && result;
//	result = getAttrValue(element,"fpsrate", info->fpsrate) && result;
//	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"id1", info->id1) && result;
	result = getAttrValue(element,"id2", info->id2) && result;
	result = getAttrValue(element,"id3", info->id3) && result;
	result = getAttrValue(element,"name", info->name) && result;
	result = getAttrValue(element,"option_a1", info->option_a1) && result;
	result = getAttrValue(element,"option_a2", info->option_a2) && result;
	result = getAttrValue(element,"option_a3", info->option_a3) && result;
	result = getAttrValue(element,"option_a4", info->option_a4) && result;
	result = getAttrValue(element,"option_type", info->option_type) && result;
	result = getAttrValue(element,"super_id1", info->super_id1) && result;
	result = getAttrValue(element,"super_id2", info->super_id2) && result;
	result = getAttrValue(element,"super_id3", info->super_id3) && result;
	result = getAttrValue(element,"super_value1", info->super_value1) && result;
	result = getAttrValue(element,"super_value2", info->super_value2) && result;
	result = getAttrValue(element,"super_value3", info->super_value3) && result;
	result = getAttrValue(element,"talk_type", info->talk_type) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;
	result = getAttrValue(element,"value3", info->value3) && result;
//	result = getAttrValue(element,"x", info->x) && result;
//	result = getAttrValue(element,"y", info->y) && result;
	TitleInfoMap_t::iterator it = m_Title__res_map.find(info->id);
	if (it != m_Title__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TITLE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Title__res_map.insert(std::make_pair(info->id, info));
	return result;
}
