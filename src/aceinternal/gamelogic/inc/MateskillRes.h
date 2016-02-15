#ifndef MATESKILL__RES_HPP
#define MATESKILL__RES_HPP
struct MateskillInfo
{
	MateskillInfo()
	: next_id(0)
	, skill_id(0)
	{}
	uint32 next_id;
	uint32 skill_id;
};
#endif
