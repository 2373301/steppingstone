#include "Logger.h"
#include "ManageRanchInfoRes.h"
bool ManageRanchInfoRes::loadContent(Document * xml_doc)
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
const RanchInfoInfoVec_t & ManageRanchInfoRes::getRanchInfoInfo()
{
	return m_Ranch_Info_res_vec;
}
bool ManageRanchInfoRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RanchInfoInfo * info = new RanchInfoInfo();
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"value", info->value) && result;

	result = getAttrValue(element,"ranch_lv", info->ranch_lv) && result;
	result = getAttrValue(element,"riding_lv", info->riding_lv) && result;
	m_Ranch_Info_res_vec.push_back(info);
	return result;
}
