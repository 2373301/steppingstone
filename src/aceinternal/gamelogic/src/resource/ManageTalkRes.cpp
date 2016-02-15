
#include "ManageTalkRes.h"
#include "Logger.h"

bool ManageTalkRes::loadContent(Document * xml_doc)
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

const TalkResInfoMap_t & ManageTalkRes::getTalkInfoMap()
{
	return m_talk_info_map;
}

bool ManageTalkRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	TalkResInfo * info = new TalkResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "or", info->or_value) && result;
	result = getAttrValue(element, "enable", info->enable) && result;


	TalkResInfoMap_t::iterator it = m_talk_info_map.find(info->id);
	if (it != m_talk_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Talk id : <%d>\n", info->id);
		return false;
	}

	m_talk_info_map.insert(std::make_pair(info->id, info));

	return result;
}