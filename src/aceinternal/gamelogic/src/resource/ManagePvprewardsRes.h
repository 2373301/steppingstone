#ifndef MANAGE_PVPREWARDS__RES_HPP
#define MANAGE_PVPREWARDS__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PvprewardsRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManagePvprewardsRes
{
public:
	~ManagePvprewardsRes()
	{
		cleanVectorContent(m_pvprewards__res_vec);
	}
	bool loadContent(Document * xml_doc);
	const PvprewardsInfoVec_t & getPvprewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	PvprewardsInfoVec_t m_pvprewards__res_vec;
};
#endif
