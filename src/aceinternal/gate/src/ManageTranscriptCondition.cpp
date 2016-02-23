
#include <ace/Date_Time.h>
#include <boost/lexical_cast.hpp>
#include "ManageTranscriptCondition.h"
#include "GateLogger.h"

ManageTranscriptCondition::ManageTranscriptCondition()
{

}

ManageTranscriptCondition::~ManageTranscriptCondition()
{

}

bool ManageTranscriptCondition::init(const string & transcript_condition_file)
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR("Failed to call XMLFactory::create_document\n");
		return false;
	}

	//try
	{
		if (auto_doc->load_file(transcript_condition_file))
		{
			Element * root_ele = auto_doc->get_root();
			return loadTranscript(root_ele);
		}
		else
		{
			GATE_LOG_ERROR("Failed to parse file : <%s>\n", transcript_condition_file.c_str());
			return false;
		}
	}
	//catch (...)
	//{
	//	return false;
	//}
	return true;
}

TranscriptCondition * ManageTranscriptCondition::getTranscriptCondition(int map_template)
{
	TranscriptConditionMap_t::iterator it = m_transcript_condition_map.find(map_template);
	if (it != m_transcript_condition_map.end())
	{
		return &it->second;
	}
	else
	{
		GATE_LOG_ERROR("failed to get Transcript Condition by map template id <%d>\n", map_template);
		return NULL;
	}
}

bool ManageTranscriptCondition::loadTranscript(Element * root_element)
{
	bool result = true;
	Elements dungeon_ele = root_element->get_elements("dungeon");
	for (Elements::iterator it = dungeon_ele.begin(); it != dungeon_ele.end(); ++it)
	{
		TranscriptCondition trans_condition;
		result = loadOutpoint(*it, trans_condition.out_point) && result;
		Attributes attrs = (*it)->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "id")
			{
				trans_condition.id = attr->as_int();
			}
		}
		m_transcript_condition_map.insert(std::make_pair(trans_condition.id, trans_condition));
	}
	return result;
}

bool ManageTranscriptCondition::loadOutpoint(Element * transcript_element, Outpoint & out_point)
{
	Element * outpoint_ele = transcript_element->get_element("outpoint");
	if (NULL != outpoint_ele)
	{
		Attributes attrs = outpoint_ele->get_attributes();
		for (Attributes::iterator it_attr = attrs.begin(); it_attr != attrs.end(); ++it_attr)
		{
			Attribute * attr = *it_attr;
			if (attr->get_name() == "src_x")
			{
				out_point.x = attr->as_int();
			}
			else if (attr->get_name() == "src_y")
			{
				out_point.y = attr->as_int();
			}
			else if (attr->get_name() == "map_template")
			{
				out_point.map_template = attr->as_int();
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}