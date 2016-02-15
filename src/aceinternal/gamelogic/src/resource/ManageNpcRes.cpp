
#include "ManageNpcRes.h"
#include "Logger.h"
#include "share_fun.h"

ManageNpcRes::~ManageNpcRes()
{
	cleanMapSecondContent(m_npc_res_info_map);
}

bool ManageNpcRes::loadNpcContent(Document * xml_doc)
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
	Elements npc_ele_list = root_ele->get_elements();
	for (Elements::iterator it = npc_ele_list.begin(); it != npc_ele_list.end(); ++it)
	{
		result = loadNpcInfo(*it) && result;
	}

	return result;
}

const NpcResInfoMap_t & ManageNpcRes::getNpcInfoMap()
{
	return m_npc_res_info_map;
}

bool ManageNpcRes::loadNpcInfo(Element * npc_element)
{
	if (NULL == npc_element)
	{
		return false;
	}

	bool result = true;

	NpcResInfo * npc_info = new NpcResInfo();

	result = getAttrValue(npc_element, "id", npc_info->id) && result;
	result = getAttrValue(npc_element, "local", npc_info->map_id) && result;
	result = getAttrValue(npc_element, "x", npc_info->x) && result;
	result = getAttrValue(npc_element, "y", npc_info->y) && result;
	int32 function_v = 0;
	Int32Vec_t p_v;
	for (int i = 1; i <= 5; ++i)
	{
		p_v.clear();
		result = getAttrValue(npc_element, string("function") + boost::lexical_cast<string>(i), function_v) && result;
		result = getAttrValue(npc_element, string("p") + boost::lexical_cast<string>(i), p_v) && result;
		npc_info->function_with_p.push_back(PairInt32VectorInt32(function_v, p_v));
	}

	NpcResInfoMap_t::iterator it = m_npc_res_info_map.find(npc_info->id);
	if (it != m_npc_res_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate npc id : <%d>\n", npc_info->id);
		delete npc_info;
		return false;
	}

	m_npc_res_info_map.insert(std::make_pair(npc_info->id, npc_info));

	return result;
}