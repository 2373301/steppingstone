
#include <iostream>
#include "ManageConfig.h"

#define CONFIG_NAME "guid_config.xml"

ManageConfig::ManageConfig()
: m_config_doc(NULL)
, m_seed_attr(NULL)
, m_index_attr(NULL)
{

}

ManageConfig::~ManageConfig()
{
	if (NULL != m_config_doc)
	{
		delete m_config_doc;
		m_config_doc = NULL;
	}
}

int ManageConfig::init()
{
	m_config_doc = XMLFactory::create_document();

	if (NULL == m_config_doc)
	{
		std::cout << "failed to create xml document" << std::endl;
		return -1;
	}

	if (loadConfig(CONFIG_NAME))
	{
		return 0;
	}
	else
	{
		std::cout << "failed to load xml document" << std::endl;
		return -1;
	}
}

int ManageConfig::stop()
{
	return 0;
}

int ManageConfig::finit()
{
	return 0;
}

const GuidCfg & ManageConfig::getGuidCfg()
{
	return m_guid_cfg;
}

bool ManageConfig::writeGuidCfg(uint32 seed_value, uint64 index)
{
	if ((NULL == m_config_doc) || (NULL == m_index_attr) || (NULL == m_seed_attr)) 
	{
		return false;
	}

	m_seed_attr->set_string(boost::lexical_cast<string>(seed_value));
	m_index_attr->set_string(boost::lexical_cast<string>(index));
	return m_config_doc->save_file(CONFIG_NAME);
}

bool ManageConfig::loadConfig(const string & cfg_path)
{
	bool result = true;
	if (m_config_doc->load_file(cfg_path))
	{
		Element * root_ele = m_config_doc->get_root();
		if (NULL != root_ele)
		{
			m_seed_attr = root_ele->get_attribute("seed");
			m_index_attr = root_ele->get_attribute("index");
			Attribute * addr_attr = root_ele->get_attribute("addr");

			if (NULL == m_seed_attr)
			{
				return false;
			}

			 if (NULL == m_index_attr)
			 {
				 return false;
			 }

			 if (NULL == addr_attr)
			 {
				 return false;
			 }

			 m_guid_cfg.seed_value = m_seed_attr->as_int();
			 m_guid_cfg.index = boost::lexical_cast<uint64>(m_index_attr->as_cstr());
			 m_guid_cfg.addr = addr_attr->as_cstr();

			 return true;
		}
	}

	return false;
}