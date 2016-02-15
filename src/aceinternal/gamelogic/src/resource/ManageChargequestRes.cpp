#include "Logger.h"
#include "ManageChargequestRes.h"
bool ManageChargequestRes::loadContent(Document * xml_doc)
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
const ChargequestInfo & ManageChargequestRes::getChargequestInfo()
{
	return m_ChargeQuest__res_;
}
bool ManageChargequestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	ChargequestInfo * info = &m_ChargeQuest__res_;
	result = getAttrValue(element,"complete_coin", info->complete_coin) && result;
//	result = getAttrValue(element,"id", info->id) && result;
//	result = getAttrValue(element,"quest_name", info->quest_name) && result;
//	result = getAttrValue(element,"questdesc", info->questdesc) && result;
	result = getAttrValue(element,"reward", info->reward) && result;
	result = getAttrValue(element,"star_id", info->star_id) && result;
//	result = getAttrValue(element,"star_type", info->star_type) && result;

	return result;
}
