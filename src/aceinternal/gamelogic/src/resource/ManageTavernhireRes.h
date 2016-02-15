#ifndef MANAGE_TAVERNHIRE__RES_HPP
#define MANAGE_TAVERNHIRE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TavernhireRes.h"
#include "share_fun.h"

#define DEL_POINTER(p) if(p!=NULL){ delete p; p=NULL; }

class GAMELOGIC_EXOPRT ManageTavernhireRes
{
public:
	~ManageTavernhireRes()
	{
		set<TavernhireInfo*> deleted_set;
		for(TavernhireInfoMap_t::iterator it = m_tavernhire_res_map.begin(); it != m_tavernhire_res_map.end(); ++it)
		{
			if (deleted_set.find(it->second) == deleted_set.end())
			{
				deleted_set.insert(it->second);
				DEL_POINTER(it->second);
			}
		}
		m_tavernhire_res_map.clear();
	}
	bool loadContent(Document * xml_doc);
	const TavernhireInfoMap_t & getTavernhireInfo();
protected:
	bool loadInfo(Element * element);
private:
	TavernhireInfoMap_t m_tavernhire_res_map;
};
#endif
