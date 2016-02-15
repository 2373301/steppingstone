#include "Logger.h"
#include "ManageMailRes.h"
bool ManageMailRes::loadContent(Document * xml_doc)
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
const MailInfoMap_t & ManageMailRes::getMailInfo()
{
	return m_mail__res_map;
}
bool ManageMailRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MailInfo * info = new MailInfo();
	result = getAttrValue(element,"attachment", info->attachment) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"mail_survival", info->mail_survival) && result;
	result = getAttrValue(element,"mailbox_limit", info->mailbox_limit) && result;
	result = getAttrValue(element,"mass_lilmit", info->mass_lilmit) && result;
	result = getAttrValue(element,"pay_mail", info->pay_mail) && result;
	result = getAttrValue(element,"title_limit", info->title_limit) && result;
	result = getAttrValue(element,"word_limit", info->word_limit) && result;
	MailInfoMap_t::iterator it = m_mail__res_map.find(info->id);
	if (it != m_mail__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MAIL, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_mail__res_map.insert(std::make_pair(info->id, info));
	return result;
}
