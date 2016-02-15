
#ifndef MANAGE_SYNTHESIS_RES_HPP
#define MANAGE_SYNTHESIS_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageSynthesisRes
{
public:
	~ManageSynthesisRes()
	{
		cleanMapSecondContent(m_synthesis_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const SynthesisResInfoMap_t & getSynthesisInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	SynthesisResInfoMap_t m_synthesis_info_map;
};

#endif