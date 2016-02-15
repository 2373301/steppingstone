
#ifndef MANAGE_RAISE_RES_PP
#define MANAGE_RAISE_RES_PP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRaiseRes
{
public:
	~ManageRaiseRes()
	{
		cleanVectorContent(m_raise_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const RaiseResInfoVec_t & getRaiseInfoVec();
protected:
	bool loadInfo(Element * element);

private:

	RaiseResInfoVec_t m_raise_info_vec;
};

#endif