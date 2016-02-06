// client.cpp 
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

int ACE_MAIN (int, ACE_TCHAR *[])
{
  short port = 60000;
  const char *ip = "162.168.1.96";
  int ret = 0;

  ACE_INET_Addr srvr (port, ip);  
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream peer;
    
  ACE_Time_Value time(0,0);  
  ret= connector.connect (peer, srvr,&time);
    
  //ret= connector.connect (peer, srvr);
  if (ret == -1)
  {
     ACE_ERROR_RETURN ((LM_ERROR,   
      ACE_TEXT ("%p\n"), ACE_TEXT ("connect")), 1);  
  }
   
  peer.send_n ("hello word!\n", 12);     
  peer.close (); 
 
  return (0);
}
