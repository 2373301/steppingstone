
#include <memory>
#include <iostream>
#include "Logger.h"
#include "ManageConfig.h"
#include "CheckRoleName.h"

#define MAKE_GUID(type, ggen, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(ggen) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

#define MAKE_MAP_GUID(line, id) MAKE_GUID(0x0C, 15, ((((uint64)(line) << 40) | uint64(id))))

ManageConfig::ManageConfig()
: m_press_test(0)
, m_line_id(0)
{
}

ManageConfig::~ManageConfig()
{
}

int ManageConfig::init()
{
	if (loadConfig("char_config.xml"))
	{
		CheckRoleName::instance()->init();
		reportParam();
		return 0;
	}
	else
	{
		std::cout << ("Failed to load config file.\n");
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

bool ManageConfig::loadConfig(const string & config_file)
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		std::cout << ("Failed to call XMLFactory::create_document()\n");
		return false;
	}

	try
	{
		if (auto_doc->load_file(config_file))
		{
			result = true;
			Element * root_ele = auto_doc->get_root();

			result = loadAccountSQLCfg(root_ele) && result;
			result = loadGateCfg(root_ele) && result;
			result = loadCacheConfig(root_ele) && result;
			result = loadGUIDAddr(root_ele) && result;
			result = loadUnlawfulInfo(root_ele) && result;
			//result = loadProfession(root_ele) && result;
			result = loadLogsysInfo(root_ele) && result;
			result = loadInitialization(root_ele) && result;
			result = loadLineInfo(root_ele) && result;
		}
		else
		{
			result = false;
			std::cout << "Failed to parse file : " << config_file.c_str() << std::endl;
		}
	}
	catch (...)
	{
		std::cout << "Raise unknown exception in ManageConfig::loadConfig, the last error is " << ACE_OS::last_error();
		result = false;
	}
	auto_doc->clear();
	return result;
}

const CacheConfigVec_t & ManageConfig::getCacheConfig()
{
	return m_cache_cfg;
}

const GateCfgVec_t & ManageConfig::getGateCfg()
{
	return m_gate_cfg;
}

const SQLCfg & ManageConfig::getAccountSQLCfg()
{
	return m_account_sql_cfg;
}

const GUIDCfg & ManageConfig::getGUIDCfg() const
{
	return m_guid_cfg;
}

const string & ManageConfig::getGUIDAddr() const
{
	return m_guid_addr;
}

const string & ManageConfig::getUnfawfulFile() const
{
	return m_unlawful_file;
}

const LogsysCfg & ManageConfig::getLogsysCfg() const
{
	return m_logsys_cfg;
}

const string & ManageConfig::getInitPlayerCfg() const
{
	return m_init_player_file;
}

const string & ManageConfig::getInitJobCfg() const
{
	return m_init_job_file;
}

const string & ManageConfig::getRoleCfg() const
{
	return m_role_file;
}

const string & ManageConfig::getRandomNameCfg() const 
{
	return m_random_name_file;
}

int ManageConfig::getLineId() const
{
	return m_line_id;
}

bool ManageConfig::loadAccountSQLCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << "The root element is NULL\n";
		return false;
	}
	Element * sql_ele = root_element->get_element("account-sql");
	if (NULL == sql_ele)
	{
		std::cout << ("Failed to get sql element\n");
		return false;
	}
	Attributes attrs = sql_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "serverIP")
		{
			m_account_sql_cfg.server_ip = attr->as_cstr();
		}
		else if (attr->get_name() == "databaseName")
		{
			m_account_sql_cfg.database_name = attr->as_cstr();
		}
		else if (attr->get_name() == "userName")
		{
			m_account_sql_cfg.user_name = attr->as_cstr();
		}
		else if (attr->get_name() == "password")
		{
			m_account_sql_cfg.password = attr->as_cstr();
		}
		else if (attr->get_name() == "port")
		{
			m_account_sql_cfg.port = attr->as_int();
		}
		else if (attr->get_name() == "connPoolNumber")
		{
			m_account_sql_cfg.pool_number = attr->as_int();
		}
		else if (attr->get_name() == "pingMYSQLInterval")
		{
			m_account_sql_cfg.ping_MYSQL_interval = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadGateCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}

	Element * gates_ele = root_element->get_element("gates");
	if (NULL == gates_ele)
	{
		std::cout << ("Failed to get gates element\n");
		return false;
	}

	Elements gate_list = gates_ele->get_elements();
	
	for (Elements::iterator gate_it = gate_list.begin(); gate_it != gate_list.end(); ++gate_it)
	{
		GateCfg gate_cfg;
		Attributes attrs = (*gate_it)->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "ip")
			{
				gate_cfg.ip = attr->as_cstr();
			}
			else if (attr->get_name() == "port")
			{
				gate_cfg.port = attr->as_int();
			}
		}
		m_gate_cfg.push_back(gate_cfg);
	}
	return true;
}

bool ManageConfig::loadCacheConfig(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}

	Element * cache_ele = root_element->get_element("hash-cache");
	if (NULL == cache_ele)
	{
		std::cout << ("Failed to get hash-cache element\n");
		return false;
	}

	Elements cache_list = cache_ele->get_elements();

	for (Elements::iterator cache_it = cache_list.begin(); cache_it != cache_list.end(); ++cache_it)
	{
		CacheConfig cache_cfg;
		Attributes attrs = (*cache_it)->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "ip")
			{
				cache_cfg.ip = attr->as_cstr();
			}
		}
		m_cache_cfg.push_back(cache_cfg);
	}
	return true;
}

//bool ManageConfig::loadGenGUID(Element * root_element)
//{
//	bool result = false;
//	if (NULL == root_element)
//	{
//		////TRACELog(LM_ERROR, ACE_TEXT("The root element is NULL\n"));
//		return false;
//	}
//
//	Element * guid_ele = root_element->get_element("guid");
//	if (NULL == guid_ele)
//	{
//		////TRACELog(LM_ERROR, ACE_TEXT("Failed to get ls element: guid\n"));
//		return false;
//	}
//
//	Attributes attrs = guid_ele->get_attributes();
//	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
//	{
//		Attribute * attr = *it;
//		if (attr->get_name() == "ggen")
//		{
//			m_guid_cfg.m_ggen_value = attr->as_int();
//		}
//	}
//
//	FILE * guid_stream = fopen("guid_index.txt", "r");
//	if (guid_stream)
//	{
//		if (fseek(guid_stream, 0, SEEK_SET) == 0)
//		{
//			char buf[30] = {0};
//			int r = fread(buf, 30, 30, guid_stream);
//			if (r >= 0)
//			{
//				try
//				{
//					if ('\n' == buf[strlen(buf) - 1])
//					{
//						buf[strlen(buf) - 1] = '\0';
//					}
//					m_guid_cfg.m_index_value = boost::lexical_cast<uint64>(buf);
//					result = true;
//				}
//				catch(boost::bad_lexical_cast & e)
//				{
//					result = false;
//					//////TRACELogp(LM_ERROR, ACE_TEXT("Catch bad_lexical_cast exception ,msg is <%s>\n"), e.what());
//				}
//			}
//		}
//		fclose(guid_stream);
//	}
//	else
//	{
//		////TRACELog(LM_ERROR, ACE_TEXT("Failed to open guid_index.txt faile.\n"));
//	}
//
//	return result;
//}

bool ManageConfig::loadUnlawfulInfo(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}
	Element * unlawfulWord_ele = root_element->get_element("unlawfulWord");
	if (NULL == unlawfulWord_ele)
	{
		std::cout << ("Failed to get sql element\n");
		return false;
	}
	Attributes attrs = unlawfulWord_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "file")
		{
			m_unlawful_file = attr->as_cstr();
		}
	}

	return true;
}

bool ManageConfig::loadLogsysInfo(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}

	Element * logsys_ele = root_element->get_element("logsys");
	if (NULL == logsys_ele)
	{
		std::cout << ("Failed to get hash-cache element\n");
		return false;
	}

	Attributes attrs = logsys_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "ip")
		{
			m_logsys_cfg.ip = attr->as_cstr();
		}
		else if (attr->get_name() == "port")
		{
			m_logsys_cfg.port = attr->as_cstr();
		}
	}
	return true;
}

bool ManageConfig::loadGUIDAddr(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}
	Element * guid_ele = root_element->get_element("guid");
	if (NULL == guid_ele)
	{
		std::cout << ("Failed to get sql element\n");
		return false;
	}
	Attributes attrs = guid_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "addr")
		{
			m_guid_addr = attr->as_cstr();
		}
	}

	return true;
}

bool ManageConfig::loadInitialization(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}
	Element * init_ele = root_element->get_element("initialization");
	if (NULL == init_ele)
	{
		std::cout << ("failed to get initialization\n");
		return false;
	}

	Attribute * attr = init_ele->get_attribute("player");
	if (NULL == attr)
	{
		return false;
	}

	m_init_player_file = attr->as_cstr();

	attr = init_ele->get_attribute("job");

	if (NULL == attr)
	{
		return false;
	}

	m_init_job_file = attr->as_cstr();


	attr = init_ele->get_attribute("role");

	if (NULL == attr)
	{
		return false;
	}

	m_role_file = attr->as_cstr();


	attr = init_ele->get_attribute("randomName");

	if (NULL == attr)
	{
		return false;
	}

	m_random_name_file = attr->as_cstr();

	return true;
}

bool ManageConfig::loadLineInfo(Element * root_element)
{
	if (NULL == root_element)
	{
		std::cout << ("The root element is NULL\n");
		return false;
	}
	Element * line_ele = root_element->get_element("line");
	if (NULL == line_ele)
	{
		std::cout << ("failed to get line\n");
		return false;
	}

	Attribute * attr = line_ele->get_attribute("line_id");
	if (NULL == attr)
	{
		return false;
	}

	m_line_id = attr->as_int();

	return true;
}

void ManageConfig::reportParam()
{
}