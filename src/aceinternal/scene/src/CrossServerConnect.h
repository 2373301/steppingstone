
#ifndef CROSS_SERVER_CONNECT_HPP
#define CROSS_SERVER_CONNECT_HPP

#include "SingleConnection.h"
#include "Scene.h"

using namespace netstream;

class CrossServerConnect : public SingleConnection
{
public:
	CrossServerConnect();
	~CrossServerConnect();
public:
	virtual void handlePacket(Packet * packet);

	virtual void newConnection(PacketQue_t & output_packet_que);

	virtual void connectionClosed();
public:
	void setServerInfo(const ServerCfg & server_cfg, const string & security_key, CrossServerInput * cross_server_input);
protected:
private:
	CrossServerInput * m_cross_server_input;
	ServerCfg m_server_cfg;
	string m_security_key;
};
#endif