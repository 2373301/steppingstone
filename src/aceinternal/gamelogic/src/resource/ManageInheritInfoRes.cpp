
#include "ManageInheritInfoRes.h"
#include "Logger.h"

bool ManageInheritInfoRes::loadContent(Document * xml_doc)
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

const InheritInfoRes & ManageInheritInfoRes::getInheritInfoRes()
{
	return m_inherit_info_res;
}

bool ManageInheritInfoRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;

	result = getAttrValue(element,"heritagerate1_free", m_inherit_info_res.free_level_pecent) && result;
	result = getAttrValue(element,"heritagerate2_free", m_inherit_info_res.free_training_pecent) && result;
	result = getAttrValue(element,"heritagerate3_free", m_inherit_info_res.free_edu_pecent) && result;
	result = getAttrValue(element,"heritagerate4_free", m_inherit_info_res.free_pellet_pecent) && result;
	result = getAttrValue(element,"heritagerate1_gold", m_inherit_info_res.coin_level_pecent) && result;
	result = getAttrValue(element,"heritagerate2_gold", m_inherit_info_res.coin_training_pecent) && result;
	result = getAttrValue(element,"heritagerate3_gold", m_inherit_info_res.coin_edu_pecent) && result;
	result = getAttrValue(element,"heritagerate4_gold", m_inherit_info_res.coin_pellet_pecent) && result;

	Uint32Vec_t level_vec;
	Uint32Vec_t money_vec;
	Uint32Vec_t coin_vec;
	result = getAttrValue(element,"level", level_vec) && result;
	result = getAttrValue(element,"money", money_vec) && result;
	result = getAttrValue(element,"goleblessme_id", coin_vec) && result;

	if ((level_vec.size() != money_vec.size())
		|| (level_vec.size() != coin_vec.size()))
	{
		DEF_LOG_ERROR("the inherit size is not equal, level <%d>, money <%d>, goldblessme <%d>\n", level_vec.size(), money_vec.size(), coin_vec.size());
		return false;
	}

	uint32 pre_level = 1;
	uint32 money = 0;
	uint32 coin = 0;
	for (int i = 0; i < level_vec.size(); ++i)
	{
		uint32 next_level = level_vec[i];

		if (0 == i)
		{
			money = money_vec[i];
			coin = coin_vec[i];
			continue;
		}

		for (int j = pre_level; j < next_level; ++j)
		{
			LevelCost * level_cost = new LevelCost();
			level_cost->level = j;
			level_cost->money = money;
			level_cost->goldblessme_id = coin;
			m_inherit_info_res.level_cost_map.insert(std::make_pair(level_cost->level, level_cost));
		}

		pre_level = next_level;
		money = money_vec[i];
		coin = coin_vec[i];
	}

	for (int i = pre_level; i < 200; ++i)
	{
		LevelCost * level_cost = new LevelCost();
		level_cost->level = i;
		level_cost->money = money;
		level_cost->goldblessme_id = coin;
		m_inherit_info_res.level_cost_map.insert(std::make_pair(level_cost->level, level_cost));
	}

	return result;
}
