
#include "Logger.h"
#include "ManageSystemUnlockRes.h"

bool ManageSystemUnlockRes::loadContent(Document * xml_doc)
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

const SystemUnlockRes & ManageSystemUnlockRes::getSystemUnlockRes()
{
	return m_system_unlock_res;
}

bool ManageSystemUnlockRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	uint32 type_value;
	Uint32Vec_t num_vec;
	Uint32Vec_t unlock_vec;
	result = getAttrValue(element, "type", type_value) && result;
	result = getAttrValue(element, "num", num_vec) && result;
	result = getAttrValue(element, "unlock", unlock_vec) && result;

	if (num_vec.size() != unlock_vec.size())
	{
		DEF_LOG_ERROR("the num size is not equal unlock size, num size is <%d>, unlock size is <%d>\n", num_vec.size(), unlock_vec.size());
		return false;
	}

	if (1 == type_value)
	{
		for (int i = 0; i < num_vec.size(); ++i)
		{
			m_system_unlock_res.level_unlock_map[num_vec[i]].push_back(unlock_vec[i]);
			m_system_unlock_res.level_unlock_map2[unlock_vec[i]].push_back(num_vec[i]);
		}
	}
	else if (2 == type_value)
	{
		for (int i = 0; i < num_vec.size(); ++i)
		{
			m_system_unlock_res.stage_unlock_map[num_vec[i] ].push_back(unlock_vec[i]);
		}
	}
	else if (3 == type_value)
	{
		for (int i = 0; i < num_vec.size(); ++i)
		{
			m_system_unlock_res.quest_unlock_map[num_vec[i] ].push_back(unlock_vec[i]);
		}
	}

	return result;
}