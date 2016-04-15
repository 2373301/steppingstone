
#ifndef REMOTE_CACHE_HPP
#define REMOTE_CACHE_HPP

#include "typedef.h"
#include "Cache.h"

class RemoteCache 
	: public Cache
{
public:
	RemoteCache();
	~RemoteCache();

	// 增加log 而已
	virtual int session_on_read() override;
	void output(Packet * ps);

public:
	virtual void session_on_read_error(int recv_value, int last_error) override;
public:
	virtual int finit() override;

protected:
	virtual int initing() override;
};
#endif