#include "Logger.h"
#include "ManageGemsynthesisRes.h"
bool ManageGemsynthesisRes::loadContent(Document * xml_doc)
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
const GemsynthesisInfoMap_t & ManageGemsynthesisRes::getGemsynthesisInfo()
{
	return m_gemsynthesis__res_map;
}
bool ManageGemsynthesisRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	GemsynthesisInfo * info = new GemsynthesisInfo();
	result = getAttrValue(element,"cost", info->cost) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"needitem", info->needitem) && result;
	result = getAttrValue(element,"neednum", info->neednum) && result;
	GemsynthesisInfoMap_t::iterator it = m_gemsynthesis__res_map.find(info->needitem);
	if (it != m_gemsynthesis__res_map.end())
	{
		DEF_LOG_ERROR("failed to load GEMSYNTHESIS, get reduplicate id <%d>\n",info->needitem);
		return false;
	}
	m_gemsynthesis__res_map.insert(std::make_pair(info->needitem, info));
	return result;
}
