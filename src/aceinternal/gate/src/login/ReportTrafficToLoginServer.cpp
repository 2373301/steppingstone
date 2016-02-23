
#include <ace/SOCK_Dgram.h>
#include "ReportTrafficToLoginServer.h"
#include "GateLogger.h"
#include "ManageConfig.h"
#include "PacketStream.h"
//#include "protocol/msg_login.pb.h"
#include "ManageStat.h"
#include "opcode.h"

ReportTrafficToLoginServer::ReportTrafficToLoginServer()
{
}

ReportTrafficToLoginServer::~ReportTrafficToLoginServer()
{
}

int ReportTrafficToLoginServer::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	reportTraffic();
	return 0;
}

void ReportTrafficToLoginServer::reportTraffic()
{
//	try
	//{
	//	for (LSCfg::LoginServerList::const_iterator it = sMsgCfg->getLSCfg().login_server_list.begin(); it != sMsgCfg->getLSCfg().login_server_list.end(); ++it)
	//	{
	//		ACE_INET_Addr addr(sMsgCfg->getLSCfg().recv_port, it->c_str());
	//		ACE_SOCK_Dgram dgram;
	//		dgram.ACE_SOCK::open(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0);

	//		LoginServer::Gateway_Msg_Update gateway_msg;
	//		gateway_msg.set_connnumber(ManageStat::instance()->getOnLineClientNuber());


	//		PacketStream packet_stream(GATEWAY_MSG_UPDATE, sMsgCfg->getGateID(), gateway_msg.SerializeAsString());
	//		dgram.send(packet_stream.stream(), packet_stream.stream_size(), addr);
	//	}
	//}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ReportTrafficToLoginServer::loadConfig, the last error is <%d>\n"), ACE_OS::last_error());
	//}
}