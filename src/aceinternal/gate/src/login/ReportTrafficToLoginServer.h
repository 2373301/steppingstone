
#ifndef _REPORT_TRAFFIC_TO_LOGIN_SERVER_HPP
#define _REPORT_TRAFFIC_TO_LOGIN_SERVER_HPP

#include <ace/Event_Handler.h>

class ReportTrafficToLoginServer : public ACE_Event_Handler
{
public:
	ReportTrafficToLoginServer();
	~ReportTrafficToLoginServer();
public:
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);

protected:
private:
	void reportTraffic();
};

#endif