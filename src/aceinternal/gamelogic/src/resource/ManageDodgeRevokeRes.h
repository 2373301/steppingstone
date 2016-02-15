#ifndef MANAGE_DODGE_REVOKE_RES_HPP
#define MANAGE_DODGE_REVOKE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DodgeRevokeRes.h"
class GAMELOGIC_EXOPRT ManageDodgeRevokeRes
{
public:
	bool loadContent(Document * xml_doc);
	const DodgeRevokeInfo & getDodgeRevokeInfo();
protected:
	bool loadInfo(Element * element);
private:
	DodgeRevokeInfo m_dodge_revoke_res;
};
#endif
