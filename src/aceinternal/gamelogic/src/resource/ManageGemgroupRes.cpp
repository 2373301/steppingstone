#include "Logger.h"
#include "ManageGemgroupRes.h"
bool ManageGemgroupRes::loadContent(Document * xml_doc)
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
const GemgroupInfoVec_t & ManageGemgroupRes::getGemgroupInfo()
{
	return m_gemgroup__res_vec;
}
bool ManageGemgroupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	GemgroupInfo * info = new GemgroupInfo();
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"part", info->part) && result;
	m_gemgroup__res_vec.push_back(info);
	return result;
}
