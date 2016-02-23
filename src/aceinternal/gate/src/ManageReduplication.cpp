
#include "GateLogger.h"
#include "ManageReduplication.h"

int ManageReduplication::init()
{
	int result = -1;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call XMLFactory::create_document()\n"));
		return result;
	}

	string config_file = "../config/reduplication.xml";
	if (auto_doc->load_file(config_file))
	{
		result = true;
		Element * root_ele = auto_doc->get_root();
		Elements eles = root_ele->get_elements();
		for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
		{
			Element * ele = *it;
			Attributes attrs = ele->get_attributes();
			
			RedulicationInfo * map_info = new RedulicationInfo();

			for (Attributes::iterator sub_it = attrs.begin(); sub_it != attrs.end(); ++sub_it)
			{
				Attribute * attr = *sub_it;
				if (attr->get_name() == "map_id")
				{
					map_info->map_id = attr->as_int();
				}
				else if (attr->get_name() == "target")
				{
					map_info->target_map_id = attr->as_int();
				}
				else if (attr->get_name() == "target_x")
				{
					map_info->src_x = attr->as_int();
				}
				else if (attr->get_name() == "target_y")
				{
					map_info->src_y = attr->as_int();
				}
			}

			m_map_info_map.insert(std::make_pair(map_info->map_id, map_info));
		}

		result = 0;
	}
	else
	{
		result = -1;
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse file : <%s>\n"), config_file.c_str());
	}

	return result;
}

uint32 ManageReduplication::getMapId(uint32 map_id)
{
	MapInfoMap_t::iterator it = m_map_info_map.find(map_id);
	if (it != m_map_info_map.end())
	{
		return it->second->target_map_id;
	}

	return map_id;
}