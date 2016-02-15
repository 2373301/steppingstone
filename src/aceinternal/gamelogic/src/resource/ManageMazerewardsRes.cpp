#include "Logger.h"
#include "ManageMazerewardsRes.h"
bool ManageMazerewardsRes::loadContent(Document * xml_doc)
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
const MazerewardsInfoVec_t & ManageMazerewardsRes::getMazerewardsInfo()
{
	return m_Mazerewards__res_vec;
}
bool ManageMazerewardsRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MazerewardsInfo * info = new MazerewardsInfo();
	result = getAttrValue(element,"event", info->event) && result;
	result = getAttrValue(element,"mazeid", info->mazeid) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"percent", info->percent) && result;
	result = getAttrValue(element,"roomid", info->roomid) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"maxlevel", info->maxLevel) && result;
	result = getAttrValue(element,"minlevel", info->minLevel) && result;

	m_Mazerewards__res_vec.push_back(info);
	return result;
}
