
#include "ManageEggRes.h"
#include "Logger.h"

bool ManageEggRes::loadContent(Document * xml_doc)
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

const EggResInfoVec_t & ManageEggRes::getEggInfo()
{
	return m_egg_info_vec;
}

bool ManageEggRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	EggResInfo * info = new EggResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "playerlv", info->playerlv) && result;
	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "a", info->a) && result;
	result = getAttrValue(element, "b", info->b) && result;
	result = getAttrValue(element, "weight", info->weight) && result;

	m_egg_info_vec.push_back(info);

	return result;
}