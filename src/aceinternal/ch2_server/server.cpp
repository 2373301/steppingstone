#include "ace/Log_Msg.h"
#include "acceptor.h"
#include "handle_data.h"

ACE_INT32 main( ACE_INT32 argc, char **argv )
{
	ACE_UINT16  port = 0;
	if (argc < 2)
		port = 60000;
	else
		port = ACE_OS::atoi(argv[1]);
	Acceptor    *accept = NULL;
	
	ACE_NEW_RETURN( accept, Acceptor( ACE_Reactor::instance () ),-1 );
	if (accept->open( port ) == -1)
	{
		accept->handle_close();
		ACE_DEBUG( (LM_DEBUG, "main open error!\n") );
		return -1;
	}	
	if (ACE_Reactor::run_event_loop() ==-1)
	{
		accept->handle_close();
		ACE_DEBUG( (LM_DEBUG, "main run event loop error!\n") );
		return -1;
	}
	accept->handle_close();
	return 0;
}