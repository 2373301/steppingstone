#include "Logger.h"
#include "ManageMateupRes.h"
bool ManageMateupRes::loadContent(Document * xml_doc)
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
const MateupInfoMap_t & ManageMateupRes::getMateupInfo()
{
	return m_mateup__res_map;
}
bool ManageMateupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MateupInfo * info = new MateupInfo();
	result = getAttrValue(element,"cost", info->cost) && result;
	result = getAttrValue(element,"crit_pro", info->crit_pro) && result;
	result = getAttrValue(element,"exp", info->exp) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"pay", info->pay) && result;

	MateupInfoMap_t::iterator it = m_mateup__res_map.find(info->id);
	if (it != m_mateup__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATEUP, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_mateup__res_map.insert(std::make_pair(info->id, info));
	return result;
}
