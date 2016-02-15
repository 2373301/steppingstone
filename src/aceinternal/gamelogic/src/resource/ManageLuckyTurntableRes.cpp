#include "Logger.h"
#include "ManageLuckyTurntableRes.h"
bool ManageLuckyTurntableRes::loadContent(Document * xml_doc)
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
const LuckyTurntableInfo & ManageLuckyTurntableRes::getLuckyTurntableInfo()
{
	return m_Lucky_Turntable_res_;
}
bool ManageLuckyTurntableRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"end_time", m_Lucky_Turntable_res_.end_time) && result;
	result = getAttrValue(element,"gold_percent", m_Lucky_Turntable_res_.gold_percent) && result;
	result = getAttrValue(element,"initial_fund", m_Lucky_Turntable_res_.initial_fund) && result;
	result = getAttrValue(element,"lucky_item", m_Lucky_Turntable_res_.lucky_item) && result;
	result = getAttrValue(element,"need_gold", m_Lucky_Turntable_res_.need_gold) && result;
	result = getAttrValue(element,"num", m_Lucky_Turntable_res_.num) && result;
	result = getAttrValue(element,"start_time", m_Lucky_Turntable_res_.start_time) && result;
    result = getAttrValue(element,"fund_max", m_Lucky_Turntable_res_.fund_max) && result;
    result = getAttrValue(element,"fund_min", m_Lucky_Turntable_res_.fund_min) && result;

    result = getAttrValue(element,"discount", m_Lucky_Turntable_res_.discount) && result;
    result = getAttrValue(element,"accum_num", m_Lucky_Turntable_res_.accum_num) && result;
    result = getAttrValue(element,"accum_id", m_Lucky_Turntable_res_.accum_id) && result;

	return result;
}
