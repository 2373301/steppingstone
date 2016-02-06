#include "ace/Log_Msg.h"
#include "acceptor.h"
#include "handle_data.h"

ACE_INT32 Acceptor::open( ACE_UINT16 port )
{
	
	ACE_INET_Addr  addr;
	addr.set( port, (ACE_UINT32)INADDR_ANY );
	if (acceptor_.open( addr ) == -1)
	{
		ACE_DEBUG( (LM_DEBUG, "accept open error\n") );
		return -1;
	}
	return reactor()->register_handler( this, ACE_Event_Handler::ACCEPT_MASK );	
}


ACE_INT32 Acceptor::handle_input( ACE_HANDLE handle )
{
	Handle_data  *handle_data = 0;
	ACE_NEW_RETURN( handle_data, Handle_data( reactor() ), -1 );
	
	if (acceptor_.accept( handle_data->get_peer() ) == -1)
	{
		ACE_DEBUG( (LM_DEBUG, "accept handle input accept error!\n") );
		handle_data->handle_close();
		return -1;
	}
	else if (handle_data->open() == -1)
	{
		ACE_DEBUG( (LM_DEBUG, "accept handle input open error!\n") );
		handle_data->handle_close();
		return -1;
	}
	else
	{
		ACE_DEBUG( (LM_DEBUG, "accept handle input ok!\n") );
		return 0;
	}	
}


ACE_INT32 Acceptor::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask mask )
{
	acceptor_.close();
	delete this;
	ACE_DEBUG( (LM_DEBUG, "accept handle close ok!\n") );
	return 0;	
}

