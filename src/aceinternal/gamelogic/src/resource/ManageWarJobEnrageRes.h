
#ifndef MANAGE_WAR_JOB_ENRAGE_RES_HPP
#define MANAGE_WAR_JOB_ENRAGE_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageWarJobEnrageRes
{
public:
	~ManageWarJobEnrageRes()
	{
		cleanMapSecondContent(m_war_job_enrage_res_info);
	}
public:
	bool loadWarJobEnrageContent(Document * xml_doc);

	const WarJobEnrageResInfoMap_t & getWarJobEnrageResInfo();
protected:
	bool loadWarJobInfo(Element * warjob_element);

private:
	WarJobEnrageResInfoMap_t m_war_job_enrage_res_info;
};
#endif