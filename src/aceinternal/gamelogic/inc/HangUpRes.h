
#ifndef HANG_UP_RES_HPP
#define HANG_UP_RES_HPP

struct HangUpRes
{
	HangUpRes()
		: a(0)
		, b(0)
		, energy(0)
	{}

	
	uint32 a;
	uint32 b;
	uint32 energy;
};
#endif