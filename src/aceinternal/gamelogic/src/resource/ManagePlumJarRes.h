#ifndef MANAGE_PLUM_JAR_RES_HPP
#define MANAGE_PLUM_JAR_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PlumJarRes.h"
class GAMELOGIC_EXOPRT ManagePlumJarRes
{
public:
	bool loadContent(Document * xml_doc);
	const PlumJarInfoMap_t & getPlumJarInfo();
protected:
	bool loadInfo(Element * element);
private:
	PlumJarInfoMap_t m_plum_jar_res_map;
};
#endif
