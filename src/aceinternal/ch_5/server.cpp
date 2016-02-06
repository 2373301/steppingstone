
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/os_include/os_netdb.h"
#include "server.h"

#include "ace/Log_Msg.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"

typedef ACE_Acceptor<Handle_data, ACE_SOCK_ACCEPTOR>  ClientAcceptor;

int
Handle_data::open (void *p)
{
    if (super::open (p) == -1)
    {
        return -1;
    }

    ACE_TCHAR peer_name[MAXHOSTNAMELEN];
    ACE_INET_Addr peer_addr;
    if (this->peer ().get_remote_addr (peer_addr) == 0 && peer_addr.addr_to_string (peer_name, MAXHOSTNAMELEN) == 0)
    {
        ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) Connection from %s\n"), peer_name));
    }
    return 0;
}

int
Handle_data::handle_input (ACE_HANDLE)
{
    const size_t INPUT_SIZE = 4096;
    char buffer[INPUT_SIZE] ={0};
    ssize_t recv_cnt, send_cnt;

    recv_cnt = this->peer ().recv (buffer, sizeof(buffer));
    if (recv_cnt <= 0)
    {
        ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) Connection closed\n")));
        return -1;
    }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("rece data %s.\n"),buffer));   
    return 0;
}

int
Handle_data::handle_close (ACE_HANDLE h, ACE_Reactor_Mask mask)
{
     return super::handle_close (h, mask);
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
    ACE_INET_Addr port_to_listen (60000);
    ClientAcceptor acceptor;
    if (acceptor.open (port_to_listen,  ACE_Reactor::instance (),  ACE_NONBLOCK) == -1)
    {
        return -1;
    }

    ACE_Reactor::instance ()->run_reactor_event_loop ();
    return (0);
}

