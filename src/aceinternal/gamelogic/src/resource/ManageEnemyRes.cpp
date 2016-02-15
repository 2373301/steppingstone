
#include "ManageEnemyRes.h"
#include "Logger.h"
#include "share_fun.h"

ManageEnemyRes::~ManageEnemyRes()
{
	cleanMapSecondContent(m_enemy_info_map);
}

bool ManageEnemyRes::loadEnemyContent(Document * xml_doc)
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
	Elements enemy_ele_list = root_ele->get_elements();
	for (Elements::iterator it = enemy_ele_list.begin(); it != enemy_ele_list.end(); ++it)
	{
		result = loadEnemyInfo(*it) && result;
	}

	return result;
}

const EnemyResInfoMap_t & ManageEnemyRes::getEnemyInfo()
{
	return m_enemy_info_map;
}

bool ManageEnemyRes::loadEnemyInfo(Element * enemy_element)
{
	if (NULL == enemy_element)
	{
		return false;
	}

	bool result = true;

	EnemyResInfo * enemy_info = new EnemyResInfo();

	result = getAttrValue(enemy_element, "id", enemy_info->enemy_id) && result;
	result = getAttrValue(enemy_element, "type", enemy_info->type) && result;
	result = getAttrValue(enemy_element, "sequence", enemy_info->sequence) && result;
	result = getAttrValue(enemy_element, "level", enemy_info->level) && result;
	result = getAttrValue(enemy_element, "money", enemy_info->money) && result;
	result = getAttrValue(enemy_element, "exp", enemy_info->exp) && result;
	result = getAttrValue(enemy_element, "yueli", enemy_info->yueli) && result;
	result = getAttrValue(enemy_element, "bonus", enemy_info->bonus) && result;
	result = getAttrValue(enemy_element, "talk", enemy_info->talk) && result;
	result = getAttrValue(enemy_element, "name", enemy_info->name) && result;

	if (1 == enemy_info->type)
	{
		string slot_name;
		uint32 slot_value = 0;
		for (int i = 1; i <= 9; ++i)
		{
			slot_name = "slot" + boost::lexical_cast<string>(i);
			result = getAttrValue(enemy_element, slot_name, slot_value) && result;
			enemy_info->slot.push_back(slot_value);
		}
	}
	else if (2 == enemy_info->type)
	{
		// do nothing
	}
	else
	{
		DEF_LOG_ERROR("get unknown enemy type : <%d>\n", enemy_info->type);
		return false;
	}

	result = getAttrValue(enemy_element, "boss_type", enemy_info->boss_type) && result;
	if (enemy_info->type == 1 && enemy_info->boss_type.size() != 9)
	{
		DEF_LOG_ERROR("enemy_info->boss_type size error : <%d>\n", enemy_info->boss_type.size());
		return -1;
	}

	//result = getAttrValue(enemy_element, "resouce", enemy_info->resouce) && result;
	//result = getAttrValue(enemy_element, "walkspeed", enemy_info->walk_speed) && result;
	result = getAttrValue(enemy_element, "map", enemy_info->map) && result;
	//result = getAttrValue(enemy_element, "x", enemy_info->x) && result;
	//result = getAttrValue(enemy_element, "y", enemy_info->y) && result;
	//result = getAttrValue(enemy_element, "r", enemy_info->r) && result;

	EnemyResInfoMap_t::iterator it = m_enemy_info_map.find(enemy_info->enemy_id);
	if (it != m_enemy_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate enemy id : <%d>\n", enemy_info->enemy_id);
		delete enemy_info;
		return false;
	}

	m_enemy_info_map.insert(std::make_pair(enemy_info->enemy_id, enemy_info));

	return result;
}