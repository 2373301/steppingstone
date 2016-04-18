
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <ace/Time_Value.h>
#include "ManageBattleReport.h"
#include "Logger.h"
#include "typedef.h"
#include "protocol/msg_report.pb.h"

#define REPORT_MAX_NUMBER 0xFFFFFFFF

#define MAKE_REPORT_ID(platform_id, line_id, report_index)	(((uint64(platform_id) << 48) & 0xFF00000000000000) | ((uint64(line_id) << 32) & 0x00FF000000000000) | (uint64(report_index) & 0x00000000FFFFFFFF))

string report_id_file_name = "report_index.txt";
uint64 report_id_assign_interval = 10000;


ManageBattleReport::ManageBattleReport()
: m_line_id(0)
, m_report_index(0)
, m_report_assign_index(0)
{

}

ManageBattleReport::~ManageBattleReport()
{

}

int ManageBattleReport::init(const string & remote_addr)
{
	int result = SingleConnection::init(remote_addr);

	if (!loadReportId())
	{
		DEF_LOG_ERROR("failed to load report id\n");
		return -1;
	}

	return result;
}

void ManageBattleReport::handlePacket(Packet * packet)
{
	// do nothing
	delete packet;
}

void ManageBattleReport::setSecurityKey(const ServerCfg & server_cfg, const string & security_key)
{
	m_security_key = security_key;
	m_server_cfg = server_cfg;
	m_line_id = atoi(m_server_cfg.server_id.c_str());
}

uint64 ManageBattleReport::getBattleReportId()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_report_index_mutex, 0);
	if (m_report_index >= m_report_assign_index)
	{
		m_report_assign_index += report_id_assign_interval;
		if (m_report_assign_index >= REPORT_MAX_NUMBER)
		{
			m_report_index = 0;
			m_report_assign_index = m_report_index + report_id_assign_interval;
		}
		saveReportId(m_report_assign_index);
	}

	++m_report_index;

	uint64 result = MAKE_REPORT_ID(m_server_cfg.platform_id, m_line_id, m_report_index);
	
	return result;
}

void ManageBattleReport::ISessionPoolEvent_newConnection(PacketQue_t & output_packet_que)
{
	// todo
	typed::protocol::stor_verification sv;
	sv.set_operator_id(m_server_cfg.platform_id);
	sv.set_line_id(m_line_id);
	sv.set_security_key(m_security_key);
	MAKE_NEW_PACKET(ps, 991, 0, sv.SerializeAsString());
	output_packet_que.push(ps);
}

bool ManageBattleReport::loadReportId()
{
	boost::filesystem::path path(report_id_file_name);
	if (boost::filesystem::exists(path))
	{
		char buf[100] = {0};
		fstream f;
		f.open(report_id_file_name.c_str(), ios_base::in);
		f.getline(buf, 100);
		m_report_index = atoi(buf);
	}
	else
	{
		// it's the first time, create the file
		m_report_index = 0;
		saveReportId(m_report_index);
	}

	m_report_assign_index = m_report_index;

	return true;
}

void ManageBattleReport::saveReportId(uint64 report_index)
{
	string buf = boost::lexical_cast<string>(report_index);
	fstream f;
	f.open(report_id_file_name.c_str(), ios_base::out | ios_base::trunc);
	f.write(buf.c_str(), buf.size());
	f.flush();
	f.close();
}

//int ManageBattleReport::svc()
//{
//	PacketVec_t battle_report_packet;
//
//	ACE_Time_Value sleep_time(0, 10000);
//
//	while (true)
//	{
//		{
//			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battle_report_packet_mutex, 1);
//			std::copy(m_battle_report_packet.begin(), m_battle_report_packet.end(), std::back_inserter(battle_report_packet));
//			m_battle_report_packet.clear();
//		}
//
//		if (battle_report_packet.size() == 0)
//		{
//			ACE_OS::sleep(sleep_time);
//			continue;
//		}
//
//		for (PacketVec_t::iterator it = battle_report_packet.begin(); it != battle_report_packet.end(); ++it)
//		{
//			savePacket(*it);
//			delete *it;
//		}
//
//		battle_report_packet.clear();
//	}
//	return 0;
//}

//int ManageBattleReport::init(int map_id, const string & packet_dir)
//{
//	m_map_id = map_id;
//	m_packet_dir = packet_dir;
//
//	boost::filesystem::path path(m_packet_dir);
//	if (!boost::filesystem::exists(path))
//	{
//		boost::filesystem::create_directory(path);
//	}
//
//	if (this->activate() == -1)
//	{
//		DEF_LOG_ERROR("failed to active manage battle report\n");
//		return -1;
//	}
//
//	return 0;
//}
//
//void ManageBattleReport::handleBattleReport(Packet * packet)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battle_report_packet_mutex, );
//	m_battle_report_packet.push_back(packet);
//}
//
//void ManageBattleReport::savePacket(Packet * packet)
//{
//	++m_save_packet_index;
//
//	char file_name[100] = {0};
//	string guid = boost::lexical_cast<string>(packet->guid());
//	sprintf(file_name, "%s/%d_%d_%d_%s.pks", m_packet_dir.c_str(), m_map_id, m_save_packet_index, packet->opcode(), guid.c_str());
//	std::fstream file_stream;
//	file_stream.open(file_name, ios_base::out | ios_base::app | ios_base::binary);
//	if (packet->body_size() > 0)
//	{
//		file_stream.rdbuf()->sputn(packet->ch_body(), packet->body_size());
//	}
//	file_stream.flush();
//	file_stream.close();
//
//}