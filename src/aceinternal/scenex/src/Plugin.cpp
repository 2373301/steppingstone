
#include "Plugin.h"
#include "PlayerFunction.h"

MessageTypeMap IPlugin::m_message_type_map;

bool extractPacket(Packet * packet, MSG_TYPE *& protobuf_msg)
{
	MSG_TYPE * msg = NULL;
	if (IPlugin::getProtobufMsgByOpcode(packet->opcode(), msg))
	{
		if (NULL != msg)
		{
			protobuf_msg = msg->New();
			if (PARSE_PACKET(packet, protobuf_msg))
			{
				return true;
			}
			else
			{
				delete protobuf_msg;
				protobuf_msg = NULL;
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void ParamConfig::append(const string & param_name, const string & param_value)
{
	std::pair<ParamPair_t::iterator, bool> p = m_param_pair.insert(std::make_pair(param_name, param_value));
	if (!p.second)
	{
		DEF_LOG_ERROR("Failed to append param name and value, name is <%s>, value is <%s>\n", param_name.c_str(), param_value.c_str());
	}
}

bool ParamConfig::getStr(const string & param_name, string & param_value)
{
	ParamPair_t::iterator it = m_param_pair.find(param_name);
	if (it != m_param_pair.end())
	{
		param_value = it->second;
		return true;
	}
	else
	{
		return false;
	}

}

bool ParamConfig::getInt(const string & param_name, int & param_value)
{
	string pv;
	if (getStr(param_name, pv))
	{
		try
		{
			param_value = boost::lexical_cast<int>(pv);
			return true;
		}
		catch (...)
		{
		}
	}

	return false;
}

bool ParamConfig::getUint64(const string & param_name, uint64 & param_value)
{
	string pv;
	if (getStr(param_name, pv))
	{
		try
		{
			param_value = boost::lexical_cast<uint64>(pv);
			return true;
		}
		catch (...)
		{
		}
	}

	return false;
}

void ParamConfig::clear()
{
	m_param_pair.clear();
}

IPlugin::IPlugin()
: m_plugin_type(PT_BASE)
, m_plugin_scene_type(PST_BOTH)
, m_startup_success(true)
, m_shutdown_success(true)
{
}

IPlugin::~IPlugin()
{
}

int IPlugin::IPlugin_init(const PluginCfg & plugin_cfg)
{
	m_plugin_cfg = plugin_cfg;

	return IPlugin_initing();
}

PluginType IPlugin::getType()
{
	return m_plugin_type;
}

PluginSceneType IPlugin::getSceneType()
{
	return m_plugin_scene_type;
}

int IPlugin::transferMsgToLineScene(const PackInfo & pack_info)
{
	MSG_TYPE * msg_value = NULL;
	if (NULL != pack_info.msg)
	{
		msg_value = cloneMessage(pack_info.msg);
	}

	PackInfo * pi = new PackInfo(pack_info.opcode, pack_info.guid, msg_value);
	//m_plugin_cfg.line_scene->scenePackInput(pi);
	return 0;
}

int IPlugin::transferMsgToPVPScene(const PackInfo & pack_info)
{
	MSG_TYPE * msg_value = NULL;
	if (NULL != pack_info.msg)
	{
		msg_value = cloneMessage(pack_info.msg);
	}

	PackInfo * pi = new PackInfo(pack_info.opcode, pack_info.guid, msg_value);
	//m_plugin_cfg.line_scene->sendSceneMsg(QUALIFY_MAP_ID, pi);

	return 0;
}

bool IPlugin::IPlugin_isStartupSuccess()
{
	// default 
	return m_startup_success;
}

bool IPlugin::IPlugin_isShutdownSuccess()
{
	return m_shutdown_success;
}

PluginCfg & IPlugin::getPluginCfg()
{
	return m_plugin_cfg;
}

bool IPlugin::getProtobufMsgByOpcode(int op_code, MSG_TYPE *& protobuf_msg)
{
	MessageTypeMap::iterator it = m_message_type_map.find(op_code);
	if (m_message_type_map.end() != it)
	{
		protobuf_msg = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

const Input_Msg_Type_Map & IPlugin::getInputMsgMap()
{
	return m_input_msg_type_map;
}

const Request_Msg_Type_Map & IPlugin::getRequestMsgMap()
{
	return m_request_msg_type_map;
}

const Notify_Msg_Type_Map & IPlugin::getNofityMsgMap()
{
	return m_notify_msg_type_map;
}

const Gmcmd_Msg_Type_Map & IPlugin::getGmcmdMsgMap()
{
	return m_gmcmd_msg_type_map;
}