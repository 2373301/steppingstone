

#include "ManageWarScoreRes.h"
#include "Logger.h"

bool ManageWarScoreRes::loadContent(Document * xml_doc)
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

const WarScoreResInfoVec_t & ManageWarScoreRes::getWarScoreInfoVec()
{
	return m_war_score_info_vec;
}

bool ManageWarScoreRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	WarScoreResInfo * info = new WarScoreResInfo();

	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "subtype", info->subtype) && result;
	result = getAttrValue(element, "value", info->value) && result;
	result = getAttrValue(element, "value_score", info->value_score) && result;

	m_war_score_info_vec.push_back(info);

	return result;
}