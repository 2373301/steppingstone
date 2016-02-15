
#ifndef MANAGE_BUFFER_RES_HPP
#define MANAGE_BUFFER_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBufferRes
{
public:
	~ManageBufferRes()
	{
		cleanMapSecondContent(m_buffer_info_map);
	}
public:
	bool loadBufferContent(Document * xml_doc);

	const BufferResInfoMap_t & getBufferInfo();
protected:
	bool loadBufferInfo(Element * buffer_element);
private:
	BufferResInfoMap_t m_buffer_info_map;
};

#endif