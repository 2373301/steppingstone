
#include "RoleSession.h"
#include "RouteRolePacket.h"

RoleSession::RoleSession()
{

}

RoleSession::~RoleSession()
{

}

int RoleSession::netConnected()
{
	NetStream::netConnected();
	RouteRolePacket::instance()->collectRoleSession(this);
	m_handle_net_packet = RouteRolePacket::instance();
	return 0;
}

void RoleSession::netClosed(ACE_Reactor_Mask close_mask)
{
	if (!m_net_state.closed)
	{
		NetStream::netClosed(close_mask);
		RouteRolePacket::instance()->removeRoleSession(this);
		this->reactor()->remove_handler(this, ACE_Event_Handler::ALL_EVENTS_MASK);
		ACE_OS::sleep(2);
		delete this;
	}
}
