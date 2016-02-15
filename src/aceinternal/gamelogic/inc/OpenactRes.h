#ifndef OPENACT__RES_HPP
#define OPENACT__RES_HPP
struct OpenactInfo
{
	OpenactInfo()
	: id(0)
	, type(0)
	, startDay(0)
	, endDay(0)
	, start_time(0)
	, end_time(0)
	{}
	
	uint32 id;
	uint32 startDay; // 0：代表当天
	uint32 start_time; // 相对于0点的秒数
	uint32 endDay; // 0 :代表不结束
	uint64 end_time; // 相对于0点的秒数
	uint32 type;
	Uint32Vec_t value_a;
	Uint32Vec_t value_b;
	Uint32Vec_t rewards;
};
#endif
