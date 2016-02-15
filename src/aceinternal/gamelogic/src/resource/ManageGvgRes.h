#ifndef MANAGE_GVG__RES_HPP
#define MANAGE_GVG__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GvgRes.h"

class GAMELOGIC_EXOPRT ManageGvgRes
{
public:
	bool loadContent(Document * xml_doc);
	const GvgInfo & getGvgInfo();
protected:
	bool loadInfo(Element * element);
private:
	GvgInfo m_gvg_res_map;
};
#endif
