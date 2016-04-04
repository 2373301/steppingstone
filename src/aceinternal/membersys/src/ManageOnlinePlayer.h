
#ifndef MANAGE_ONLINE_PLAYER_HPP
#define MANAGE_ONLINE_PLAYER_HPP

#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Task.h>
#include "typedef.h"
#include "Singleton.h"
#include "msg_member.pb.h"
#include <mysql++.h>

class ManageOnlinePlayer : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageOnlinePlayer();
	~ManageOnlinePlayer();
public:
	static ManageOnlinePlayer * instance()
	{
		return Singleton<ManageOnlinePlayer>::instance();
	}
public:
	int init();

	int svc();
public:
	void playerLoginSuccess(uint32 line_no, string account);

	void playerLoginoff(uint32 line_no, string account);

	uint32 getOnlineNumber(uint32 line_no);

	void handleReportOnlineNumber(typed::protocol::smsg_report_online_number & sr_online_number);

    uint32 getDailyTopPlayers(string const& date);
    uint32 getDailyAveragePlayers(string const& date);

protected:
	void updateOnlineNumberToDb(int online_number);
private:
	typedef define_unordered_set<string> StringSet_t;

	typedef define_unordered_map<uint32, StringSet_t> OnlinePlayer_t;

	OnlinePlayer_t m_online_player;

	ACE_Thread_Mutex m_online_player_mutex;

	int m_current_online_number;

	int m_max_oinline_number;

	mysqlpp::Connection * m_mysql_conn;

	ACE_Thread_Mutex m_mysql_conn_mutex;
};
#endif