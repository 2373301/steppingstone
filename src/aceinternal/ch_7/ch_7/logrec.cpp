/*
*/
#include "stdio.h"
#include "ace/ACE.h"
#include "ace/Message_Block.h"
#include "ace/Module.h"
#include "ace/SString.h"
#include "ace/Stream.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include <fstream>

const size_t LineSize = 512;
char  file_buf[LineSize] ;

template <class TASK>
class Logrec_Module : public ACE_Module<ACE_SYNCH>
{
   public:
     Logrec_Module (const ACE_TCHAR *name)
     {
        this->open (name, &task_,   0, 0, ACE_Module<ACE_SYNCH>::M_DELETE_READER);
        ACE_DEBUG ((LM_DEBUG, "Logrec_Module start %s. \n",name));
     }
   private:
     TASK task_;
};


#define LOGREC_MODULE(NAME)  typedef Logrec_Module<NAME> NAME##_Module 
  
class Logrec_Reader : public ACE_Task<ACE_SYNCH>
{
  private:
  	ifstream fin;  
  public: 
    Logrec_Reader (const ACE_TString &file) 
    {
      fin.open(file.c_str());
      if (!fin) 
      {
      	ACE_DEBUG ((LM_DEBUG, "Logrec_Reader open file %s error.\n",file.c_str()));
        return ;
      }
    }  
    
    virtual int open (void *) 
    {       
      return activate ();
    }
    
    virtual int svc () 
    {  	   
  	   //int  i =0;
  	   ACE_Message_Block *mblk;  	   
	   int  len = 0;
	   
       while(!fin.eof())
      {      	 
      	 fin.getline(file_buf, LineSize);      	 
      	 len = ACE_OS::strlen( file_buf );
         ACE_NEW_RETURN (mblk, ACE_Message_Block (len+200), 0);
         if (file_buf[len-1] == '\r')
         {
         	len = len - 1;
         }          
	     mblk->copy (file_buf, len+1 ); 
	     // 通过put_next函数，将消息传递给下一个过滤器
	     put_next (mblk); 
	  }
       ACE_NEW_RETURN(mblk, ACE_Message_Block (0, ACE_Message_Block::MB_STOP),  0);
	   put_next (mblk);
	   fin.close();
	   ACE_DEBUG ((LM_DEBUG, "read svc return .\n")); 
       return 0;
    }
};

class Logrec_Reader_Module : public ACE_Module<ACE_SYNCH>
{
  public:
    Logrec_Reader_Module (const ACE_TString &filename) : task_ (filename)
    {
      this->open (ACE_TEXT ("Logrec Reader"), &task_, 0, 0, ACE_Module<ACE_SYNCH>::M_DELETE_READER);
      ACE_DEBUG ((LM_DEBUG, "Logrec_Reader_Module. \n"));
    }
  private:
    Logrec_Reader task_;
};




class Logrec_Timer : public ACE_Task<ACE_SYNCH>
{   
  public:
    virtual int put (ACE_Message_Block *mblk, ACE_Time_Value *) 
    {
    
      for (ACE_Message_Block *temp = mblk;      temp != 0;   temp = temp->cont ())    
      {     
      	 if (temp->msg_type () != ACE_Message_Block::MB_STOP)
        {
        	format_data(temp);
        }
      }
      
      return put_next (mblk);
   } 

   static void format_data (ACE_Message_Block *mblk) 
   {
  	  char timestamp_t[64]={0};
     char timestamp[64]={0}; 
    
  	  struct tm       *today = NULL;
     time_t      ltime = 0;
    
     time(&ltime);
     today = ACE_OS::localtime(&ltime);
     ACE_OS::strftime(timestamp, 64," [%Y-%m-%d %H:%M:%S] " , today);	
            	
     size_t fmt_len (ACE_OS::sprintf (mblk->wr_ptr ()-1, "%s", timestamp));
     mblk->wr_ptr (fmt_len);
     return;
   }
};


LOGREC_MODULE (Logrec_Timer);

class Logrec_Suffix : public ACE_Task<ACE_SYNCH>
{    
  public:
    virtual int put (ACE_Message_Block *mblk, ACE_Time_Value *) 
    {
      if (mblk->msg_type () != ACE_Message_Block::MB_STOP)
      {
        size_t fmt_len (ACE_OS::sprintf (mblk->wr_ptr ()-1,  "%s", "attention please!\r\n"  ));
        mblk->wr_ptr (fmt_len);	
      }
      //ACE_DEBUG ((LM_DEBUG, "suffix:%@.%s\n",mblk,mblk->base()));
      return put_next (mblk);
    }
};

LOGREC_MODULE (Logrec_Suffix);
class Logrec_Writer : public ACE_Task<ACE_SYNCH>
{
  public:  
    virtual int open (void *) 
    { 
	   ACE_DEBUG ((LM_DEBUG, "Logrec_Writer. \n"));
	   return activate (); 
	 }


    virtual int put (ACE_Message_Block *mblk, ACE_Time_Value *to)
    { 
      return putq (mblk, to); 
    }

  virtual int svc () 
  {
    int stop = 0;          
    for (ACE_Message_Block *mb; !stop && getq (mb) != -1; ) 
    {
      if (mb->msg_type () == ACE_Message_Block::MB_STOP)
      {
        stop = 1;
      }
      else
      {  	      	    
  	    ACE_DEBUG ((LM_DEBUG, "%s",mb->base()));    	                           
      }
      put_next (mb);
    }
    return 0;
  }
};

LOGREC_MODULE (Logrec_Writer);


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
   if (argc != 2)
   {
       ACE_ERROR_RETURN ((LM_ERROR,   "usage: %s logfile\n", argv[0]), 1);
   }
  
   ACE_TString logfile (argv[1]);
 
   ACE_Stream<ACE_SYNCH> stream;

   if (stream.push(new Logrec_Writer_Module (ACE_TEXT ("Writer"))) != -1
     && stream.push(new Logrec_Suffix_Module (ACE_TEXT ("Separator"))) != -1
     && stream.push(new Logrec_Timer_Module (ACE_TEXT ("Formatter"))) != -1
     && stream.push(new Logrec_Reader_Module (logfile)) != -1)
   {
  	   ACE_DEBUG ((LM_DEBUG, "push ok \n"));
   }
   ACE_Thread_Manager::instance()->wait() ;
}

