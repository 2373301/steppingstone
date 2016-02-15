#ifndef MANAGE_MAZE__RES_HPP
#define MANAGE_MAZE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MazeRes.h"
class GAMELOGIC_EXOPRT ManageMazeRes
{
public:
	bool loadContent(Document * xml_doc);
	const MazeInfoMap_t & getMazeInfo();
protected:
	bool loadInfo(Element * element);
private:
	MazeInfoMap_t m_Maze__res_map; 
};
#endif
