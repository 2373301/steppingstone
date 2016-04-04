
#ifndef MANAGE_GATE_SESSION_HPP
#define MANAGE_GATE_SESSION_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include "typedef.h"
#include "Singleton.h"
#include "GateSession.h"
#include "Packet.h"
#include "MemberProtocol.h"

struct GateInfo 
{
	GateInfo()
		: gate_session(NULL)
		, is_connected(false)
		, assigned(false)
		, player_num(0)
	{}

	GateSession *	gate_session;
	bool			is_connected;
	bool			assigned;
	int				player_num;
	string			gate_addr;
};

typedef vector<GateInfo *> GateInfoVec_t;

class ManageGateSession : public GateHandleInput, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageGateSession();
	~ManageGateSession();
public:
	static ManageGateSession * instance()
	{
		return Singleton<ManageGateSession>::instance();
	}
public:
	virtual void input(Packet * packet);

	virtual int svc();

	int init();

	void handleOutputStream(Packet * packet);
public:
	void handleMemberLogin(membersys::mmsg_member_login & member_login);

	void handleMemberCharge(membersys::mmsg_member_charge & member_charge);

	void handleMemberGiveGift(membersys::mmsg_member_give_gift & member_give_gift);

protected:
	void handleGateLoginRespond(Packet * packet);

	void handleMemberLoginConfirm(Packet * packet);

	void handleMemberChargeConfirm(Packet * packet);

	void handleMemberGiveGiftPacket(Packet * packet);

	void handleReportOnlineNumber(Packet * packet);

	void handlePlayerLogoff(Packet * packet);
protected:
	bool connectToServer();

	bool connectToServer(GateInfo * gate_info);

	// no data return 1, lost connection return 2
	int processRead(GateSession * gate_session);

	// no data return 1, lost connection return 2
	int processWrite(GateSession * gate_session);

	GateInfo * getGateInfo();

	string getRoleGuid(char * role_guid);
private:
	bool m_is_connected;

	//GateSession * m_gate_session;

	GateInfoVec_t	m_telecom_gate;

	GateInfoVec_t	m_united_gate;

	ACE_Thread_Mutex m_gate_mutex;

	PacketQue_t	m_output_packet_que;

	PacketQue_t	m_copy_output_packet_que;

	ACE_Thread_Mutex	m_output_packet_que_mutex;
};
#endif
