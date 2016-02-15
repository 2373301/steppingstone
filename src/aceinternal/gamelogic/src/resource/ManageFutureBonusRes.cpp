#include "Logger.h"
#include "ManageFutureBonusRes.h"
bool ManageFutureBonusRes::loadContent(Document * xml_doc)
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
const FutureBonusInfoVec_t & ManageFutureBonusRes::getFutureBonusInfo()
{
	return m_Future_Bonus_res_vec;
}
bool ManageFutureBonusRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	FutureBonusInfo * info = new FutureBonusInfo();
	result = getAttrValue(element,"bonus1", info->bonus1) && result;
	result = getAttrValue(element,"bonus10", info->bonus10) && result;
	result = getAttrValue(element,"bonus2", info->bonus2) && result;
	result = getAttrValue(element,"bonus3", info->bonus3) && result;
	result = getAttrValue(element,"bonus4", info->bonus4) && result;
	result = getAttrValue(element,"bonus5", info->bonus5) && result;
	result = getAttrValue(element,"bonus6", info->bonus6) && result;
	result = getAttrValue(element,"bonus7", info->bonus7) && result;
	result = getAttrValue(element,"bonus8", info->bonus8) && result;
	result = getAttrValue(element,"bonus9", info->bonus9) && result;
	result = getAttrValue(element,"end_date", info->end_date) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"principal", info->principal) && result;
	result = getAttrValue(element,"star_date", info->star_date) && result;
	m_Future_Bonus_res_vec.push_back(info);
	return result;
}
