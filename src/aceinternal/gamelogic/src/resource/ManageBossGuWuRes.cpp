
#include "ManageBossGuWuRes.h"
#include "Logger.h"

bool ManageBossGuWuRes::loadContent(Document * xml_doc)
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

const BossGuWuResInfoMap_t & ManageBossGuWuRes::getBossGuWuInfo()
{
	return m_boss_guwu_info_map;
}

bool ManageBossGuWuRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	BossGuWuResInfo * info = new BossGuWuResInfo();

	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "cost", info->cost) && result;
	result = getAttrValue(element, "rate", info->rate) && result;
	result = getAttrValue(element, "wait_time", info->wait_time) && result;
	result = getAttrValue(element, "wait_timemax", info->wait_timemax) && result;
	result = getAttrValue(element, "buffid", info->buffid) && result;
	result = getAttrValue(element, "effect", info->effect, ";") && result;

	if (info->buffid.size() != 10)
	{
		return false;
	}

	BossGuWuResInfoMap_t::iterator it = m_boss_guwu_info_map.find(info->type);
	if (it != m_boss_guwu_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate boss enemy : <%d>\n", info->type);
		return false;
	}
	m_boss_guwu_info_map.insert(std::make_pair(info->type, info));


	return result;
}