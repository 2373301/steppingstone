#ifndef MANAGE_MAIL__RES_HPP
#define MANAGE_MAIL__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MailRes.h"
class GAMELOGIC_EXOPRT ManageMailRes
{
public:
	bool loadContent(Document * xml_doc);
	const MailInfoMap_t & getMailInfo();
protected:
	bool loadInfo(Element * element);
private:
	MailInfoMap_t m_mail__res_map;
};
#endif
