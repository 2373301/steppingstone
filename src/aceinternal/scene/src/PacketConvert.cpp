
#include <ace/OS.h>
#include "PacketConvert.h"
#include "SceneImp.h"
#include "opcode.h"
#include "share_fun.h"

PacketConvert::PacketConvert(SceneImp * scene_imp)
: m_scene_imp(scene_imp)
, m_is_stop(false)
{}

PacketConvert::~PacketConvert()
{
	cleanVectorContent(m_packet_vec);
}

int PacketConvert::svc()
{
	REPORT_THREAD_INFO();

	Packet * packet = NULL;
	GOOGLE_MESSAGE_TYPE * protobuf_msg = NULL;
	PackInfo pack_info;
	PacketVec_t packet_vec;

	ACE_Time_Value sleep_time(0, 1 * 1000);

	while (!m_is_stop)
	{
		packet_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, -2);
			std::copy(m_packet_vec.begin(), m_packet_vec.end(), std::back_inserter(packet_vec));
			m_packet_vec.clear();
		}

		if (packet_vec.empty())
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
		{
			packet = *it;
			if (packet->opcode() == SMSG_TIMER_OCCUR)
			{
				pack_info.guid = packet->guid();
				pack_info.opcode = packet->opcode();
				pack_info.msg = NULL;
				m_scene_imp->packInput(new PackInfo(pack_info.opcode, pack_info.guid, pack_info.msg));
			}
			else
			{
				protobuf_msg = NULL;
				if (extractPacket(packet, protobuf_msg))
				{
					pack_info.guid = packet->guid();
					pack_info.opcode = packet->opcode();
					pack_info.msg = protobuf_msg;
					PackInfo * pi = new PackInfo(pack_info.opcode, pack_info.guid, pack_info.msg);
					pi->owner = packet->getOwner();
					m_scene_imp->packInput(pi);
				}
				else
				{
					DEF_LOG_ERROR("failed to extract message from packet, opcode is <%d>, guid is <%s>\n", packet->opcode(), boost::lexical_cast<string>(packet->guid()).c_str());
				}
			}

			delete packet;
		}
	}

	return 0;
}

int PacketConvert::stop()
{
	m_is_stop = true;
	return 0;
}

void PacketConvert::processPacket(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, );
	m_packet_vec.push_back(packet);
}
