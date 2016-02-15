#include "Logger.h"
#include "ManageTalentsystemRes.h"
bool ManageTalentsystemRes::loadContent(Document * xml_doc)
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
const TalentsystemInfo & ManageTalentsystemRes::getTalentsystemInfo()
{
	return m_TalentSystem__res_;
}
bool ManageTalentsystemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"level", m_TalentSystem__res_.level) && result;
	result = getAttrValue(element,"need_color", m_TalentSystem__res_.need_color) && result;
	result = getAttrValue(element,"need_gold1", m_TalentSystem__res_.need_gold1) && result;
	result = getAttrValue(element,"need_gold2", m_TalentSystem__res_.need_gold2) && result;
	result = getAttrValue(element,"need_item1", m_TalentSystem__res_.need_item1) && result;
	result = getAttrValue(element,"need_item2", m_TalentSystem__res_.need_item2) && result;
	result = getAttrValue(element,"unlock_gold", m_TalentSystem__res_.unlock_gold) && result;

	return result;
}
