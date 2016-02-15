
#include "ManageMonsterRes.h"
#include "Logger.h"
#include "share_fun.h"

ManageMonsterRes::~ManageMonsterRes()
{
	cleanMapSecondContent(m_monster_info_map);
}

bool ManageMonsterRes::loadMonsterContent(Document * xml_doc)
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
	Elements monster_ele_list = root_ele->get_elements();
	for (Elements::iterator it = monster_ele_list.begin(); it != monster_ele_list.end(); ++it)
	{
		result = loadMonsterInfo(*it) && result;
	}

	return result;
}

const MonsterResInfoMap_t & ManageMonsterRes::getMonsterInfoMap()
{
	return m_monster_info_map;
}

bool ManageMonsterRes::loadMonsterInfo(Element * monster_element)
{
	if (NULL == monster_element)
	{
		return false;
	}

	bool result = true;

	MonsterResInfo * monster_info = new MonsterResInfo();

	result = getAttrValue(monster_element, "id", monster_info->id) && result;
	result = getAttrValue(monster_element, "lv", monster_info->level) && result;
	result = getAttrValue(monster_element, "sex", monster_info->sex) && result;
	result = getAttrValue(monster_element, "job", monster_info->job) && result;
	result = getAttrValue(monster_element, "char_ability", monster_info->char_ability) && result;
	result = getAttrValue(monster_element, "str", monster_info->str) && result;
	result = getAttrValue(monster_element, "agi", monster_info->agi) && result;
	result = getAttrValue(monster_element, "int", monster_info->int_value) && result;
	result = getAttrValue(monster_element, "phy_att", monster_info->phy_att) && result;
	result = getAttrValue(monster_element, "phy_def", monster_info->phy_def) && result;
	result = getAttrValue(monster_element, "agi_att", monster_info->agi_att) && result;
	result = getAttrValue(monster_element, "agi_def", monster_info->agi_def) && result;
	result = getAttrValue(monster_element, "int_att", monster_info->int_att) && result;
	result = getAttrValue(monster_element, "int_def", monster_info->int_def) && result;
	result = getAttrValue(monster_element, "hp", monster_info->hp) && result;
	result = getAttrValue(monster_element, "critical", monster_info->critical) && result;
	result = getAttrValue(monster_element, "critical_damage", monster_info->critical_damage) && result;
	result = getAttrValue(monster_element, "block", monster_info->block) && result;
	result = getAttrValue(monster_element, "block_defence", monster_info->block_defence) && result;
	result = getAttrValue(monster_element, "block_damage", monster_info->block_damage) && result;
	result = getAttrValue(monster_element, "hit", monster_info->hit) && result;
	result = getAttrValue(monster_element, "fending", monster_info->fending) && result;
	result = getAttrValue(monster_element, "normal_attack", monster_info->normal_skill) && result;
	result = getAttrValue(monster_element, "special_attack", monster_info->extremely_skill) && result;
	result = getAttrValue(monster_element, "anti_attack", monster_info->strike_back) && result;
	result = getAttrValue(monster_element, "role", monster_info->role_id) && result;
	result = getAttrValue(monster_element, "talk", monster_info->talk) && result;

	result = getAttrValue(monster_element, "combo", monster_info->combo) && result;
	result = getAttrValue(monster_element, "joint", monster_info->joint) && result;
	result = getAttrValue(monster_element, "combo_down", monster_info->combo_down) && result;
	result = getAttrValue(monster_element, "joint_down", monster_info->joint_down) && result;
	result = getAttrValue(monster_element, "combo_up", monster_info->combo_up) && result;
	result = getAttrValue(monster_element, "joint_up", monster_info->joint_up) && result;
	result = getAttrValue(monster_element, "dodge_attack1", monster_info->dodge_attack1) && result;
	result = getAttrValue(monster_element, "dodge_attack2", monster_info->dodge_attack2) && result;

	result = getAttrValue(monster_element, "enarge", monster_info->enarge) && result;

	result = getAttrValue(monster_element, "name", monster_info->name) && result;
	//result = getAttrValue(monster_element, "name", monster_info->name) && result;
	//result = getAttrValue(monster_element, "avatar", monster_info->avatar) && result;

	MonsterResInfoMap_t::iterator it = m_monster_info_map.find(monster_info->id);
	if (it != m_monster_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate monster id : <%d>\n", monster_info->id);
		delete monster_info;
		return false;
	}

	m_monster_info_map.insert(std::make_pair(monster_info->id, monster_info));

	return result;
}
