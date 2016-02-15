
#include "ManageQuestRes.h"
#include "Logger.h"

bool ManageQuestRes::loadContent(Document * xml_doc)
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

const QuestResInfoMap_t & ManageQuestRes::getQuestInfoMap()
{
	return m_quest_info_map;
}

bool ManageQuestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	QuestResInfo * info = new QuestResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "canbecancel", info->canbecancel) && result;
	result = getAttrValue(element, "sort", info->sort) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "accept_npcid", info->accept_npcid) && result;
	result = getAttrValue(element, "unlock_questid", info->unlock_questid) && result;
	result = getAttrValue(element, "limit_playerlv_low", info->limit_playerlv_low) && result;
	result = getAttrValue(element, "limit_playerlv_high", info->limit_playerlv_high) && result;
	result = getAttrValue(element, "isitemempty", info->isitemempty) && result;
	result = getAttrValue(element, "ispanterempty", info->ispanterempty) && result;
	result = getAttrValue(element, "sex", info->sex) && result;
	result = getAttrValue(element, "job", info->job) && result;
	result = getAttrValue(element, "sect", info->sect) && result;

	uint32 acceptdo_item = 0;
	uint32 acceptdo_itemnum = 0;
	result = getAttrValue(element, "acceptdo_item1", acceptdo_item) && result;
	result = getAttrValue(element, "acceptdo_itemnum1", acceptdo_itemnum) && result;
	if (acceptdo_item != 0)
	{
		info->acceptdo_item.push_back(PairUint32Uint32(acceptdo_item, acceptdo_itemnum));
	}
	result = getAttrValue(element, "acceptdo_item2", acceptdo_item) && result;
	result = getAttrValue(element, "acceptdo_itemnum2", acceptdo_itemnum) && result;
	if (acceptdo_item != 0)
	{
		info->acceptdo_item.push_back(PairUint32Uint32(acceptdo_item, acceptdo_itemnum));
	}
	result = getAttrValue(element, "acceptdo_item3", acceptdo_item) && result;
	result = getAttrValue(element, "acceptdo_itemnum3", acceptdo_itemnum) && result;
	if (acceptdo_item != 0)
	{
		info->acceptdo_item.push_back(PairUint32Uint32(acceptdo_item, acceptdo_itemnum));
	}

	result = getAttrValue(element, "acceptdo_jump", info->acceptdo_jump) && result;
	result = getAttrValue(element, "acceptdo_jumpx", info->acceptdo_jumpx) && result;
	result = getAttrValue(element, "acceptdo_jumpy", info->acceptdo_jumpy) && result;
	result = getAttrValue(element, "isdead", info->isdead) && result;
	result = getAttrValue(element, "isoffline", info->isoffline) && result;
	result = getAttrValue(element, "complete_npcid", info->complete_npcid) && result;

	uint32 complete_reqa = 0;
	uint32 complete_reqb = 0;
	uint32 complete_reqc = 0;
	result = getAttrValue(element, "complete_reqia", complete_reqa) && result;
	result = getAttrValue(element, "complete_reqib", complete_reqb) && result;
	result = getAttrValue(element, "complete_reqic", complete_reqc) && result;
	if (complete_reqa != 0)
	{
		info->complete_req.push_back(ThreeUint32(complete_reqa, complete_reqb, complete_reqc));
	}
	result = getAttrValue(element, "complete_reqiia", complete_reqa) && result;
	result = getAttrValue(element, "complete_reqiib", complete_reqb) && result;
	result = getAttrValue(element, "complete_reqiic", complete_reqc) && result;
	if (complete_reqa != 0)
	{
		info->complete_req.push_back(ThreeUint32(complete_reqa, complete_reqb, complete_reqc));
	}

	result = getAttrValue(element, "complete_reqiiia", complete_reqa) && result;
	result = getAttrValue(element, "complete_reqiiib", complete_reqb) && result;
	result = getAttrValue(element, "complete_reqiiic", complete_reqc) && result;
	if (complete_reqa != 0)
	{
		info->complete_req.push_back(ThreeUint32(complete_reqa, complete_reqb, complete_reqc));
	}

	result = getAttrValue(element, "complete_itemempty", info->complete_itemempty) && result;
	result = getAttrValue(element, "completedo_addexp", info->completedo_addexp) && result;
	result = getAttrValue(element, "completedo_addteamexp", info->completedo_addteamexp) && result;
	result = getAttrValue(element, "completedo_addfamilyexp", info->completedo_addfamilyexp) && result;
	result = getAttrValue(element, "completedo_addmoney", info->completedo_addmoney) && result;
	result = getAttrValue(element, "completedo_addcoin", info->completedo_addcoin) && result;
	result = getAttrValue(element, "completedo_addframe", info->completedo_addframe) && result;
	result = getAttrValue(element, "completedo_addseniority", info->completedo_addseniority) && result;

	uint32 completedo_additema = 0;
	uint32 completedo_additemb = 0;
	result = getAttrValue(element, "completedo_additemia", completedo_additema) && result;
	result = getAttrValue(element, "completedo_additemib", completedo_additemb) && result;
	if (0 != completedo_additema)
	{
		info->completedo_additem.push_back(PairUint32Uint32(completedo_additema, completedo_additemb));
	}

	result = getAttrValue(element, "completedo_additemiia", completedo_additema) && result;
	result = getAttrValue(element, "completedo_additemiib", completedo_additemb) && result;
	if (0 != completedo_additema)
	{
		info->completedo_additem.push_back(PairUint32Uint32(completedo_additema, completedo_additemb));
	}

	result = getAttrValue(element, "completedo_additemiiia", completedo_additema) && result;
	result = getAttrValue(element, "completedo_additemiiib", completedo_additemb) && result;
	if (0 != completedo_additema)
	{
		info->completedo_additem.push_back(PairUint32Uint32(completedo_additema, completedo_additemb));
	}

	result = getAttrValue(element, "completedo_additemiva", completedo_additema) && result;
	result = getAttrValue(element, "completedo_additemivb", completedo_additemb) && result;
	if (0 != completedo_additema)
	{
		info->completedo_additem.push_back(PairUint32Uint32(completedo_additema, completedo_additemb));
	}

	result = getAttrValue(element, "completedo_additemva", completedo_additema) && result;
	result = getAttrValue(element, "completedo_additemvb", completedo_additemb) && result;
	if (0 != completedo_additema)
	{
		info->completedo_additem.push_back(PairUint32Uint32(completedo_additema, completedo_additemb));
	}

	uint32 completedo_selectitema = 0;
	uint32 completedo_selectitemb = 0;
	result = getAttrValue(element, "completedo_selectitemia", completedo_selectitema) && result;
	result = getAttrValue(element, "completedo_selectitemib", completedo_selectitemb) && result;
	if (0 != completedo_selectitema)
	{
		info->completedo_selectitem.push_back(PairUint32Uint32(completedo_selectitema, completedo_selectitemb));
	}

	result = getAttrValue(element, "completedo_selectitemiia", completedo_selectitema) && result;
	result = getAttrValue(element, "completedo_selectitemiib", completedo_selectitemb) && result;
	if (0 != completedo_selectitema)
	{
		info->completedo_selectitem.push_back(PairUint32Uint32(completedo_selectitema, completedo_selectitemb));
	}

	result = getAttrValue(element, "completedo_selectitemiiia", completedo_selectitema) && result;
	result = getAttrValue(element, "completedo_selectitemiiib", completedo_selectitemb) && result;
	if (0 != completedo_selectitema)
	{
		info->completedo_selectitem.push_back(PairUint32Uint32(completedo_selectitema, completedo_selectitemb));
	}

	result = getAttrValue(element, "completedo_selectitemiva", completedo_selectitema) && result;
	result = getAttrValue(element, "completedo_selectitemivb", completedo_selectitemb) && result;
	if (0 != completedo_selectitema)
	{
		info->completedo_selectitem.push_back(PairUint32Uint32(completedo_selectitema, completedo_selectitemb));
	}

	result = getAttrValue(element, "completedo_selectitemva", completedo_selectitema) && result;
	result = getAttrValue(element, "completedo_selectitemvb", completedo_selectitemb) && result;
	if (0 != completedo_selectitema)
	{
		info->completedo_selectitem.push_back(PairUint32Uint32(completedo_selectitema, completedo_selectitemb));
	}

	result = getAttrValue(element, "completedo_unlockrole", info->completedo_unlockrole) && result;
	result = getAttrValue(element, "completedo_deleteitem", info->completedo_deleteitem) && result;
	result = getAttrValue(element, "completedo_jumpmap", info->completedo_jumpmap) && result;
	result = getAttrValue(element, "completedo_jumpx", info->completedo_jumpx) && result;
	result = getAttrValue(element, "completedo_jumpy", info->completedo_jumpy) && result;
	result = getAttrValue(element, "completedo_selectitemmodel", info->completedo_selectitemmodel) && result;
	result = getAttrValue(element, "level_unlock", info->level_unlock) && result;
	result = getAttrValue(element, "auto_accept", info->auto_accept) && result;

	QuestResInfoMap_t::iterator it = m_quest_info_map.find(info->id);
	if (it != m_quest_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Quest id : <%d>\n", info->id);
		return false;
	}

	m_quest_info_map.insert(std::make_pair(info->id, info));

	return result;
}