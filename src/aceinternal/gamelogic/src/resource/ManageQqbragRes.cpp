#include "Logger.h"
#include "ManageQqbragRes.h"
bool ManageQqbragRes::loadContent(Document * xml_doc)
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
const QqbragInfoMap_t & ManageQqbragRes::getQqbragInfo()
{
	return m_QQBrag__res_map;
}
bool ManageQqbragRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	QqbragInfo * info = new QqbragInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"id", info->id) && result;
//	result = getAttrValue(element,"img", info->img) && result;
	result = getAttrValue(element,"type", info->type) && result;
	QqbragInfoMap_t::iterator it = m_QQBrag__res_map.find(info->id);
	if (it != m_QQBrag__res_map.end())
	{
		DEF_LOG_ERROR("failed to load QQBRAG, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_QQBrag__res_map.insert(std::make_pair(info->id, info));
	return result;
}
