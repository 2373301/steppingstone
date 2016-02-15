#ifndef MANAGE_GUILD_RES_H_
#define MANAGE_GUILD_RES_H_

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GuildRes.h"
#include "share_fun.h"

class ManageGuildRes
{
public:
	~ManageGuildRes()
	{
		cleanMapSecondContent(m_guild_nity_map);
	}
public:
	bool loadGuildNity(Document * xml_doc);
	bool loadGuildActivity(Document * xml_doc);

	const Guild_NityMap_t & getGuildNityMap();
	const Guild_ActivityVector_t & getGuildActivityVector();

protected:
	bool loadGuildExpInfo(Element * element);
	bool loadGuildNityMapInfo(Element * element);
	bool loadGuildActivityInfo(Element * element);

private:

	Guild_NityMap_t  m_guild_nity_map;
	Guild_ActivityVector_t m_guild_activity_vector;
};

#endif