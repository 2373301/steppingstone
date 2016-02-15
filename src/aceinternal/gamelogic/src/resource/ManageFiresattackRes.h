#ifndef MANAGE_FIRESATTACK__RES_HPP
#define MANAGE_FIRESATTACK__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FiresattackRes.h"
class GAMELOGIC_EXOPRT ManageFiresattackRes
{
public:
	bool loadContent(Document * xml_doc);
	const FiresattackInfoMap_t & getFiresattackInfo();
protected:
	bool loadInfo(Element * element);
private:
	FiresattackInfoMap_t m_FiresAttack__res_map;
};
#endif
