#include "Logger.h"
#include "ManageStageExpRes.h"
bool ManageStageExpRes::loadContent(Document * xml_doc)
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
const StageExpInfoMap_t & ManageStageExpRes::getStageExpInfo()
{
	return m_Stage_Exp_res_map;
}
bool ManageStageExpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	StageExpInfo * info = new StageExpInfo();
	result = getAttrValue(element,"a", info->a) && result;
	result = getAttrValue(element,"b", info->b) && result;
	result = getAttrValue(element,"exp", info->exp) && result;
	result = getAttrValue(element,"level", info->level) && result;
	StageExpInfoMap_t::iterator it = m_Stage_Exp_res_map.find(info->level);
	if (it != m_Stage_Exp_res_map.end())
	{
		DEF_LOG_ERROR("failed to load STAGEEXP, get reduplicate id <%d>\n",info->level);
		return false;
	}
	m_Stage_Exp_res_map.insert(std::make_pair(info->level, info));
	return result;
}
