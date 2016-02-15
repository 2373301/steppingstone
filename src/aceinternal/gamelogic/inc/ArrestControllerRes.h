#ifndef ARREST_CONTROLLER_RES_HPP
#define ARREST_CONTROLLER_RES_HPP
struct ArrestControllerInfo
{
	ArrestControllerInfo()
	: limit(0)
	, sundry(0)
	, type(0)
	{}
	Uint32Vec_t goldblessme_id;
	uint32 limit;
	uint32 sundry;
	uint32 type;
	Uint32Vec_t vip_id1;
	Uint32Vec_t vip_id2;
};
#endif
