
#ifndef ALLOC_GUID_HPP
#define ALLOC_GUID_HPP

#include "typedef.h"
#include "Packet.h"

class AllocGuid
{
public:
	AllocGuid();

	~AllocGuid();
public:
	int init(uint32 seed_value, uint64 last_index);

public:
	Packet * processRequest(Packet * packet);

protected:
	bool generateGuid(uint32 request_no, uint32 & generate_no, uint64 & start_index);

private:
	uint32 m_seed;

	uint64 m_index;
};
#endif