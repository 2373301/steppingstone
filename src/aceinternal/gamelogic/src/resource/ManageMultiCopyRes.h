
#ifndef MANAGE_MULTI_COPY_RES_HPP
#define MANAGE_MULTI_COPY_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MultiCopyRes.h"
#include "share_fun.h"

class ManageMultiCopyRes
{
public:
	~ManageMultiCopyRes()
	{
		cleanMapSecondContent(m_multi_copy_res_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const MultiCopyResMap_t & getMultiCopyResMap();
protected:
	bool loadInfo(Element * element);

private:

	MultiCopyResMap_t m_multi_copy_res_map;
};
#endif