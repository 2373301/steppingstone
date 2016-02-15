#include "Logger.h"
#include "ManageDodgeMiniRes.h"
bool ManageDodgeMiniRes::loadContent(Document * xml_doc)
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
const DodgeMiniInfoMap_t & ManageDodgeMiniRes::getDodgeMiniInfo()
{
	return m_dodge_mini_res_map;
}
bool ManageDodgeMiniRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	DodgeMiniInfo * info = new DodgeMiniInfo();
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"itemfor_id", info->itemfor_id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"needdodge_num", info->needdodge_num) && result;
	result = getAttrValue(element, "max", info->max) && result;
	result = getAttrValue(element, "type", info->_type) && result;
	result = getAttrValue(element, "needlevel", info->need_level) && result;

	uint32 prop = 0; 
	uint32 valu	= 0;
	result = getAttrValue(element,"property_type1", prop) && result;
	result = getAttrValue(element,"value1", valu) && result;
	if (prop != 0)
	{
		info->property_add.push_back(prop);
		info->value_add.push_back(valu);
	}
	result = getAttrValue(element,"property_type2", prop) && result;
	result = getAttrValue(element,"value2", valu) && result;
	if (prop != 0)
	{
		info->property_add.push_back(prop);
		info->value_add.push_back(valu);
	}
	result = getAttrValue(element,"property_type3", prop) && result;
	result = getAttrValue(element,"value3", valu) && result;
	if (prop != 0)
	{
		info->property_add.push_back(prop);
		info->value_add.push_back(valu);
	}
	result = getAttrValue(element,"property_type4", prop) && result;
	result = getAttrValue(element,"value4", valu) && result;
	if (prop != 0)
	{
		info->property_add.push_back(prop);
		info->value_add.push_back(valu);
	}

	DodgeMiniInfoMap_t::iterator it = m_dodge_mini_res_map.find(info->id);
	if (it != m_dodge_mini_res_map.end())
	{
		DEF_LOG_ERROR("failed to load DODGEMINI, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_dodge_mini_res_map.insert(std::make_pair(info->id, info));
	return result;
}
