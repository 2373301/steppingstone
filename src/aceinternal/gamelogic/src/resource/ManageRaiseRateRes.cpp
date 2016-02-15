

#include "ManageRaiseRateRes.h"
#include "Logger.h"

bool ManageRaiseRateRes::loadContent(Document * xml_doc)
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

const RaiseRateResInfoVec_t & ManageRaiseRateRes::getRaiseRateInfoVec()
{
	return m_war_score_info_vec;
}

bool ManageRaiseRateRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	RaiseRateResInfo * info = new RaiseRateResInfo();

	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "pool_1", info->pool_1) && result;
	result = getAttrValue(element, "rate_1", info->rate_1) && result;
	result = getAttrValue(element, "pool_2", info->pool_2) && result;
	result = getAttrValue(element, "rate_2", info->rate_2) && result;
	result = getAttrValue(element, "pool_3", info->pool_3) && result;
	result = getAttrValue(element, "rate_3", info->rate_3) && result;
	result = getAttrValue(element, "pool_4", info->pool_4) && result;
	result = getAttrValue(element, "rate_4", info->rate_4) && result;
	result = getAttrValue(element, "pool_5", info->pool_5) && result;
	result = getAttrValue(element, "rate_5", info->rate_5) && result;
	result = getAttrValue(element, "basecount_1", info->basecount_1) && result;
	result = getAttrValue(element, "pool_a", info->pool_a) && result;
	result = getAttrValue(element, "basecount_2", info->basecount_2) && result;
	result = getAttrValue(element, "pool_b", info->pool_b) && result;
	result = getAttrValue(element, "basecount_0", info->basecount_0) && result;
	result = getAttrValue(element, "pool_0", info->pool_0) && result;

	m_war_score_info_vec.push_back(info);

	return result;
}