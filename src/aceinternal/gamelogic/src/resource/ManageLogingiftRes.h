#ifndef MANAGE_LOGINGIFT__RES_HPP
#define MANAGE_LOGINGIFT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "LogingiftRes.h"
class GAMELOGIC_EXOPRT ManageLogingiftRes
{
public:
	bool loadContent(Document * xml_doc);
	const LogingiftInfo & getLogingiftInfo();
protected:
	bool loadInfo(Element * element);
private:
	LogingiftInfo m_LoginGift__res_;
};
#endif
