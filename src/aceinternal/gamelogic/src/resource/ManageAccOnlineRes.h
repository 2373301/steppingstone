#ifndef MANAGE_ACC_ONLINE_RES_HPP
#define MANAGE_ACC_ONLINE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "AccOnlineRes.h"
class GAMELOGIC_EXOPRT ManageAccOnlineRes
{
public:
	bool loadContent(Document * xml_doc);
	const AccOnlineInfo & getAccOnlineInfo();
protected:
	bool loadInfo(Element * element);
private:
	AccOnlineInfo m_Acc_Online_res_;
};
#endif
