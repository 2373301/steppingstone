#include "Logger.h"
#include "ManageShlUnlockRes.h"
bool ManageShlUnlockRes::loadContent(Document * xml_doc)
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
const ShlUnlockInfoVec_t & ManageShlUnlockRes::getShlUnlockInfo()
{
	return m_SHL_Unlock_res_vec;
}
bool ManageShlUnlockRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	ShlUnlockInfo * info = new ShlUnlockInfo();
	result = getAttrValue(element,"colour", info->colour) && result;
	result = getAttrValue(element,"subtype", info->subtype) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"unlock_id", info->unlock_id) && result;
	m_SHL_Unlock_res_vec.push_back(info);
	return result;
}
