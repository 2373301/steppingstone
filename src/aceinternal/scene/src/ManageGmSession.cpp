
#include "ManageGmSession.h"

ManageGmSession::ManageGmSession()
: m_line_scene(NULL)
{
}

ManageGmSession::~ManageGmSession()
{
}

void ManageGmSession::handlePacket(Packet * packet)
{
	m_line_scene->input(packet);
}

void ManageGmSession::setScene(Scene * line_scene)
{
	m_line_scene = line_scene;
}