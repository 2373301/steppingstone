
#ifndef DATA_RECORD_IMP_HPP
#define DATA_RECORD_IMP_HPP

#include <fstream>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "typedef.h"
#include "coredef.h"
#include "Line.h"
#include "../netstream/inc/Session.h"

using namespace std;

class DataRecordImp : public DataRecord, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	DataRecordImp();
	~DataRecordImp();
public:
	int svc();
public:
	int init(Line * line_scene, const string & record_server_addr);

	int stop();

	virtual void record(int scene_id, int module_id, int src_from, uint64 guid, const string & name, int level, uint64 param_1 = 0, uint64 param_2 = 0, uint64 param_3 = 0, uint64 param_4 = 0, uint64 param_5 = 0, uint64 param_6 = 0, uint64 param_7 = 0, uint64 param_8 = 0, uint64 param_9 = 0, uint64 param_10 = 0);

	virtual void record(int scene_id, int module_id, int src_from, uint64 guid, uint64 param_1 = 0, uint64 param_2 = 0, uint64 param_3 = 0, uint64 param_4 = 0, uint64 param_5 = 0, uint64 param_6 = 0, uint64 param_7 = 0, uint64 param_8 = 0, uint64 param_9 = 0, uint64 param_10 = 0);
protected:
	bool connectToServer();

private:
	Line * m_line_scene;

	bool m_is_stop;

	int m_connect_exchange_status;

	Session * m_session;

	string m_record_server_addr;

	PacketQue_t m_packet_que;

	ACE_Thread_Mutex m_packet_que_mutex;
};
#endif