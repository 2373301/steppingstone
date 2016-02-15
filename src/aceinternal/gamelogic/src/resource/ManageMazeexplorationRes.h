#ifndef MANAGE_MAZEEXPLORATION__RES_HPP
#define MANAGE_MAZEEXPLORATION__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MazeexplorationRes.h"
class GAMELOGIC_EXOPRT ManageMazeexplorationRes
{
public:
	bool loadContent(Document * xml_doc);
	const MazeexplorationInfo & getMazeexplorationInfo();
protected:
	bool loadInfo(Element * element);
private:
	MazeexplorationInfo m_Mazeexploration__res_;
};
#endif
