#ifndef MANAGE_MAIL_FORMAT__RES_HPP
#define MANAGE_MAIL_FORMAT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MailFormatRes.h"
class GAMELOGIC_EXOPRT ManageMail_formatRes
{
public:
	bool loadContent(Document * xml_doc);
	const Mail_formatInfoMap_t & getMail_formatInfo();
protected:
	bool loadInfo(Element * element);
private:
	Mail_formatInfoMap_t m_mail_format__res_map;
};
#endif
