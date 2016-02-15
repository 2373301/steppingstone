#ifndef MANAGE_WEB_VIP_RES_HPP
#define MANAGE_WEB_VIP_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "WebVipRes.h"
class GAMELOGIC_EXOPRT ManageWebVipRes
{
public:
	bool loadContent(Document * xml_doc);
	const WebVipInfo & getWebVipInfo();
protected:
	bool loadInfo(Element * element);
private:
	WebVipInfo m_web_vip_res_;
};
#endif
