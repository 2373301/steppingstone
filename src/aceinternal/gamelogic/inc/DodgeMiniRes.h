#ifndef DODGE_MINI_RES_HPP
#define DODGE_MINI_RES_HPP
struct DodgeMiniInfo
{
	DodgeMiniInfo()
	: group(0)
	, id(0)
	, itemfor_id(0)
	, level(0)
	, needdodge_num(0)
	, max(0)
	, _type(0)
	, need_level(0)
	//, property_type1(0)
	//, property_type2(0)
	//, property_type3(0)
	//, property_type4(0)
	//, value1(0)
	//, value2(0)
	//, value3(0)
	//, value4(0)
	{}
	uint32 group;
	uint32 id;
	uint32 itemfor_id;
	uint32 level;
	uint32 needdodge_num;
	//uint32 property_type1;
	//uint32 property_type2;
	//uint32 property_type3;
	//uint32 property_type4;
	//uint32 value1;
	//uint32 value2;
	//uint32 value3;
	//uint32 value4;
	Uint32Vec_t property_add;
	Uint32Vec_t value_add;
	uint32 max;
	uint32 _type;
	uint32 need_level;
};
#endif
