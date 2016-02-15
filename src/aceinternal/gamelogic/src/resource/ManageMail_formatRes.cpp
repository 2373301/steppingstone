#include "Logger.h"
#include "ManageMail_formatRes.h"
bool ManageMail_formatRes::loadContent(Document * xml_doc)
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
const Mail_formatInfoMap_t & ManageMail_formatRes::getMail_formatInfo()
{
	return m_mail_format__res_map;
}
bool ManageMail_formatRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	Mail_formatInfo * info = new Mail_formatInfo();
	result = getAttrValue(element,"addresser", info->addresser) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"pay_type", info->pay_type) && result;
	result = getAttrValue(element,"pay_value", info->pay_value) && result;
	result = getAttrValue(element,"title", info->title) && result;
	result = getAttrValue(element,"word", info->word) && result;
	Mail_formatInfoMap_t::iterator it = m_mail_format__res_map.find(info->id);
	if (it != m_mail_format__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MAIL_FORMAT, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_mail_format__res_map.insert(std::make_pair(info->id, info));
	return result;
}
