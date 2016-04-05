
#include "ManageRoleRes.h"
#include "Logger.h"
#include "ManageConfig.h"

int ManageRoleRes::init()
{
	return loadRoleFile(ManageConfig::instance()->getRoleCfg());
}

bool ManageRoleRes::loadRoleFile(const string & role_file)
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());

	if (!doc->load_file(role_file))
	{
		DEF_LOG_ERROR("failed to load role file\n");
		return false;
	}

	Element * root_ele = doc->get_root();

	if (NULL == root_ele)
	{
		return false;
	}

	bool result = true;
	Elements role_ele_list = root_ele->get_elements();
	for (Elements::iterator it = role_ele_list.begin(); it != role_ele_list.end(); ++it)
	{
		result = loadRoleInfo(*it) && result;
	}

	return result;
}

const RoleResInfoMap_t & ManageRoleRes::getRoleInfoMap()
{
	return m_role_info_map;
}

RoleResInfo * ManageRoleRes::getRoleResInfo(int role_id)
{
	RoleResInfoMap_t::iterator it = m_role_info_map.find(role_id);
	if (m_role_info_map.end() != it)
	{
		return it->second;
	}
	return NULL;
}

bool ManageRoleRes::loadRoleInfo(Element * role_element)
{
	if (NULL == role_element)
	{
		return false;
	}

	bool result = true;

	RoleResInfo * role_info = new RoleResInfo();

	result = getAttrValue(role_element, "id", role_info->role_id) && result;
	result = getAttrValue(role_element, "lv", role_info->level) && result;
	result = getAttrValue(role_element, "sex", role_info->job) && result;
	result = getAttrValue(role_element, "char_ability", role_info->char_ability) && result;
	result = getAttrValue(role_element, "str", role_info->str) && result;
	result = getAttrValue(role_element, "agi", role_info->agi) && result;
	result = getAttrValue(role_element, "int", role_info->int_value) && result;
	result = getAttrValue(role_element, "phy_att", role_info->phy_att) && result;
	result = getAttrValue(role_element, "phy_def", role_info->phy_def) && result;
	result = getAttrValue(role_element, "agi_att", role_info->agi_att) && result;
	result = getAttrValue(role_element, "agi_def", role_info->agi_def) && result;
	result = getAttrValue(role_element, "int_att", role_info->int_att) && result;
	result = getAttrValue(role_element, "int_def", role_info->int_def) && result;
	result = getAttrValue(role_element, "hp", role_info->hp) && result;
	result = getAttrValue(role_element, "hp_growth", role_info->hp_growth) && result;
	result = getAttrValue(role_element, "critical", role_info->critical) && result;
	result = getAttrValue(role_element, "critical_damage", role_info->critical_damage) && result;
	result = getAttrValue(role_element, "block", role_info->block) && result;
	result = getAttrValue(role_element, "block_defence", role_info->block_defence) && result;
	result = getAttrValue(role_element, "block_damage", role_info->block_damage) && result;
	result = getAttrValue(role_element, "hit", role_info->hit) && result;
	result = getAttrValue(role_element, "fending", role_info->dodge) && result;
	result = getAttrValue(role_element, "normal_skill", role_info->normal_skill) && result;
	result = getAttrValue(role_element, "special_skill", role_info->extremely_skill) && result;
	result = getAttrValue(role_element, "avatar", role_info->avatar_id) && result;

	result = getAttrValue(role_element, "hp_percent", role_info->hp_percent) && result;
	result = getAttrValue(role_element, "phy_att_percent", role_info->phy_att_percent) && result;
	result = getAttrValue(role_element, "phy_def_percent", role_info->phy_def_percent) && result;
	result = getAttrValue(role_element, "agi_att_percent", role_info->agi_att_percent) && result;
	result = getAttrValue(role_element, "agi_def_percent", role_info->agi_def_percent) && result;
	result = getAttrValue(role_element, "int_att_percent", role_info->int_att_percent) && result;
	result = getAttrValue(role_element, "int_def_percent", role_info->int_def_percent) && result;

	RoleResInfoMap_t::iterator it = m_role_info_map.find(role_info->role_id);
	if (it != m_role_info_map.end())
	{
		DEF_LOG_ERROR("get redumplicate role id <%d>\n", role_info->role_id);
		delete role_info;
		return false;
	}

	m_role_info_map.insert(std::make_pair(role_info->role_id, role_info));

	return result;
}
