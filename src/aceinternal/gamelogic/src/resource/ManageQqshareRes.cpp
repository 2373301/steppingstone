#include "Logger.h"
#include "ManageQqshareRes.h"
bool ManageQqshareRes::loadContent(Document * xml_doc)
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
const QqshareInfo & ManageQqshareRes::getQqshareInfo()
{
	return m_QQShare__res_;
}
bool ManageQqshareRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	QqshareInfo * info = &m_QQShare__res_;
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"day", info->day) && result;
	result = getAttrValue(element,"everyday_bonus", info->everyday_bonus) && result;
	result = getAttrValue(element,"id", info->id) && result;


	return result;
}
