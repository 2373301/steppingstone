
#include "Logger.h"
#include "ManageConfig.h"

ManageConfig::ManageConfig()
: m_line_id(0)
, m_telecom_index(0)
, m_united_index(0)
, m_update_interval(60)
{
}

ManageConfig::~ManageConfig()
{
}

int ManageConfig::init(const string & config_file)
{
	if (!loadConfig(config_file))
	{
		DEF_LOG_ERROR("failed to load config <%s>\n", config_file.c_str());
		return -1;
	}
	else
	{
		DEF_LOG_INFO("success to load config\n");
	}
	return 0;
}

const string & ManageConfig::getMemberAddr()
{
	return m_member_addr;
}

const GateAddrVec_t & ManageConfig::getTelecomGateAddrs()
{
	return m_telecom_gate;
}

const GateAddrVec_t & ManageConfig::getUnitedGatesAddrs()
{
	return m_united_gate;
}

uint32 ManageConfig::getLineId()
{
	return m_line_id;
}

const string & ManageConfig::getMemberSecurityKey()
{
	return m_member_security_key;
}

const string ManageConfig::getGateAddrByType(int network_type)
{
	if (1 == network_type)
	{
		return m_client_addr_1;
	}
	else
	{
		return m_client_addr_2;
	}
}

const DbConfig & ManageConfig::getAccountDb()
{
	return m_account_db;
}

const DbConfig & ManageConfig::getGameDb()
{
	return m_game_db;
}

const DbConfig & ManageConfig::getGmlogDb()
{
	return m_gmlog_db;
}

int ManageConfig::getUpdateInterval()
{
	return m_update_interval;
}

bool ManageConfig::loadConfig(const string & config_file)
{
	bool result = false;
	std::auto_ptr<Document> xml_doc(XMLFactory::create_document());
	if (xml_doc->load_file(config_file))
	{
		Element * root_ele = xml_doc->get_root();

		result = true;
		result = loadMemberCfg(root_ele) && result;
		result = loadGateCfg(root_ele) && result;
		result = loadLineCfg(root_ele) && result;
		result = loadMemberSecurityKeyCfg(root_ele) && result;
		result = loadAccountDb(root_ele) && result;
		result = loadGameDb(root_ele) && result;
		result = loadGmlogDb(root_ele) && result;
		result = loadUpdateInterval(root_ele) && result;
	}
	else
	{
		DEF_LOG_ERROR("failed to load config by xml document : %s\n", config_file.c_str());
	}

	return result;
}

bool ManageConfig::loadMemberCfg(Element * root_element)
{
	Element * mem_ele = root_element->get_element("member");
	if (NULL == mem_ele)
	{
		DEF_LOG_ERROR("failed to get member element\n");
		return false;
	}

	Attribute * attr = mem_ele->get_attribute("addr");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get addr of member\n");
		return false;
	}

	m_member_addr = attr->get_value();

	return true;
}

bool ManageConfig::loadGateCfg(Element * root_element)
{
	Element * gate_ele = root_element->get_element("gates");
	if (NULL == gate_ele)
	{
		DEF_LOG_ERROR("the gates element is NULL\n");
		return false;
	}

	Attribute * client1 = gate_ele->get_attribute("client1");
	Attribute * client2 = gate_ele->get_attribute("client2");
	if ((NULL == client1) || (NULL == client2))
	{
		DEF_LOG_ERROR("failed to get client1 or client2 attribute of gates\n");
		return false;
	}

	m_client_addr_1 = client1->get_value();

	m_client_addr_2 = client2->get_value();

	Elements gate_eles = gate_ele->get_elements("gate");

	if (gate_eles.size() == 0)
	{
		DEF_LOG_ERROR("the gate element size is zero\n");
		return false;
	}

	for (Elements::iterator it = gate_eles.begin(); it != gate_eles.end(); ++it)
	{
		Element * gate_ele = *it;
		Attribute * addr_attr = gate_ele->get_attribute("addr");
		if (NULL == addr_attr)
		{
			DEF_LOG_ERROR("failed to get the gate addr or type attribute\n");
			return false;
		}

		GateAddr gate_addr;
		gate_addr.gate_member = addr_attr->get_value();
		gate_addr.gate_client = "";
		m_telecom_gate.push_back(gate_addr);
	}

	return true;
}

bool ManageConfig::loadLineCfg(Element * root_element)
{
	Element * line_ele = root_element->get_element("line");
	if (NULL == line_ele)
	{
		DEF_LOG_ERROR("failed to get line element\n");
		return false;
	}

	Attribute * attr = line_ele->get_attribute("id");

	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get id of line\n");
		return false;
	}

	m_line_id = attr->as_int(0);

	return true;
}

bool ManageConfig::loadMemberSecurityKeyCfg(Element * root_element)
{
	Element * key_ele = root_element->get_element("security_key");
	if (NULL == key_ele)
	{
		DEF_LOG_ERROR("failed to get security key element\n");
		return false;
	}

	Attribute * attr = key_ele->get_attribute("key");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get key of security key element\n");
		return false;
	}

	m_member_security_key = attr->get_value();

	return true;
}

bool ManageConfig::loadAccountDb(Element * root_element)
{
	Element * account_ele = root_element->get_element("account_database");
	if (NULL == account_ele)
	{
		DEF_LOG_ERROR("failed to get account database element\n");
		return false;
	}

	Attributes attrs = account_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "server_name")
		{
			m_account_db.db_name = attr->get_value();
		}
		else if (attr->get_name() == "ip")
		{
			m_account_db.ip = attr->get_value();
		}
		else if (attr->get_name() == "port")
		{
			m_account_db.port = attr->as_int();
		}
		else if (attr->get_name() == "user")
		{
			m_account_db.user_name = attr->get_value();
		}
		else if (attr->get_name() == "password")
		{
			m_account_db.password = attr->get_value();
		}
	}

	return true;
}

bool ManageConfig::loadGameDb(Element * root_element)
{
	Element * game_ele = root_element->get_element("game_database");
	if (NULL == game_ele)
	{
		DEF_LOG_ERROR("failed to get game database element\n");
		return false;
	}

	Attributes attrs = game_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "server_name")
		{
			m_game_db.db_name = attr->get_value();
		}
		else if (attr->get_name() == "ip")
		{
			m_game_db.ip = attr->get_value();
		}
		else if (attr->get_name() == "port")
		{
			m_game_db.port = attr->as_int();
		}
		else if (attr->get_name() == "user")
		{
			m_game_db.user_name = attr->get_value();
		}
		else if (attr->get_name() == "password")
		{
			m_game_db.password = attr->get_value();
		}
	}

	return true;
}

bool ManageConfig::loadGmlogDb(Element * root_element)
{
	Element * game_ele = root_element->get_element("log_database");
	if (NULL == game_ele)
	{
		DEF_LOG_ERROR("failed to get gmlog database element\n");
		return false;
	}

	Attributes attrs = game_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "server_name")
		{
			m_gmlog_db.db_name = attr->get_value();
		}
		else if (attr->get_name() == "ip")
		{
			m_gmlog_db.ip = attr->get_value();
		}
		else if (attr->get_name() == "port")
		{
			m_gmlog_db.port = attr->as_int();
		}
		else if (attr->get_name() == "user")
		{
			m_gmlog_db.user_name = attr->get_value();
		}
		else if (attr->get_name() == "password")
		{
			m_gmlog_db.password = attr->get_value();
		}
	}

	return true;
}

bool ManageConfig::loadUpdateInterval(Element * root_element)
{
	Element * ele = root_element->get_element("update");
	if (NULL != ele)
	{
		Attribute * attr = ele->get_attribute("interval");
		if (NULL != attr)
		{
			m_update_interval = attr->as_int();
		}
	}

	return true;
}

const string ManageConfig::getTelecomGateAddr()
{
	if (m_telecom_gate.size() == 0)
	{
		return "";
	}

	uint32 index = ++m_telecom_index % m_telecom_gate.size();

	return m_telecom_gate[index].gate_client;
}

const string ManageConfig::getUnitedGatesAddr()
{
	if (m_united_gate.size() == 0)
	{
		return "";
	}

	uint32 index = ++m_united_index % m_united_gate.size();

	return m_united_gate[index].gate_client;
}
