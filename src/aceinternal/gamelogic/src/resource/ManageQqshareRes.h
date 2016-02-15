#ifndef MANAGE_QQSHARE__RES_HPP
#define MANAGE_QQSHARE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "QqshareRes.h"
class GAMELOGIC_EXOPRT ManageQqshareRes
{
public:
	bool loadContent(Document * xml_doc);
	const QqshareInfo & getQqshareInfo();
protected:
	bool loadInfo(Element * element);
private:
	QqshareInfo m_QQShare__res_;
};
#endif
