
#include <ace/Time_Value.h>
#include <ace/OS.h>
#include "PacketSerializeAssistant.h"
#include "Logger.h"

PacketSerializeAssistant::PacketSerializeAssistant()
: m_message_length(1024000)
, m_message_no(30)
, m_packet_producer(NULL)
, m_is_stop(false)
{
}

PacketSerializeAssistant::~PacketSerializeAssistant()
{
	cleanPacketVec(m_packet_vec);

	while(m_message_block_que.size() > 0)
	{
		delete m_message_block_que.front();
		m_message_block_que.pop_front();
	}
}

int PacketSerializeAssistant::init(int thread_no, PacketProducer * packet_producer, int msg_length)
{
	if (NULL == packet_producer)
	{
		return -1;
	}

	m_message_length = msg_length;
	m_packet_producer = packet_producer;

	ACE_Message_Block * msg_block = NULL;
	for (int i = 0; i < m_message_no; ++i)
	{
		msg_block = new ACE_Message_Block(m_message_length);
		m_message_block_que.push_back(msg_block);
	}

	if (this->activate(THR_JOINABLE, thread_no) == -1)
	{
		return -1;
	}

	return 0;
}

int PacketSerializeAssistant::stop()
{
	m_is_stop = true;
	return 0;
}

int PacketSerializeAssistant::svc ()
{
	REPORT_THREAD_INFO();

	PacketVec_t packet_vec;
	uint32	msg_index = 0;
	bool	is_process = false;

	ACE_Time_Value sleep_time(0, 1000);

	while (!m_is_stop)
	{
		packet_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, -1);
			std::copy(m_packet_vec.begin(), m_packet_vec.end(), std::back_inserter(packet_vec));
			m_packet_vec.clear();

			if (packet_vec.size() > 0)
			{
				if (packet_vec.size() > 10000)
				{
					DEF_LOG_ERROR("getting packet vec size larger in PacketSerializeAssistant : <%d>\n", packet_vec.size());
				}

				is_process = true;
				msg_index = m_packet_producer->getMessageIndex(1);
			}
			else
			{
				is_process = false;
			}
		}

		if (!is_process)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		serializePacket(msg_index, packet_vec);
	}

	return 0;
}

void PacketSerializeAssistant::handlePacket(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, );
	m_packet_vec.push_back(packet);
}

void PacketSerializeAssistant::returnMsgBlock(ACE_Message_Block * msg_block)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_message_block_que_mutex, );
	if (m_message_block_que.size() < m_message_no)
	{
		m_message_block_que.push_back(msg_block);
	}
	else
	{
		delete msg_block;
	}
}

void PacketSerializeAssistant::serializePacket(uint32 msg_index, PacketVec_t & packet_vec)
{
	MsgBlockQue_t msg_block_que;

	ACE_Message_Block * msg_block = getMessageBlock();

	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;
		if (packet->stream_size() > msg_block->space())
		{
			msg_block_que.push_back(msg_block);
			msg_block = getMessageBlock();			
		}

		msg_block->copy(packet->stream(), packet->stream_size());

		delete packet;
	}

	msg_block_que.push_back(msg_block);
	m_packet_producer->outputBuffer(msg_index, msg_block_que);
}

ACE_Message_Block * PacketSerializeAssistant::getMessageBlock()
{
	ACE_Message_Block * msg_block = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_message_block_que_mutex, NULL);
	if (m_message_block_que.size() > 0)
	{
		msg_block = m_message_block_que.front();
		msg_block->rd_ptr(msg_block->base());
		msg_block->wr_ptr(msg_block->base());

		m_message_block_que.pop_front();
	}
	else
	{
		msg_block = new ACE_Message_Block(m_message_length);
	}

	return msg_block;
}