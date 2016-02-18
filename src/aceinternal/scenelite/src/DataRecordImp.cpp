
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include <ace/Date_Time.h>
#include <boost/filesystem.hpp>
#include "Logger.h"
#include "DataRecordImp.h"
#include "share_fun.h"
#include "protocol/msg_record.pb.h"
#include "opcode.h"

DataRecordImp::DataRecordImp()
: m_line_scene(NULL)
, m_is_stop(false)
, m_connect_exchange_status(0)
, m_session(NULL)
{
}

DataRecordImp::~DataRecordImp()
{
}

int DataRecordImp::svc()
{
	ACE_Time_Value sleep_time(0, 1000);
	
	PacketQue_t packet_que;

	int write_result = 0;

	while (!m_is_stop)
	{
		if (NULL == m_session)
		{
			if (!connectToServer())
			{
				ACE_OS::sleep(1);
				continue;
			}
		}

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_que_mutex, 1);
			while (m_packet_que.size() > 0)
			{
				packet_que.push(m_packet_que.front());
				m_packet_que.pop();
			}
		}

		if (packet_que.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		while (packet_que.size() > 0)
		{
			Packet * ps = packet_que.front();
			if (m_session->output(ps->stream(), ps->stream_size()))
			{
				delete ps;
				packet_que.pop();
			}
			else
			{
				break;
			}

			write_result = m_session->wt_stream();

			if (1 == write_result)
			{
				ACE_OS::sleep(sleep_time);
			}

			if (-1 == write_result)
			{
				// socket closed
				DEF_LOG_ERROR("the data record socket closed\n");
				m_session->peer().close();
				delete m_session;
				m_session = NULL;
				m_connect_exchange_status = 2;
				break;
			}
		}
	}

	return 0;
}

int DataRecordImp::init(Line * line_scene, const string & record_server_addr)
{
	m_line_scene = line_scene;
	m_record_server_addr = record_server_addr;

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active the data record, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	while (0 == m_connect_exchange_status)
	{
		ACE_OS::sleep(1);
	}

	if (1 != m_connect_exchange_status)
	{
		DEF_LOG_ERROR("failed to listen at addr <%s>\n", m_record_server_addr.c_str());
		return -1;
	}

	return 0;
}

int DataRecordImp::stop()
{
	m_is_stop = true;
	return 0;
}

void DataRecordImp::record(int scene_id, int module_id, int src_from, uint64 guid, const string & name, int level, uint64 param_1, uint64 param_2, uint64 param_3, uint64 param_4, uint64 param_5, uint64 param_6, uint64 param_7, uint64 param_8, uint64 param_9, uint64 param_10)
{
	char record_time[30] = {0};
	ACE_Date_Time tv(ACE_OS::gettimeofday());
	sprintf(record_time, "%4d-%02d-%02dT%02d:%02d:%02d", tv.year(), tv.month(), tv.day(), tv.hour(), tv.minute(), tv.second());

	typed::protocol::smsg_data_record sd_record;
	sd_record.set_guid(guid);
	sd_record.set_scene_id(scene_id);
	sd_record.set_module_id(module_id);
	sd_record.set_src_from(src_from);
	sd_record.set_name(name);
	sd_record.set_level(level);
	sd_record.set_param_1(param_1);
	sd_record.set_param_2(param_2);
	sd_record.set_param_3(param_3);
	sd_record.set_param_4(param_4);
	sd_record.set_param_5(param_5);
	sd_record.set_param_6(param_6);
	sd_record.set_param_7(param_7);
	sd_record.set_param_8(param_8);
	sd_record.set_param_9(param_9);
	sd_record.set_param_10(param_10);
	sd_record.set_record_time(record_time);

	MAKE_NEW_PACKET(ps, SMSG_DATA_RECORD, guid, sd_record.SerializeAsString());

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_que_mutex, );
	m_packet_que.push(ps);
}

void DataRecordImp::record(int scene_id, int module_id, int src_from, uint64 guid, uint64 param_1, uint64 param_2, uint64 param_3, uint64 param_4, uint64 param_5, uint64 param_6, uint64 param_7, uint64 param_8, uint64 param_9, uint64 param_10)
{
	PlayerBriefInfo * player_bi = m_line_scene->getPlayerBriefInfo(guid);
	if (NULL != player_bi)
	{
		this->record(scene_id, module_id, src_from, guid, player_bi->short_info->name(), player_bi->short_info->level(), 
			param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10);
	}
	else
	{
		DEF_LOG_ERROR("failed to get player brief info by guid while record, guid <%llu>\n", guid);
	}
}

bool DataRecordImp::connectToServer()
{
	ACE_INET_Addr addr(m_record_server_addr.c_str());
	m_session = new netstream::Session();
	ACE_SOCK_Connector connector;
	if (connector.connect(m_session->peer(), addr) == -1)
	{
		m_session->peer().close();
		delete m_session;
		m_session = NULL;
		m_connect_exchange_status = 2;
		return false;
	}
	else
	{
		m_connect_exchange_status = 1;
		DEF_LOG_INFO("success to connect the data record server <%s>\n", m_record_server_addr.c_str());
		m_session->open();
		m_session->setHandleInput(NULL);
		return true;
	}
}