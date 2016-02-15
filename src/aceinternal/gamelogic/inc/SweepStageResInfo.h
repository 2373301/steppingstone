
#ifndef SWEEP_STAGE_RES_INFO_HPP
#define SWEEP_STAGE_RES_INFO_HPP

struct SweepStageResInfo
{
	SweepStageResInfo()
	: sweep_interval(0)
	, tired_interval(0)
	, total_tired_time(0)
	{}

	uint32 sweep_interval;
	uint32 tired_interval;
	uint32 total_tired_time;
};

#endif