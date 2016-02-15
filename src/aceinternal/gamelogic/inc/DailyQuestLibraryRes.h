
#ifndef DAILY_QUEST_LIBRARY_RES_HPP
#define DAILY_QUEST_LIBRARY_RES_HPP

struct DailyQuestLibraryInfo
{
	DailyQuestLibraryInfo()
	: id(0)
	, lv(0)
	, num(0)
	{
	}
	uint32 id;
	uint32 lv;
	uint32 num;
	Uint32Vec_t vec_quest;
};

#endif