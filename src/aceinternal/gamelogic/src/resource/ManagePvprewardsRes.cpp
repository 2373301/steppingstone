#include "Logger.h"
#include "ManagePvprewardsRes.h"
bool ManagePvprewardsRes::loadContent(Document * xml_doc)
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
const PvprewardsInfoVec_t & ManagePvprewardsRes::getPvprewardsInfo()
{
	return m_pvprewards__res_vec;
}
bool ManagePvprewardsRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PvprewardsInfo * info = new PvprewardsInfo();
	result = getAttrValue(element,"bonusid", info->bonusid) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"reward", info->reward) && result;
	result = getAttrValue(element,"servers", info->servers) && result;
	result = getAttrValue(element,"rewardtype", info->rewardtype) && result;
	m_pvprewards__res_vec.push_back(info);
	return result;
}
