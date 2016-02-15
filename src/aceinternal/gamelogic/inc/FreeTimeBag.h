
#ifndef FREE_TIME_BAG_HPP
#define FREE_TIME_BAG_HPP

struct FreeTimeBag
{
	FreeTimeBag()
		: id_value(0)
		, time_value(0)
	{}

	uint32 id_value;
	uint32 time_value;
};
#endif