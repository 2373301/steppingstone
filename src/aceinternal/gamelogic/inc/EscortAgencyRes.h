#ifndef ESCORT_AGENCY_RES_HPP
#define ESCORT_AGENCY_RES_HPP
struct EscortAgencyInfo
{
	EscortAgencyInfo()
	: goldblessme_id(0)
	,id(0)
	,level(0)
	,nature(0)
	,_type(0)
	,is_max(0)
	,buff_id(0)
	{}
	uint32 goldblessme_id;
	uint32 id;
	uint32 level;
	uint32 nature;
	uint32 _type;
	uint32 is_max;
	uint32 buff_id;
};
#endif
