#include "Logger.h"
#include "ManagePushRes.h"
bool ManagePushRes::loadContent(Document * xml_doc)
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
const PushInfoMap_t & ManagePushRes::getPushInfo()
{
	return m_push__res_map;
}
bool ManagePushRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PushInfo * info = new PushInfo();
	result = getAttrValue(element,"get_id", info->get_id) && result;
	result = getAttrValue(element,"gold_id", info->gold_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"push_coin", info->push_coin) && result;
	result = getAttrValue(element,"push_id", info->push_id) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"push_coin2", info->push_coin2) && result;
	result = getAttrValue(element,"vip", info->vip) && result;
	result = getAttrValue(element,"push_num", info->push_num) && result;
	result = getAttrValue(element,"time", info->time) && result;
	result = getAttrValue(element,"unclock", info->unclock) && result;
	result = getAttrValue(element,"vip_exp", info->vip_exp) && result;
	PushInfoMap_t::iterator it = m_push__res_map.find(info->id);
	if (it != m_push__res_map.end())
	{
		DEF_LOG_ERROR("failed to load PUSH, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_push__res_map.insert(std::make_pair(info->id, info));
	return result;
}
