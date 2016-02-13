
#include <memory>
#include "Logger.h"
#include "ManageConfig.h"

ManageConfig::ManageConfig()
{

}

ManageConfig::~ManageConfig()
{

}

int ManageConfig::init()
{
	static bool is_loading = false;
	if (is_loading)
	{
		return 0;
	}

	is_loading = true;
	if (loadConfig("convert_config.xml"))
	{
		reportParam();
		return 0;
	}
	else
	{
		// error
		return -1;
	}
}

/// AppNotifyInterface fini, stop
int ManageConfig::finit()
{
	return 0;
}

CacheCfg & ManageConfig::getCacheCfg()
{
	return m_cache_cfg;
}

SQLCfg & ManageConfig::getSQLCfg()
{
	return m_sql_cfg;
}

SerializeCfg & ManageConfig::getSerializeCfg()
{
	return m_serialize_cfg;
}

LogCfg & ManageConfig::getLogCfg()
{
	return m_log_cfg;
}

/// AppNotifyInterface wait for finish
int ManageConfig::stop()
{
	return 0;
}

bool ManageConfig::loadConfig(const string & config_file)
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		// error
		return false;
	}

	try
	{
		if (auto_doc->load_file(config_file))
		{
			result = true;
			Element * root_ele = auto_doc->get_root();

			result = loadSQLCfg(root_ele) && result;
			result = loadSerializeCfg(root_ele) && result;
			result = loadCacheCfg(root_ele) && result;
			result = loadLogCfg(root_ele) && result;
			result = loadRemoteLogCfg(root_ele) && result;
		}
		else
		{
			result = false;
			
		}
	}
	catch (...)
	{
		
		result = false;
	}
	
	return result;
}

bool ManageConfig::loadSQLCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		
		return false;
	}

	Element * sql_ele = root_element->get_element("sql");
	if (NULL == sql_ele)
	{
		
		return false;
	}
	Attributes attrs = sql_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "serverIP")
		{
			m_sql_cfg.server_ip = attr->as_cstr();
		}
		else if (attr->get_name() == "databaseName")
		{
			m_sql_cfg.database_name = attr->as_cstr();
		}
		else if (attr->get_name() == "userName")
		{
			m_sql_cfg.user_name = attr->as_cstr();
		}
		else if (attr->get_name() == "password")
		{
			m_sql_cfg.password = attr->as_cstr();
		}
		else if (attr->get_name() == "port")
		{
			m_sql_cfg.port = attr->as_int();
		}
		else if (attr->get_name() == "connPoolNumber")
		{
			m_sql_cfg.pool_number = attr->as_int();
		}
		else if (attr->get_name() == "pingMYSQLInterval")
		{
			m_sql_cfg.ping_MYSQL_interval = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadSerializeCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		return false;
	}
	Element * serialize_ele = root_element->get_element("serialize");
	if (NULL == serialize_ele)
	{
		return false;
	}
	Attributes attrs = serialize_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "backup_dir")
		{
			m_serialize_cfg.backup_directory = attr->as_cstr();
		}
	}

	return true;
}

bool ManageConfig::loadCacheCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		return false;
	}

	Element * caches_ele = root_element->get_element("caches");
	if (NULL == caches_ele)
	{
		return false;
	}

	Elements cache_list = caches_ele->get_elements();
	for (Elements::iterator it = cache_list.begin(); it != cache_list.end(); ++it)
	{
		Element * cache_ele = *it;
		Attributes attrs = cache_ele->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "addr")
			{
				m_cache_cfg.cache_vec.push_back(attr->get_value());
			}
		}

	}

	return true;
}

bool ManageConfig::loadLogCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		return false;
	}

	Element * log_ele = root_element->get_element("log");
	if (NULL == log_ele)
	{
		return false;
	}

	Attributes attrs = log_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "level")
		{
			if (attr->get_value() == "LL_DEBUG")
			{
				m_log_cfg.log_level = LL_DEBUG;
			}
			else if (attr->get_value() == "LL_INFO")
			{
				m_log_cfg.log_level = LL_INFO;
			}
			else if (attr->get_value() == "LL_WARNING")
			{
				m_log_cfg.log_level = LL_WARNING;
			}
			else if (attr->get_value() == "LL_ERROR")
			{
				m_log_cfg.log_level = LL_ERROR;
			}
			else if (attr->get_value() == "LL_FATAL")
			{
				m_log_cfg.log_level = LL_FATAL;
			}
		}
		else if (attr->get_name() == "print")
		{
			m_log_cfg.is_print = attr->as_bool();
		}
		else if (attr->get_name() == "enable_remote")
		{
			m_log_cfg.enable_remote = attr->as_bool();
		}
	}

	return true;
}

bool ManageConfig::loadRemoteLogCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		return false;
	}

	Element * log_ele = root_element->get_element("remotlog");
	if (NULL == log_ele)
	{
		return false;
	}

	Attributes attrs = log_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "level")
		{
			if (attr->get_value() == "LL_DEBUG")
			{
				m_log_cfg.remote_log_level = LL_DEBUG;
			}
			else if (attr->get_value() == "LL_INFO")
			{
				m_log_cfg.remote_log_level = LL_INFO;
			}
			else if (attr->get_value() == "LL_WARNING")
			{
				m_log_cfg.remote_log_level = LL_WARNING;
			}
			else if (attr->get_value() == "LL_ERROR")
			{
				m_log_cfg.remote_log_level = LL_ERROR;
			}
			else if (attr->get_value() == "LL_FATAL")
			{
				m_log_cfg.remote_log_level = LL_FATAL;
			}
		}
		else if (attr->get_name() == "addr")
		{
			m_log_cfg.remote_addr = attr->as_cstr();
		}
	}

	return true;
}

void ManageConfig::reportParam()
{
	//GATELog(LM_INFO, ACE_TEXT("***********    sql    ************\n"));
	//GATELogp(LM_INFO, ACE_TEXT("sql server ip is :               <%s>\n"), m_sql_cfg.server_ip.c_str());
	//GATELogp(LM_INFO, ACE_TEXT("sql database name is :           <%s>\n"), m_sql_cfg.database_name.c_str());
	//GATELogp(LM_INFO, ACE_TEXT("sql user name is :               <%s>\n"), m_sql_cfg.user_name.c_str());
	//GATELogp(LM_INFO, ACE_TEXT("sql password is :                <%s>\n"), m_sql_cfg.password.c_str());
	//GATELogp(LM_INFO, ACE_TEXT("sql port is :                    <%d>\n"), m_sql_cfg.port);
	//GATELogp(LM_INFO, ACE_TEXT("sql pool number is:              <%d>\n"), m_sql_cfg.pool_number);
	//GATELog(LM_INFO, ACE_TEXT("**********   serialize   *********\n"));
	//GATELogp(LM_INFO, ACE_TEXT("serialize directory is :         <%s>\n"), m_serialize_cfg.directory.c_str());
}