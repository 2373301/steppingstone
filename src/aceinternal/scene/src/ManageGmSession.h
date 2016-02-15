
#ifndef MANAGE_GM_SESSION_HPP
#define MANAGE_GM_SESSION_HPP

#include "Scene.h"
#include "Line.h"
#include "SingleConnection.h"

using namespace netstream;

class ManageGmSession : public SingleConnection
{
public:
	ManageGmSession();
	~ManageGmSession();
public:
	virtual void handlePacket(Packet * packet);

	void setScene(Scene * line_scene);
protected:
private:
	Scene * m_line_scene;
};

#endif