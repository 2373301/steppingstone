#include "Logger.h"
#include "ManageOfflinehangupRes.h"
bool ManageOfflinehangupRes::loadContent(Document * xml_doc)
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
const OfflinehangupInfo & ManageOfflinehangupRes::getOfflinehangupInfo()
{
	return m_offlinehangup_res;
}
bool ManageOfflinehangupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;

	result = getAttrValue(element,"a", m_offlinehangup_res.a) && result;
	result = getAttrValue(element,"b", m_offlinehangup_res.b) && result;
	result = getAttrValue(element,"time", m_offlinehangup_res.time_v) && result;
	result = getAttrValue(element,"maxtime", m_offlinehangup_res.maxtime) && result;

	return result;
}
