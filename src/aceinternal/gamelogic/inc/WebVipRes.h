#ifndef WEB_VIP_RES_HPP
#define WEB_VIP_RES_HPP
struct WebVipInfo
{
	WebVipInfo()
	: limit_require(0)
	, vip1_item(0)
	, vip_transform(0)
	, yearvip_item(0)
	{}
	uint32 limit_require;
	uint32 vip1_item;
	uint32 vip_transform;
	uint32 yearvip_item;
};
#endif
