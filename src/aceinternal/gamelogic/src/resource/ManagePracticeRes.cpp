
#include "ManagePracticeRes.h"
#include "Logger.h"

bool ManagePracticeRes::loadWineTank(Document * xml_doc)
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
		result = loadWineTankInfo(*it) && result;
	}

	return result;
}

bool ManagePracticeRes::loadTankUpgrade(Document * xml_doc)
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

	Elements ele_list = root_ele->get_elements();
	Elements::iterator it = ele_list.begin();
	if( it == ele_list.end())
		return false;
	
	if(NULL == *it)
		return false;

	bool result = true;
	Uint32Vec_t buf;
	result = getAttrValue(*it, "expup", buf) && result;
	m_wine_tank_upgrade_res_info.exp_up = buf;

	buf.clear();
	result = getAttrValue(*it, "need_vip", buf) && result;
	m_wine_tank_upgrade_res_info.need_vip = buf;

	buf.clear();
	result = getAttrValue(*it, "goldblessme", buf) && result;
	m_wine_tank_upgrade_res_info.up_goldblessme = buf;

	return result;
}

bool ManagePracticeRes::loadGourd(Document * xml_doc)
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

	Elements ele_list = root_ele->get_elements();
	Elements::iterator it = ele_list.begin();
	if( it == ele_list.end())
		return false;

	if(NULL == *it)
		return false;

	bool result = true;
	result = getAttrValue(*it, "gourdnum", m_gourd_res_info.gourd_num) && result;
	result = getAttrValue(*it, "renew", m_gourd_res_info.renew) && result;
	result = getAttrValue(*it, "a", m_gourd_res_info.a) && result;
	result = getAttrValue(*it, "b", m_gourd_res_info.b) && result;

	return result;
}


bool ManagePracticeRes::loadStomach(Document * xml_doc)
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

	Elements ele_list = root_ele->get_elements();
	Elements::iterator it = ele_list.begin();
	if( it == ele_list.end())
		return false;

	if(NULL == *it)
		return false;

	bool result = true;
	result = getAttrValue(*it, "bellynum", m_stomach_res_info.belly_num) && result;
	result = getAttrValue(*it, "renew_time", m_stomach_res_info.renew_time) && result;
	result = getAttrValue(*it, "renew_coin", m_stomach_res_info.renew_coin) && result;
	result = getAttrValue(*it, "expend", m_stomach_res_info.expand) && result;

	return result;
}


bool ManagePracticeRes::loadWineIncrease(Document * xml_doc)
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

	Elements ele_list = root_ele->get_elements();
	Elements::iterator it = ele_list.begin();
	if( it == ele_list.end())
		return false;

	if(NULL == *it)
		return false;

	bool result = true;
	result = getAttrValue(*it, "volume", m_wine_increase_res_info.volume) && result;
	result = getAttrValue(*it, "renew", m_wine_increase_res_info.renew) && result;
	result = getAttrValue(*it, "a", m_wine_increase_res_info.a) && result;
	result = getAttrValue(*it, "b", m_wine_increase_res_info.b) && result;
	result = getAttrValue(*it, "goldblessme_renew", m_wine_increase_res_info.goldblessme_renew) && result;

	return result;
}



bool ManagePracticeRes::loadWineTankInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	WineTankResInfo * info = new WineTankResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "vip_goldblessme", info->vip_gold_param) && result;
	result = getAttrValue(element, "unlock", info->unlock) && result;

	WineTankResInfoMap_t::iterator it = m_wine_tank_res_map.find(info->id);
	if (it != m_wine_tank_res_map.end())
	{
		DEF_LOG_ERROR("failed to load vip, get reduplicate wine tank function id <%d>\n", info->id);
		return false;
	}

	m_wine_tank_res_map.insert(std::make_pair(info->id, info));

	return result;
}

const WineTankResInfoMap_t & ManagePracticeRes::getWineTankResInfoMap()
{
	return m_wine_tank_res_map;
}

const GourdResInfo & ManagePracticeRes::getGourdResInfo()
{
	return m_gourd_res_info;
}

const StomachResInfo & ManagePracticeRes::getStomachResInfo()
{
	return m_stomach_res_info;
}

const WineIncreaseResInfo & ManagePracticeRes::getWineIncreaseResInfo()
{
	return m_wine_increase_res_info;
}

const WineTankUpgradeResInfo & ManagePracticeRes::getWineTankUpgradeResInfo()
{
	return m_wine_tank_upgrade_res_info;
}


