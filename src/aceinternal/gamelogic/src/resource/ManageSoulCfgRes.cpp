#include "Logger.h"
#include "ManageSoulCfgRes.h"
bool ManageSoulCfgRes::loadContent(Document * xml_doc)
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
const SoulCfgInfo & ManageSoulCfgRes::getSoulCfgInfo()
{
	return m_Soul_Cfg_res_;
}
bool ManageSoulCfgRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"buy_cross", m_Soul_Cfg_res_.buy_cross) && result;
	result = getAttrValue(element,"buy_rob", m_Soul_Cfg_res_.buy_rob) && result;
	result = getAttrValue(element,"cross_cost", m_Soul_Cfg_res_.cross_cost) && result;
	result = getAttrValue(element,"cross_costnum", m_Soul_Cfg_res_.cross_costnum) && result;
	result = getAttrValue(element,"cross_freenum", m_Soul_Cfg_res_.cross_freenum) && result;
	result = getAttrValue(element,"hit", m_Soul_Cfg_res_.hit) && result;
	result = getAttrValue(element,"id", m_Soul_Cfg_res_.id) && result;
	result = getAttrValue(element,"raids_cd", m_Soul_Cfg_res_.raids_cd) && result;
	result = getAttrValue(element,"raids_vip", m_Soul_Cfg_res_.raids_vip) && result;
	result = getAttrValue(element,"refresh_buy", m_Soul_Cfg_res_.refresh_buy) && result;
	result = getAttrValue(element,"refresh_cd", m_Soul_Cfg_res_.refresh_cd) && result;
	result = getAttrValue(element,"refresh_player", m_Soul_Cfg_res_.refresh_player) && result;
	result = getAttrValue(element,"reset_buy", m_Soul_Cfg_res_.reset_buy) && result;
	result = getAttrValue(element,"reset_cost", m_Soul_Cfg_res_.reset_cost) && result;
	result = getAttrValue(element,"reset_costnum", m_Soul_Cfg_res_.reset_costnum) && result;
	result = getAttrValue(element,"reset_freenum", m_Soul_Cfg_res_.reset_freenum) && result;
	result = getAttrValue(element,"rob_a", m_Soul_Cfg_res_.rob_a) && result;
	result = getAttrValue(element,"rob_b", m_Soul_Cfg_res_.rob_b) && result;
	result = getAttrValue(element,"rob_cost", m_Soul_Cfg_res_.rob_cost) && result;
	result = getAttrValue(element,"rob_costnum", m_Soul_Cfg_res_.rob_costnum) && result;
	result = getAttrValue(element,"rob_freenum", m_Soul_Cfg_res_.rob_freenum) && result;
	result = getAttrValue(element,"search_a", m_Soul_Cfg_res_.search_a) && result;

	return result;
}
