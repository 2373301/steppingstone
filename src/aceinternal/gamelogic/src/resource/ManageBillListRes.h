#ifndef MANAGE_BILL_LIST_RES_HPP
#define MANAGE_BILL_LIST_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "BillListRes.h"
class GAMELOGIC_EXOPRT ManageBillListRes
{
public:
	bool loadContent(Document * xml_doc);
	const BillListInfoMap_t & getBillListInfo();
protected:
	bool loadInfo(Element * element);
private:
	BillListInfoMap_t m_Bill_List_res_map;
};
#endif
