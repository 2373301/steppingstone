#include "Logger.h"
#include "ManageMateinheritRes.h"

ManageMateinheritRes::ManageMateinheritRes()
{

}


bool ManageMateinheritRes::loadContent(Document * xml_doc)
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
const MateinheritInfoMap_t & ManageMateinheritRes::getMateinheritInfo()
{
	return m_mateinherit__res_map;
}
bool ManageMateinheritRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MateinheritInfo * info = new MateinheritInfo();
	result = getAttrValue(element,"att_inherit", info->att_inherit) && result;
	result = getAttrValue(element,"exp_inherit", info->exp_inherit) && result;
	result = getAttrValue(element,"id", info->id) && result;
	MateinheritInfoMap_t::iterator it = m_mateinherit__res_map.find(info->id);
	if (it != m_mateinherit__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATEINHERIT, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_mateinherit__res_map.insert(std::make_pair(info->id, info));
	return result;
}
