
#include "ManageLevelExpRes.h"
#include "Logger.h"

bool ManageLevelExpRes::loadContent(Document * xml_doc)
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

const LevelExpResInfoMap_t & ManageLevelExpRes::getLevelExpInfoMap()
{
	return m_level_exp_info_map;
}

bool ManageLevelExpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	LevelExpResInfo * info = new LevelExpResInfo();

	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "exp", info->exp) && result;

	LevelExpResInfoMap_t::iterator it = m_level_exp_info_map.find(info->level);
	if (m_level_exp_info_map.end() != it)
	{
		DEF_LOG_ERROR("get reduplicate roleexp id : <%d>\n", info->level);
		delete info;
		return false;
	}

	m_level_exp_info_map.insert(std::make_pair(info->level, info));

	return result;
}