
#include "Logger.h"
#include "ManageDailyQuestLibraryRes.h"

bool ManageDailyQuestLibraryRes::loadContent(Document * xml_doc)
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

const DailyQuestLibraryInfoVec_t & ManageDailyQuestLibraryRes::getDailyQuestLibraryInfo()
{
	return m_daily_quest_library_res_map;
}

bool ManageDailyQuestLibraryRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	DailyQuestLibraryInfo * info = new DailyQuestLibraryInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "lv", info->lv) && result;
	result = getAttrValue(element, "num", info->num) && result;
	result = getAttrValue(element, "questid", info->vec_quest) && result;

	m_daily_quest_library_res_map.push_back(info);

	return result;
}
