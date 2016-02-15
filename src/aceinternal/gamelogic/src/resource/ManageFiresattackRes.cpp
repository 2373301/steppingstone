#include "Logger.h"
#include "ManageFiresattackRes.h"
bool ManageFiresattackRes::loadContent(Document * xml_doc)
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
const FiresattackInfoMap_t & ManageFiresattackRes::getFiresattackInfo()
{
	return m_FiresAttack__res_map;
}
bool ManageFiresattackRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	FiresattackInfo * info = new FiresattackInfo();
	result = getAttrValue(element,"boss", info->boss) && result;
	result = getAttrValue(element,"fail_reward", info->fail_reward) && result;
	result = getAttrValue(element,"first_reward", info->first_reward) && result;
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
//	result = getAttrValue(element,"id_name", info->id_name) && result;
//	result = getAttrValue(element,"map", info->map) && result;
//	result = getAttrValue(element,"name", info->name) && result;
//	result = getAttrValue(element,"pic", info->pic) && result;
	result = getAttrValue(element,"sub_group", info->sub_group) && result;
	result = getAttrValue(element,"sub_id", info->sub_id) && result;
	result = getAttrValue(element,"unlock_id", info->unlock_id) && result;
	result = getAttrValue(element,"win_reward", info->win_reward) && result;
	FiresattackInfoMap_t::iterator it = m_FiresAttack__res_map.find(info->sub_id);
	if (it != m_FiresAttack__res_map.end())
	{
		DEF_LOG_ERROR("failed to load FIRESATTACK, get reduplicate id <%d>\n",info->sub_id);
		return false;
	}
	m_FiresAttack__res_map.insert(std::make_pair(info->sub_id, info));
	return result;
}
