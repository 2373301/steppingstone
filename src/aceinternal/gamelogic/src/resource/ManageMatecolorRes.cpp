#include "Logger.h"
#include "ManageMatecolorRes.h"
bool ManageMatecolorRes::loadContent(Document * xml_doc)
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
const MatecolorInfoMap_t & ManageMatecolorRes::getMatecolorInfo()
{
	return m_matecolor__res_map;
}
bool ManageMatecolorRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MatecolorInfo * info = new MatecolorInfo();
	result = getAttrValue(element,"blue_exp", info->blue_exp) && result;
	result = getAttrValue(element,"green_exp", info->green_exp) && result;
	result = getAttrValue(element,"lv", info->lv) && result;
	result = getAttrValue(element,"orange_exp", info->orange_exp) && result;
	result = getAttrValue(element,"purple_exp", info->purple_exp) && result;
	MatecolorInfoMap_t::iterator it = m_matecolor__res_map.find(info->lv);
	if (it != m_matecolor__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATECOLOR, get reduplicate id <%d>\n",info->lv);
		return false;
	}
	m_matecolor__res_map.insert(std::make_pair(info->lv, info));
	return result;
}
