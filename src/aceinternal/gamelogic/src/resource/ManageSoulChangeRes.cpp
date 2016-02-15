#include "Logger.h"
#include "ManageSoulChangeRes.h"
bool ManageSoulChangeRes::loadContent(Document * xml_doc)
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
const SoulChangeInfo & ManageSoulChangeRes::getSoulChangeInfo()
{
	return m_soul_change_res_;
}
bool ManageSoulChangeRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"batch", m_soul_change_res_.batch) && result;
	result = getAttrValue(element,"buy_soul", m_soul_change_res_.buy_soul) && result;
	result = getAttrValue(element,"buy_soulid", m_soul_change_res_.buy_soulid) && result;
	result = getAttrValue(element,"coin_color", m_soul_change_res_.coin_color) && result;
	result = getAttrValue(element,"coin_group", m_soul_change_res_.coin_group) && result;
	result = getAttrValue(element,"cost_a", m_soul_change_res_.cost_a) && result;
	result = getAttrValue(element,"cost_b", m_soul_change_res_.cost_b) && result;
	result = getAttrValue(element,"id", m_soul_change_res_.id) && result;
	result = getAttrValue(element,"important_soul", m_soul_change_res_.important_soul) && result;
	result = getAttrValue(element,"limit_coina", m_soul_change_res_.limit_coina) && result;
	result = getAttrValue(element,"limit_coinb", m_soul_change_res_.limit_coinb) && result;
	result = getAttrValue(element,"limit_soula", m_soul_change_res_.limit_soula) && result;
	result = getAttrValue(element,"limit_soulb", m_soul_change_res_.limit_soulb) && result;
	result = getAttrValue(element,"lock_cost", m_soul_change_res_.lock_cost) && result;
	result = getAttrValue(element,"lock_costid", m_soul_change_res_.lock_costid) && result;
	result = getAttrValue(element,"nine_open", m_soul_change_res_.nine_open) && result;
	result = getAttrValue(element,"open_limit", m_soul_change_res_.open_limit) && result;
	result = getAttrValue(element,"soul_color", m_soul_change_res_.soul_color) && result;
	result = getAttrValue(element,"soul_group", m_soul_change_res_.soul_group) && result;

	return result;
}
