#include "Logger.h"
#include "ManageGvgRes.h"
bool ManageGvgRes::loadContent(Document * xml_doc)
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

const GvgInfo & ManageGvgRes::getGvgInfo()
{
	return m_gvg_res_map;
}

bool ManageGvgRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	
	result = getAttrValue(element,"begin1", m_gvg_res_map.begin1) && result;
	result = getAttrValue(element,"begin2", m_gvg_res_map.begin2) && result;
	result = getAttrValue(element,"begin3", m_gvg_res_map.begin3) && result;
	result = getAttrValue(element,"begin4", m_gvg_res_map.begin4) && result;
	result = getAttrValue(element,"end1", m_gvg_res_map.end1) && result;
	result = getAttrValue(element,"end2", m_gvg_res_map.end2) && result;
	result = getAttrValue(element,"end3", m_gvg_res_map.end3) && result;
	result = getAttrValue(element,"end4", m_gvg_res_map.end4) && result;
	result = getAttrValue(element,"enroll1", m_gvg_res_map.enroll1) && result;
	result = getAttrValue(element,"enroll2", m_gvg_res_map.enroll2) && result;
	result = getAttrValue(element,"exist", m_gvg_res_map.exist) && result;
	result = getAttrValue(element,"kill", m_gvg_res_map.kill) && result;
	result = getAttrValue(element,"level", m_gvg_res_map.level) && result;
	result = getAttrValue(element,"lifenum", m_gvg_res_map.lifenum) && result;
	result = getAttrValue(element,"num1", m_gvg_res_map.num1) && result;
	result = getAttrValue(element,"num2", m_gvg_res_map.num2) && result;
	result = getAttrValue(element,"num3", m_gvg_res_map.num3) && result;
	result = getAttrValue(element,"num4", m_gvg_res_map.num4) && result;
	result = getAttrValue(element,"revivetime", m_gvg_res_map.revivetime) && result;
	result = getAttrValue(element,"rund1", m_gvg_res_map.rund1) && result;
	result = getAttrValue(element,"rund2", m_gvg_res_map.rund2) && result;
	result = getAttrValue(element,"rund3", m_gvg_res_map.rund3) && result;
	result = getAttrValue(element,"rund4", m_gvg_res_map.rund4) && result;
	result = getAttrValue(element,"sceneid", m_gvg_res_map.sceneid) && result;
	result = getAttrValue(element,"cdmin", m_gvg_res_map.cdmin) && result;
	result = getAttrValue(element,"cdmax", m_gvg_res_map.cdmax) && result;
	result = getAttrValue(element,"cdlast", m_gvg_res_map.cdlast) && result;

	return result;
}
