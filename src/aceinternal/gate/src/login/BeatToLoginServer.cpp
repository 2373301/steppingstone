
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include "BeatToLoginServer.h"
#include "ManageConfig.h"
#include "opcode.h"
#include "PacketStream.h"

BeatToLoginServer::BeatToLoginServer()
{

}

BeatToLoginServer::~BeatToLoginServer()
{

}

int BeatToLoginServer::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	beatToLogin();
	return 0;
}

void BeatToLoginServer::beatToLogin()
{
//	try
	{
		for (LSCfg::LoginServerList::const_iterator it = sMsgCfg->getLSCfg().login_server_list.begin(); it != sMsgCfg->getLSCfg().login_server_list.end(); ++it)
		{
			ACE_INET_Addr addr(sMsgCfg->getLSCfg().recv_port, it->c_str());
			ACE_SOCK_Dgram dgram;
			dgram.ACE_SOCK::open(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0);

			//PacketStream packet_stream(Gateway_ping, sMsgCfg->getGateID(), "0");
			//dgram.send(packet_stream.stream(), packet_stream.stream_size(), addr);
		}

	}
	//catch (...)
	//{
	//}
}