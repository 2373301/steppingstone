
#ifndef MANAGE_CACHE_SESSION_HPP
#define MANAGE_CACHE_SESSION_HPP

#include "SessionPoolx.h"
#include "ManageConfig.h"

class ManageCacheSession : public HandleInputx, public netcore::HandleSessionRouterAddRemove, public netcore::HandleSessionOpenClosed
{
public:
	ManageCacheSession();
	~ManageCacheSession();
public:
	int init(const CacheConfigVec_t & cache_cfg);

	void setHandleInput(HandleInputx * handle_input);

public:
	virtual void input(Packet * packet);

	virtual void output(Packet * packet, uint64 player_guid);

	virtual void sessionOpen(Sessionx * session);

	virtual void sessionClosed(Sessionx * session);

	virtual void sessionRouterAdd(Packet * packet) override;

	virtual void sessionRouterRemove(uint64 guid) override;

	virtual Sessionx * getSession(Packet * packet);
protected:
private:
	typedef vector<Sessionx *> SessionVec_t;

	typedef map<Packet *, uint64> PacketPlayerGuid_t;

	netcore::SessionPoolx * m_session_pool;

	HandleInputx * m_handle_input;

	SessionVec_t m_session_vec;

	PacketPlayerGuid_t m_packet_player_guid;

	ACE_Thread_Mutex m_packet_player_guid_mutex;
};

#endif