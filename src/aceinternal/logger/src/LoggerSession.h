
#ifndef LOGGER_SESSION_HPP
#define LOGGER_SESSION_HPP

#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include "typedef.h"

class LoggerSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	LoggerSession();
	~LoggerSession();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	virtual int open(void * p);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
public:
	int handleLogBuffer(vector<char *> & log_buffer_vec);

	int recv_data(ACE_Message_Block & msg_block);
protected:

	int sendLogBuffer();
private:
	ACE_Message_Block m_output_block;
};

#endif