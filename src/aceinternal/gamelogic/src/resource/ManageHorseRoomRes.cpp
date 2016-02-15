#include "Logger.h"
#include "ManageHorseRoomRes.h"
bool ManageHorseRoomRes::loadContent(Document * xml_doc)
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
const HorseRoomInfoMap_t & ManageHorseRoomRes::getHorseRoomInfo()
{
	return m_Horse_Room_res_map;
}
bool ManageHorseRoomRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	HorseRoomInfo * info = new HorseRoomInfo();
	result = getAttrValue(element,"horse_id", info->horse_id) && result;
	result = getAttrValue(element,"ranch_lv", info->ranch_lv) && result;
	HorseRoomInfoMap_t::iterator it = m_Horse_Room_res_map.find(info->horse_id);
	if (it != m_Horse_Room_res_map.end())
	{
		DEF_LOG_ERROR("failed to load HORSEROOM, get reduplicate id <%d>\n",info->horse_id);
		return false;
	}
	m_Horse_Room_res_map.insert(std::make_pair(info->horse_id, info));
	return result;
}
