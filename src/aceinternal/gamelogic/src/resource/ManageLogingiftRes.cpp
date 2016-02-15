#include "Logger.h"
#include "ManageLogingiftRes.h"
bool ManageLogingiftRes::loadContent(Document * xml_doc)
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
const LogingiftInfo & ManageLogingiftRes::getLogingiftInfo()
{
	return m_LoginGift__res_;
}
bool ManageLogingiftRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	LogingiftInfo * info = &m_LoginGift__res_;
	result = getAttrValue(element,"percent", info->percent) && result;
	result = getAttrValue(element,"rate1", info->rate1) && result;
	result = getAttrValue(element,"rate2", info->rate2) && result;
	result = getAttrValue(element,"rate3", info->rate3) && result;

	return result;
}
