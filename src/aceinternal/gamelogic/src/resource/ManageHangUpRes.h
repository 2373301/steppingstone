
#ifndef MANAGE_HANG_UP_RES_HPP
#define MANAGE_HANG_UP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "HangUpRes.h"

class ManageHangUpRes
{
public:
	bool loadContent(Document * xml_doc);

	const HangUpRes & getHangUpRes();
protected:
	bool loadInfo(Element * element);

private:

	HangUpRes m_hang_up_res;
};
#endif