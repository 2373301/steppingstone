#ifndef MANAGE_OPENACT2__RES_HPP
#define MANAGE_OPENACT2__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Openact2Res.h"
class GAMELOGIC_EXOPRT ManageOpenact2Res
{
public:
	bool loadContent(Document * xml_doc);
	const Openact2InfoVec_t & getOpenact2Info();
protected:
	bool loadInfo(Element * element);
private:
	Openact2InfoVec_t m_openact2__res_vec;
};
#endif
