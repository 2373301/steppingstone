
#ifndef MANAGE_FREE_TIME_BAG_HPP
#define MANAGE_FREE_TIME_BAG_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FlowerRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageFreeTimeBag
{
public:
	~ManageFreeTimeBag();
public:
	bool loadContent(Document * xml_doc);

	const FreeTimeBagVec_t & getFreeTimeBagVec();

protected:
	bool loadInfo(Element * element);
protected:
private:
	FreeTimeBagVec_t m_free_time_bag_vec;
};
#endif