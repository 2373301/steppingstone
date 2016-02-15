
#include "ManageBonusRes.h"
#include "Logger.h"

bool ManageBonusRes::loadContent(Document * xml_doc)
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

const BonusResInfoMap_t & ManageBonusRes::getBonusInfoMap()
{
	return m_bonus_info_map;
}

bool ManageBonusRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	BonusResInfo * info = new BonusResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "produce", info->produce) && result;
	result = getAttrValue(element, "divide", info->divide) && result;
	result = getAttrValue(element, "need_bags", info->need_pos_num) && result;

	string attr_item_name;
	string attr_num_name;
	string attr_probability_name;
	BonusItemResInfo bonus_item_res;
	Attribute * attr = NULL;
	for (int i = 1; i <= 10; ++i)
	{
		attr_item_name = "item" + boost::lexical_cast<string>(i);
		result = getAttrValue(element, attr_item_name, bonus_item_res.item_id) && result;
		if ((1 == info->type_value) && (0 == bonus_item_res.item_id))
		{
			continue;
		}

		attr_probability_name = "probability" + boost::lexical_cast<string>(i);
		result = getAttrValue(element, attr_probability_name, bonus_item_res.probability) && result;

		if ((0 == bonus_item_res.item_id) && (0 == bonus_item_res.probability))
		{
			continue;
		}

		attr_num_name = "num" + boost::lexical_cast<string>(i);
		result = getAttrValue(element, attr_num_name, bonus_item_res.item_no) && result;

		if (bonus_item_res.probability < TEN_THOUSAND_PERCENT_BASE)
		{
			info->total_probability += bonus_item_res.probability;
			bonus_item_res.curr_probability = info->total_probability;
		}

		info->bonus_item_vec.push_back(bonus_item_res);
	}

	BonusResInfoMap_t::iterator it = m_bonus_info_map.find(info->id);
	if (it != m_bonus_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Bonus id : <%d>\n", info->id);
		return false;
	}

	m_bonus_info_map.insert(std::make_pair(info->id, info));

	return result;
}