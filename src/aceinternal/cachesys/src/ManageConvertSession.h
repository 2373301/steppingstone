
#ifndef MANAGE_CONVERT_SESSION_HPP
#define MANAGE_CONVERT_SESSION_HPP

#include <ace/Thread_Mutex.h>
#include <ace/Task.h>
#include <ace/Singleton.h>
#include "SessionPool.h"
#include "Singleton.h"

class ManageConvertSession 
	: public HandleInput
	, public netcore::HandleSessionOpenClosed
	, public netcore::HandleSessionRouterAddRemove
	, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageConvertSession();
	~ManageConvertSession();
public:
	static ManageConvertSession * instance()
	{
		return Singleton<ManageConvertSession>::instance();
	}
public:
	int init(const string & cache_listen_addr);

	int stop();

	int finit();

public:
	int svc();

public:
	typedef vector<string>	FilePathVec_t;

	virtual void input(Packet * packet);

public:
	virtual void sessionOpen(Session * session);

	virtual void sessionClosed(Session * session);

public:
	virtual void sessionRouterAdd(Packet * packet);

	virtual void sessionRouterRemove(uint64 guid);

	virtual Session * getSession(Packet * packet);

public:
	void handleFile(const string & file_name);

protected:
	void handleBinaryRecord(Packet * packet);

	void transferBinaryFile(bool convert_available, FilePathVec_t & file_path_vec);

private:
	typedef vector<Session *> SessionVec_t;

	netcore::SessionPool * m_session_pool;

	bool m_stop;

	bool m_wait;

	string m_cache_addr;

	SessionVec_t m_session_vec;

	ACE_Thread_Mutex m_session_vec_mutex;

	FilePathVec_t	m_file_path_vec;

	ACE_Thread_Mutex m_file_path_vec_mutex;

	bool m_convert_is_available;
};

#endif