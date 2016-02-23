
#include "HandleGSOutputStream.h"
#include "ManageGSStream.h"


HandleGSOutputStream::HandleGSOutputStream(GSSession * gs_session)
: m_gs_session(gs_session)
{
	m_type = GSST_Output;
}

HandleGSOutputStream::~HandleGSOutputStream()
{
}

int HandleGSOutputStream::doing()
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

			r = m_gs_session->wt_stream();
			if (0 == r)
			{
				continue;
			}
			else if (1 == r)
			{
				/// reset m_gs_session output
			}
			else if (2 == r)
			{
				break;
			}
			else
			{
				result = -1;
				m_gs_session->handle_close(NULL, ACE_Event_Handler::WRITE_MASK);
				break;
			}
		}

	}
	return result;
}
