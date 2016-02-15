#include "Logger.h"
#include "ManageHorsemanLevelupRes.h"
bool ManageHorsemanLevelupRes::loadContent(Document * xml_doc)
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
const HorsemanLevelupInfoVec_t & ManageHorsemanLevelupRes::getHorsemanLevelupInfo()
{
	return m_Horseman_Levelup_res_vec;
}
bool ManageHorsemanLevelupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	HorsemanLevelupInfo * info = new HorsemanLevelupInfo();
	result = getAttrValue(element,"riding_exp", info->riding_exp) && result;
	result = getAttrValue(element,"riding_lv", info->riding_lv) && result;
	m_Horseman_Levelup_res_vec.push_back(info);
	return result;
}
