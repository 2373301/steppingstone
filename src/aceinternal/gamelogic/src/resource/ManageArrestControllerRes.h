#ifndef MANAGE_ARREST_CONTROLLER_RES_HPP
#define MANAGE_ARREST_CONTROLLER_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ArrestControllerRes.h"
class GAMELOGIC_EXOPRT ManageArrestControllerRes
{
public:
	bool loadContent(Document * xml_doc);
	const ArrestControllerInfoVec_t & getArrestControllerInfo();
protected:
	bool loadInfo(Element * element);
private:
	ArrestControllerInfoVec_t m_Arrest_Controller_res_vec;
};
#endif
