#include "Logger.h"
#include "ManagePlumSystemRes.h"
bool ManagePlumSystemRes::loadContent(Document * xml_doc)
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
const PlumSystemInfo & ManagePlumSystemRes::getPlumSystemInfo()
{
	return m_plum_system_res;
}
bool ManagePlumSystemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
//	PlumSystemInfo * info = new PlumSystemInfo();
	result = getAttrValue(element,"change_times", m_plum_system_res.change_times) && result;
	result = getAttrValue(element,"free_times", m_plum_system_res.free_times) && result;
	result = getAttrValue(element,"library", m_plum_system_res.library) && result;

	return result;
}