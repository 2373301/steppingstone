
#ifndef DOUBLEEXP_RES_HPP
#define DOUBLEEXP_RES_HPP

struct DoubleexpRes
{
	DoubleexpRes()
	: id(0)
	, group(0)
	, level(0)
	, times(0)
	, mutil_value(0)
	{}

	uint32 id;
	uint32 group;
	uint32 level;
	uint32 times;
	uint32 mutil_value;
};

#endif