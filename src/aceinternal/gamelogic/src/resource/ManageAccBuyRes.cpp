#include "Logger.h"
#include "ManageAccBuyRes.h"
bool ManageAccBuyRes::loadContent(Document * xml_doc)
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
const AccBuyInfoVec_t & ManageAccBuyRes::getAccBuyInfo()
{
	return m_Acc_Buy_res_vec;
}
bool ManageAccBuyRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	AccBuyInfo * info = new AccBuyInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"end_date", info->end_date) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"star_date", info->star_date) && result;
	m_Acc_Buy_res_vec.push_back(info);
	return result;
}
