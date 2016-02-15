#ifndef MANAGE_DANYAO_HECHENG_RES_HPP
#define MANAGE_DANYAO_HECHENG_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DanyaoHechengRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageDanyaoHechengRes
{
public:
	~ManageDanyaoHechengRes()
	{
		cleanMapSecondContent(m_danyao_hecheng_res_map);
	}
	bool loadContent(Document * xml_doc);
	const DanyaoHechengInfoMap_t & getDanyaoHechengInfo();
protected:
	bool loadInfo(Element * element);
private:
	DanyaoHechengInfoMap_t m_danyao_hecheng_res_map;
};
#endif
