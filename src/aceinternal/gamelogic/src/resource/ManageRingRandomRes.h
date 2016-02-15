#ifndef MANAGE_RING_RANDOM_RES_HPP
#define MANAGE_RING_RANDOM_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RingRandomRes.h"
class GAMELOGIC_EXOPRT ManageRingRandomRes
{
public:
	bool loadContent(Document * xml_doc);
	const RingRandomInfoVec_t & getRingRandomInfo();
protected:
	bool loadInfo(Element * element);
private:
	RingRandomInfoVec_t m_ring_random_res_vec;
};
#endif
