
#ifndef ROUTE_ROLE_PACKET_HPP
#define ROUTE_ROLE_PACKET_HPP

#include <map>
#include <vector>
#include <ace/Task.h>
#include "typedef.h"
#include "Singleton.h"
#include "PacketStream.h"
#include "AppNotifyInterface.h"
#include "RoleSession.h"

class RouteRolePacket : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface, public HandleNetPacket
{
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	RouteRolePacket();

	~RouteRolePacket();
public:
	static RouteRolePacket * instance()
	{
		return Singleton<RouteRolePacket>::instance();
	}
public:
	typedef vector<PacketStream *> PackageStreamList;					///存放接收的客户端的PacketStream列表

	typedef vector<RoleSession *> RoleSessionList;

	virtual int  svc(void);
public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();
public:
	virtual int handlePacket(PacketStream * packet_stream);

public:
	void handleClientPackage(PacketStream * packet_stream);

	void collectRoleSession(RoleSession * role_session);

	void removeRoleSession(RoleSession * role_session);
protected:
	void doHandlePackage(PacketStream * packet_stream);

	void handlePlayerMake(PacketStream * packet_stream);

	void handlePlayerList(PacketStream * packet_stream);

	void setRoleSession();
private:
	bool m_stop;

	bool m_wait;

	RoleSession * m_role_session;

	PackageStreamList m_packet_stream_list;

	ACE_Thread_Mutex m_packet_mutex;

	RoleSessionList m_role_session_list;

	ACE_Thread_Mutex m_role_session_mutex;
};
#endif