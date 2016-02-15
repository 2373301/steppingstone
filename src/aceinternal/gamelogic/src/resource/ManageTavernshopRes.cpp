#include "Logger.h"
#include "ManageTavernshopRes.h"
bool ManageTavernshopRes::loadContent(Document * xml_doc)
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
const TavernshopInfoVector_t & ManageTavernshopRes::getTavernshopInfo()
{
	return m_tavernshop_res_;
}
bool ManageTavernshopRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TavernshopInfo * info = new TavernshopInfo();
	result = getAttrValue(element,"get_itemid", info->get_itemid) && result;
	result = getAttrValue(element,"get_itemnum", info->get_itemnum) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;

	m_tavernshop_res_.push_back(info);
	return result;
}
