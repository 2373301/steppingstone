#include "Logger.h"
#include "ManageMateRes.h"
bool ManageMateRes::loadContent(Document * xml_doc)
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
const MateInfoMap_t & ManageMateRes::getMateInfo()
{
	return m_mate__res_map;
}
bool ManageMateRes::loadInfo(Element * element)
{ 
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MateInfo * info = new MateInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"role_id", info->role_id) && result;
	result = getAttrValue(element,"share_damage", info->share_damage) && result;
	result = getAttrValue(element,"skill_id", info->skill_id) && result;
	result = getAttrValue(element,"up_level", info->up_level) && result;
	result = getAttrValue(element,"mate_coin", info->matecoin) && result;
	result = getAttrValue(element,"mate_yinliang", info->mateyinling) && result;
	result = getAttrValue(element,"limit_coin", info->limitcoin) && result;
	result = getAttrValue(element,"limit_yinliang", info->limityinliang) && result;
	result = getAttrValue(element,"default_mate", info->isdefault) && result;
	result = getAttrValue(element,"name", info->name) && result;
	result = getAttrValue(element,"color", info->color) && result;
	result = getAttrValue(element,"hp", info->hp) && result;
	result = getAttrValue(element,"hp_growth", info->hp_grow) && result;
	MateInfoMap_t::iterator it = m_mate__res_map.find(info->id);
	if (it != m_mate__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_mate__res_map.insert(std::make_pair(info->id, info));
	return result;
}
