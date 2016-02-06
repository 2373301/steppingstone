#include "ace/Service_Object.h"
#include "ace/Log_Msg.h"

class DynamicObj : public ACE_Service_Object 
{
public:
    DynamicObj() {};

    virtual ~DynamicObj() {};

    virtual int init (int argc, ACE_TCHAR *argv[])
    {        
         ACE_DEBUG( (LM_DEBUG, "That DynamicObj::init------\n") ); 
         return 0;
    }

    virtual int suspend()
    {        
        ACE_DEBUG( (LM_DEBUG, "That DynamicObj::init------\n") ); 
        return 0;
    }

    virtual int resume()
    {        
        ACE_DEBUG( (LM_DEBUG, "That DynamicObj::init------\n") ); 
        return 0;
    }

    virtual int fini()
    {        
        ACE_DEBUG( (LM_DEBUG, "That DynamicObj::init------\n") ); 
        return 0;
    }
};

ACE_FACTORY_DEFINE (ACE, DynamicObj);
//DynamicObj obj;