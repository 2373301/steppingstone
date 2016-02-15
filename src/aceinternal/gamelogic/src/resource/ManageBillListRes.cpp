#include "Logger.h"
#include "ManageBillListRes.h"
#include <algorithm>
bool ManageBillListRes::loadContent(Document * xml_doc)
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
const BillListInfoMap_t & ManageBillListRes::getBillListInfo()
{
	return m_Bill_List_res_map;
}
bool ManageBillListRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	BillListInfo * info = new BillListInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"end_time", info->end_time) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"max", info->max) && result;
	result = getAttrValue(element,"start_time", info->start_time) && result;
	result = getAttrValue(element,"target_id", info->target_id) && result;
	result = getAttrValue(element,"target_max", info->target_max) && result;
	result = getAttrValue(element,"target_min", info->target_min) && result;

    Uint32Vec_t::size_type min_size = info->target_id.size();
    Uint32Vec_t::size_type max_size = info->target_id.size();

    min_size = std::min(min_size, info->target_min.size());
    max_size = std::max(max_size, info->target_min.size());

    min_size = std::min(min_size, info->target_max.size());
    max_size = std::max(max_size, info->target_max.size());

    min_size = std::min(min_size, info->bonus.size());
    max_size = std::max(max_size, info->bonus.size());

    min_size = std::min(min_size, info->max.size());
    max_size = std::max(max_size, info->max.size());
    if( min_size != max_size)
    {
        DEF_LOG_ERROR("failed to load BILLLIST, <%d> array size is not equal\n",info->id);
        return false;
    }

	BillListInfoMap_t::iterator it = m_Bill_List_res_map.find(info->id);
	if (it != m_Bill_List_res_map.end())
	{
		DEF_LOG_ERROR("failed to load BILLLIST, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Bill_List_res_map.insert(std::make_pair(info->id, info));
	return result;
}
