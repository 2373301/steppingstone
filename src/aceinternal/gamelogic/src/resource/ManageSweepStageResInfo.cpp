
#include "ManageSweepStageResInfo.h"

bool ManageSweepStageResInfo::loadContent(Document * xml_doc)
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

const SweepStageResInfo & ManageSweepStageResInfo::getSweepStageResInfo()
{
	return m_sweep_stage_res_info;
}

bool ManageSweepStageResInfo::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	result = getAttrValue(element, "time", m_sweep_stage_res_info.sweep_interval) && result;

	uint32 tired_interval = 0;
	result = getAttrValue(element, "fatigue", tired_interval) && result;
	m_sweep_stage_res_info.tired_interval = tired_interval * 60;

	uint32 tired_number = 0;
	result = getAttrValue(element, "fatigue_mul", tired_number) && result;
	m_sweep_stage_res_info.total_tired_time = tired_interval * 60 * tired_number;

	return result;
}