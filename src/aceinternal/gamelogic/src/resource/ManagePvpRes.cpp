#include "Logger.h"
#include "ManagePvpRes.h"
bool ManagePvpRes::loadContent(Document * xml_doc)
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
const PvpInfo & ManagePvpRes::getPvpInfo()
{
	return m_pvp_res_;
}
bool ManagePvpRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"battlemap1", m_pvp_res_.battlemap1) && result;
	result = getAttrValue(element,"battlemap2", m_pvp_res_.battlemap2) && result;
	result = getAttrValue(element,"battletimes", m_pvp_res_.battletimes) && result;
	result = getAttrValue(element,"defeatedtimes1", m_pvp_res_.defeatedtimes1) && result;
	result = getAttrValue(element,"defeatedtimes2", m_pvp_res_.defeatedtimes2) && result;
	result = getAttrValue(element,"joinlevel", m_pvp_res_.joinlevel) && result;
	result = getAttrValue(element,"jointime1", m_pvp_res_.jointime1) && result;
	result = getAttrValue(element,"jointime2", m_pvp_res_.jointime2) && result;
	result = getAttrValue(element,"part1begintime", m_pvp_res_.part1begintime) && result;
	result = getAttrValue(element,"part1num", m_pvp_res_.part1num) && result;
	//result = getAttrValue(element,"part1resttime", m_pvp_res_.part1resttime) && result;
	result = getAttrValue(element,"part1rundtime", m_pvp_res_.part1rundtime) && result;
	result = getAttrValue(element,"part2begintime1", m_pvp_res_.part2begintime1) && result;
	result = getAttrValue(element,"part2begintime2", m_pvp_res_.part2begintime2) && result;
	result = getAttrValue(element,"part2begintime3", m_pvp_res_.part2begintime3) && result;
	result = getAttrValue(element,"part2begintime4", m_pvp_res_.part2begintime4) && result;
	result = getAttrValue(element,"part2begintime5", m_pvp_res_.part2begintime5) && result;
	result = getAttrValue(element,"part2rundtime", m_pvp_res_.part2rundtime) && result;
	result = getAttrValue(element,"start_time", m_pvp_res_.start_time) && result;
	return result;
}
