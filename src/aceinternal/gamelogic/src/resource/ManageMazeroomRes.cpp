#include "Logger.h"
#include "ManageMazeroomRes.h"
bool ManageMazeroomRes::loadContent(Document * xml_doc)
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
const MazeroomInfoVec_t & ManageMazeroomRes::getMazeroomInfo()
{
	return m_Mazeroom__res_vec;
}
bool ManageMazeroomRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MazeroomInfo * info = new MazeroomInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"isallopen", info->isallopen) && result;
	result = getAttrValue(element,"mapid", info->mapid) && result;
	result = getAttrValue(element,"maxtrip", info->maxtrip) && result;
	result = getAttrValue(element,"mazeid", info->mazeid) && result;
	result = getAttrValue(element,"opentype", info->opentype) && result;
	result = getAttrValue(element,"tripcannotseemax", info->tripcannotseemax) && result;
	result = getAttrValue(element,"tripcannotseemin", info->tripcannotseemin) && result;
	result = getAttrValue(element,"tripcanseemax", info->tripcanseemax) && result;
	result = getAttrValue(element,"tripcanseemin", info->tripcanseemin) && result;
	result = getAttrValue(element,"unlock", info->unlock) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;

	m_Mazeroom__res_vec.push_back(info);
	return result;
}
