#include "Logger.h"
#include "ManageOpenact1Res.h"
bool ManageOpenact1Res::loadContent(Document * xml_doc)
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
const Openact1InfoVec_t & ManageOpenact1Res::getOpenact1Info()
{
	return m_openact1__res_vec;
}
bool ManageOpenact1Res::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	Openact1Info * info = new Openact1Info();
	result = getAttrValue(element,"end_day", info->end_day) && result;
	result = getAttrValue(element,"gap", info->gap) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"start_day", info->start_day) && result;
	result = getAttrValue(element,"target", info->target) && result;
	result = getAttrValue(element,"target_num", info->target_num) && result;
	result = getAttrValue(element,"vip", info->vip) && result;
	result = getAttrValue(element,"bonus", info->bouns) && result;

	m_openact1__res_vec.push_back(info);
	return result;
}
