#include "ace/Get_Opt.h"
#include "ace/os_include/netinet/os_tcp.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "handle_data.h"

static int duplex = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int loglevel =2;       // 0 full , 1 only errors

class Acceptor : public ACE_Asynch_Acceptor<Handle_data>
{
    public:
        Acceptor ();
        virtual ~Acceptor (void);       
        Handle_data *make_handler (void);   
};

// *************************************************************
Acceptor::Acceptor ()  
{}

Acceptor::~Acceptor (void)
{
    this->cancel ();
}

Handle_data *Acceptor::make_handler (void)
{  
    Handle_data *p;
    ACE_NEW_RETURN (p, Handle_data (), 0);
    return p;
}

// ***************************************************
Handle_data::Handle_data ()
  : handle_   (ACE_INVALID_HANDLE),
    io_count_ (0),
    flg_cancel_(0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{}

Handle_data::~Handle_data (void)
{
    ACE_DEBUG ((LM_DEBUG,   ACE_TEXT ("(%t) Handle_data dtor; %d sends (%B bytes); ")  ACE_TEXT ("%d recvs (%B bytes)\n"),
              this->total_w_, this->total_snd_, this->total_r_, this->total_rcv_));
              
    if (this->io_count_ != 0)
    {
        ACE_ERROR ((LM_WARNING,  ACE_TEXT ("(%t) Handle_data deleted with ")  ACE_TEXT ("%d I/O outstanding\n"),
               this->io_count_));
    }
    
    int issue_data_warning = 0;
    if (this->total_snd_ > this->total_rcv_)
    {
        if (this->total_rcv_ == 0)
        {
            issue_data_warning = 1;
        }
        else if (this->total_snd_ / this->total_rcv_ > 2)
        {
            issue_data_warning = 1;
        }
    }
    else
    {
        if (this->total_snd_ == 0)
        {
            issue_data_warning = 1;
        }
        else if (this->total_rcv_ / this->total_snd_ > 2)
        {
        	issue_data_warning = 1;
        }
    }
    if (issue_data_warning)
    {
        ACE_DEBUG ((LM_WARNING, ACE_TEXT ("(%t) Above byte counts look odd; need review\n")));
    }

    if (this->handle_ != ACE_INVALID_HANDLE)
    {
        ACE_OS::closesocket (this->handle_);
    }    
    this->handle_= ACE_INVALID_HANDLE;
}

void Handle_data::cancel ()
{
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);
    this->flg_cancel_ = 1;
    this->ws_.cancel ();
    this->rs_.cancel ();
    return;
}


void Handle_data::addresses (const ACE_INET_Addr& peer, const ACE_INET_Addr&)
{
    ACE_TCHAR str[256];
    if (0 == peer.addr_to_string (str, sizeof (str) / sizeof (ACE_TCHAR)))
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Handle_data connection from %s\n"),  str));
    }
    else
    {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) Handle_data %p\n"),  ACE_TEXT ("addr_to_string")));
    }
    return;
}


void Handle_data::open (ACE_HANDLE handle, ACE_Message_Block &)
{
    {
        ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

        // Don't buffer serial sends.
        this->handle_ = handle;
        int nodelay = 1;
        ACE_SOCK_Stream option_setter (handle);
        if (-1 == option_setter.set_option (ACE_IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay)))
        {
        	 ACE_ERROR ((LM_ERROR, "%p\n", "set_option"));
        }

        if (this->ws_.open (*this, this->handle_) == -1)
        {
            ACE_ERROR ((LM_ERROR,   ACE_TEXT ("(%t) %p\n"),  ACE_TEXT ("Handle_data::ACE_Asynch_Write_Stream::open")));
        }
        else if (this->rs_.open (*this, this->handle_) == -1)
        {
        	ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),  ACE_TEXT ("Handle_data::ACE_Asynch_Read_Stream::open")));
        }
        else
        {
            this->initiate_read_stream ();
        }

        if (this->io_count_ > 0)
        {
            return;
        }
    }
    delete this;
}

int Handle_data::initiate_read_stream (void)
{
    if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    {
        return -1;
    }

    ACE_Message_Block *mb = 0;
    ACE_NEW_RETURN (mb, ACE_Message_Block (1024), -1);

    // Inititiate read
    if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
        mb->release ();
        ACE_ERROR_RETURN ((LM_ERROR,  ACE_TEXT ("(%t) Handle_data %p\n"),   ACE_TEXT ("read")),  -1);
    }

    this->io_count_++;
    this->total_r_++;
    return 0;
}

int Handle_data::initiate_write_stream (ACE_Message_Block &mb, size_t nbytes)
{
    if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    {
          mb.release ();
          return -1;
    }

    if (nbytes == 0)
    {
        mb.release ();
        ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("(%t) Handle_data::ACE_Asynch_Write_Stream::write nbytes <0 ")), -1);
    }

    if (this->ws_.write (mb, nbytes) == -1)
    {
        mb.release ();
        ACE_ERROR_RETURN((LM_ERROR,  ACE_TEXT ("(%t) Handle_data %d, %p\n"),  ACE_TEXT ("write")),  -1);
    }

    this->io_count_++;
    this->total_w_++;
    return 0;
}

void Handle_data::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
    {
        ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_ );

        ACE_Message_Block & mb = result.message_block ();

        // Reset pointers.
        mb.rd_ptr ()[result.bytes_transferred ()] = '\0';

        if (loglevel > 1)
        {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) **** Handle_data  handle_read_stream() ****\n")));
            ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("%s = %s\n"),
               ACE_TEXT ("message_block"),    mb.rd_ptr ()));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("**** end of message ****************\n")));
        }
        

        if (result.error () == 0 && result.bytes_transferred () > 0)
        {
            this->total_rcv_ += result.bytes_transferred ();

            if (this->initiate_write_stream (mb,  result.bytes_transferred ()) == 0)
            {
                if (duplex != 0)  // Initiate new read from the stream.
                {
                    this->initiate_read_stream ();
                }
            }
        }
        else
        {
            mb.release ();
        }

        this->io_count_--;
        if (this->io_count_ > 0)
        {
            return;
        }
    }
    delete this;
}

void Handle_data::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
    {
        ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

        ACE_Message_Block & mb = result.message_block ();

        if (loglevel > 1)
        {
            //mb.rd_ptr () [0] = '\0';
            mb.rd_ptr (mb.rd_ptr () - result.bytes_transferred ());

            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Handle_data handle_write_stream() ****\n") ));
            ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),   mb.rd_ptr ()));
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("**** end of message ****************\n")));
        }
       
        mb.release ();

        if (result.error () == 0 && result.bytes_transferred () > 0)
        {
            this->total_snd_ += result.bytes_transferred ();

            if (duplex == 0)
            {
                this->initiate_read_stream ();
            }
        }

        this->io_count_--;
        if (this->io_count_ > 0)
        {
            return;
        }
    }
    delete this;
}



static int parse_args (int argc, ACE_TCHAR *argv[])
{    

    if (argc == 1) // no arguments , so one button test
    {
        return 0;
    }
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"));
    int c;

    while ((c = get_opt ()) != EOF)
    {
        switch (c)
        {
        
          case 'p':         // port number
            port = ACE_OS::atoi (get_opt.opt_arg ());
            break;
        
          default:
            return -1;
        } 
    } 

    return 0;
}

int main (int argc, ACE_TCHAR *argv[])
{    
	int ret;
    Acceptor  acceptor ;    
    parse_args(argc, argv);         
    ACE_INET_Addr addr (port);
    
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("listen the port %d.\n "),port));
    
    if ((ret=acceptor.open (addr)) == -1)
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("acceptor open fail.\n ")));
        return -1;
    }
       
    ACE_Proactor::instance ()->proactor_run_event_loop (); 
    return 0;
}


