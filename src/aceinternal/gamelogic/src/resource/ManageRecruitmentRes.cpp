
#include "ManageRecruitmentRes.h"
#include "Logger.h"

bool ManageRecruitmentRes::loadContent(Document * xml_doc)
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

const RecruitmentResInfoMap_t & ManageRecruitmentRes::getRecruitmentInfoMap()
{
	return m_recruitment_info_map;
}

bool ManageRecruitmentRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	RecruitmentResInfo * info = new RecruitmentResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "lock", info->lock) && result;
	result = getAttrValue(element, "function", info->function_value) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "num", info->num) && result;
	result = getAttrValue(element, "cti", info->cti) && result;
	result = getAttrValue(element, "upgrade1", info->upgrade1) && result;
	result = getAttrValue(element, "upgrade2", info->upgrade2) && result;
	result = getAttrValue(element, "weight", info->weight) && result;
	result = getAttrValue(element, "money", info->money) && result;
	result = getAttrValue(element, "gold", info->gold) && result;

	RecruitmentResInfoMap_t::iterator it = m_recruitment_info_map.find(info->id);
	if (it != m_recruitment_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Recruitment id : <%d>\n", info->id);
		return false;
	}

	m_recruitment_info_map.insert(std::make_pair(info->id, info));

	return result;

}
