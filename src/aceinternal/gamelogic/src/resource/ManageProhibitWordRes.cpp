
#include "ManageProhibitWordRes.h"
#include "Logger.h"

bool ManageProhibitWordRes::loadContent(Document * xml_doc)
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
	result = loadInfoChar(root_ele) && result;
	result = loadInfoName(root_ele) && result;

	return result;
}

const ProhibitWordResVec_t & ManageProhibitWordRes::getProhibitWordInfoMap()
{
	return m_prohibit_char_info_map;
}

const ProhibitWordResVec_t & ManageProhibitWordRes::getProhibitNameInfoMap()
{
	return m_prohibit_name_info_map;
}

bool ManageProhibitWordRes::loadInfoChar(Element * map_element)
{
	Element * chat_ele = map_element->get_element("Chat");
	if (NULL == chat_ele)
	{
		DEF_LOG_ERROR("failed to get Chat element\n");
		return false;
	}

	Elements eles = chat_ele->get_elements();
	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		Element * ele = *it;
		m_prohibit_char_info_map.push_back(ele->get_text());
	}

	return true;
}

bool ManageProhibitWordRes::loadInfoName(Element * map_element)
{
	Element * name_ele = map_element->get_element("Account");
	if (NULL == name_ele)
	{
		DEF_LOG_ERROR("failed to get Account element\n");
		return false;
	}

	Elements eles = name_ele->get_elements();
	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		Element * ele = *it;
		m_prohibit_name_info_map.push_back(ele->get_text());
	}

	return true;
}