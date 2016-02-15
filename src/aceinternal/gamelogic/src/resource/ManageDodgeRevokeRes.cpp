#include "Logger.h"
#include "ManageDodgeRevokeRes.h"
bool ManageDodgeRevokeRes::loadContent(Document * xml_doc)
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
const DodgeRevokeInfo & ManageDodgeRevokeRes::getDodgeRevokeInfo()
{
	return m_dodge_revoke_res;
}
bool ManageDodgeRevokeRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
//	DodgeRevokeInfo * info = new DodgeRevokeInfo();
	result = getAttrValue(element,"gold_return_rate", m_dodge_revoke_res.gold_return_rate) && result;
	result = getAttrValue(element,"money_cost_value", m_dodge_revoke_res.money_cost_value) && result;
	result = getAttrValue(element,"money_return_rate", m_dodge_revoke_res.money_return_rate) && result;

	return result;
}
