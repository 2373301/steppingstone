#ifndef MANAGE_TAVERN__RES_HPP
#define MANAGE_TAVERN__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TavernRes.h"
#include "share_fun.h"

#define DEL_POINTER(p) if(p!=NULL){ delete p; p=NULL; }

class GAMELOGIC_EXOPRT ManageTavernRes
{
public:
	~ManageTavernRes()
	{
		set<TavernInfo *> deleted_set;
		for(TavernInfoMap_t::iterator it = m_tavern_res_map.begin(); it != m_tavern_res_map.end(); ++it)
		{
			if (deleted_set.find(it->second) == deleted_set.end())
			{
				deleted_set.insert(it->second);
				DEL_POINTER(it->second);
			}
		}
		m_tavern_res_map.clear();
	}
	bool loadContent(Document * xml_doc);
	const TavernInfoMap_t & getTavernInfo();
protected:
	bool loadInfo(Element * element);
private:
	TavernInfoMap_t m_tavern_res_map;
};
#endif
