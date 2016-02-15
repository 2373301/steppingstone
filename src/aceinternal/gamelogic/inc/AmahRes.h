#ifndef AMAH__RES_HPP
#define AMAH__RES_HPP
struct AmahInfo
{
	AmahInfo()
	: goldblessme_id(0)
	, id(0)
	, level(0)
	, rewardrate(0)
	, sysname("")
	{}
	uint32 goldblessme_id;
	uint32 id;
	uint32 level;
	uint32 rewardrate;
	std::string sysname;
};
#endif
