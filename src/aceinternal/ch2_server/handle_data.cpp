#include "ace/SOCK_Stream.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Timer_Queue.h"
#include "handle_data.h"

ACE_INT32 Handle_data::open( )
{	
	ACE_INT32 ret = 0;
	ACE_INET_Addr remote_addr;	
	get_peer().get_remote_addr( remote_addr );		
	ACE_DEBUG( (LM_DEBUG, "the remote addr is %s\n", remote_addr.get_host_addr())  );
	
	ret = reactor()->register_handler( this, ACE_Event_Handler::READ_MASK );
	if (ret != -1)	
	{		
		ACE_DEBUG( (LM_DEBUG, "handle data register ok!\n")  );		
	}	
	return ret;
}





ACE_INT32 Handle_data::handle_close( ACE_HANDLE , ACE_Reactor_Mask )
{	
	get_peer().close();
	ACE_DEBUG( (LM_DEBUG, "handle data close.\n") );
	delete this;
	return 0;
}

ACE_INT32 Handle_data::handle_input( ACE_HANDLE )
{	
	ACE_INT8 buf[512] = {0};
	ssize_t len;
	  
	len = get_peer().recv( buf, 500);
	if (len > 0)
	{
		ACE_DEBUG( (LM_DEBUG, "recv data %s, len:%d.\n", buf, len) );		
	    return 0;
	}
	else if (len == 0)
	{
		ACE_DEBUG( (LM_DEBUG, "recv data len is 0, client exit.\n") );
		return -1;
	}
	else
	{
		ACE_DEBUG( (LM_DEBUG, "recv data error len < 0" ) );
		return -1;
	}	
}