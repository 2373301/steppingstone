
#ifndef MANAGE_FORM_RES_HPP
#define MANAGE_FORM_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageFormRes
{
public:
	~ManageFormRes();
public:
	bool loadFormContent(Document * xml_doc);

	const FormResInfoMap_t & getFormInfoMap();
protected:
	bool loadFormInfo(Element * form_element);
private:
	FormResInfoMap_t m_form_res_info_map;
};

#endif