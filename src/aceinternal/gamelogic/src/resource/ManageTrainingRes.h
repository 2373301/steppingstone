#ifndef MANAGE_TRAINING_RES_HPP
#define MANAGE_TRAINING_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TrainingRes.h"
#include "share_fun.h"


class GAMELOGIC_EXOPRT ManageTrainingRes
{
public:
	bool loadTrainingPartnersInfo(Document * xml_doc);
	bool loadTrainingLevelTable(Document * xml_doc);
	~ManageTrainingRes();

public:
	const TrainingPartnersInfoMap_t& getTrainingPartnersInfoMap();
	const TrainingQualityMap_t& getTrainingQualityMap();


private:
	bool  loadPartnerInfo(Element* element);
	bool  loadLevelInfo(Element* element);

	
private:
	TrainingPartnersInfoMap_t m_partnersInfoMap;
	TrainingQualityMap_t qualityMap_;
	

};


#endif