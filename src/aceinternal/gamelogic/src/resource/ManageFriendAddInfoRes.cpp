
#include "ManageFriendAddInfoRes.h"
#include "Logger.h"

bool ManageFriendAddInfoRes::loadContent(Document * xml_doc)
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

	if (m_friend_add_info_res.add_level.size() == 0)
	{
		DEF_LOG_ERROR("the friend add info size is zero\n");
		return false;
	}

	return result;
}

const FriendAddInfoRes & ManageFriendAddInfoRes::getFriendAddInfoRes()
{
	return m_friend_add_info_res;
}

bool ManageFriendAddInfoRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;

	uint32 level = 0;
	result = getAttrValue(element,"lv", level) && result;

	if (level > 0)
	{
		m_friend_add_info_res.add_level.insert(level);
	}

	return result;
}
