#ifndef MANAGE_TAVERNPROBABILITY__RES_HPP
#define MANAGE_TAVERNPROBABILITY__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TavernprobabilityRes.h"
#include "share_fun.h"

#define DEL_POINTER(p) if(p!=NULL){ delete p; p=NULL; }

class GAMELOGIC_EXOPRT ManageTavernprobabilityRes
{
public:
	~ManageTavernprobabilityRes()
	{
		set<TavernprobabilityInfo *> deleted_set;
		for(TavernprobabilityInfoMap_t::iterator it = m_tavernprobability_res_map.begin(); it != m_tavernprobability_res_map.end(); ++it)
		{
			if (deleted_set.find(it->second) == deleted_set.end())
			{
				deleted_set.insert(it->second);
				DEL_POINTER(it->second);
			}
		}

		m_tavernprobability_res_map.clear();
	}
	bool loadContent(Document * xml_doc);
	const TavernprobabilityInfoMap_t & getTavernprobabilityInfo();
protected:
	bool loadInfo(Element * element);
private:
	TavernprobabilityInfoMap_t m_tavernprobability_res_map;
};
#endif
