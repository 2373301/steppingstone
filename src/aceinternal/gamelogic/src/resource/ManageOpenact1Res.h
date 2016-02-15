#ifndef MANAGE_OPENACT1__RES_HPP
#define MANAGE_OPENACT1__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Openact1Res.h"
class GAMELOGIC_EXOPRT ManageOpenact1Res
{
public:
	bool loadContent(Document * xml_doc);
	const Openact1InfoVec_t & getOpenact1Info();
protected:
	bool loadInfo(Element * element);
private:
	Openact1InfoVec_t m_openact1__res_vec;
};
#endif
