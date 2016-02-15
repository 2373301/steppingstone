#include "Logger.h"
#include "ManageFiresRes.h"
bool ManageFiresRes::loadContent(Document * xml_doc)
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
const FiresInfo & ManageFiresRes::getFiresInfo()
{
	return m_Fires__res_;
}
bool ManageFiresRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	FiresInfo * info = &m_Fires__res_;
	result = getAttrValue(element,"copy", info->copy) && result;
	result = getAttrValue(element,"critical_rate1", info->critical_rate1) && result;
	result = getAttrValue(element,"critical_rate2", info->critical_rate2) && result;
	result = getAttrValue(element,"critical_rate3", info->critical_rate3) && result;
	result = getAttrValue(element,"critical_rate4", info->critical_rate4) && result;
	result = getAttrValue(element,"critical_value1", info->critical_value1) && result;
	result = getAttrValue(element,"critical_value2", info->critical_value2) && result;
	result = getAttrValue(element,"critical_value3", info->critical_value3) && result;
	result = getAttrValue(element,"critical_value4", info->critical_value4) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"patch_exp", info->patch_exp) && result;
	result = getAttrValue(element,"time", info->time) && result;
	result = getAttrValue(element,"vip", info->vip) && result;
	result = getAttrValue(element,"yang_entrance", info->yang_entrance) && result;
	result = getAttrValue(element,"yin_entrance", info->yin_entrance) && result;

	return result;
}
