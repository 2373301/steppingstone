#ifndef ACHIEVEMENT_RES_HPP
#define ACHIEVEMENT_RES_HPP

struct AchievementUnlock
{
	AchievementUnlock()
		: functionid(0)
		, gold_need(0)
	{}

	uint32 functionid;
	uint32 gold_need;
	string options;
};

struct AchievementInfo
{
	AchievementInfo()
		: id(0)
		, type(0)
		, fun1(0)
		, fun2(0)
		, bouns(0)
		, point(0)
		,unlock(0)
	{}

	uint32 id;
	uint32 type;
	uint32 fun1;
	uint32 fun2;
	uint32 bouns;
	uint32 point;
	uint32 unlock;
};



struct AchievementReward
{
	AchievementReward()
		: id(0)
		, point(0)
		, bonus(0)
		, block(0)
		,dailybonus(0)
	{}

	uint32 id;
	uint32 point;
	uint32 bonus;
	uint32 block;
	uint32 dailybonus;
};

#endif