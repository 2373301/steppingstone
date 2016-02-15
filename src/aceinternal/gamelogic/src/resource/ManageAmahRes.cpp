#include "Logger.h"
#include "ManageAmahRes.h"
bool ManageAmahRes::loadContent(Document * xml_doc)
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
const AmahInfoMap_t & ManageAmahRes::getAmahInfo()
{
	return m_amah__res_map;
}
bool ManageAmahRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	AmahInfo * info = new AmahInfo();
	result = getAttrValue(element,"goldblessme_id", info->goldblessme_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"rewardrate", info->rewardrate) && result;
	result = getAttrValue(element,"sysname", info->sysname) && result;
	AmahInfoMap_t::iterator it = m_amah__res_map.find(info->id);
	if (it != m_amah__res_map.end())
	{
		DEF_LOG_ERROR("failed to load AMAH, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_amah__res_map.insert(std::make_pair(info->id, info));
	return result;
}
