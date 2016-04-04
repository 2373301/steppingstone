
#include "ManageMemberSessionOutput.h"
#include "Logger.h"
#include "ManageMemberSession.h"

ManageMemberSessionOutput::ManageMemberSessionOutput()
{

}

ManageMemberSessionOutput::~ManageMemberSessionOutput()
{

}

int ManageMemberSessionOutput::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}
	return 0;
}

int ManageMemberSessionOutput::svc()
{
	ACE_Time_Value sleep_time(0, 1000);
	PacketVec_t	output_packet_vec;
	while (true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_vec_mutex, 1);
			std::copy(m_output_packet_vec.begin(), m_output_packet_vec.end(), std::back_inserter(output_packet_vec));
			m_output_packet_vec.clear();
		}

		if (output_packet_vec.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}
	
		for (PacketVec_t::iterator it = output_packet_vec.begin(); it != output_packet_vec.end(); ++it)
		{
			Packet * packet = *it;
			processPacket(packet);
		}

		output_packet_vec.clear();
	}
	return 0;
}

void ManageMemberSessionOutput::handleOutputPacket(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_vec_mutex, );
	m_output_packet_vec.push_back(packet);
}

void ManageMemberSessionOutput::processPacket(Packet * packet)
{   

    // 处理32位 64位兼容问题...
    uint64 index = 0;
#if defined(WIN32) || defined(_WIN32)
    uint32 temp = (uint32)packet->getOwner();
    index = temp;
#else
    index = (uint64)packet->getOwner();
#endif

	MemberSession * member_session = ManageMemberSession::instance()->getMemberByRoute(index);
	if (NULL != member_session)
	{
		member_session->processWrite(packet);
	}
	else
	{
		DEF_LOG_ERROR("failed to get the member session while output packet, opcode is <%d>, size is <%d>, owner is <%x>\n", packet->opcode(), packet->body_size(), packet->getOwner());
		delete packet;
	}
}
