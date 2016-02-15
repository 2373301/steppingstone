
#include "ManageBossExpRes.h"
#include "Logger.h"

bool ManageBossExpRes::loadContent(Document * xml_doc)
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

const BossExpResInfoVec_t & ManageBossExpRes::getBossExpInfo()
{
	return m_boss_exp_info_vec;
}

bool ManageBossExpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	BossExpResInfo * info = new BossExpResInfo();

	result = getAttrValue(element, "times", info->times) && result;
	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "hp1", info->hp1) && result;
	result = getAttrValue(element, "bonus1", info->bonus1) && result;
	result = getAttrValue(element, "hp2", info->hp2) && result;
	result = getAttrValue(element, "bonus2", info->bonus2) && result;
	result = getAttrValue(element, "hp3", info->hp3) && result;
	result = getAttrValue(element, "bonus3", info->bonus3) && result;

	m_boss_exp_info_vec.push_back(info);

	return result;
}