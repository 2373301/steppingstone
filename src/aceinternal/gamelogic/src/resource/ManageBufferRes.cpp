
#include "ManageBufferRes.h"
#include "Logger.h"

bool ManageBufferRes::loadBufferContent(Document * xml_doc)
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
	Elements buffer_ele_list = root_ele->get_elements();
	for (Elements::iterator it = buffer_ele_list.begin(); it != buffer_ele_list.end(); ++it)
	{
		result = loadBufferInfo(*it) && result;
	}

	return result;
}

const BufferResInfoMap_t & ManageBufferRes::getBufferInfo()
{
	return m_buffer_info_map;
}

bool ManageBufferRes::loadBufferInfo(Element * buffer_element)
{
	if (NULL == buffer_element)
	{
		return false;
	}

	bool result = true;

	BufferResInfo * buffer_info = new BufferResInfo();

	result = getAttrValue(buffer_element, "id", buffer_info->buffer_id) && result;
	result = getAttrValue(buffer_element, "type", buffer_info->type) && result;
	result = getAttrValue(buffer_element, "add", buffer_info->pile_type) && result;
	result = getAttrValue(buffer_element, "round", buffer_info->pile_limit) && result;
	result = getAttrValue(buffer_element, "value1", buffer_info->pile_percent) && result;
	result = getAttrValue(buffer_element, "value2", buffer_info->pile_value) && result;
	result = getAttrValue(buffer_element, "group", buffer_info->group) && result;
	result = getAttrValue(buffer_element, "level", buffer_info->level) && result;
	result = getAttrValue(buffer_element, "confirm", buffer_info->confirm) && result;
	result = getAttrValue(buffer_element, "others", buffer_info->buffer_feature) && result;
	result = getAttrValue(buffer_element, "round", buffer_info->round) && result;
	result = getAttrValue(buffer_element, "target", buffer_info->target) && result;
	//result = getAttrValue(buffer_element, "trigger1", buffer_info->normal_trigger_skill) && result;
	//result = getAttrValue(buffer_element, "trigger2", buffer_info->unusual_trigger_skill) && result;
	//result = getAttrValue(buffer_element, "damagedisplay", buffer_info->damage_display) && result;

	Attribute * attr = NULL;
	string fun_name;
	string rate_name;
	string fa_name;
	string fb_name;
	string fc_name;
	string fd_name;
	string fe_name;

	for (int i = 1; i <= 5; ++i)
	{
		fun_name = "function" + boost::lexical_cast<string>(i);
		attr = buffer_element->get_attribute(fun_name);
		if (NULL == attr)
		{
			continue;
		}

		if (strlen(attr->as_cstr()) == 0)
		{
			continue;
		}

		if (string("0") == attr->as_cstr() )
		{
			continue;
		}

		rate_name = "rate" + boost::lexical_cast<string>(i);
		fa_name = "fa" + boost::lexical_cast<string>(i);
		fb_name = "fb" + boost::lexical_cast<string>(i);
		fc_name = "fc" + boost::lexical_cast<string>(i);
		fd_name = "fd" + boost::lexical_cast<string>(i);
		fe_name = "fe" + boost::lexical_cast<string>(i);

		BufferFunResInfo buffer_fun_info;

		buffer_fun_info.fun_id = attr->as_int();
		result = getAttrValue(buffer_element, rate_name, buffer_fun_info.rate) && result;
		result = getAttrValue(buffer_element, fa_name, buffer_fun_info.a) && result;
		result = getAttrValue(buffer_element, fb_name, buffer_fun_info.b) && result;
		result = getAttrValue(buffer_element, fc_name, buffer_fun_info.c) && result;
		result = getAttrValue(buffer_element, fd_name, buffer_fun_info.d) && result;
		result = getAttrValue(buffer_element, fe_name, buffer_fun_info.e) && result;

		if ((7 == buffer_fun_info.fun_id) || (10 == buffer_fun_info.fun_id))
		{
			buffer_info->buffer_fun_vec.insert(buffer_info->buffer_fun_vec.begin(), buffer_fun_info);
			//buffer_info->buffer_fun_vec.push_back(buffer_fun_info);
		}
		else
		{
			buffer_info->buffer_fun_vec.push_back(buffer_fun_info);
		}
	}

	BufferResInfoMap_t::iterator it = m_buffer_info_map.find(buffer_info->buffer_id);
	if (it != m_buffer_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate buffer id : <%d>\n", buffer_info->buffer_id);
		delete buffer_info;
		return false;
	}

	m_buffer_info_map.insert(std::make_pair(buffer_info->buffer_id, buffer_info));

	return result;
}