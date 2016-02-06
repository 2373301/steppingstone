#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

class StaticObj : public ACE_Service_Object  
{
public:
       StaticObj(ACE_Reactor *r = ACE_Reactor::instance())
              :ACE_Service_Object(r){}       

       virtual ~StaticObj(){}   
       virtual int init (int argc, ACE_TCHAR *argv[])
       {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("---StaticObj::init\n" )));
              return 0;
       }      
       virtual int fini()
       {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("---StaticObj::fini\n")));
              return 0;
       }
       virtual int suspend()
       {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("---StaticObj::suspend\n")));
              return 0;
       }
       virtual int resume() 
       {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("---StaticObj::resume\n")));
              return 0;
       }
};

ACE_FACTORY_DEFINE (ACE, StaticObj);

ACE_STATIC_SVC_DEFINE (
  StaticObj_Discriptor,
  ACE_TEXT ("StaticObj"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (StaticObj),
  ACE_Service_Type::DELETE_THIS
  | ACE_Service_Type::DELETE_OBJ,
  1 
)

ACE_STATIC_SVC_REQUIRE (StaticObj_Discriptor)

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) 
{
       //ACE_STATIC_SVC_REGISTER (MyStaticObj-Discriptor);
       ACE_Service_Config::open
              (argc, argv, ACE_DEFAULT_LOGGER_KEY, 0);
       ACE_Reactor::instance ()->run_reactor_event_loop ();
       	
       return 0;
}


