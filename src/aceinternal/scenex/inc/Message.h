#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Scenex_def.h"
#include "typedef.h"

class Packet;
struct PackInfo;
class SCENEX_EXOPRT IMessage
{
public:
	~IMessage(){}

	// player's message
	virtual void IMessage_player(Packet * packet) = 0;

	virtual void IMessage_inlineBroad(Packet * packet) = 0;

	// plugin's message
	virtual void IMessage_notifyToPlugins(const PackInfo & pack_info) = 0;
	virtual bool IMessage_requestToPlugins(const PackInfo & pack_info) = 0;
	virtual bool IMessage_gmcmdToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) = 0;

	// scene request message
	virtual void IMessage_packInput(PackInfo * pack_info) = 0;
};

#endif