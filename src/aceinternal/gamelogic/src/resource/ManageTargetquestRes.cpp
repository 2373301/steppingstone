#include "Logger.h"
#include "ManageTargetquestRes.h"
bool ManageTargetquestRes::loadContent(Document * xml_doc)
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
const TargetquestInfoVec_t & ManageTargetquestRes::getTargetquestInfo()
{
	return m_TargetQuest__res_vec;
}
bool ManageTargetquestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TargetquestInfo * info = new TargetquestInfo();
	result = getAttrValue(element,"a", info->a) && result;
	result = getAttrValue(element,"b", info->b) && result;
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"land", info->land) && result;
	result = getAttrValue(element,"needlevel", info->needlevel) && result;
	result = getAttrValue(element,"quest", info->quest) && result;
	result = getAttrValue(element,"reward", info->reward) && result;
	result = getAttrValue(element,"reward_num", info->reward_num) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
	m_TargetQuest__res_vec.push_back(info);
	return result;
}
