
#include "ManageFlowerResInfo.h"
#include "Logger.h"

bool ManageFlowerResInfo::loadContent(Document * xml_doc)
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

const FlowerResMap_t & ManageFlowerResInfo::getFlowerResInfo()
{
	return m_flower_res_info;
}

bool ManageFlowerResInfo::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	FlowerRes * flower_res = new FlowerRes();

	result = getAttrValue(element, "id", flower_res->id) && result;
	result = getAttrValue(element, "num", flower_res->num) && result;
	result = getAttrValue(element, "value", flower_res->reword_value) && result;
	result = getAttrValue(element, "type", flower_res->type_value) && result;
	result = getAttrValue(element, "pay", flower_res->pay_value) && result;

	FlowerResMap_t::iterator it = m_flower_res_info.find(flower_res->id);
	if (it != m_flower_res_info.end())
	{
		DEF_LOG_ERROR("failed to laod flower res, redumplicate id <%d>\n", flower_res->id);
		return false;
	}

	m_flower_res_info.insert(std::make_pair(flower_res->id, flower_res));

	return result;
}