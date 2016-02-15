#ifndef MANAGE_SHL_UNLOCK_RES_HPP
#define MANAGE_SHL_UNLOCK_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "ShlUnlockRes.h"
class GAMELOGIC_EXOPRT ManageShlUnlockRes
{
public:
	bool loadContent(Document * xml_doc);
	const ShlUnlockInfoVec_t & getShlUnlockInfo();
protected:
	bool loadInfo(Element * element);
private:
	ShlUnlockInfoVec_t m_SHL_Unlock_res_vec;
};
#endif
