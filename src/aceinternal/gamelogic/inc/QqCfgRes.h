#ifndef QQ_CFG_RES_HPP
#define QQ_CFG_RES_HPP
struct QqCfgInfo
{
	QqCfgInfo()
	: bonus(0)
	, diamond(0)
	, id(0)
	, level(0)
	, p(0)
	, spe_bonus(0)
	, type(0)
	, value(0)
	{}
	uint32 bonus;
	uint32 diamond;
	uint32 id;
	uint32 level;
	Uint32Vec_t p;
	uint32 spe_bonus;
	uint32 type;
	Int32Vec_t value;
};
#endif
