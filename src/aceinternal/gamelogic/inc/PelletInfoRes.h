
#ifndef PELLET_INFO_RES_HPP
#define PELLET_INFO_RES_HPP

struct PelletInfoRes
{
	PelletInfoRes()
	: type_id(0)
	, grade(0)
	, need_item(0)
	, need_level(0)
	, max_number(0)
	, reward_property_id(0)
	, reward_property_value(0)
	, goldblessme_id(0)
	, property_id(0)
	, property_value(0)
	{}

	uint32 type_id;
	uint32 grade;
	uint32 need_item;
	uint32 need_level;
	uint32 max_number;
	uint32 reward_property_id;
	uint32 reward_property_value;
	uint32 goldblessme_id;
	uint32 property_id;
	uint32 property_value;
};

#endif
