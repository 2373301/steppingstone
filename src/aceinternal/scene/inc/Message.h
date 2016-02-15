
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Scend_def.h"

class SCENE_EXOPRT Message
{
public:
	~Message()
	{}

public:
	// player's message
	virtual void playerMsg(Packet * packet) = 0;

	virtual void inlineBroadMsg(Packet * packet) = 0;

	// plugin's message
	virtual void notifyMsgToPlugins(const PackInfo & pack_info) = 0;

	virtual bool requestMsgToPlugins(const PackInfo & pack_info) = 0;

	virtual bool gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) = 0;

	// scene request message
	virtual void packInput(PackInfo * pack_info) = 0;
protected:
private:
};

#endif