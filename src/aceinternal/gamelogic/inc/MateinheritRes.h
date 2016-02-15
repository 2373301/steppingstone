#ifndef MATEINHERIT__RES_HPP
#define MATEINHERIT__RES_HPP
struct MateinheritInfo
{
	MateinheritInfo()
	: att_inherit(0)
	, exp_inherit(0)
	, id(0)
	{}
	uint32 att_inherit;
	uint32 exp_inherit;
	uint32 id;
};
#endif
