#include "Logger.h"
#include "ManagePlumRes.h"
bool ManagePlumRes::loadContent(Document * xml_doc)
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
const PlumInfoMap_t & ManagePlumRes::getPlumInfo()
{
	return m_plum__res_map;
}
bool ManagePlumRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PlumInfo * info = new PlumInfo();
	result = getAttrValue(element,"capacity_silver", info->capacity_silver) && result;
	result = getAttrValue(element,"capacity_gold", info->capacity_gold) && result;
	result = getAttrValue(element,"guarantees", info->guarantees) && result;
	result = getAttrValue(element,"id", info->id) && result;

	PlumInfoMap_t::iterator it = m_plum__res_map.find(info->id);
	if (it != m_plum__res_map.end())
	{
		DEF_LOG_ERROR("failed to load PLUM, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_plum__res_map.insert(std::make_pair(info->id, info));
	return result;
}
