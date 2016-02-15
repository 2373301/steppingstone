

#include "ManageRaiseRes.h"
#include "Logger.h"

bool ManageRaiseRes::loadContent(Document * xml_doc)
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

const RaiseResInfoVec_t & ManageRaiseRes::getRaiseInfoVec()
{
	return m_raise_info_vec;
}

bool ManageRaiseRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	RaiseResInfo * info = new RaiseResInfo();

	result = getAttrValue(element, "groupid", info->group_id) && result;
	result = getAttrValue(element, "serial", info->sequence_id) && result;
	result = getAttrValue(element, "str", info->str) && result;
	result = getAttrValue(element, "agi", info->agi) && result;
	result = getAttrValue(element, "int", info->int_value) && result;

	m_raise_info_vec.push_back(info);

	return result;
}