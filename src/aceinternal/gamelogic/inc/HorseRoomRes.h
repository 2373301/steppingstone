#ifndef HORSE_ROOM_RES_HPP
#define HORSE_ROOM_RES_HPP
struct HorseRoomInfo
{
	HorseRoomInfo()
	: horse_id(0)
	, ranch_lv(0)
	{}
	uint32 horse_id;
	uint32 ranch_lv;
};
#endif
