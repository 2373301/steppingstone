#ifndef MANAGE_TALENTSYSTEM__RES_HPP
#define MANAGE_TALENTSYSTEM__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TalentsystemRes.h"
class GAMELOGIC_EXOPRT ManageTalentsystemRes
{
public:
	bool loadContent(Document * xml_doc);
	const TalentsystemInfo & getTalentsystemInfo();
protected:
	bool loadInfo(Element * element);
private:
	TalentsystemInfo m_TalentSystem__res_;
};
#endif
