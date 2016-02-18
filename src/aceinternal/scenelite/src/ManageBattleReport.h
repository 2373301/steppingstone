
#ifndef MANAGE_BATTLE_REPORT_HPP
#define MANAGE_BATTLE_REPORT_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Packet.h"
#include "SingleConnection.h"
#include "Scene.h"

using namespace netstream;

class ManageBattleReport : public SingleConnection
{
public:
	ManageBattleReport();
	~ManageBattleReport();
public:
	virtual int init(const string & remote_addr);

	virtual void handlePacket(Packet * packet);

	void setSecurityKey(const ServerCfg & server_cfg, const string & security_key);

	uint64 getBattleReportId();
protected:
	virtual void newConnection(PacketQue_t & output_packet_que);

	bool loadReportId();

	void saveReportId(uint64 report_index);
private:
	int m_line_id;

	uint64 m_report_index;

	uint64 m_report_assign_index;

	string m_security_key;

	ServerCfg m_server_cfg;

	ACE_Thread_Mutex m_report_index_mutex;
};
#endif