#include "ManageTrainingRes.h"
#include "Logger.h"

ManageTrainingRes::~ManageTrainingRes()
{
	cleanMapSecondContent(m_partnersInfoMap);

	BOOST_AUTO(it, qualityMap_.begin());
	for(; it != qualityMap_.end(); ++it)
	{
		if(NULL == it->second)
			continue;

		BOOST_AUTO(iter, it->second->begin());
		for(; iter != it->second->end(); ++iter)
		{
			delete iter->second;
		}

		delete it->second;
	}

}

bool 
ManageTrainingRes::loadTrainingPartnersInfo(Document * xml_doc)
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
		result = loadPartnerInfo(*it) && result;
	}

	return result;
}

bool
ManageTrainingRes::loadPartnerInfo(Element* element)
{
	bool result = true;

	TrainingPartnersInfo * info = new TrainingPartnersInfo();
	
	uint32 hire_id = 0;
	result = getAttrValue(element, "hireid", hire_id) && result;
	result = getAttrValue(element, "levelmax", info->level_max) && result;
	result = getAttrValue(element, "failepro", info->placebo) && result;
	result = getAttrValue(element, "num", info->placebo_num) && result;

	TrainingPartnersInfoMap_t::iterator it = m_partnersInfoMap.find(hire_id);
	if (it != m_partnersInfoMap.end() || false == result)
	{
		DEF_LOG_ERROR("failed to load partners_hire.xml, get reduplicate hire id <%d>\n", hire_id);
		return false;
	}

	m_partnersInfoMap.insert(std::make_pair(hire_id, info));

	return result;
	
}


bool 
ManageTrainingRes::loadTrainingLevelTable(Document * xml_doc)
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
		result = loadLevelInfo(*it) && result;
	}

	return result;
}

bool
ManageTrainingRes::loadLevelInfo(Element* element)
{
	bool result = true;

	TrainingLevelTable * info = new TrainingLevelTable();

	uint32 quality = 0;
	uint32 level = 0;
	result = getAttrValue(element, "level", level) && result;
	result = getAttrValue(element, "quality", quality) && result;

	result = getAttrValue(element, "str", info->phy_v) && result;
	result = getAttrValue(element, "agi", info->agi_v) && result;
	result = getAttrValue(element, "int", info->int_v) && result;
	result = getAttrValue(element, "phy_att", info->phy_att) && result;
	result = getAttrValue(element, "agi_att", info->agi_att) && result;
	result = getAttrValue(element, "int_att", info->int_att) && result;
	result = getAttrValue(element, "phy_def", info->phy_def) && result;
	result = getAttrValue(element, "agi_def", info->agi_def) && result;
	result = getAttrValue(element, "int_def", info->int_def) && result;
	result = getAttrValue(element, "hp", info->hp) && result;
	result = getAttrValue(element, "rate", info->rate) && result;
	result = getAttrValue(element, "drop", info->drop) && result;
	//>>cast->cost不知道会不会变动.
	result = getAttrValue(element, "cast", info->cost) && result;


	result = getAttrValue(element, "group", info->group) && result;
	result = getAttrValue(element, "need_item", info->need_item) && result;
	result = getAttrValue(element, "need_num", info->need_num) && result;
    result = getAttrValue(element, "first_strike", info->first_strike) && result;

	if(!result)
	{
		DEF_LOG_ERROR("failed to load partners.xml, quality id <%d>, level id <%d>\n", quality, level);
		return result;
	}
	
	
	TrainingLevelMap_t* current_level_map = NULL;
	bool current_level_existed = true;
	TrainingQualityMap_t::iterator it = qualityMap_.find(quality);
	if (it == qualityMap_.end())
	{
		current_level_map = new TrainingLevelMap_t;
		current_level_existed = false;
	}
	else
	{
		current_level_map = it->second;	
	}

	TrainingLevelMap_t::iterator level_it = current_level_map->find(level);
	if(level_it != current_level_map->end())
	{
		DEF_LOG_ERROR("failed to load partners.xml, get reduplicate level id, quality id<%d>,level id <%d>\n", quality, level);
		return false;
	}

	current_level_map->insert(std::make_pair(level, info));
	
	//>>没有的情况下才需要插入
	if(!current_level_existed)
		qualityMap_.insert(std::make_pair(quality, current_level_map));

	return result;
}

const TrainingPartnersInfoMap_t& 
ManageTrainingRes::getTrainingPartnersInfoMap()
{
	return m_partnersInfoMap;
}


const TrainingQualityMap_t& 
ManageTrainingRes::getTrainingQualityMap()
{
	return qualityMap_;
}
