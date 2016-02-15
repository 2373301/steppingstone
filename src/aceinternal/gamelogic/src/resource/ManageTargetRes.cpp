#include "Logger.h"
#include "ManageTargetRes.h"
bool ManageTargetRes::loadContent(Document * xml_doc)
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
const TargetInfoMap_t & ManageTargetRes::getTargetInfo()
{
	return m_Target__res_map;
}
bool ManageTargetRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TargetInfo * info = new TargetInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"reward_num", info->reward_num) && result;
	result = getAttrValue(element,"spe_reward", info->spe_reward) && result;
	result = getAttrValue(element,"target", info->target) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
	TargetInfoMap_t::iterator it = m_Target__res_map.find(info->id);
	if (it != m_Target__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TARGET, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Target__res_map.insert(std::make_pair(info->id, info));
	return result;
}
