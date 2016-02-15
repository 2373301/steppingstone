
#ifndef MANAGE_EXP_ADDITION_HPP
#define MANAGE_EXP_ADDITION_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageExpAddition
{
public:
	~ManageExpAddition()
	{
		cleanMapSecondContent(m_exp_addition_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const ExpAdditionResInfoMap_t & getExpAdditionInfo();
protected:
	bool loadInfo(Element * element);
private:
	ExpAdditionResInfoMap_t m_exp_addition_info_map;
};

#endif