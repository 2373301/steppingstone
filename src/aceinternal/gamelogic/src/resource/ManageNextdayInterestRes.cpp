#include "Logger.h"
#include "ManageNextdayInterestRes.h"
bool ManageNextdayInterestRes::loadContent(Document * xml_doc)
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
const NextdayInterestInfoVec_t & ManageNextdayInterestRes::getNextdayInterestInfo()
{
	return m_Nextday_Interest_res_vec;
}
bool ManageNextdayInterestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	NextdayInterestInfo * info = new NextdayInterestInfo();
	result = getAttrValue(element,"back_max", info->back_max) && result;
	result = getAttrValue(element,"back_rate", info->back_rate) && result;
	result = getAttrValue(element,"end_date", info->end_date) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"range", info->range) && result;
	result = getAttrValue(element,"star_date", info->star_date) && result;
	m_Nextday_Interest_res_vec.push_back(info);
	return result;
}
