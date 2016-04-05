
#include "GateSession.h"
#include "RouteGatePacket.h"
#include "ManageGateSession.h"

GateSession::GateSession()
{

}

GateSession::~GateSession()
{

}

int GateSession::netConnected()
{
	int rc = NetStream::netConnected();
	m_handle_net_packet = RouteGatePacket::instance();
	RouteGatePacket::instance()->collectGateSession(this);
	return rc;
}

void GateSession::netClosed(ACE_Reactor_Mask close_mask)
{
	if (!m_net_state.closed)
	{
		RouteGatePacket::instance()->removeGateSession(this);
		ManageGateSession::instance()->gateDisconnect();
	}

	NetStream::netClosed(close_mask);
}
