#include "Logger.h"
#include "ManageWebVipRes.h"
bool ManageWebVipRes::loadContent(Document * xml_doc)
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
const WebVipInfo & ManageWebVipRes::getWebVipInfo()
{
	return m_web_vip_res_;
}
bool ManageWebVipRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	WebVipInfo * info = &m_web_vip_res_;
	result = getAttrValue(element,"limit_require", info->limit_require) && result;
	result = getAttrValue(element,"vip1_item", info->vip1_item) && result;
	result = getAttrValue(element,"vip_transform", info->vip_transform) && result;
	result = getAttrValue(element,"yearvip_item", info->yearvip_item) && result;

	return result;
}
