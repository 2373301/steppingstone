#include "Logger.h"
#include "ManageGamblingRes.h"
bool ManageGamblingRes::loadContent(Document * xml_doc)
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
const GamblingInfo & ManageGamblingRes::getGamblingInfo()
{
	return m_gambling__res_;
}
bool ManageGamblingRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	result = getAttrValue(element,"hit1", m_gambling__res_.hit1) && result;
	result = getAttrValue(element,"hit16", m_gambling__res_.hit16) && result;
	result = getAttrValue(element,"hit2", m_gambling__res_.hit2) && result;
	result = getAttrValue(element,"hit4", m_gambling__res_.hit4) && result;
	result = getAttrValue(element,"hit8", m_gambling__res_.hit8) && result;
	result = getAttrValue(element,"hitvalue1", m_gambling__res_.hitvalue1) && result;
	result = getAttrValue(element,"hitvalue16", m_gambling__res_.hitvalue16) && result;
	result = getAttrValue(element,"hitvalue2", m_gambling__res_.hitvalue2) && result;
	result = getAttrValue(element,"hitvalue4", m_gambling__res_.hitvalue4) && result;
	result = getAttrValue(element,"hitvalue8", m_gambling__res_.hitvalue8) && result;
	result = getAttrValue(element,"id", m_gambling__res_.id) && result;
	result = getAttrValue(element,"leveleffect", m_gambling__res_.leveleffect) && result;
	result = getAttrValue(element,"leveleffect2", m_gambling__res_.leveleffect2) && result;
	result = getAttrValue(element,"levelrange", m_gambling__res_.levelrange) && result;
	result = getAttrValue(element,"levelrange1", m_gambling__res_.levelrange1) && result;
	result = getAttrValue(element,"losspercent", m_gambling__res_.losspercent) && result;
	result = getAttrValue(element,"losspercentm", m_gambling__res_.losspercentm) && result;
	result = getAttrValue(element,"losspercenty", m_gambling__res_.losspercenty) && result;
	result = getAttrValue(element,"losspercenty2", m_gambling__res_.losspercenty2) && result;
	result = getAttrValue(element,"luckydmoney", m_gambling__res_.luckydmoney) && result;
	result = getAttrValue(element,"luckyeffect", m_gambling__res_.luckyeffect) && result;
	result = getAttrValue(element,"luckylevel", m_gambling__res_.luckylevel) && result;
	result = getAttrValue(element,"maxmoney2", m_gambling__res_.maxmoney2) && result;
	result = getAttrValue(element,"minmoney2", m_gambling__res_.minmoney2) && result;
	result = getAttrValue(element,"money", m_gambling__res_.money) && result;
	result = getAttrValue(element,"opentime", m_gambling__res_.opentime) && result;
	result = getAttrValue(element,"returnvalue", m_gambling__res_.returnvalue) && result;
	result = getAttrValue(element,"returnvalue2", m_gambling__res_.returnvalue2) && result;
	result = getAttrValue(element,"value", m_gambling__res_.value) && result;

	return result;
}
