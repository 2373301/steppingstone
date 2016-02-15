
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include "Logger.h"
#include "ManageExchange.h"
#include "../netstream/inc/Session.h"
#include "protocol/msg_exchange.pb.h"
#include "opcode.h"
#include "LineScene.h"

ManageExchange::ManageExchange()
: m_connect_exchange_status(0)
, m_is_stop(false)
{

}

ManageExchange::~ManageExchange()
{
	cleanPacketQue(m_output_packet_que);
}

int ManageExchange::svc()
{
	netstream::Session * session = NULL;
	ACE_INET_Addr addr(m_exchange_cfg.exchange_addr.c_str());

	int read_result = 0;
	int write_result = 0;
	ACE_Time_Value sleep_time(0, 1000);

	PacketQue_t output_packet_que;

	while (!m_is_stop)
	{
		if (NULL == session)
		{
			session = new netstream::Session();
			ACE_SOCK_Connector connector;
			if (connector.connect(session->peer(), addr) == -1)
			{
				session->peer().close();
				delete session;
				session = NULL;
				m_connect_exchange_status = 2;
				ACE_OS::sleep(10);
				continue;
			}
			else
			{
				m_connect_exchange_status = 1;
				DEF_LOG_INFO("success to connect the exchange server <%s>\n", m_exchange_cfg.exchange_addr.c_str());
				session->open();
				session->setHandleInput(this);

				typed::protocol::smsg_exchange_register se_register;
				se_register.set_line_id(m_exchange_cfg.line_id);
				se_register.set_security_key(m_exchange_cfg.security_key);

				MAKE_NEW_PACKET(ps, SMSG_EXCHANGE_REGISTER, 0, se_register.SerializeAsString());
				output_packet_que.push(ps);
			}
		}

		read_result = session->rd_stream();

		getOutputPacket(output_packet_que);

		while (output_packet_que.size() > 0)
		{
			Packet * packet = output_packet_que.front();
			if (session->output(packet->stream(), packet->stream_size()))
			{
				delete packet;
				output_packet_que.pop();
			}
			else
			{
				break;
			}
		}

		write_result = session->wt_stream();

		if (1 == write_result)
		{
			ACE_OS::sleep(sleep_time);
		}

		if ((-1 == read_result) || (-1 == write_result))
		{
			// socket closed
			DEF_LOG_ERROR("the exchange socket closed\n");
			session->peer().close();
			delete session;
			session = NULL;
			m_connect_exchange_status = 2;

			while (output_packet_que.size() > 0)
			{
				delete output_packet_que.front();
				output_packet_que.pop();
			}
		}
	}

	if (NULL != session)
	{
		delete session;
	}

	return 0;
}

int ManageExchange::init(const ExchangeCfg & exchange_cfg)
{
	m_exchange_cfg = exchange_cfg;
	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active ManageExchange, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	while (0 == m_connect_exchange_status)
	{
		ACE_OS::sleep(1);
	}

	if (1 != m_connect_exchange_status)
	{
		DEF_LOG_ERROR("failed to listen at addr <%s>\n", m_exchange_cfg.exchange_addr.c_str());
		return -1;
	}

	return 0;
}

int ManageExchange::stop()
{
	m_is_stop = true;
	return 0;
}

void ManageExchange::input(Session * session, ACE_Message_Block & msg_block)
{
	PacketVec_t packet_vec;
	parsePacketFromStream(session, msg_block, packet_vec);
	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;

		if (packet->opcode() == EMSG_EXCHANGE_REGISTER)
		{
			typed::protocol::emsg_exchange_register ee_register;
			if (parsePacket(packet, &ee_register))
			{
				if (ee_register.result() == 1)
				{
					DEF_LOG_INFO("success to register exchange, line id is <%d>\n", m_exchange_cfg.line_id);
				}
				else
				{
					DEF_LOG_ERROR("failed to register exchange, line id is <%d>\n", m_exchange_cfg.line_id);
				}
			}
			else
			{
				DEF_LOG_ERROR("failed to parse message EMSG_EXCHANGE_REGISTER, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
			}

			delete packet;
		}
		else
		{
			m_exchange_cfg.line_scene->input(packet);
		}
	}
}

int ManageExchange::handleOutputPacket(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, 0);
	m_output_packet_que.push(packet);
	return 0;
}

void ManageExchange::getOutputPacket(PacketQue_t & output_packet_que)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, );
	while (m_output_packet_que.size() > 0)
	{
		output_packet_que.push(m_output_packet_que.front());
		m_output_packet_que.pop();
	}
}