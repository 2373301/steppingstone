
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "ManagePlayerInit.h"
#include "SimpleXML.h"
#include "Logger.h"

bool ManagePlayerInit::loadPlayerContent(Document * xml_doc)
{
	if (loadConfig(xml_doc))
	{
		return true;
	}
	return false;
}

InitializationCfg & ManagePlayerInit::getPlayerInitCfg()
{
	return m_init_cfg;
}

bool ManagePlayerInit::loadConfig(Document * xml_doc)
{
	if (NULL == xml_doc)
	{
		return false;
	}

	Element * root_ele = xml_doc->get_root();
	return loadInitCfg(root_ele);

	return false;
}

bool ManagePlayerInit::loadInitCfg(Element * root_ele)
{
	if (NULL == root_ele)
	{
		return false;
	}

	Element * init_ele = root_ele->get_element("initialization");
	if (NULL == init_ele)
	{
		return NULL;
	}

	bool result = true;

	result = getAttrValue(init_ele, "level", m_init_cfg.level) && result;
	result = getAttrValue(init_ele, "exp", m_init_cfg.exp) && result;
	result = getAttrValue(init_ele, "money", m_init_cfg.money) && result;
	result = getAttrValue(init_ele, "coin", m_init_cfg.coin) && result;
	result = getAttrValue(init_ele, "src_x", m_init_cfg.src_x) && result;
	result = getAttrValue(init_ele, "src_y", m_init_cfg.src_y) && result;
	result = getAttrValue(init_ele, "seniority", m_init_cfg.seniority) && result;
	result = getAttrValue(init_ele, "coin_exp", m_init_cfg.coin_exp) && result;
	result = getAttrValue(init_ele, "frame", m_init_cfg.frame) && result;
	result = getAttrValue(init_ele, "energy", m_init_cfg.energy) && result;
	result = getAttrValue(init_ele, "training_slot", m_init_cfg.training_slot) && result;
	result = getAttrValue(init_ele, "member_slot", m_init_cfg.member_slot) && result;
	result = getAttrValue(init_ele, "curr_battle_array", m_init_cfg.curr_battle_array) && result;
	result = getAttrValue(init_ele, "map_id", m_init_cfg.map_id) && result;


	result = getAttrValue(init_ele, "ba_role_location", m_init_cfg.ba_role_location) && result;
	result = getAttrValue(init_ele, "battle_array_list", m_init_cfg.battle_array_list) && result;
	result = getAttrValue(init_ele, "wonder_skill", m_init_cfg.wonder_skill) && result;
	result = getAttrValue(init_ele, "buffers", m_init_cfg.buffers) && result;
	result = getAttrValue(init_ele, "roles", m_init_cfg.roles) && result;
	result = getAttrValue(init_ele, "bag", m_init_cfg.bag) && result;
	result = getAttrValue(init_ele, "warehouse", m_init_cfg.warehouse) && result;
	result = getAttrValue(init_ele, "can_recruit_roles", m_init_cfg.can_recruit_roles) && result;
	result = getAttrValue(init_ele, "quest", m_init_cfg.quest) && result;
	result = getAttrValue(init_ele, "level_cap", m_init_cfg.level_cap) && result;
	result = getAttrValue(init_ele, "checkplint", m_init_cfg.checkplint) && result;

	result = getAttrValue(init_ele, "itemgifts", m_init_cfg.gift_item_id) && result;
	result = getAttrValue(init_ele, "itemgifts_num", m_init_cfg.gift_item_num) && result;

	if (m_init_cfg.gift_item_id.size() != m_init_cfg.gift_item_num.size())
	{
		DEF_LOG_ERROR("the size of gift item id is not equal gift item num\n");
		return -1;
	}

	return result;
}