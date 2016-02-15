#ifndef MANAGE_MAZEROOM__RES_HPP
#define MANAGE_MAZEROOM__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MazeroomRes.h"
class GAMELOGIC_EXOPRT ManageMazeroomRes
{
public:
	bool loadContent(Document * xml_doc);
	const MazeroomInfoVec_t & getMazeroomInfo();
protected:
	bool loadInfo(Element * element);
private:
	MazeroomInfoVec_t m_Mazeroom__res_vec;
};
#endif
