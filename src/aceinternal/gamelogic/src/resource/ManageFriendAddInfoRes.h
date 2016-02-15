
#ifndef MANAGE_FRIEND_ADD_INFO_RES_HPP
#define MANAGE_FRIEND_ADD_INFO_RES_HPP

#include "FriendAddInfoRes.h"
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageFriendAddInfoRes
{
public:
	bool loadContent(Document * xml_doc);
	const FriendAddInfoRes & getFriendAddInfoRes();
protected:
	bool loadInfo(Element * element);
private:
	FriendAddInfoRes m_friend_add_info_res; 
};

#endif
