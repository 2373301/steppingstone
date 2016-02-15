#ifndef MANAGE_QQINVITE__RES_HPP
#define MANAGE_QQINVITE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "QqinviteRes.h"
class GAMELOGIC_EXOPRT ManageQqinviteRes
{
public:
	bool loadContent(Document * xml_doc);
	const QqinviteInfoMap_t & getQqinviteInfo();
protected:
	bool loadInfo(Element * element);
private:
	QqinviteInfoMap_t m_QQInvite__res_map;
};
#endif
