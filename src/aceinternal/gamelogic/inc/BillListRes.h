#ifndef BILL_LIST_RES_HPP
#define BILL_LIST_RES_HPP
struct BillListInfo
{
	BillListInfo()
	: end_time(0)
	, id(0)
	, start_time(0)
	{}
	Uint32Vec_t bonus;
	uint32 end_time;
	uint32 id;
	Uint32Vec_t max;
	uint32 start_time;
	Uint32Vec_t target_id;
	Uint32Vec_t target_max;
	Uint32Vec_t target_min;
};
#endif
