
#include "ManageRecruitexpRes.h"
#include "Logger.h"

bool ManageRecruitexpRes::loadContent(Document * xml_doc)
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

const RecruitexpResInfoMap_t & ManageRecruitexpRes::getRecruitexpInfoMap()
{
	return m_recruitexp_info_map;
}

bool ManageRecruitexpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	RecruitexpResInfo * info = new RecruitexpResInfo();

	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "exp", info->exp) && result;

	RecruitexpResInfoMap_t::iterator it = m_recruitexp_info_map.find(info->level);
	if (it != m_recruitexp_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Recruitexp level : <%d>\n", info->level);
		return false;
	}

	m_recruitexp_info_map.insert(std::make_pair(info->level, info));

	return result;
}