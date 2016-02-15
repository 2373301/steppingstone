#include "Logger.h"
#include "ManageTalklinkRes.h"
bool ManageTalklinkRes::loadContent(Document * xml_doc)
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
const TalklinkInfoMap_t & ManageTalklinkRes::getTalklinkInfo()
{
	return m_talklink__res_map;
}
bool ManageTalklinkRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TalklinkInfo * info = new TalklinkInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"name", info->name) && result;
	TalklinkInfoMap_t::iterator it = m_talklink__res_map.find(info->id);
	if (it != m_talklink__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TALKLINK, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_talklink__res_map.insert(std::make_pair(info->id, info));
	return result;
}
