
#include "ManagePropertyResInfo.h"
#include "Logger.h"

bool ManagePropertyResInfo::loadPropertyContent(Document * xml_doc)
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
	Elements property_ele_list = root_ele->get_elements();
	for (Elements::iterator it = property_ele_list.begin(); it != property_ele_list.end(); ++it)
	{
		result = loadPropertyInfo(*it) && result;
	}

	return result;
}

const PropertyResInfoMap_t & ManagePropertyResInfo::getPropertyInfoMap()
{
	return m_property_info_map;
}

bool ManagePropertyResInfo::loadPropertyInfo(Element * property_element)
{
	if (NULL == property_element)
	{
		return false;
	}

	bool result = true;

	PropertyResInfo * property_info = new PropertyResInfo();

	result = getAttrValue(property_element, "id", property_info->id) && result;
	result = getAttrValue(property_element, "programid", property_info->program_id) && result;
	result = getAttrValue(property_element, "type", property_info->type) && result;
	result = getAttrValue(property_element, "value", property_info->value) && result;
	result = getAttrValue(property_element, "subtype", property_info->subtype) && result;

	PropertyResInfoMap_t::iterator it = m_property_info_map.find(property_info->id);
	if (m_property_info_map.end() != it)
	{
		DEF_LOG_ERROR("get reduplicate property id : <%d>\n", property_info->id);
		return false;
	}

	m_property_info_map.insert(std::make_pair(property_info->id, property_info));

	return result;
}