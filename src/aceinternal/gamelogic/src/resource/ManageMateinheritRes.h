#ifndef MANAGE_MATEINHERIT__RES_HPP
#define MANAGE_MATEINHERIT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MateinheritRes.h"
class GAMELOGIC_EXOPRT ManageMateinheritRes
{
public:
	ManageMateinheritRes();
	bool loadContent(Document * xml_doc);
	const MateinheritInfoMap_t & getMateinheritInfo();
protected:
	bool loadInfo(Element * element);
private:
	MateinheritInfoMap_t m_mateinherit__res_map;
};
#endif
