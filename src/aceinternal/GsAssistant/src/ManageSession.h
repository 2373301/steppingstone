
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include <string>
#include "SessionPoolx.h"

enum CheckGsType
{
	CGT_BASE		= 0,
	CGT_STARTUP		= 1,
	CGT_SHUTDOWN,
};

class ManageSession 
	: public HandleInputx
	, public netcore::HandleSessionOpenClosed
	, public netcore::HandleSessionRouterAddRemove
{
public:
	ManageSession();
	~ManageSession();
public:
	// handle input
	virtual void input(Packet * packet) override;

	// handle session event
	virtual void sessionOpen(Sessionx * session) override;

	virtual void sessionClosed(Sessionx * session) override;

	// handle session router
	virtual void sessionRouterAdd(Packet * packet) override;

	virtual void sessionRouterRemove(uint64 guid) override;

	virtual Sessionx * getSession(Packet * packet) override;

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

private:
	netcore::SessionPoolx * m_session_pool;

	Sessionx * m_session;

	CheckGsType m_check_gs_type;

	bool	m_recv_report_msg;

	bool	m_is_success;

	bool	m_is_finish;

	std::string		m_listen_port;
};

#endif