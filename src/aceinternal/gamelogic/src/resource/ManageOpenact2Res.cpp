#include "Logger.h"
#include "ManageOpenact2Res.h"
bool ManageOpenact2Res::loadContent(Document * xml_doc)
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
const Openact2InfoVec_t & ManageOpenact2Res::getOpenact2Info()
{
	return m_openact2__res_vec;
}
bool ManageOpenact2Res::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	Openact2Info * info = new Openact2Info();
	result = getAttrValue(element,"award_day", info->award_day) && result;
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"results_day", info->results_day) && result;
	result = getAttrValue(element,"start_day", info->start_day) && result;
	result = getAttrValue(element,"target1", info->target1) && result;
	result = getAttrValue(element,"target2", info->target2) && result;
	result = getAttrValue(element,"target3", info->target3) && result;
	m_openact2__res_vec.push_back(info);
	return result;
}
