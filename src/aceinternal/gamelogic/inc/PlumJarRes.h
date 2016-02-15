#ifndef PLUM_JAR_RES_HPP
#define PLUM_JAR_RES_HPP
struct PlumJarInfo
{
	PlumJarInfo()
	: item_id(0)
	, level_group(0)
	, need_capacity(0)
	, probability(0)
	, _type(0)
	{}
	uint32 item_id;
	uint32 level_group;
	uint32 need_capacity;
	uint32 probability;
	uint32 _type;
};
#endif
