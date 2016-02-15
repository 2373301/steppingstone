
#include "Logger.h"
#include "ManageHangUpRes.h"

bool ManageHangUpRes::loadContent(Document * xml_doc)
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

const HangUpRes & ManageHangUpRes::getHangUpRes()
{
	return m_hang_up_res;
}

bool ManageHangUpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	result = getAttrValue(element, "a", m_hang_up_res.a) && result;
	result = getAttrValue(element, "b", m_hang_up_res.b) && result;
	result = getAttrValue(element, "energy", m_hang_up_res.energy) && result;

	return result;
}