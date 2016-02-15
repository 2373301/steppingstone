#include "Logger.h"
#include "ManageHorseAttrRes.h"
bool ManageHorseAttrRes::loadContent(Document * xml_doc)
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
const HorseAttrInfoVec_t & ManageHorseAttrRes::getHorseAttrInfo()
{
	return m_Horse_Attr_res_vec;
}
bool ManageHorseAttrRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	HorseAttrInfo * info = new HorseAttrInfo();
	result = getAttrValue(element,"horse_id", info->horse_id) && result;
	result = getAttrValue(element,"lv", info->lv) && result;
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"value", info->value) && result;
	m_Horse_Attr_res_vec.push_back(info);
	return result;
}
