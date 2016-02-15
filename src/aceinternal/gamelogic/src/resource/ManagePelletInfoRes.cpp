
#include "Logger.h"
#include "ManagePelletInfoRes.h"

bool ManagePelletInfoRes::loadContent(Document * xml_doc)
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

const PelletInfoResMap_t & ManagePelletInfoRes::getPelletInfoResMap()
{
	return m_pellet_info_res_map;
}

bool ManagePelletInfoRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;
	PelletInfoRes *  info  = new PelletInfoRes();
	result = getAttrValue(element,"tyep", info->type_id) && result;
	result = getAttrValue(element,"grade", info->grade) && result;
	result = getAttrValue(element,"needitem", info->need_item) && result;
	result = getAttrValue(element,"needlevel", info->need_level) && result;
	result = getAttrValue(element,"max", info->max_number) && result;
	result = getAttrValue(element,"reward_type", info->reward_property_id) && result;
	result = getAttrValue(element,"reward_value", info->reward_property_value) && result;
	result = getAttrValue(element,"goleblessme_id", info->goldblessme_id) && result;

	uint32 scaler_id = PELLET_MAKE_SCALER_ID(info->type_id, info->grade);

	PelletInfoResMap_t::iterator it = m_pellet_info_res_map.find(scaler_id);
	if (it != m_pellet_info_res_map.end())
	{
		DEF_LOG_ERROR("failed to load pellet, get reduplicate id <%d>\n",scaler_id);
		return false;
	}
	m_pellet_info_res_map.insert(std::make_pair(scaler_id, info));
	return result;
}