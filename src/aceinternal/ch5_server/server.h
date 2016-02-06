#include "ace/Synch_Traits.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"
#include "ace/Message_Block.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

class Handle_data : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;
public:
    int open (void * = 0);
    virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);     
    virtual int handle_close (ACE_HANDLE handle,  ACE_Reactor_Mask close_mask);
	virtual int close(u_long flags = 0);
};

