
#ifndef MANAGE_LOGIN_STREAM_HPP
#define MANAGE_LOGIN_STREAM_HPP

#include <ace/Task.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "LoginServerSession.h"

class ManageLoginStream : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageLoginStream();
	~ManageLoginStream();
public:
	static ManageLoginStream * instance()
	{
		return Singleton<ManageLoginStream>::instance();
	}
public:
	int svc();

	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();

	void handleLoginSession(LoginServerSession * login_server_session);

	void handleOutputStream(PacketStream * packet_stream);

	typedef queue<PacketStream *> PacketStreamQue_t;

protected:
	int processRead(LoginServerSession * login_session);

	int processWrite(LoginServerSession * login_session, PacketStreamQue_t & packet_stream_que);

	int reportOnlinePlayer(LoginServerSession * login_session);
private:
	LoginServerSession * m_login_server_session;

	bool m_stop;

	bool m_wait;

	ACE_Thread_Mutex m_login_server_session_mutex;

	PacketStreamQue_t m_packet_stream_que;

	ACE_Thread_Mutex m_packet_stream_que_mutex;
};

#endif