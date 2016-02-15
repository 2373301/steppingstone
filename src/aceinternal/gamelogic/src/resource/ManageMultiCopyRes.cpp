
#include "Logger.h"
#include "ManageMultiCopyRes.h"

bool ManageMultiCopyRes::loadContent(Document * xml_doc)
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

const MultiCopyResMap_t & ManageMultiCopyRes::getMultiCopyResMap()
{
	return m_multi_copy_res_map;
}

bool ManageMultiCopyRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	MultiCopyRes * info = new MultiCopyRes();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "unlock", info->unlock) && result;
	result = getAttrValue(element, "times", info->times) && result;
	result = getAttrValue(element, "energy", info->energy) && result;
	result = getAttrValue(element, "bonus1", info->bonus1) && result;
	result = getAttrValue(element, "bonus2", info->bonus2) && result;
	result = getAttrValue(element, "enemy", info->enemy) && result;
	if (info->enemy.size() != 3)
	{
		return false;
	}

	MultiCopyResMap_t::iterator it = m_multi_copy_res_map.find(info->id);
	if (it != m_multi_copy_res_map.end())
	{
		DEF_LOG_ERROR("get reduplicate multicopy id : <%d>\n", info->id);
		return false;
	}

	m_multi_copy_res_map.insert(std::make_pair(info->id, info));

	return result;
}