#include "Logger.h"
#include "ManageEscortCarRes.h"

bool ManageEscortCarRes::loadContent(Document * xml_doc)
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
const EscortCarInfoMap_t & ManageEscortCarRes::getEscortCarInfo()
{
	return m_escort_car_res_map;
}
bool ManageEscortCarRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	EscortCarInfo * info = new EscortCarInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"pay", info->pay) && result;
	result = getAttrValue(element,"probability", info->probability) && result;
	result = getAttrValue(element,"reward_money", info->reward_money) && result;
	result = getAttrValue(element,"reward_shengwang", info->reward_shengwang) && result;
	result = getAttrValue(element,"reward_yueli", info->reward_yueli) && result;
	result = getAttrValue(element,"is_max", info->is_max) && result;

	EscortCarInfoMap_t::iterator it = m_escort_car_res_map.find(info->id);
	if (it != m_escort_car_res_map.end())
	{
		DEF_LOG_ERROR("failed to load ESCORTCAR, get reduplicate daily quest function id <%d>\n",info->id);
		return false;
	}
	m_escort_car_res_map.insert(std::make_pair(info->id, info));
	return result;
}
