
#include "ManageFreeTimeBag.h"
#include "FreeTimeBag.h"

ManageFreeTimeBag::~ManageFreeTimeBag()
{
	cleanVectorContent(m_free_time_bag_vec);
}

bool ManageFreeTimeBag::loadContent(Document * xml_doc)
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

	// check the id
	int index = 0;
	for (int i = 0; i < m_free_time_bag_vec.size(); ++i)
	{
		FreeTimeBag * free_time_bag = m_free_time_bag_vec[i];
		++index;
		if (index != free_time_bag->id_value)
		{
			result = false;
			break;
		}
	}

	return result;
}

const FreeTimeBagVec_t & ManageFreeTimeBag::getFreeTimeBagVec()
{
	return m_free_time_bag_vec;
}

bool ManageFreeTimeBag::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	FreeTimeBag * free_time_bag = new FreeTimeBag();

	result = getAttrValue(element, "id", free_time_bag->id_value) && result;
	result = getAttrValue(element, "time", free_time_bag->time_value) && result;

	free_time_bag->time_value *= 60;

	m_free_time_bag_vec.push_back(free_time_bag);

	return result;
}
