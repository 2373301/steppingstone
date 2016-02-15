
#include "ManageFormRes.h"
#include "Logger.h"

ManageFormRes::~ManageFormRes()
{
	cleanMapSecondContent(m_form_res_info_map);
}

bool ManageFormRes::loadFormContent(Document * xml_doc)
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
	Elements form_ele_list = root_ele->get_elements();
	for (Elements::iterator it = form_ele_list.begin(); it != form_ele_list.end(); ++it)
	{
		result = loadFormInfo(*it) && result;
	}

	return result;
}

const FormResInfoMap_t & ManageFormRes::getFormInfoMap()
{
	return m_form_res_info_map;
}

bool ManageFormRes::loadFormInfo(Element * form_element)
{
	if (NULL == form_element)
	{
		return false;
	}

	bool result = true;

	FormResInfo * form_info = new FormResInfo();

	result = getAttrValue(form_element, "id", form_info->form_id) && result;
	result = getAttrValue(form_element, "group", form_info->group) && result;
	result = getAttrValue(form_element, "level", form_info->level) && result;
	
	for (int i = 1; i <= 3; ++i)
	{
		string str_v = "v" + boost::lexical_cast<string>(i);
		string str_p = "p" + boost::lexical_cast<string>(i);
		int32 int_v = 0;
		int32 int_p = 0;
		result = getAttrValue(form_element, str_v, int_v) && result;
		result = getAttrValue(form_element, str_p, int_p) && result;

		if (int_p > 0)
		{
			form_info->attr_increase.push_back(std::make_pair(int_p, int_v));
		}
	}

	result = getAttrValue(form_element, "slotunlock", form_info->slot) && result;

	FormResInfoMap_t::iterator it = m_form_res_info_map.find(form_info->form_id);
	if (it != m_form_res_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate form id : <%d>\n", form_info->form_id);
		delete form_info;
		return false;
	}

	m_form_res_info_map.insert(std::make_pair(form_info->form_id, form_info));

	return result;
}
