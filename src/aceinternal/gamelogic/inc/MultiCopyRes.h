
#ifndef MULTI_COPY_RES_HPP
#define MULTI_COPY_RES_HPP

struct MultiCopyRes
{
	MultiCopyRes()
		: id(0)
		, level(0)
		, unlock(0)
		, times(0)
		, energy(0)
		//, bonus1(0)
		//, bonus2(0)
	{}
	uint32 id;
	uint32 level;
	uint32 unlock;
	uint32 times;
	uint32 energy;
	Uint32Vec_t bonus1;
	Uint32Vec_t bonus2;
	Uint32Vec_t enemy;
};

#endif