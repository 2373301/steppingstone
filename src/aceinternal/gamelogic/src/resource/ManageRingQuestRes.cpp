#include "Logger.h"
#include "ManageRingQuestRes.h"
bool ManageRingQuestRes::loadContent(Document * xml_doc)
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
const RingQuestInfoVec_t & ManageRingQuestRes::getRingQuestInfo()
{
	return m_ring_quest_res_vec;
}
bool ManageRingQuestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RingQuestInfo * info = new RingQuestInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"lv", info->lv) && result;
	result = getAttrValue(element,"questid1", info->questid1) && result;
	result = getAttrValue(element,"questid2", info->questid2) && result;
	m_ring_quest_res_vec.push_back(info);
	return result;
}
