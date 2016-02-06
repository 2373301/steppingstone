#include "ace/Event_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/Time_Value.h"

class Handle_data: public ACE_Event_Handler
{
	public:		
	    Handle_data( ACE_Reactor *r = ACE_Reactor::instance() ):ACE_Event_Handler(r)  {}
	    ACE_INT32 open( );
	    ACE_INT32 handle_input( ACE_HANDLE = ACE_INVALID_HANDLE );
	    ACE_INT32 handle_close( ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = 0 );
	    
	    ACE_HANDLE get_handle( ) const
		{
			return peer_.get_handle();
		}
		ACE_SOCK_Stream &get_peer()
		{
			return peer_;
		}
	private:	
		~Handle_data(){ACE_DEBUG( (LM_DEBUG, "handle data ~dctor .\n") );};
	private:	
		ACE_SOCK_Stream  peer_;		
};


