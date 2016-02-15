#ifndef MANAGE_GAMBLING__RES_HPP
#define MANAGE_GAMBLING__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GamblingRes.h"
class GAMELOGIC_EXOPRT ManageGamblingRes
{
public:
	bool loadContent(Document * xml_doc);
	const GamblingInfo & getGamblingInfo();
protected:
	bool loadInfo(Element * element);
private:
	GamblingInfo m_gambling__res_;
};
#endif
