#include "Logger.h"
#include "ManageMazeRes.h"
bool ManageMazeRes::loadContent(Document * xml_doc)
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
const MazeInfoMap_t & ManageMazeRes::getMazeInfo()
{
	return m_Maze__res_map;
}
bool ManageMazeRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MazeInfo * info = new MazeInfo();
//	result = getAttrValue(element,"bosstarget", info->bosstarget) && result;
	result = getAttrValue(element,"entrance", info->entrance) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"item", info->item) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"lowtrip", info->lowtrip) && result;
	result = getAttrValue(element,"maxrewards", info->maxrewards) && result;
	result = getAttrValue(element,"opemtime1", info->opemtime1) && result;
	result = getAttrValue(element,"opemtime2", info->opemtime2) && result;
	result = getAttrValue(element,"opentimes", info->opentimes) && result;
	result = getAttrValue(element,"periodtime", info->periodtime) && result;
	result = getAttrValue(element,"subtype", info->subtype) && result;
	result = getAttrValue(element,"target", info->target) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"goldblessmeid", info->goldblessmeid) && result;
	result = getAttrValue(element,"bossroom", info->bossroom) && result;
	MazeInfoMap_t::iterator it = m_Maze__res_map.find(info->id);
	if (it != m_Maze__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MAZE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Maze__res_map.insert(std::make_pair(info->id, info));
	return result;
}
