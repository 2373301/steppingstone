#include "Logger.h"
#include "ManageOpenactRes.h"
#include <boost/algorithm/string/split.hpp>
#include <ace/Date_Time.h>
#include <boost/foreach.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>

void split_to_uint(std::string &strSrc, std::string spliter, std::vector<uint32> &vecInt)
{
	std::vector<std::string> vecString;

	boost::split(vecString, strSrc, boost::is_any_of(spliter), boost::token_compress_on);


	for (int i = 0; i < vecString.size(); ++i)
	{
		vecInt.push_back(atoi(vecString[i].c_str()));
	}
}

bool ManageOpenactRes::loadContent(Document * xml_doc)
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
const OpenactInfoMap_t & ManageOpenactRes::getOpenactInfo()
{
	return m_OpenAct__res_map;
}
bool ManageOpenactRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	OpenactInfo * info = new OpenactInfo();

	std::string strStartTime, strEndTime;
	std::string strVa, strVb, strRewards;
	result = getAttrValue(element,"end_time", strEndTime) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"rewards", strRewards) && result;
	result = getAttrValue(element,"start_time", strStartTime) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value_a", strVa) && result;
	result = getAttrValue(element,"value_b", strVb) && result;

	std::vector<uint32> vecTime;
	split_to_uint(strStartTime, ";", vecTime);
	if (vecTime.size() != 3)
	{
		DEF_LOG_ERROR("failed to load OPENACT, get error start time id<%d>\n",info->id);
		return false;
	}

	info->startDay = vecTime[0];
	info->start_time = vecTime[1]*24*60+vecTime[2]*60;
	
	vecTime.clear();
	if (strEndTime.compare("0") == 0)
	{ // 
		info->endDay = 0;
		info->end_time = 0;
	}
	else 
	{
		split_to_uint(strEndTime, ";", vecTime);
		if (vecTime.size() != 3)
		{
			DEF_LOG_ERROR("failed to load OPENACT, get error end time id<%d>\n",info->id);
			return false;
		}

		info->endDay = vecTime[0];
		info->end_time = vecTime[1] * 24*60 + vecTime[2]*60;
	
		if (info->end_time+info->endDay*24*60 <= info->startDay*24*60+info->start_time)
		{
			DEF_LOG_ERROR("failed to load OPENACT, entime less then start time id<%d>\n",info->id);
			return false;
		}
	}

	split_to_uint(strVa, ";", info->value_a);
	split_to_uint(strVb, ";", info->value_b);
	split_to_uint(strRewards, ";", info->rewards);
	if (info->value_a.size() != info->value_b.size()
		|| info->value_a.size() != info->rewards.size())
	{
		DEF_LOG_ERROR("failed to load OPENACT, valuea, valueb, rewards not equal id<%d>\n",info->id);
		return false;
	}

	OpenactInfoMap_t::iterator it = m_OpenAct__res_map.find(info->id);
	if (it != m_OpenAct__res_map.end())
	{
		DEF_LOG_ERROR("failed to load OPENACT, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_OpenAct__res_map.insert(std::make_pair(info->id, info));
	return result;
}
