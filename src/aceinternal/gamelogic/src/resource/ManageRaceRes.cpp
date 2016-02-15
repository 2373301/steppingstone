#include "Logger.h"
#include "ManageRaceRes.h"
bool ManageRaceRes::loadContent(Document * xml_doc)
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
const RaceInfoVec_t & ManageRaceRes::getRaceInfo()
{
	return m_race__res_vec;
}
bool ManageRaceRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RaceInfo * info = new RaceInfo();
	result = getAttrValue(element,"fail_bonus_id", info->fail_bonus_id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"victory_bonus_id", info->victory_bonus_id) && result;
	m_race__res_vec.push_back(info);
	return result;
}
