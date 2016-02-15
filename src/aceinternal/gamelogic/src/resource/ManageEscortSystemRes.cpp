#include "Logger.h"
#include "ManageEscortSystemRes.h"
bool ManageEscortSystemRes::loadContent(Document * xml_doc)
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
const EscortSystemInfo & ManageEscortSystemRes::getEscortSystemInfo()
{
	return m_escort_system_res;
}
bool ManageEscortSystemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	EscortSystemInfo * info = &m_escort_system_res;
	result = getAttrValue(element,"add_speed", info->add_speed) && result;
	result = getAttrValue(element,"completeyabiao_goldblessme_id", info->completeyabiao_goldblessme_id) && result;
	result = getAttrValue(element,"losscd_goldblessme_id", info->losscd_goldblessme_id) && result;
	result = getAttrValue(element,"max_pay", info->max_pay) && result;
	result = getAttrValue(element,"reward", info->reward) && result;
	result = getAttrValue(element,"speciallost1", info->speciallost1) && result;
	result = getAttrValue(element,"speciallost2", info->speciallost2) && result;
	result = getAttrValue(element,"speed_goldblessme_id", info->speed_goldblessme_id) && result;
	result = getAttrValue(element,"speed_up", info->speed_up) && result;
	result = getAttrValue(element,"speed_upgoleblessme_id", info->speed_upgoleblessme_id) && result;
	result = getAttrValue(element,"time", info->time) && result;
	result = getAttrValue(element,"yabiao_friendget", info->yabiao_friendget) && result;
	result = getAttrValue(element,"yabiao_friendtimes", info->yabiao_friendtimes) && result;
	result = getAttrValue(element,"yabiao_havelosstimes", info->yabiao_havelosstimes) && result;
	result = getAttrValue(element,"yabiao_loss", info->yabiao_loss) && result;
	result = getAttrValue(element,"yabiao_losscd", info->yabiao_losscd) && result;
	result = getAttrValue(element,"yabiao_losstimes", info->yabiao_losstimes) && result;
	result = getAttrValue(element,"yabiao_times", info->yabiao_times) && result;
	result = getAttrValue(element,"initial_place", info->initial_place) && result;
	result = getAttrValue(element,"speed_reward", info->speed_reward) && result;
//	m_escort_system_res_vec.push_back(info);
	return result;
}
