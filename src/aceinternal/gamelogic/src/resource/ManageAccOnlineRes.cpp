#include "Logger.h"
#include "ManageAccOnlineRes.h"
bool ManageAccOnlineRes::loadContent(Document * xml_doc)
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
const AccOnlineInfo & ManageAccOnlineRes::getAccOnlineInfo()
{
	return m_Acc_Online_res_;
}
bool ManageAccOnlineRes::loadInfo(Element * element)
{
    if (NULL == element)
    {
        return false;
    }
    bool result = true;
    result = getAttrValue(element,"days", m_Acc_Online_res_.days) && result;
    result = getAttrValue(element,"rewards", m_Acc_Online_res_.rewards) && result;
    result = getAttrValue(element,"start_time", m_Acc_Online_res_.start_time) && result;

    return result;
}
