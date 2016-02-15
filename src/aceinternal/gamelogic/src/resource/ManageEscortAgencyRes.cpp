#include "Logger.h"
#include "ManageEscortAgencyRes.h"
bool ManageEscortAgencyRes::loadContent(Document * xml_doc)
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
const EscortAgencyInfoMap_t & ManageEscortAgencyRes::getEscortAgencyInfo()
{
	return m_escort_agency_res_map;
}
bool ManageEscortAgencyRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	EscortAgencyInfo * info = new EscortAgencyInfo();
	result = getAttrValue(element,"goldblessme_id", info->goldblessme_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"nature", info->nature) && result;
	result = getAttrValue(element,"type", info->_type) && result;
	result = getAttrValue(element,"is_max", info->is_max) && result;
	result = getAttrValue(element,"buff_id", info->buff_id) && result;
	EscortAgencyInfoMap_t::iterator it = m_escort_agency_res_map.find(info->id);
	if (it != m_escort_agency_res_map.end())
	{
		DEF_LOG_ERROR("failed to load ESCORTAGENCY, get reduplicate daily quest function id <%d>\n",info->id);
		return false;
	}
	m_escort_agency_res_map.insert(std::make_pair(info->id, info));
	return result;
}
