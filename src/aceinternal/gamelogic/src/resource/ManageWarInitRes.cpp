
#include "ManageWarInitRes.h"
#include "Logger.h"

bool ManageWarInitRes::loadWarInitContent(Document * xml_doc)
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
	Elements war_init_ele_list = root_ele->get_elements();
	for (Elements::iterator it = war_init_ele_list.begin(); it != war_init_ele_list.end(); ++it)
	{
		result = loadWarInitInfo(*it) && result;
	}

	return result;
}

const WarInitResInfo & ManageWarInitRes::getWarInitResInfo()
{
	return m_war_init_res_info;
}

bool ManageWarInitRes::loadWarInitInfo(Element * war_init_element)
{
	if (NULL == war_init_element)
	{
		return false;
	}

	bool result = true;
	result = getAttrValue(war_init_element, "round", m_war_init_res_info.round) && result;
	result = getAttrValue(war_init_element, "str", m_war_init_res_info.str) && result;
	result = getAttrValue(war_init_element, "agi", m_war_init_res_info.agi) && result;
	result = getAttrValue(war_init_element, "int", m_war_init_res_info.int_v) && result;

	result = getAttrValue(war_init_element, "str_att", m_war_init_res_info.str_att) && result;
	result = getAttrValue(war_init_element, "str_def", m_war_init_res_info.str_def) && result;
	result = getAttrValue(war_init_element, "agi_att", m_war_init_res_info.agi_att) && result;
	result = getAttrValue(war_init_element, "agi_def", m_war_init_res_info.agi_def) && result;
	result = getAttrValue(war_init_element, "int_att", m_war_init_res_info.int_att) && result;
	result = getAttrValue(war_init_element, "int_def", m_war_init_res_info.int_def) && result;
	result = getAttrValue(war_init_element, "float", m_war_init_res_info.float_value) && result;

	result = getAttrValue(war_init_element, "def_gas_effect", m_war_init_res_info.def_gas_effect) && result;
	result = getAttrValue(war_init_element, "min_damage", m_war_init_res_info.min_damage) && result;
	result = getAttrValue(war_init_element, "attack_gas_effect", m_war_init_res_info.attack_gas_effect) && result;
	result = getAttrValue(war_init_element, "attack_gas_max", m_war_init_res_info.attack_gas_max) && result;
	result = getAttrValue(war_init_element, "attack_hp_range", m_war_init_res_info.attack_hp_range) && result;
	result = getAttrValue(war_init_element, "attack_hp_damage", m_war_init_res_info.attack_hp_damage) && result;
	if (m_war_init_res_info.attack_hp_range.size() != m_war_init_res_info.attack_hp_damage.size())
	{
		result = false;
	}
	result = getAttrValue(war_init_element, "hp_range", m_war_init_res_info.hp_range) && result;
	result = getAttrValue(war_init_element, "hp_damage", m_war_init_res_info.hp_damage) && result;
	if (m_war_init_res_info.hp_range.size() != m_war_init_res_info.hp_damage.size())
	{
		result = false;
	}
	result = getAttrValue(war_init_element, "attack_att_max", m_war_init_res_info.attack_att_max) && result;
	result = getAttrValue(war_init_element, "attack_att_maxrate", m_war_init_res_info.attack_att_maxrate) && result;

	return result;
}