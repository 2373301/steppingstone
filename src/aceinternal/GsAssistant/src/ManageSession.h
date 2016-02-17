
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

class ManageSession 
	: public HandleInput
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
	virtual void sessionOpen(Session * session) override;

	virtual void sessionClosed(Session * session) override;

	// handle session router
	virtual void sessionRouterAdd(Packet * packet) override;

	virtual void sessionRouterRemove(uint64 guid) override;

	virtual Session * getSession(Packet * packet) override;

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