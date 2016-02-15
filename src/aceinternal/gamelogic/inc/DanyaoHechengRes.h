#ifndef DANYAO_HECHENG_RES_HPP
#define DANYAO_HECHENG_RES_HPP
struct DanyaoHechengInfo
{
	DanyaoHechengInfo()
	: gold(0)
	, goldblessme_id(0)
	, id(0)
	, needitem(0)
	, neednum(0)
	{}
	uint32 gold;
	uint32 goldblessme_id;
	uint32 id;
	uint32 needitem;
	uint32 neednum;
};
#endif
