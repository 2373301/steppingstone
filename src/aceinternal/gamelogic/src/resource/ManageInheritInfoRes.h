
#ifndef MANAGE_INHERIT_INFO_RES_HPP
#define MANAGE_INHERIT_INFO_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "InheritInfoRes.h"

class GAMELOGIC_EXOPRT ManageInheritInfoRes
{
public:
	bool loadContent(Document * xml_doc);
	const InheritInfoRes & getInheritInfoRes();
protected:
	bool loadInfo(Element * element);
private:
	InheritInfoRes m_inherit_info_res; 
};

#endif
