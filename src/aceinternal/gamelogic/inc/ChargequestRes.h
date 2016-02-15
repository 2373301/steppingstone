#ifndef CHARGEQUEST__RES_HPP
#define CHARGEQUEST__RES_HPP
struct ChargequestInfo
{
	ChargequestInfo()
	: complete_coin(0)
	, id(0)
	, quest_name(0)
	, questdesc(0)
	, reward(0)
	, star_id(0)
	, star_type(0)
	{}
	uint32 complete_coin;
	uint32 id;
	uint32 quest_name;
	uint32 questdesc;
	uint32 reward;
	uint32 star_id;
	uint32 star_type;
};
#endif
