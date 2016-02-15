#include "Logger.h"
#include "ManageQqinviteRes.h"
bool ManageQqinviteRes::loadContent(Document * xml_doc)
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
const QqinviteInfoMap_t & ManageQqinviteRes::getQqinviteInfo()
{
	return m_QQInvite__res_map;
}
bool ManageQqinviteRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	QqinviteInfo * info = new QqinviteInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
//	result = getAttrValue(element,"desc", info->desc) && result;
	result = getAttrValue(element,"id", info->id) && result;
// 	result = getAttrValue(element,"img", info->img) && result;
// 	result = getAttrValue(element,"msg", info->msg) && result;
// 	result = getAttrValue(element,"sort", info->sort) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;
	QqinviteInfoMap_t::iterator it = m_QQInvite__res_map.find(info->id);
	if (it != m_QQInvite__res_map.end())
	{
		DEF_LOG_ERROR("failed to load QQINVITE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_QQInvite__res_map.insert(std::make_pair(info->id, info));
	return result;
}
