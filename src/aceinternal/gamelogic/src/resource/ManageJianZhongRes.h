#ifndef MANAGE_JIAN_ZHONG_RES_HPP
#define MANAGE_JIAN_ZHONG_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "JianZhongRes.h"
class GAMELOGIC_EXOPRT ManageJianZhongRes
{
public:
	bool loadContent(Document * xml_doc);
	const JianZhongInfoMap_t & getJianZhongInfo();
protected:
	bool loadInfo(Element * element);
private:
	JianZhongInfoMap_t m_Jian_Zhong_res_map;
};
#endif
