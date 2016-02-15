#include "Logger.h"
#include "ManageMazeexplorationRes.h"
bool ManageMazeexplorationRes::loadContent(Document * xml_doc)
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
const MazeexplorationInfo & ManageMazeexplorationRes::getMazeexplorationInfo()
{
	return m_Mazeexploration__res_;
}
bool ManageMazeexplorationRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MazeexplorationInfo * info = &m_Mazeexploration__res_;
	result = getAttrValue(element,"explorationid", info->explorationid) && result;
	result = getAttrValue(element,"explorationvalue", info->explorationvalue) && result;
	result = getAttrValue(element,"maxexploration", info->maxexploration) && result;
	result = getAttrValue(element,"resumetime", info->resumetime) && result;
	result = getAttrValue(element,"resumevalue", info->resumevalue) && result;
	result = getAttrValue(element,"roomclearid", info->roomclearid) && result;

	return true;
}