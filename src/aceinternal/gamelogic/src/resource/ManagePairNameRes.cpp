#include "Logger.h"
#include "ManagePairNameRes.h"
bool ManagePairNameRes::loadContent(Document * xml_doc)
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
const PairNameInfoVec_t & ManagePairNameRes::getPairNameInfo()
{
	return m_pair_name_res_vec;
}
bool ManagePairNameRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PairNameInfo * info = new PairNameInfo();
	result = getAttrValue(element,"colour", info->colour) && result;
	result = getAttrValue(element,"get_way1", info->get_way1) && result;
	result = getAttrValue(element,"get_way2", info->get_way2) && result;
	result = getAttrValue(element,"get_way3", info->get_way3) && result;
	result = getAttrValue(element,"get_way4", info->get_way4) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"name", info->name) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;
	result = getAttrValue(element,"value3", info->value3) && result;
	result = getAttrValue(element,"value4", info->value4) && result;
	m_pair_name_res_vec.push_back(info);
	return result;
}
