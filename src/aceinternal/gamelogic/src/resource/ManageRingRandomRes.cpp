#include "Logger.h"
#include "ManageRingRandomRes.h"
bool ManageRingRandomRes::loadContent(Document * xml_doc)
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
const RingRandomInfoVec_t & ManageRingRandomRes::getRingRandomInfo()
{
	return m_ring_random_res_vec;
}
bool ManageRingRandomRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RingRandomInfo * info = new RingRandomInfo();
	result = getAttrValue(element,"bonus_id", info->bonus_id) && result;
	result = getAttrValue(element,"guil_bonus", info->guil_bonus) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"lv", info->lv) && result;
	m_ring_random_res_vec.push_back(info);
	return result;
}
