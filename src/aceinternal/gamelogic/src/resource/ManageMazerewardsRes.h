#ifndef MANAGE_MAZEREWARDS__RES_HPP
#define MANAGE_MAZEREWARDS__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MazerewardsRes.h"
class GAMELOGIC_EXOPRT ManageMazerewardsRes
{
public:
	bool loadContent(Document * xml_doc);
	const MazerewardsInfoVec_t & getMazerewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	MazerewardsInfoVec_t m_Mazerewards__res_vec;
};
#endif
