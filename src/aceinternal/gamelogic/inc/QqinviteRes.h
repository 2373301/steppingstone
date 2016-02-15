#ifndef QQINVITE__RES_HPP
#define QQINVITE__RES_HPP
struct QqinviteInfo
{
	QqinviteInfo()
	: bonus(0)
	, desc(0)
	, id(0)
	, img(0)
	, msg(0)
	, sort(0)
	, type(0)
	, value1(0)
	, value2(0)
	{}
	uint32 bonus;
	uint32 desc;
	uint32 id;
	Uint32Vec_t img;
	uint32 msg;
	uint32 sort;
	uint32 type;
	uint32 value1;
	uint32 value2;
};
#endif
