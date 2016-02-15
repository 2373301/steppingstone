
#ifndef MANAGE_BOSS_EXP_RES_HPP
#define MANAGE_BOSS_EXP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBossExpRes
{
public:
	~ManageBossExpRes()
	{
		cleanVectorContent(m_boss_exp_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const BossExpResInfoVec_t & getBossExpInfo();
protected:
	bool loadInfo(Element * element);

private:

	BossExpResInfoVec_t m_boss_exp_info_vec;
};

#endif