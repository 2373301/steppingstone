#include "Logger.h"
#include "ManageFishingRes.h"
bool ManageFishingRes::loadContent(Document * xml_doc)
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
const FishingInfoMap_t & ManageFishingRes::getFishingInfo()
{
	return m_fishing__res_map;
}
bool ManageFishingRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	FishingInfo * info = new FishingInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"time", info->time) && result;

	if (info->bonus.size() != info->level.size())
	{
		DEF_LOG_ERROR("load fish config, but the bonus size is not equal level size, id is <%d>\n", info->id);
		return false;
	}

	FishingInfoMap_t::iterator it = m_fishing__res_map.find(info->id);
	if (it != m_fishing__res_map.end())
	{
		DEF_LOG_ERROR("failed to load FISHING, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_fishing__res_map.insert(std::make_pair(info->id, info));
	return result;
}
