#include "Logger.h"
#include "ManageRunesSystemRes.h"
bool ManageRunesSystemRes::loadContent(Document * xml_doc)
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
const RunesSystemInfo & ManageRunesSystemRes::getRunesSystemInfo()
{
	return m_runes_system_res;
}
bool ManageRunesSystemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"change_rate", m_runes_system_res.change_rate) && result;
	result = getAttrValue(element,"critical_rate1", m_runes_system_res.critical_rate1) && result;
	result = getAttrValue(element,"critical_rate2", m_runes_system_res.critical_rate2) && result;
	result = getAttrValue(element,"critical_value1", m_runes_system_res.critical_value1) && result;
	result = getAttrValue(element,"critical_value2", m_runes_system_res.critical_value2) && result;
	result = getAttrValue(element,"critical_value3", m_runes_system_res.critical_value3) && result;
	result = getAttrValue(element,"critical_value4", m_runes_system_res.critical_value4) && result;
	result = getAttrValue(element,"runes1", m_runes_system_res.runes1) && result;
	result = getAttrValue(element,"runes2", m_runes_system_res.runes2) && result;
	result = getAttrValue(element,"runes3", m_runes_system_res.runes3) && result;
	result = getAttrValue(element,"runes4", m_runes_system_res.runes4) && result;
	result = getAttrValue(element,"runes5", m_runes_system_res.runes5) && result;
	result = getAttrValue(element,"runes_stone_exp", m_runes_system_res.runes_stone_exp) && result;
	result = getAttrValue(element,"unlock_stone_num", m_runes_system_res.unlock_stone_num) && result;

	return result;
}
