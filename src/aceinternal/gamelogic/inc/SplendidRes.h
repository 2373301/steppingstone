
#ifndef SPLENDID_RES_HPP
#define SPLENDID_RES_HPP

struct SplendidVoteTime 
{
	SplendidVoteTime()
	: id(0)
	, type_value(0)
	, start_time(0)
	, period_time(0)
	, min_time(0)
	{}

	uint32 id;
	uint32 type_value;
	uint32 start_time;
	Int32Vec_t vote_start_time;
	Int32Vec_t vote_end_time;
	uint32 period_time;
	uint32 min_time;
};

struct SplendidRankReward
{
	SplendidRankReward()
	: id(0)
	, bonus_id(0)
	{}

	uint32 id;
	uint32 bonus_id;
};

struct SplendidWorshipReward 
{
	SplendidWorshipReward()
	: level(0)
	, worship_reward(0)
	, be_worshiped_reward(0)
	, vote_reward(0)
	{}
	
	uint32 level;
	uint32 worship_reward;
	uint32 be_worshiped_reward;
	Uint32Vec_t worship_num_vec;
	Uint32Vec_t worship_num_reward_vec;
	uint32 vote_reward;
	uint32 worshiped_limit;
};

#endif