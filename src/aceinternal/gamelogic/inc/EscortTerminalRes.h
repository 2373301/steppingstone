#ifndef ESCORT_TERMINAL_RES_HPP
#define ESCORT_TERMINAL_RES_HPP
struct EscortTerminalInfo
{
	EscortTerminalInfo()
	: id(0)
	,time(0)
	,unlock(0)
	{}
	uint32 id;
	Uint32Vec_t level;
	Uint32Vec_t reward_money;
	Uint32Vec_t reward_shengwang;
	Uint32Vec_t reward_yueli;
	uint32 time;
	uint32 unlock;
};
#endif
