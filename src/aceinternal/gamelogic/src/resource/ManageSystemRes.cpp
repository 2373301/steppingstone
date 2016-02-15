
#include "ManageSystemRes.h"
#include "Logger.h"

#define CHECK_BOOLEAN_WITH_LOG_RETURN(res, result)	\
	if (!(res))	\
	{	\
	DEF_LOG_ERROR("failed to check boolean <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

bool ManageSystemRes::loadContent(Document * xml_doc)
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

	result = loadInfo(root_ele) && result;

	return result;
}

const SystemResInfo & ManageSystemRes::getSystemInfo()
{
	return m_system_info;
}

bool ManageSystemRes::loadInfo(Element * element)
{
	Element * ele = element->get_element("system");
	if (NULL == ele)
	{
		return false;
	}

	bool result = true;

	result = getAttrValue(ele, "edition", m_system_info.edition) && result;
	result = getAttrValue(ele, "level_cap", m_system_info.level_cap) && result;
	result = getAttrValue(ele, "quality_cap", m_system_info.quality_cap) && result;

	result = getAttrValue(ele, "itemstreng_op", m_system_info.itemstreng_op) && result;
	result = getAttrValue(ele, "illusion_op", m_system_info.illusion_op) && result;
	result = getAttrValue(ele, "itemstreng_get", m_system_info.itemstreng_get) && result;

	result = getAttrValue(ele, "strenger_cd", m_system_info.strenger_cd) && result;
	result = getAttrValue(ele, "strengerlevel_cap", m_system_info.strengerlevel_cap) && result;
	result = getAttrValue(ele, "itemstreng_cd", m_system_info.itemstreng_cd) && result;
	result = getAttrValue(ele, "itemstreng_multiple", m_system_info.itemstreng_multiple) && result;
	result = getAttrValue(ele, "strengbox1", m_system_info.strengbox1) && result;
	CHECK_BOOLEAN_WITH_LOG_RETURN((m_system_info.strengbox1.size() == 2), false);

	result = getAttrValue(ele, "strengbox2", m_system_info.strengbox2) && result;
	CHECK_BOOLEAN_WITH_LOG_RETURN((m_system_info.strengbox2.size() == 2), false);

	result = getAttrValue(ele, "strengbox3", m_system_info.strengbox3) && result;
	CHECK_BOOLEAN_WITH_LOG_RETURN((m_system_info.strengbox3.size() == 2), false);

	result = getAttrValue(ele, "martial", m_system_info.martial) && result;
	result = getAttrValue(ele, "itemstreng_coin", m_system_info.itemstreng_coin) && result;

	result = getAttrValue(ele, "step_down", m_system_info.step_down) && result;
	result = getAttrValue(ele, "listing", m_system_info.listing) && result;

	result = getAttrValue(ele, "returnitemnum", m_system_info.returnitemnum) && result;
	result = getAttrValue(ele, "returnitemtimes", m_system_info.returnitemtimes) && result;
	result = getAttrValue(ele, "tili", m_system_info.tili) && result;
	result = getAttrValue(ele, "tilireturn", m_system_info.tilireturn) && result;
	result = getAttrValue(ele, "streng_success", m_system_info.streng_success) && result;
	
	result = getAttrValue(ele, "jjwin", m_system_info.jjwin) && result;

    result = getAttrValue(ele, "specialshop_unlock", m_system_info.specialshop_unlock) && result;
	CHECK_BOOLEAN_WITH_LOG_RETURN((m_system_info.specialshop_unlock.size() == 2), false);

	result = getAttrValue(ele, "cardku", m_system_info.cardku) && result;
	result = getAttrValue(ele, "cardhad", m_system_info.cardhad) && result;
	result = getAttrValue(ele, "cardlun", m_system_info.cardlun) && result;
	result = getAttrValue(ele, "eggnum", m_system_info.eggnum) && result;
	result = getAttrValue(ele, "lawcd", m_system_info.lawcd) && result;
	result = getAttrValue(ele, "waittime", m_system_info.waittime) && result;
	result = getAttrValue(ele, "intwaittime", m_system_info.intwaittime) && result;
	result = getAttrValue(ele, "inspire_time", m_system_info.inspire_time) && result;

	result = getAttrValue(ele, "boss_start1", m_system_info.boss_start1) && result;
	result = getAttrValue(ele, "boss_start2", m_system_info.boss_start2) && result;

    result = getAttrValue(ele, "chakranum", m_system_info.chakra_box) && result;
    result = getAttrValue(ele, "chakranget_num", m_system_info.chakra_caches_num) && result;

	result = getAttrValue(ele, "copy_playernum_limit", m_system_info.copy_playernum_limit) && result;
	result = getAttrValue(ele, "copy_times_free", m_system_info.copy_times_free) && result;

	result = getAttrValue(ele, "give_tili", m_system_info.give_tili) && result;
	result = getAttrValue(ele, "givetili_top", m_system_info.givetili_top) && result;

    result = getAttrValue(ele, "maxpcopygood_a", m_system_info.maxpcopygood_a) && result;
    result = getAttrValue(ele, "maxpcopygood_b", m_system_info.maxpcopygood_b) && result;
	result = getAttrValue(ele, "confraternity_pay", m_system_info.confraternity_pay) && result;
	result = getAttrValue(ele, "recruitment_special", m_system_info.recruitment_special) && result;

	//>>仗剑江湖自动推图时间间隔
	result = getAttrValue(ele, "checkpoints_cd", m_system_info.checkpoints_cd) && result;
	//>>射雕轮次间隔, 射雕清除冷却时间消费表id
	result = getAttrValue(ele, "clearning_cd", m_system_info.cleaning_cd) && result;
	result = getAttrValue(ele, "clearningcd_clean", m_system_info.cleaningcd_clean) && result;

	//
	result = getAttrValue(ele, "egg_cd", m_system_info.egg_cd) && result;
	result = getAttrValue(ele, "eggcd_clean", m_system_info.eggcd_clean) && result;
	result = getAttrValue(ele, "pvp_times", m_system_info.pvp_times) && result;
	result = getAttrValue(ele, "pvp_past", m_system_info.pvp_past) && result;
	result = getAttrValue(ele, "pvp_past_num", m_system_info.pvp_past_num) && result;
	
	result = getAttrValue(ele, "tavern_times1", m_system_info.tavern_times1) && result;
	result = getAttrValue(ele, "tavern_times2", m_system_info.tavern_times2) && result;
	result = getAttrValue(ele, "tavern_cd", m_system_info.tavern_cd) && result;
	result = getAttrValue(ele, "tavern_cdmax", m_system_info.tavern_cdmax) && result;
	result = getAttrValue(ele, "tavern_nums", m_system_info.tavern_nums) && result;

	result = getAttrValue(ele, "canshu_a", m_system_info.rank_param) && result;

	result = getAttrValue(ele, "physical_reward_time", m_system_info.energy_reward_time) && result;
	result = getAttrValue(ele, "physical_reward_value", m_system_info.energy_reward_value) && result;

	result = getAttrValue(ele, "raise_a", m_system_info.raise_a) && result;
	result = getAttrValue(ele, "raise_b", m_system_info.raise_b) && result;
	result = getAttrValue(ele, "level_first_strike", m_system_info.level_first_strike) && result;
	result = getAttrValue(ele, "sanwei_first_strike_1", m_system_info.sanwei_first_strike_1) && result;
	result = getAttrValue(ele, "sanwei_first_strike_2", m_system_info.sanwei_first_strike_2) && result;
	result = getAttrValue(ele, "sanwei_first_strike_3", m_system_info.sanwei_first_strike_3) && result;
	result = getAttrValue(ele, "race_num", m_system_info.race_num) && result;

	result = getAttrValue(ele, "voting_cd2", m_system_info.splendid_force_cool_time) && result;
	result = getAttrValue(ele, "voting_cd", m_system_info.splendid_cool_time) && result;
	result = getAttrValue(ele, "voting_cd3", m_system_info.splendid_canvass_cool_time) && result;
	result = getAttrValue(ele, "worship_times", m_system_info.splendid_worship_time) && result;
	result = getAttrValue(ele, "arena_time", m_system_info.arena_time) && result;
	result = getAttrValue(ele, "raise_max_a", m_system_info.raise_max_a) && result;
	result = getAttrValue(ele, "raise_max_b", m_system_info.raise_max_b) && result; 

	result = getAttrValue(ele, "tresure_shop_limittime", m_system_info.tresure_shop_limittime) && result;
	result = getAttrValue(ele, "tresure_shop_cd", m_system_info.tresure_shop_cd) && result;
	result = getAttrValue(ele, "tresure_shop_time", m_system_info.tresure_shop_time) && result;
	result = getAttrValue(ele, "tavern_nums2", m_system_info.tavern_nums2) && result;

	if (m_system_info.rank_param <= 0)
	{
		DEF_LOG_ERROR("failed to check the rank param <%d>\n", m_system_info.rank_param);
		return -1;
	}

	if (m_system_info.energy_reward_time.size() <= 0)
	{
		DEF_LOG_ERROR("failed to check energy reward time size zero\n");
		return -1;
	}

	if (m_system_info.pvp_past.size() != m_system_info.pvp_past_num.size())
	{
		DEF_LOG_ERROR("m_system_info.pvp_past.size() != m_system_info.pvp_past_num.size()!");
		return -1;
	}
	return result;
}
