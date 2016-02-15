#include "Logger.h"
#include "ManageJianZhongRes.h"
bool ManageJianZhongRes::loadContent(Document * xml_doc)
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
const JianZhongInfoMap_t & ManageJianZhongRes::getJianZhongInfo()
{
	return m_Jian_Zhong_res_map;
}
bool ManageJianZhongRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	JianZhongInfo * info = new JianZhongInfo();
	result = getAttrValue(element,"battle_buy", info->battle_buy) && result;
	result = getAttrValue(element,"battle_cd", info->battle_cd) && result;
	result = getAttrValue(element,"battle_num", info->battle_num) && result;
	result = getAttrValue(element,"defend", info->defend) && result;
	result = getAttrValue(element,"fail_gain", info->fail_gain) && result;
	result = getAttrValue(element,"gain", info->gain) && result;
	result = getAttrValue(element,"gain_cd", info->gain_cd) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"in_level", info->in_level) && result;
	result = getAttrValue(element,"last_time", info->last_time) && result;
	result = getAttrValue(element,"place", info->place) && result;
	result = getAttrValue(element,"safe_cd", info->safe_cd) && result;
	result = getAttrValue(element,"star_time", info->star_time) && result;
	result = getAttrValue(element,"win_gain", info->win_gain) && result;
	JianZhongInfoMap_t::iterator it = m_Jian_Zhong_res_map.find(info->id);
	if (it != m_Jian_Zhong_res_map.end())
	{
		DEF_LOG_ERROR("failed to load JIANZHONG, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Jian_Zhong_res_map.insert(std::make_pair(info->id, info));
	return result;
}
