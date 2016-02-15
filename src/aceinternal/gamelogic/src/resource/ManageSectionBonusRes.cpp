#include "Logger.h"
#include "ManageSectionBonusRes.h"
bool ManageSectionBonusRes::loadContent(Document * xml_doc)
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
const SectionBonusInfoVec_t & ManageSectionBonusRes::getSectionBonusInfo()
{
	return m_Section_Bonus_res_vec;
}
bool ManageSectionBonusRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SectionBonusInfo * info = new SectionBonusInfo();
	result = getAttrValue(element,"bonus1", info->bonus1) && result;
	result = getAttrValue(element,"bonus2", info->bonus2) && result;
	result = getAttrValue(element,"end_date", info->end_date) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"star_date", info->star_date) && result;
	m_Section_Bonus_res_vec.push_back(info);
	return result;
}
