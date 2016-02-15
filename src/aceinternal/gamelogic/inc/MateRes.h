#ifndef MATE__RES_HPP
#define MATE__RES_HPP
struct MateInfo
{
	MateInfo()
	: id(0)
	, role_id(0)
	, share_damage(0)
	, skill_id(0)
	, up_level(0)
	{}
	uint32 id;
	uint32 role_id;
	uint32 share_damage;
	uint32 skill_id;
	uint32 up_level;
	uint32 matecoin;
	uint32 mateyinling;
	uint32 limitcoin;
	uint32 limityinliang;
	uint32 isdefault;
	std::string name;
	uint32 color;
	uint32 hp;
	uint32 hp_grow;
};
#endif
