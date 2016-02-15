#include "Logger.h"
#include "ManageRanchCfgRes.h"
bool ManageRanchCfgRes::loadContent(Document * xml_doc)
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
const RanchCfgInfoVec_t & ManageRanchCfgRes::getRanchCfgInfo()
{
	return m_Ranch_Cfg_res_vec;
}
bool ManageRanchCfgRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RanchCfgInfo * info = new RanchCfgInfo();
	result = getAttrValue(element,"coin_up", info->coin_up) && result;
	result = getAttrValue(element,"color_refresh", info->color_refresh) && result;
	result = getAttrValue(element,"crit", info->crit) && result;
	result = getAttrValue(element,"crit_color", info->crit_color) && result;
	result = getAttrValue(element,"default_open1", info->default_open1) && result;
	result = getAttrValue(element,"default_open2", info->default_open2) && result;
	result = getAttrValue(element,"default_open3", info->default_open3) && result;
	result = getAttrValue(element,"default_open4", info->default_open4) && result;
	result = getAttrValue(element,"forage_item", info->forage_item) && result;
	result = getAttrValue(element,"forage_up", info->forage_up) && result;
	result = getAttrValue(element,"help_open1", info->help_open1) && result;
	result = getAttrValue(element,"help_open2", info->help_open2) && result;
	result = getAttrValue(element,"help_open3", info->help_open3) && result;
	result = getAttrValue(element,"help_open4", info->help_open4) && result;
	result = getAttrValue(element,"pt_item", info->pt_item) && result;
	result = getAttrValue(element,"pt_refresh", info->pt_refresh) && result;
	result = getAttrValue(element,"riding_up", info->riding_up) && result;
	m_Ranch_Cfg_res_vec.push_back(info);
	return result;
}
