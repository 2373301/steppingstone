
#include "CrossServerConnect.h"
#include "protocol/msg_cross.pb.h"
#include "opcode.h"

CrossServerConnect::CrossServerConnect()
: m_cross_server_input(NULL)
{

}

CrossServerConnect::~CrossServerConnect()
{

}

void CrossServerConnect::handlePacket(Packet * packet)
{
	if (NULL != m_cross_server_input)
	{
		m_cross_server_input->crossServerInput(packet);
	}
	else
	{
		delete packet;
	}
}

void CrossServerConnect::newConnection(PacketQue_t & output_packet_que)
{
	typed::protocol::smsg_cross_server_register scs_register;
	scs_register.set_line_id(m_server_cfg.server_id);
	scs_register.set_platform(m_server_cfg.platform);
	scs_register.set_security_key(m_security_key);

	MAKE_NEW_PACKET(ps, SMSG_CROSS_SERVER_REGISTER, 0, scs_register.SerializeAsString());
	output_packet_que.push(ps);
}

void CrossServerConnect::connectionClosed()
{
	
}

void CrossServerConnect::setServerInfo(const ServerCfg & server_cfg, const string & security_key, CrossServerInput * cross_server_input)
{
	m_cross_server_input = cross_server_input;
	m_server_cfg = server_cfg;
	m_security_key = security_key;
}
