#include "Logger.h"
#include "ManageHelperInfoRes.h"
bool ManageHelperInfoRes::loadContent(Document * xml_doc)
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
const HelperInfoInfoVec_t & ManageHelperInfoRes::getHelperInfoInfo()
{
	return m_Helper_Info_res_vec;
}
bool ManageHelperInfoRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	HelperInfoInfo * info = new HelperInfoInfo();
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"help_color", info->help_color) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"rate", info->rate) && result;
	result = getAttrValue(element,"refresh_limit", info->refresh_limit) && result;
	result = getAttrValue(element,"value", info->value) && result;
	m_Helper_Info_res_vec.push_back(info);
	return result;
}
