#include "Logger.h"
#include "ManageGetmoneyRes.h"
bool ManageGetmoneyRes::loadContent(Document * xml_doc)
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
const GetmoneyInfoVec_t & ManageGetmoneyRes::getGetmoneyInfo()
{
	return m_GetMoney__res_vec;
}
bool ManageGetmoneyRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	GetmoneyInfo * info = new GetmoneyInfo();
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"rate", info->rate) && result;
	m_GetMoney__res_vec.push_back(info);
	return result;
}
