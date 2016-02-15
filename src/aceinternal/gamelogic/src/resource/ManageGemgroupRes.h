#ifndef MANAGE_GEMGROUP__RES_HPP
#define MANAGE_GEMGROUP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GemgroupRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageGemgroupRes
{
public:
	~ManageGemgroupRes()
	{
		cleanVectorContent(m_gemgroup__res_vec);
	}

	bool loadContent(Document * xml_doc);
	const GemgroupInfoVec_t & getGemgroupInfo();
protected:
	bool loadInfo(Element * element);
private:
	GemgroupInfoVec_t m_gemgroup__res_vec;
};
#endif
