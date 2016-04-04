
#include <ace/Time_Value.h>
#include <ace/Date_Time.h>
#include <ace/OS_NS_unistd.h>
#include <numeric>
#include "ManageOnlinePlayer.h"
#include "Logger.h"
#include "ManageConfig.h"
#include "Logger.h"

ManageOnlinePlayer::ManageOnlinePlayer()
: m_current_online_number(0)
, m_max_oinline_number(0)
, m_mysql_conn(NULL)
{}

ManageOnlinePlayer::~ManageOnlinePlayer()
{}

int ManageOnlinePlayer::init()
{
	const DbConfig & gmlog_db = ManageConfig::instance()->getGmlogDb();
	m_mysql_conn = new mysqlpp::Connection(false);
	if (!m_mysql_conn->connect(gmlog_db.db_name.c_str(), gmlog_db.ip.c_str(), gmlog_db.user_name.c_str(), gmlog_db.password.c_str(), gmlog_db.port))
	{
		DEF_LOG_ERROR("failed to connect the database <%s>, db <%s>\n", gmlog_db.ip.c_str(), gmlog_db.db_name.c_str());
		return -1;
	}

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active ManageOnlinePlayer\n");
		return -1;
	}

	return 0;
}

int ManageOnlinePlayer::svc()
{
	ACE_Time_Value sleep_time(1, 0);
	ACE_Time_Value last_update_time = ACE_OS::gettimeofday();
	int update_interval = 5 * 60;
	while (true)
	{
		ACE_Time_Value time_diff = ACE_OS::gettimeofday() - last_update_time;
		if (time_diff.sec() < update_interval)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		last_update_time = ACE_OS::gettimeofday();

		updateOnlineNumberToDb(m_current_online_number);
	}
	return 0;
}

void ManageOnlinePlayer::playerLoginSuccess(uint32 line_no, string account)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_player_mutex, );
	StringSet_t & online_set = m_online_player[line_no];

	StringSet_t::iterator it = online_set.find(account);
	if (it != online_set.end())
	{
		DEF_LOG_ERROR("player is online, but the player is already onlined : <%s>, line no is <%d>\n", account.c_str(), line_no);
	}
	else
	{
		online_set.insert(account);

		DEF_LOG_INFO("player is online : <%s>, line is <%d>\n", account.c_str(), line_no);
	}
	
	//1、2、3、4合区后，在线玩家只能统计到1区的，因为2、3、4区号id不同，特此修改
	//m_current_online_number = online_set.size();
	int online_number = 0;
	for (OnlinePlayer_t::iterator iter = m_online_player.begin(); iter != m_online_player.end(); ++iter)
	{
		online_number += iter->second.size();
	}
	m_current_online_number = online_number;

	if (m_current_online_number > m_max_oinline_number)
	{
		m_max_oinline_number = m_current_online_number;
		updateOnlineNumberToDb(m_max_oinline_number);
	}
}

void ManageOnlinePlayer::playerLoginoff(uint32 line_no, string account)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_player_mutex, );
	StringSet_t & online_set = m_online_player[line_no];

	StringSet_t::iterator it = online_set.find(account);
	if (it != online_set.end())
	{
		DEF_LOG_INFO("player is loginoff : <%s>, line is <%d>\n", account.c_str(), line_no);
		online_set.erase(it);
	}
	else
	{
		DEF_LOG_ERROR("player is logoff, but did not find the player : <%s>, line no is <%d>\n", account.c_str(), line_no);
	}
}

uint32 ManageOnlinePlayer::getOnlineNumber(uint32 line_no)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_player_mutex, 0);
	return m_online_player[line_no].size();
}

void ManageOnlinePlayer::handleReportOnlineNumber(typed::protocol::smsg_report_online_number & sr_online_number)
{
	DEF_LOG_INFO("recv report online number message, start to collect, size is <%d>\n", sr_online_number.online_info().size());
	for (int i = 0; i < sr_online_number.online_info_size(); ++i)
	{
		playerLoginSuccess(sr_online_number.online_info(i).line_id(), sr_online_number.online_info(i).account());
	}
	DEF_LOG_INFO("end to collect \n");
}

void SumPlayersNum(mysqlpp::StoreQueryResult &result, Uint32Vec_t &nums)
{
    mysqlpp::StoreQueryResult::iterator it;
    for( it = result.begin(); it != result.end(); ++it)
    {   
        mysqlpp::Row& row = *it;
        nums.push_back(row[0]);
    }
}

uint32 ManageOnlinePlayer::getDailyTopPlayers(string const& date)
{
    string start_date;
    start_date = date;
    start_date +="T00:00:00";
    string end_date;
    end_date = date;
    end_date += "T23:59:59";

    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mysql_conn_mutex, 0);
    mysqlpp::Query query = m_mysql_conn->query();
    query << "select number from online_number where date_time >= '";
    query << start_date;
    query << "' and date_time <= '";
    query << end_date << "'";

    mysqlpp::StoreQueryResult res = query.store();
    if (res)
    {
        Uint32Vec_t nums;
        SumPlayersNum(res, nums);
        std::sort(nums.begin(), nums.end(), std::greater<uint32>());
        if(!nums.empty())
            return nums[0];
    }

   return 0;
}

uint32 ManageOnlinePlayer::getDailyAveragePlayers(string const& date)
{
    string start_date;
    start_date = date;
    start_date +="T00:00:00";
    string end_date;
    end_date = date;
    end_date += "T23:59:59";

    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mysql_conn_mutex, 0);
    mysqlpp::Query query = m_mysql_conn->query();
    query << "select number from online_number where date_time >= '";
    query << start_date;
    query << "' and date_time <= '";
    query << end_date << "'";

    mysqlpp::StoreQueryResult res = query.store();
    if (res)
    {
        Uint32Vec_t nums;
        SumPlayersNum(res, nums);
        if(!nums.empty())
        {
            uint32 sum = std::accumulate(nums.begin(), nums.end(), 0);
            uint32 count = sum / nums.size();
            count += sum % nums.size() ? 1 : 0;
            return count;
        }
    }

    return 0;
}

void ManageOnlinePlayer::updateOnlineNumberToDb(int online_number)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mysql_conn_mutex, );
	mysqlpp::Query query = m_mysql_conn->query();
	string num_str = boost::lexical_cast<string>(online_number);
	
	ACE_Date_Time curr_time;
	string str_time;
	str_time.resize(50, '\0');
	sprintf((char *)str_time.data(), "%.04d-%.02d-%.02dT%.02d:%.02d:%.02d", curr_time.year(), curr_time.month(), curr_time.day(), curr_time.hour(), curr_time.minute(), curr_time.second());
	
	query << "insert into online_number(number, date_time) values(";
	query << num_str.c_str();
	query << ", '";
	query << str_time.c_str();
	query << "')";

	mysqlpp::SimpleResult res = query.execute();
	if (res)
	{
		// 
		DEF_LOG_INFO("success to update online player number to db <%d>\n", online_number);
	}
	else
	{
		DEF_LOG_ERROR("failed to update online player number to db <%d>\n", online_number);
	}
}