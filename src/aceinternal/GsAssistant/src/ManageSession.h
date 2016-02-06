
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include <string>
#include "SessionPool.h"

enum CheckGsType
{
	CGT_BASE		= 0,
	CGT_STARTUP		= 1,
	CGT_SHUTDOWN,
};

class ManageSession : public HandleInput, public netcore::HandleSessionEvent, public netcore::HandleSessionRouter
{
public:
	ManageSession();
	~ManageSession();
public:
	// handle input
	virtual void input(Packet * packet);

	// handle session event
	virtual void newConnection(Session * session);

	virtual void connectionClosed(Session * session);

	// handle session router
	virtual void addRoute(Packet * packet);

	virtual void removeRoute(uint64 guid);

	virtual Session * getSession(Packet * packet);

public:
	int init(int monitor_type, char * port);

	void finit();

	void setCheckType(CheckGsType check_gs_type);

	void setPort(const std::string & port);

	bool isFinish();

	bool isSuccess();

protected:
	void handleGsStartupReport(Packet * packet);

	void handleGsShutdownReport(Packet * packet);
protected:
private:
	netcore::SessionPool * m_session_pool;

	Session * m_session;

	CheckGsType m_check_gs_type;

	bool	m_recv_report_msg;

	bool	m_is_success;

	bool	m_is_finish;

	std::string		m_listen_port;
};

#endif