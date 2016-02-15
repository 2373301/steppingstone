#ifndef MANAGE_MATEUP__RES_HPP
#define MANAGE_MATEUP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MateupRes.h"
class GAMELOGIC_EXOPRT ManageMateupRes
{
public:
	bool loadContent(Document * xml_doc);
	const MateupInfoMap_t & getMateupInfo();
protected:
	bool loadInfo(Element * element);
private:
	MateupInfoMap_t m_mateup__res_map;
};
#endif
