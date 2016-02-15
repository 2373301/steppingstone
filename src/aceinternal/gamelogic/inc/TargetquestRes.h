#ifndef TARGETQUEST__RES_HPP
#define TARGETQUEST__RES_HPP
struct TargetquestInfo
{
	TargetquestInfo()
	: a(0)
	, b(0)
	, group(0)
	, id(0)
	, land(0)
	, needlevel(0)
	, quest(0)
	, type(0)
	, value(0)
	{}
	uint32 a;
	uint32 b;
	uint32 group;
	uint32 id;
	uint32 land;
	uint32 needlevel;
	uint32 quest;
	Uint32Vec_t reward;
	Uint32Vec_t reward_num;
	uint32 type;
	uint32 value;
};
#endif
