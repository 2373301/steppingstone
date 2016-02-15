#include "Logger.h"
#include "ManagePairGamblingRes.h"
bool ManagePairGamblingRes::loadContent(Document * xml_doc)
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
const PairGamblingInfoVec_t & ManagePairGamblingRes::getPairGamblingInfo()
{
	return m_pair_gambling_res_vec;
}
bool ManagePairGamblingRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PairGamblingInfo * info = new PairGamblingInfo();
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"lose_rate", info->lose_rate) && result;
	result = getAttrValue(element,"money", info->money) && result;
	result = getAttrValue(element,"round", info->round) && result;
	result = getAttrValue(element,"subtype", info->subtype) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"win_rate", info->win_rate) && result;
	m_pair_gambling_res_vec.push_back(info);
	return result;
}
