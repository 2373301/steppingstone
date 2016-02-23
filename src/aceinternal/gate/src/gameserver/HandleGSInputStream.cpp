
#include "HandleGSInputStream.h"
#include "RouteGSPacket.h"
#include "ManageGSStream.h"

HandleGSInputStream::HandleGSInputStream(GSSession * gs_session)
: m_gs_session(gs_session)
{
	m_type = GSST_Input;
}

HandleGSInputStream::~HandleGSInputStream()
{

}

int HandleGSInputStream::doing()
{
	int result = 0;
	int r = 0;
	if (NULL != m_gs_session)
	{
		while (!ManageGSStream::instance()->isWait())
		{
			if (m_gs_session->isShutdown())
			{
				return -1;
			}

			r = m_gs_session->rd_stream();
			if (0 == r)
			{
				continue;
			}
			else
			{
				/// finish to recv a packet
				if (1 == r)
				{
				}
				else if (2 == r)
				{
					result = 0;
					break;
				}
				else
				{
					result = -1;
					m_gs_session->handle_close(NULL, ACE_Event_Handler::READ_MASK);
					break;
				}
			}
		}

	}
	return result;
}