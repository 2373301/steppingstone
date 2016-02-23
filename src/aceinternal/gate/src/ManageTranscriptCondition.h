
#ifndef MANAGE_TRANSCRIPT_CONDITION_HPP
#define MANAGE_TRANSCRIPT_CONDITION_HPP

#include <string>
#include <vector>
#include <map>
#include "SimpleXML.h"
#include "typedef.h"
#include "AppNotifyInterface.h"
#include "Singleton.h"

using namespace std;

enum TranscriptCheckResult
{
	TCR_PASS = 0,
	TCR_MAP_DISABLE,
	TCR_NOT_IN_TEAM,
	TCR_NOT_LEADER,
	TCR_ROLE_LEVEL,
	TCR_ROLE_SAME_MAP,
	TCR_TICKET_MISS,
	TCR_KEY_MISS,
	TCR_MEMBER_AMOUNT,
	TCR_INTER_ERROR
};

enum WeekDay
{
	WD_SUN = 0,
	WD_MON,
	WD_TUE,
	WD_WED,
	WD_THU,
	WD_FRI,
	WD_SAT
};

#define WEEKDAY_SUN 0x1
#define WEEKDAY_MON 0x2
#define WEEKDAY_TUE 0x4
#define WEEKDAY_WED 0x8
#define WEEKDAY_THU 0x10
#define WEEKDAY_FRI 0x20
#define WEEKDAY_SAT 0x40

struct Ticket 
{
	Ticket()
	{}

	int id;
	int sum;
};

struct Outpoint 
{
	Outpoint()
	: x(0)
	, y(0)
	, map_template(0)
//	, map_instance(0)
	{}

	int x;
	int y;
	int map_template;
	//uint64 map_instance;
};

struct TranscriptCondition
{
	TranscriptCondition()
	:id(0)
	, min_level(0)
	, max_level(0)
	, open_day(0)
	, min_pc(0)
	, max_pc(0)
	, save_time(0)
	{}

	int		id;
	int		min_level;
	int		max_level;
	int		open_day;
	int     min_pc;
	int     max_pc;
	int		save_time;
	string	name;
	long	begin_time;
	long	end_time;
	Outpoint out_point;
	vector<int> keys;
	vector<Ticket> tickets;
};

class ManageTranscriptCondition
{
public:
	ManageTranscriptCondition();
	~ManageTranscriptCondition();
public:
	static ManageTranscriptCondition * instance()
	{
		return Singleton<ManageTranscriptCondition>::instance();
	}
public:
	bool init(const string & transcript_condition_file);

	TranscriptCondition * getTranscriptCondition(int map_template);

protected:
	bool loadTranscript(Element * root_element);

	bool loadKeys(Element * transcript_element, vector<int> & keys);

	bool loadTickets(Element * transcript_element, vector<Ticket> & tickets);

	bool loadOutpoint(Element * transcript_element, Outpoint & out_point);
protected:
private:
	typedef map<int, TranscriptCondition> TranscriptConditionMap_t;

	TranscriptConditionMap_t m_transcript_condition_map;
};

#endif