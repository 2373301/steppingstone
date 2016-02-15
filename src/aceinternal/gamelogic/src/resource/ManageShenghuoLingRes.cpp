#include "Logger.h"
#include "ManageShenghuoLingRes.h"
bool ManageShenghuoLingRes::loadContent(Document * xml_doc)
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
const ShenghuoLingInfoVec_t & ManageShenghuoLingRes::getShenghuoLingInfo()
{
	return m_Shenghuo_ling_res_vec;
}
bool ManageShenghuoLingRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	ShenghuoLingInfo * info = new ShenghuoLingInfo();
	result = getAttrValue(element,"class", info->class_id) && result;
	result = getAttrValue(element,"class_need_item", info->class_need_item) && result;
	result = getAttrValue(element,"class_need_num", info->class_need_num) && result;
	result = getAttrValue(element,"color", info->color) && result;
	result = getAttrValue(element,"exp", info->exp) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"need_money", info->need_money) && result;
	result = getAttrValue(element,"p1", info->p1) && result;
	result = getAttrValue(element,"p2", info->p2) && result;
	result = getAttrValue(element,"p3", info->p3) && result;
	result = getAttrValue(element,"p4", info->p4) && result;
	result = getAttrValue(element,"p5", info->p5) && result;
	result = getAttrValue(element,"palyer_level", info->palyer_level) && result;
	result = getAttrValue(element,"patch_num", info->patch_num) && result;
	result = getAttrValue(element,"subtype", info->subtype) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;
	result = getAttrValue(element,"value3", info->value3) && result;
	result = getAttrValue(element,"value4", info->value4) && result;
	result = getAttrValue(element,"value5", info->value5) && result;
	m_Shenghuo_ling_res_vec.push_back(info);
	return result;
}
