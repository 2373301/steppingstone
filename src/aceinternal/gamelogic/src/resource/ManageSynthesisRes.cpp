
#include "ManageSynthesisRes.h"
#include "Logger.h"

bool ManageSynthesisRes::loadContent(Document * xml_doc)
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

const SynthesisResInfoMap_t & ManageSynthesisRes::getSynthesisInfoMap()
{
	return m_synthesis_info_map;
}

bool ManageSynthesisRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	SynthesisResInfo * info = new SynthesisResInfo();

	result = getAttrValue(element, "synthesis_id", info->id) && result;
	result = getAttrValue(element, "item_id", info->item_id) && result;
	result = getAttrValue(element, "item_get", info->item_get) && result;

	uint32 needitem = 0;
	uint32 num = 0;

	for (int i = 1; i <= 6; ++i)
	{
		result = getAttrValue(element, string("needitem") + boost::lexical_cast<string>(i), needitem) && result;
		result = getAttrValue(element, string("num") + boost::lexical_cast<string>(i), num) && result;
		if (needitem != 0)
		{
			info->needitem.push_back(needitem);
			info->num.push_back(num);
		}
	}

	SynthesisResInfoMap_t::iterator it = m_synthesis_info_map.find(info->id);
	if (it != m_synthesis_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Synthesis id : <%d>\n", info->id);
		return false;
	}

	m_synthesis_info_map.insert(std::make_pair(info->id, info));

	return result;
}