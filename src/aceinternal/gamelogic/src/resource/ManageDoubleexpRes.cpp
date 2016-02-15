
#include "Logger.h"
#include "ManageDoubleexpRes.h"

bool ManageDoubleexpRes::loadContent(Document * xml_doc)
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

const DoubleexpResMap_t & ManageDoubleexpRes::getDoubleexpMap()
{
	return m_doubleexp_res_map;
}

bool ManageDoubleexpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	DoubleexpRes * info = new DoubleexpRes();

	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"times", info->times) && result;
	result = getAttrValue(element,"exp_value", info->mutil_value) && result;

	DoubleexpResMap_t::iterator it = m_doubleexp_res_map.find(info->id);
	if (it != m_doubleexp_res_map.end())
	{
		DEF_LOG_ERROR("failed to load buff buff_medicine, get reduplicate id <%d>\n",info->id);
		return false;
	}

	m_doubleexp_res_map.insert(std::make_pair(info->id, info));

	return result;
}