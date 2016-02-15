#ifndef MANAGE_FIRES__RES_HPP
#define MANAGE_FIRES__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FiresRes.h"
class GAMELOGIC_EXOPRT ManageFiresRes
{
public:
	bool loadContent(Document * xml_doc);
	const FiresInfo & getFiresInfo();
protected:
	bool loadInfo(Element * element);
private:
	FiresInfo m_Fires__res_;
};
#endif
