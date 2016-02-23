
#include <memory>
#include "GateLogger.h"
#include "ManageConfig.h"
#include "ManageProfessionConfig.h"
#include "ManageTranscriptCondition.h"
//#include "EncryptionFactory.h"

ManageConfig::ManageConfig()
: m_gate_id(0)
{
}

ManageConfig::~ManageConfig()
{
}

int ManageConfig::init(int argc, ACE_TCHAR * argv[])
{
	if (loadConfig("gate_config.xml"))
	{
		reportParam();
		//if (!ManageTranscriptCondition::instance()->init(m_transcript_condition_file))
		//{
		//	return -1;
		//}
		return 0;
		//if (ManageProfessionConfig::instance()->init() >= 0)
		//{
		//	return 0;
		//}
		//else
		//{
		//	GATELog(LM_ERROR, ACE_TEXT("Failed to init ManageProfessionConfig::init\n"));
		//	return -1;
		//}
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to load config file.\n"));
		return -1;
	}
}

int ManageConfig::fini()
{
	return 0;
}

int ManageConfig::wait()
{
	return 0;
}

const SQLCfg & ManageConfig::getSQLCfg() const
{
	return m_sql_cfg;
}

const MapInfo & ManageConfig::getMapInfo() const
{
	return m_map_info;
}

const ClientCfg & ManageConfig::getClientCfg() const
{
	return m_client_cfg;
}

const GSCfg & ManageConfig::getGSCfg() const
{
	return m_gs_cfg;
}

const LSCfg & ManageConfig::getLSCfg() const
{
	return m_ls_cfg;
}

//const GUIDCfg & ManageConfig::getGUIDCfg() const
//{
//	//return m_guid_cfg;
//}

const StatCfg & ManageConfig::getStatCfg() const
{
	return m_stat_cfg;
}

const RoleCfg & ManageConfig::getRoleCfg() const
{
	return m_role_cfg;
}

const LogCfg & ManageConfig::getLogCfg() const
{
	return m_log_cfg;
}

const RemoteLogCfg & ManageConfig::getRemoteLogCfg() const
{
	return m_remote_log_cfg;
}

const string & ManageConfig::getQueryAddr() const
{
	return m_query_addr;
}

int64 ManageConfig::getGateID()
{
	return m_gate_id;
}

bool ManageConfig::loadConfig(const string & config_file)
{
	/// todo
//	return true;

	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call XMLFactory::create_document()\n"));
		return false;
	}

//	try
	{
		if (auto_doc->load_file(config_file))
		{
			result = true;
			Element * root_ele = auto_doc->get_root();
			Attribute * attr_id = root_ele->get_attribute("id");
			if (NULL != attr_id)
			{
				m_gate_id = attr_id->as_int(0);
			}
			else
			{
				GATE_LOG_ERROR(ACE_TEXT("Failed to get gate id\n"));
				result = false;
			}

	//		result = loadSQLCfg(root_ele) && result;
			//result = loadMapInfo(root_ele) && result;
			result = loadClientCfg(root_ele) && result;
			result = loadGSCfg(root_ele) && result;
			result = loadLSCfg(root_ele) && result;
			//result = loadGUIDCfg(root_ele) && result;
			result = loadStatCfg(root_ele) && result;
			result = loadRoleCfg(root_ele) && result;
			result = loadQueryCfg(root_ele) && result;
			//result = loadTranscriptConditionFile(root_ele) && result;
			//result = loadLogCfg(root_ele) && result;
			//result = loadRemoteLogCfg(root_ele) && result;
		}
		else
		{
			result = false;
			GATE_LOG_ERROR(ACE_TEXT("Failed to parse file : <%s>\n"), config_file.c_str());
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageConfig::loadConfig, the last error is <%d>\n"), ACE_OS::last_error());
	//	result = false;
	//}
	return result;
}

bool ManageConfig::loadSQLCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}
	Element * sql_ele = root_element->get_element("sql");
	if (NULL == sql_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get sql element\n"));
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

bool ManageConfig::loadMapInfo(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}
	Element * map_ele = root_element->get_element("map");
	if (NULL == map_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get sql element\n"));
		return false;
	}
	Attributes attrs = map_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "mapInstance")
		{
			m_map_info.map_instance = attr->as_cstr();
		}
		else if (attr->get_name() == "mapID")
		{
			m_map_info.map_id = attr->as_int();
		}
		else if (attr->get_name() == "bornX")
		{
			m_map_info.born_x = attr->as_int();
		}
		else if (attr->get_name() == "bornY")
		{
			m_map_info.born_y = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadClientCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * client_ele = root_element->get_element("client");
	if (NULL == client_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get client element\n"));
		return false;
	}

	Attributes attrs = client_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "port")
		{
			m_client_cfg.listen_port = attr->as_int();
		}
		else if (attr->get_name() == "handleInputStreamThreadPoolNumber")
		{
			m_client_cfg.handle_input_stream_thread_pool_number = attr->as_int();
		}
		else if (attr->get_name() == "handlePacketThreadPoolNumber")
		{
			m_client_cfg.handle_packet_thread_pool_number = attr->as_int();
		}
		else if (attr->get_name() == "beatInterval")
		{
			m_client_cfg.beat_interval = attr->as_int();
		}
		else if (attr->get_name() == "lostBeatTime")
		{
			m_client_cfg.lost_beat_time = attr->as_int();
		}
		else if (attr->get_name() == "beatCheckInterval")
		{
			m_client_cfg.beat_check_interval = attr->as_int();
		}
		else if (attr->get_name() == "clientQueueLength")
		{
			m_client_cfg.client_queue_length = attr->as_int();
		}
		else if (attr->get_name() == "maxSocket")
		{
			m_client_cfg.max_socket = attr->as_int();
		}
		else if (attr->get_name() == "socketBufferLength")
		{
			m_client_cfg.socket_buffer_length = attr->as_int();
		}
		else if (attr->get_name() == "host")
		{
			m_client_cfg.host = attr->as_cstr();
		}
		else if (attr->get_name() == "clientConnectionTimeout")
		{
			m_client_cfg.client_connection_timeout = attr->as_int();
		}
		else if (attr->get_name() == "recvBufferLength")
		{
			m_client_cfg.recv_buffer_length = attr->as_int();
		}
		else if (attr->get_name() == "robotPressTest")
		{
			m_client_cfg.robot_press_test = attr->as_int() != 0;
		}
		else if (attr->get_name() == "coolerTimeInterval")
		{
			m_client_cfg.cooler_time_interval = attr->as_int();
		}
		else if (attr->get_name() == "streamLimitSpeed")
		{
			m_client_cfg.stream_limit_spead = attr->as_int();
		}
		else if (attr->get_name() == "streamStatInterval")
		{
			m_client_cfg.stream_stat_interval = attr->as_int();
		}
		else if (attr->get_name() == "encryType")
		{
			m_client_cfg.encry_type = attr->as_int();
//			EncryptionFactory::setEncryType(EncryType(m_client_cfg.encry_type));
			//EncryptionFactory::setEncryType(ET_AES);
		}
	}

	return true;
}

bool ManageConfig::loadGSCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}
	Element * gs_ele = root_element->get_element("gs");
	if (NULL == gs_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get GS element\n"));
		return false;
	}
	Attributes attrs = gs_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "port")
		{
			m_gs_cfg.listen_port = attr->as_int();
		}
		else if (attr->get_name() == "handleStreamThreadPoolNumber")
		{
			m_gs_cfg.handle_stream_thread_pool_number = attr->as_int();
		}
		else if (attr->get_name() == "handlePacketThreadPoolNumber")
		{
			m_gs_cfg.handle_packet_thread_pool_number = attr->as_int();
		}
		else if (attr->get_name() == "host")
		{
			m_gs_cfg.host = attr->as_cstr();
		}
		else if (attr->get_name() == "socketBufferLength")
		{
			m_gs_cfg.socket_buffer_length = attr->as_int();
		}
		else if (attr->get_name() == "recvWindowSize")
		{
			m_gs_cfg.recv_window_size = attr->as_int();
		}
		else if (attr->get_name() == "sendWindowSize")
		{
			m_gs_cfg.send_window_size = attr->as_int();
		}
		//else if (attr->get_name() == "srvConfigDir")
		//{
		//	m_gs_cfg.srv_config_dir = attr->as_cstr();
		//}
	}

	return true;
}

bool ManageConfig::loadLSCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * ls_ele = root_element->get_element("ls");
	if (NULL == ls_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get ls element\n"));
		return false;
	}

	Attributes attrs = ls_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "port")
		{
			m_ls_cfg.listen_port = attr->as_int();
		}
		else if (attr->get_name() == "recvPort")
		{
			m_ls_cfg.recv_port = attr->as_int();
		}
		else if (attr->get_name() == "reportStatInfoInterval")
		{
			m_ls_cfg.report_interval = attr->as_int();
		}
		else if (attr->get_name() == "host")
		{
			m_ls_cfg.host = attr->as_cstr();
		}
		else if (attr->get_name() == "validateClient")
		{
			m_ls_cfg.check_client_auth = attr->as_int();
		}
	}

	Elements items = ls_ele->get_elements();
	for (Elements::iterator it = items.begin(); it != items.end(); ++it)
	{
		Element * item = *it;
		Attribute * attr = item->get_attribute("ip");
		if (NULL != attr)
		{
			m_ls_cfg.login_server_list.push_back(attr->as_cstr());
		}
	}

	return true;
}

//bool ManageConfig::loadGUIDCfg(Element * root_element)
//{
//	bool result = false;
//	if (NULL == root_element)
//	{
//		GATELog(LM_ERROR, ACE_TEXT("The root element is NULL\n"));
//		return false;
//	}
//
//	Element * guid_ele = root_element->get_element("guid");
//	if (NULL == guid_ele)
//	{
//		GATELog(LM_ERROR, ACE_TEXT("Failed to get ls element: guid\n"));
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
//					GATELogp(LM_ERROR, ACE_TEXT("Catch bad_lexical_cast exception ,msg is <%s>\n"), e.what());
//				}
//			}
//		}
//		fclose(guid_stream);
//	}
//	else
//	{
//		GATELog(LM_ERROR, ACE_TEXT("Failed to open guid_index.txt faile.\n"));
//	}
//
//	return result;
//}

bool ManageConfig::loadStatCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * stat_ele = root_element->get_element("stat");
	if (NULL == stat_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get stat element\n"));
		return false;
	}

	Attributes attrs = stat_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "reportInterval")
		{
			m_stat_cfg.report_interval = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadRoleCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * role_ele = root_element->get_element("role");
	if (NULL == role_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get role element\n"));
		return false;
	}

	Attributes attrs = role_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "host")
		{
			m_role_cfg.host = attr->as_cstr();
		}
		else if (attr->get_name() == "port")
		{
			m_role_cfg.listen_port = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadTranscriptConditionFile(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * transcript_ele = root_element->get_element("transcript");
	if (NULL == transcript_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get transcript element\n"));
		return false;
	}

	Attributes attrs = transcript_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "condition_file")
		{
			m_transcript_condition_file = attr->as_cstr();
		}
	}

	return true;
}

bool ManageConfig::loadLogCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * log_ele = root_element->get_element("log");
	if (NULL == log_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get log element\n"));
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
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * log_ele = root_element->get_element("remotlog");
	if (NULL == log_ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get remotlog element\n"));
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
				m_remote_log_cfg.log_level = LL_DEBUG;
			}
			else if (attr->get_value() == "LL_INFO")
			{
				m_remote_log_cfg.log_level = LL_INFO;
			}
			else if (attr->get_value() == "LL_WARNING")
			{
				m_remote_log_cfg.log_level = LL_WARNING;
			}
			else if (attr->get_value() == "LL_ERROR")
			{
				m_remote_log_cfg.log_level = LL_ERROR;
			}
			else if (attr->get_value() == "LL_FATAL")
			{
				m_remote_log_cfg.log_level = LL_FATAL;
			}
		}
		else if (attr->get_name() == "addr")
		{
			m_remote_log_cfg.addr = attr->as_cstr();
		}
	}

	return true;
}

bool ManageConfig::loadQueryCfg(Element * root_element)
{
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}

	Element * ele = root_element->get_element("query");
	if (NULL == ele)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get query element\n"));
		return false;
	}

	Attribute * attr = ele->get_attribute("addr");
	if (NULL == attr)
	{
		return false;
	}

	m_query_addr = attr->as_cstr();

	return true;
}

void ManageConfig::reportParam()
{
	GATE_LOG_INFO(ACE_TEXT("***********    sql    ************\n"));
	GATE_LOG_INFO(ACE_TEXT("sql server ip is :               <%s>\n"), m_sql_cfg.server_ip.c_str());
	GATE_LOG_INFO(ACE_TEXT("sql database name is :           <%s>\n"), m_sql_cfg.database_name.c_str());
	GATE_LOG_INFO(ACE_TEXT("sql user name is :               <%s>\n"), m_sql_cfg.user_name.c_str());
	GATE_LOG_INFO(ACE_TEXT("sql password is :                <%s>\n"), m_sql_cfg.password.c_str());
	GATE_LOG_INFO(ACE_TEXT("sql port is :                    <%d>\n"), m_sql_cfg.port);
	GATE_LOG_INFO(ACE_TEXT("sql pool number is:              <%d>\n"), m_sql_cfg.pool_number);
	GATE_LOG_INFO(ACE_TEXT("***********    client    *********\n"));
	GATE_LOG_INFO(ACE_TEXT("listin host is :                 <%s>\n"), m_client_cfg.host.c_str());
	GATE_LOG_INFO(ACE_TEXT("client port is :                 <%d>\n"), m_client_cfg.listen_port);
	GATE_LOG_INFO(ACE_TEXT("client input stream pool is  :   <%d>\n"), m_client_cfg.handle_input_stream_thread_pool_number);
	GATE_LOG_INFO(ACE_TEXT("client handle packet pool is :   <%d>\n"), m_client_cfg.handle_packet_thread_pool_number);
	GATE_LOG_INFO(ACE_TEXT("client beat interval is :        <%d>\n"), m_client_cfg.beat_interval);
	GATE_LOG_INFO(ACE_TEXT("client lost beat time is :       <%d>\n"), m_client_cfg.lost_beat_time);
	GATE_LOG_INFO(ACE_TEXT("client beat check interval is :  <%d>\n"), m_client_cfg.beat_check_interval);
	GATE_LOG_INFO(ACE_TEXT("client queue length is :         <%d>\n"), m_client_cfg.client_queue_length);
	GATE_LOG_INFO(ACE_TEXT("client connection timeout is :   <%d>\n"), m_client_cfg.client_connection_timeout);
	GATE_LOG_INFO(ACE_TEXT("client max socket is :           <%d>\n"), m_client_cfg.max_socket);
//	GATE_LOG_INFO(ACE_TEXT("profession config is :           <%s>\n"), m_client_cfg.profession_config.c_str());

	GATE_LOG_INFO(ACE_TEXT("***********    gs     ************\n"));
	GATE_LOG_INFO(ACE_TEXT("linsten host is :                <%s>\n"), m_gs_cfg.host.c_str());
	GATE_LOG_INFO(ACE_TEXT("gs port is :                     <%d>\n"), m_gs_cfg.listen_port);
	GATE_LOG_INFO(ACE_TEXT("handle stream thread number is : <%d>\n"), m_gs_cfg.handle_stream_thread_pool_number);
	GATE_LOG_INFO(ACE_TEXT("handle packet thread number is : <%d>\n"), m_gs_cfg.handle_packet_thread_pool_number);

	GATE_LOG_INFO(ACE_TEXT("***********    ls    *************\n"));
	GATE_LOG_INFO(ACE_TEXT("ls listen port is :              <%d>\n"), m_ls_cfg.listen_port);
	GATE_LOG_INFO(ACE_TEXT("ls recv port is :                <%d>\n"), m_ls_cfg.recv_port);
	GATE_LOG_INFO(ACE_TEXT("ls report interval is :          <%d>\n"), m_ls_cfg.report_interval);
	for (LSCfg::LoginServerList::iterator it = m_ls_cfg.login_server_list.begin(); it != m_ls_cfg.login_server_list.end(); ++it)
	{
	GATE_LOG_INFO(ACE_TEXT("ls server ip is :                <%s>\n"), it->c_str());
	}

	//GATE_LOG_INFO(ACE_TEXT("***********    guid info    *************\n"));
	//GATE_LOG_INFO(ACE_TEXT("GUID ggen value is :             <%d>\n"), m_guid_cfg.m_ggen_value);
	//GATE_LOG_INFO(ACE_TEXT("GUID index value is :            <%d>\n"), m_guid_cfg.m_index_value);
}
