
#ifndef MANAGE_MEMBER_SESSION_HPP
#define MANAGE_MEMBER_SESSION_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include "typedef.h"
#include "Singleton.h"
#include "MemberSession.h"
#include "Packet.h"
#include "MemberProtocol.h"

struct MemberSessionInfo 
{
};

class ManageMemberSession : public MemberHandleInput, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageMemberSession();
	~ManageMemberSession();
public:
	static ManageMemberSession * instance()
	{
		return Singleton<ManageMemberSession>::instance();
	}
public:
	virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act = 0);

public:
	virtual void input(Packet * packet);

	virtual int svc();

	int init();

//	void handleOutputStream(Packet * packet);

	void handleMemberSession(MemberSession * member_session);

	MemberSession * getMemberSession(void * pointer);

	void removeMemberSession(MemberSession * member_session);

	void addMemberRoute(uint64 request_index, MemberSession * member_session);

	MemberSession * getMemberByRoute(uint64 request_index);

	void removeMemberRoute(uint64 request_index);

protected:
	//bool connectToServer();

	// no data return 1, lost connection return 2
	//int processRead(MemberSession * member_session);

	//// no data return 1, lost connection return 2
	//int processWrite(MemberSession * member_session);

//	void handleMemberRegister(Packet * packet);

	void handleMemberLogin(Packet * packet);

	void handleMemberCharge(Packet * packet);

	void handleMemberQueryRole(Packet * packet);
    void handleQueryRegisterUserInfo(Packet * packet);
    void handleQueryDailyTopPlayers(Packet *packet);
    void handleQueryDailyAveragePlayers(Packet *packet);

	void handleMemberQueryOnlineNumber(Packet * packet);

	void handleMemberGiveGift(Packet * packet);

	void sendRegisterRespond(MemberSession * member_session);

	//void makeMemberRegisterPacket();
private:
	typedef map<MemberSession *, MemberSessionInfo>	MemberSessionInfoMap_t;

	ACE_Reactor * m_reactor;

	MemberSessionInfoMap_t m_member_session_info_map;

	ACE_Thread_Mutex m_member_session_info_map_mutex;

	typedef map<uint64, MemberSession *>	MemberSessionRoute_t;

	MemberSessionRoute_t	m_member_session_route;

	ACE_Thread_Mutex m_member_session_route_mutex;


	//PacketQue_t	m_output_packet_que;

	//ACE_Thread_Mutex	m_output_packet_que_mutex;
};
#endif
