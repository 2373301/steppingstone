
#include "PacketStream.h"

uint32 Header::temp_guid = 0;

ACE_Thread_Mutex Header::temp_guid_mutex;

int PacketStream::m_head_size = sizeof(Header);

STAT_NUMBER_IMP(PacketStream);