
#include "ManageStage1stRes.h"
#include "Logger.h"

bool ManageStage1stRes::loadContent(Document * xml_doc)
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

const Stage1stResInfoMap_t & ManageStage1stRes::getStage1stInfoMap()
{
	return m_stage1st_info_map;
}

bool ManageStage1stRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	Stage1stResInfo * info = new Stage1stResInfo();

	result = getAttrValue(element, "ordinate", info->ordinate) && result;
	result = getAttrValue(element, "sequence_id", info->stage_id) && result;
	result = getAttrValue(element, "type", info->type_value) && result;
	result = getAttrValue(element, "subtype", info->sub_type) && result;
	result = getAttrValue(element, "unlock1", info->unlock1) && result;
	result = getAttrValue(element, "unlock2", info->unlock2) && result;
	result = getAttrValue(element, "unlock3", info->unlock3) && result;
	result = getAttrValue(element, "unlock4", info->unlock4) && result;
	result = getAttrValue(element, "levelcap", info->level_cap) && result;
	result = getAttrValue(element, "times", info->times) && result;
	result = getAttrValue(element, "functionid", info->function_id) && result;
	result = getAttrValue(element, "score", info->score) && result;
	result = getAttrValue(element, "energy", info->energy) && result;
	result = getAttrValue(element, "level_limit", info->level_limit) && result;
	result = getAttrValue(element, "raids_unlock", info->raids_unlock) && result;

	Stage1stResInfoMap_t::iterator it = m_stage1st_info_map.find(info->stage_id);
	if (it != m_stage1st_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Stage1st id : <%d>\n", info->stage_id);
		return false;
	}

	m_stage1st_info_map.insert(std::make_pair(info->stage_id, info));

	return result;
}