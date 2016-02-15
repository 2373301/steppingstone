#include "Logger.h"
#include "ManageQqCfgRes.h"
bool ManageQqCfgRes::loadContent(Document * xml_doc)
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
const QqCfgInfoMap_t & ManageQqCfgRes::getQqCfgInfo()
{
	return m_QQ_Cfg_res_map;
}
bool ManageQqCfgRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	QqCfgInfo * info = new QqCfgInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"diamond", info->diamond) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"p", info->p) && result;
	result = getAttrValue(element,"spe_bonus", info->spe_bonus) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
	QqCfgInfoMap_t::iterator it = m_QQ_Cfg_res_map.find(info->id);
	if (it != m_QQ_Cfg_res_map.end())
	{
		DEF_LOG_ERROR("failed to load QQCFG, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_QQ_Cfg_res_map.insert(std::make_pair(info->id, info));
	return result;
}
