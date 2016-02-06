#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

class Handle_data;

class Acceptor: public ACE_Event_Handler
{
    public:
	    Acceptor( ACE_Reactor *r = ACE_Reactor::instance() ):ACE_Event_Handler(r){}
	    	
	    ACE_INT32 open( const ACE_UINT16 port );	
		ACE_INT32 handle_input( ACE_HANDLE = ACE_INVALID_HANDLE );
		ACE_INT32 handle_close( ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0 );
		ACE_HANDLE get_handle() const
		{
			return acceptor_.get_handle();
		}
		ACE_SOCK_Acceptor &get_acceptor(){ return acceptor_; }
	private:
	     ~Acceptor(){};
    private:
        ACE_SOCK_Acceptor  acceptor_;	
};