#include "Logger.h"
#include "ManageArrestControllerRes.h"
bool ManageArrestControllerRes::loadContent(Document * xml_doc)
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
const ArrestControllerInfoVec_t & ManageArrestControllerRes::getArrestControllerInfo()
{
	return m_Arrest_Controller_res_vec;
}
bool ManageArrestControllerRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	ArrestControllerInfo * info = new ArrestControllerInfo();
	result = getAttrValue(element,"goldblessme_id", info->goldblessme_id) && result;
	result = getAttrValue(element,"limit", info->limit) && result;
	result = getAttrValue(element,"sundry", info->sundry) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"vip_id1", info->vip_id1) && result;
	result = getAttrValue(element,"vip_id2", info->vip_id2) && result;
	m_Arrest_Controller_res_vec.push_back(info);
	return result;
}
