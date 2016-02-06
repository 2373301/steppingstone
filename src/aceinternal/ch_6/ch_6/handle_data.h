
#ifndef ACE_TESTS_PROACTOR_TEST_H
#define ACE_TESTS_PROACTOR_TEST_H

#include "ace/Asynch_IO.h"

class Handle_data : public ACE_Service_Handler
{
    public:
        Handle_data ();        
        ~Handle_data ();        
        size_t get_total_snd (void) { return this->total_snd_; }
        size_t get_total_rcv (void) { return this->total_rcv_; }
        long get_total_w   (void) { return this->total_w_; }
        long get_total_r   (void) { return this->total_r_; }
  
        virtual void addresses (const ACE_INET_Addr& peer, const ACE_INET_Addr& local);
  
        virtual void open (ACE_HANDLE handle,  ACE_Message_Block &message_block);
        void cancel ();

    protected:
  
        virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);  
        virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

    private:
        int initiate_read_stream (void);
        int initiate_write_stream (ACE_Message_Block &mb, size_t nbytes);       

        ACE_Asynch_Read_Stream rs_;
        ACE_Asynch_Write_Stream ws_;
        ACE_HANDLE handle_;
        ACE_SYNCH_MUTEX lock_;

        int io_count_;             // Number of currently outstanding I/O requests
        int flg_cancel_;
        size_t total_snd_;         // Number of bytes successfully sent
        size_t total_rcv_;         // Number of bytes successfully received
        int total_w_;              // Number of write operations
        int total_r_;              // Number of read operations
};

#endif /* ACE_TESTS_PROACTOR_TEST_H */
