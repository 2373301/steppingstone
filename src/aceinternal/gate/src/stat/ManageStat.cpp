
#include <ace/OS.h>
#include "ManageStat.h"
#include "ManageConfig.h"

ManageStat::ManageStat()
: m_stop(false)
, m_wait(true)
, m_report_interval(20)
{

}

ManageStat::~ManageStat()
{

}

int ManageStat::svc(void)
{
	FUNCTIONTRACE(ManageStat::svc);
	REPORT_THREAD_INFO("ManageStat::svc", ACE_OS::thr_self());
	while (!m_stop)
	{
		ACE_OS::sleep(m_report_interval);

		ACE_Time_Value current_time = ACE_OS::gettimeofday();

		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));
		GATE_LOG_INFO(ACE_TEXT("                      report login auth info                             \n"));
		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));
		GATE_LOG_INFO(ACE_TEXT("login auth number is :             %u\n"), m_login_stat.recv_auth_number);
		GATE_LOG_INFO(ACE_TEXT("login auth success number is :     %u\n"), m_login_stat.auth_success_number);
		GATE_LOG_INFO(ACE_TEXT("login auth failed number is :      %u\n"), m_login_stat.auth_failed_number);

		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));
		GATE_LOG_INFO(ACE_TEXT("                      report client stat info                             \n"));
		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));

		GATE_LOG_INFO(ACE_TEXT("*****************  report client input network traffic : client  *********\n"));
		ACE_Time_Value diff_time = current_time - m_client_input_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("client input package amount is :   %u\n"), m_client_input_traffic.packet_number);
		GATE_LOG_INFO(ACE_TEXT("client input byte amount is :      %.4f K\n"), m_client_input_traffic.byte_number / 1000.0);
		GATE_LOG_INFO(ACE_TEXT("client input packet speed is :     %.4f\n"), (double)m_client_input_traffic.packet_number / diff_time.sec());
		GATE_LOG_INFO(ACE_TEXT("client input byte speed is :       %.4f K\n"), (double)m_client_input_traffic.byte_number / diff_time.sec() / 1000);
		if (m_client_input_traffic.role_make_total_number > 0)
		{
		GATE_LOG_INFO(ACE_TEXT("role make average exe time is :    %.4f \n"), (double)m_client_input_traffic.role_make_total_time / m_client_input_traffic.role_make_total_number / 1000);
		}
		GATE_LOG_INFO(ACE_TEXT("role make exe number is :          %u\n"), m_client_input_traffic.role_make_total_number);

		GATE_LOG_INFO(ACE_TEXT("*****************  report client output network traffic : client  ********\n"));
		diff_time = current_time - m_client_output_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("client output package amount is :  %u\n"), m_client_output_traffic.packet_number);
		GATE_LOG_INFO(ACE_TEXT("client output byte amount is :     %.4f K\n"), m_client_output_traffic.byte_number / 1000.0);
		GATE_LOG_INFO(ACE_TEXT("client output packet speed is :    %.4f\n"), (double)m_client_output_traffic.packet_number / diff_time.sec());
		GATE_LOG_INFO(ACE_TEXT("client output byte speed is :      %.4f K\n"), (double)m_client_output_traffic.byte_number / diff_time.sec() / 1000);
		GATE_LOG_INFO(ACE_TEXT("output packet loss number is :     %u\n"), m_client_output_traffic.packet_loss);
		GATE_LOG_INFO(ACE_TEXT("current wobble network number is : %u\n"), m_client_output_traffic.current_wobble_network);
		GATE_LOG_INFO(ACE_TEXT("max wobble network number is :     %u\n"), m_client_output_traffic.max_wobble_network);

		GATE_LOG_INFO(ACE_TEXT("*****************  report client connection network traffic : client  ****\n"));
		diff_time = current_time - m_client_output_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("client online amount is :          %d\n"), m_client_connection.connection_number - m_client_connection.disconnection_number);
		GATE_LOG_INFO(ACE_TEXT("client max online amount is :      %d\n"), m_client_connection.max_online_connection);
		GATE_LOG_INFO(ACE_TEXT("client connection amount is :      %d\n"), m_client_connection.connection_number);
		GATE_LOG_INFO(ACE_TEXT("client active disconnection is :   %d\n"), m_client_connection.active_close);
		GATE_LOG_INFO(ACE_TEXT("client passive disconnection is :  %d\n"), m_client_connection.passive_close);
		GATE_LOG_INFO(ACE_TEXT("client disconnection amount is :   %d\n"), m_client_connection.disconnection_number);

		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));
		GATE_LOG_INFO(ACE_TEXT("                      report game server stat info                        \n"));
		GATE_LOG_INFO(ACE_TEXT("--------------------------------------------------------------------------\n"));

		GATE_LOG_INFO(ACE_TEXT("*****************  report gs input network traffic : client  *************\n"));
		diff_time = current_time - m_gs_input_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("gs input package amount is :       %u\n"), m_gs_input_traffic.packet_number);
		GATE_LOG_INFO(ACE_TEXT("gs input byte amount is :          %.4f K\n"), m_gs_input_traffic.byte_number / 1000.0);
		GATE_LOG_INFO(ACE_TEXT("gs input packet speed is :         %.4f\n"), (double)m_gs_input_traffic.packet_number / diff_time.sec());
		GATE_LOG_INFO(ACE_TEXT("gs input byte speed is :           %.4f K\n"), (double)m_gs_input_traffic.byte_number / diff_time.sec() / 1000);

		GATE_LOG_INFO(ACE_TEXT("*****************  report gs output network traffic : client  ************\n"));
		diff_time = current_time - m_gs_output_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("gs output package amount is :      %u\n"), m_gs_output_traffic.packet_number);
		GATE_LOG_INFO(ACE_TEXT("gs output byte amount is :         %.4f K\n"), m_gs_output_traffic.byte_number / 1000.0);
		GATE_LOG_INFO(ACE_TEXT("gs output packet speed is :        %.4f\n"), (double)m_gs_output_traffic.packet_number / diff_time.sec());
		GATE_LOG_INFO(ACE_TEXT("gs output byte speed is :          %.4f K\n"), (double)m_gs_output_traffic.byte_number / diff_time.sec() / 1000);
		GATE_LOG_INFO(ACE_TEXT("gs output packet loss number is :  %u\n"), m_gs_output_traffic.packet_loss);

		GATE_LOG_INFO(ACE_TEXT("*****************  report gs connection network traffic : client  ****\n"));
		diff_time = current_time - m_client_output_traffic.start_time;
		GATE_LOG_INFO(ACE_TEXT("gs online amount is :              %d\n"), m_gs_login_connection.connection_number - m_gs_login_connection.disconnection_number);
		GATE_LOG_INFO(ACE_TEXT("gs max online amount is :          %d\n"), m_gs_login_connection.max_online_connection);
		GATE_LOG_INFO(ACE_TEXT("gs connection amount is :          %d\n"), m_gs_login_connection.connection_number);
		GATE_LOG_INFO(ACE_TEXT("gs disconnection amount is :       %d\n"), m_gs_login_connection.disconnection_number);
	}

	m_wait = false;

	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageStat::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageStat::init(int argc, ACE_TCHAR * argv[])
{
	m_report_interval = ManageConfig::instance()->getStatCfg().report_interval;
	if (this->activate() < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call activate in ManageStat::init, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageStat\n"));
		return 0;
	}
}

int ManageStat::fini()
{
	m_stop = true;
	return 0;
}

int ManageStat::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageStat\n");
		ACE_OS::sleep(1);
	}
	
	return 0;
}

uint32 ManageStat::getOnLineClientNuber()
{
	return m_client_connection.connection_number - m_client_connection.disconnection_number;
}

void ManageStat::statClientInputTraffic(int byte_number, int packet_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_input_traffic.thread_mutex, );
	m_client_input_traffic.byte_number += byte_number;
	m_client_input_traffic.packet_number += packet_number;
}

void ManageStat::statClientOutputTraffic(int byte_number, int packet_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_output_traffic.thread_mutex, );
	m_client_output_traffic.byte_number += byte_number;
	m_client_output_traffic.packet_number += packet_number;
}

void ManageStat::statClientConnection(int connection_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_connection.thread_mutex, );
	m_client_connection.connection_number += connection_number;
	m_client_connection.max_online_connection = max(m_client_connection.max_online_connection, m_client_connection.connection_number - m_client_connection.disconnection_number);
}

void ManageStat::statClientDisconnection(int disconnection_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_connection.thread_mutex, );
	m_client_connection.disconnection_number += disconnection_number;
}

void ManageStat::statClientPacketLoss(int loss_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_output_traffic.thread_mutex, );
	m_client_output_traffic.packet_loss += loss_number;
}

void ManageStat::statClientWobbleNetwork(int wobble_network)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_output_traffic.thread_mutex, );
	if (wobble_network > 0)
	{
		m_client_output_traffic.current_wobble_network += wobble_network;
		m_client_output_traffic.max_wobble_network += wobble_network;
	}
	else
	{
		m_client_output_traffic.current_wobble_network -= abs(wobble_network);
	}
}

void ManageStat::statRoleMakeTime(int role_make_time, int role_make_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_output_traffic.thread_mutex, );
	m_client_input_traffic.role_make_total_time += role_make_time;
	m_client_input_traffic.role_make_total_number += role_make_number;
}

void ManageStat::statClientActiveClose(int close_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_connection.thread_mutex, );
	m_client_connection.active_close += 1;
}

void ManageStat::statClientPassiveClose(int close_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_client_connection.thread_mutex, );
	m_client_connection.passive_close += 1;
}

void ManageStat::statGSInputTraffic(int byte_number, int packet_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_gs_input_traffic.thread_mutex, );
	m_gs_input_traffic.byte_number += byte_number;
	m_gs_input_traffic.packet_number += packet_number;
}

void ManageStat::statGSOutputTraffic(int byte_number, int packet_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_gs_output_traffic.thread_mutex, );
	m_gs_output_traffic.byte_number += byte_number;
	m_gs_output_traffic.packet_number += packet_number;
}

void ManageStat::statGSConnection(int connection_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_gs_login_connection.thread_mutex, );
	m_gs_login_connection.connection_number += connection_number;
	m_gs_login_connection.max_online_connection = max(m_gs_login_connection.max_online_connection, m_gs_login_connection.connection_number - m_gs_login_connection.disconnection_number);
}

void ManageStat::statGSDisconnection(int disconnection_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_gs_login_connection.thread_mutex, );
	m_gs_login_connection.disconnection_number += disconnection_number;
}

void ManageStat::statGSPacketLoss(int loss_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_gs_login_connection.thread_mutex, );
	m_gs_output_traffic.packet_loss += loss_number;
}

void ManageStat::statLoginRecvAuth(int auth_number)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_login_stat.thread_mutex, );
	m_login_stat.recv_auth_number += 1;
}

void ManageStat::statLoginSuccessAuth(int success_auth)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_login_stat.thread_mutex, );
	m_login_stat.auth_success_number += 1;
}

void ManageStat::statLoginFailedAuth(int failed_auth)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_login_stat.thread_mutex, );
	m_login_stat.auth_failed_number += 1;
}