// MySocket.cpp : 实现文件
//

#include "stdafx.h"
#include "log_collect.h"
#include "MySocket.h"
#include "Data.h"
#include <boost/algorithm/string.hpp>
#include <vector>

//#define LISTEN_SOCKET 23000

// CMySocket
CData* CMySocket::m_data = NULL;
uint16 CMySocket::m_listen_port = 0;
std::string CMySocket::m_listen_ip = "";

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数

bool CMySocket::start_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return false;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return false; 
	}
	return true;
}

int CMySocket::close_socket()
{
	return	WSACleanup();
}

int CMySocket::init(std::string listen_ip, uint16 listen_port)
{
	//1 start
	if (!start_socket())
	{
		return -1;
	}

	m_listen_ip = listen_ip;
	m_listen_port = listen_port;

	return 0;
}

unsigned int CMySocket::read_proc(void*)
{


	std::vector<SOCKET> m_vec_socket;	//socket队列
	SOCKET m_listen_sock;				//监听socket
	char m_recv_buf[20480];				//读取缓冲

	typedef struct ClientRelated
	{
		ClientRelated()
			:is_first_msg(false)
			,uncomplete_last(false)
		{
			memset(buf, 0, sizeof(buf));
		}
		bool is_first_msg;		//标示是否是第一个消息
		bool uncomplete_last;	//上次是否有半个消息
		char buf[2048];			//存储上次遗留的消息
	};

	std::map<SOCKET, ClientRelated> m_map_client;	//标示是否是第一个消息


	//2 Create a listening socket
	sockaddr_in saServer;
	hostent* localHost;
	char* localIP;
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listen_sock == INVALID_SOCKET)
	{
		return 0;
	}
//	localHost = gethostbyname("");
//	localIP = inet_ntoa(*(struct in_addr *)*localHost->h_addr_list);
//	strcpy(localIP, "0.0.0.0");

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(m_listen_ip.c_str());
	saServer.sin_port = htons(m_listen_port);

	//3bind
	if (bind( m_listen_sock, (SOCKADDR*) &saServer, sizeof(saServer)) == SOCKET_ERROR) 
	{
		close_socket();
		return 0;
	}


	if (SOCKET_ERROR == listen(m_listen_sock, SOMAXCONN))
	{

		return 0;
	}

	fd_set fd_sock;
	FD_ZERO(&fd_sock);
	FD_SET(m_listen_sock, &fd_sock);
	while(TRUE)
	{
		fd_set fdRead = fd_sock;
		if (::select(0, &fdRead, NULL, NULL, NULL) <= 0)
		{
			break;
		} 

		for(int i=0; i<(int)fd_sock.fd_count; i++)
		{
			if(!FD_ISSET(fd_sock.fd_array[i], &fdRead))
				continue; 
			//监听
			if(fd_sock.fd_array[i] == m_listen_sock)
			{
				if(fd_sock.fd_count < FD_SETSIZE)
				{
					sockaddr_in addrRemote;
					int nAddrLen = sizeof(addrRemote);
					SOCKET sNew = ::accept(m_listen_sock, (SOCKADDR*)&addrRemote, &nAddrLen);
					FD_SET(sNew, &fd_sock);

					ClientRelated client_related;
					m_map_client.insert(std::make_pair(sNew, client_related));
					m_data->new_client_comming(sNew);
				}
				else
				{
					continue;
				}
			}
			else
			{	
				std::map<SOCKET, ClientRelated>::iterator iter_map = m_map_client.find(fd_sock.fd_array[i]);
				if (iter_map == m_map_client.end())
					continue;

				memset(m_recv_buf, 0, sizeof(m_recv_buf)); 
				int read_len = ::recv(fd_sock.fd_array[i], m_recv_buf, sizeof(m_recv_buf)-1, 0);
				//错误或者断开
				if(read_len <= 0)
				{
					if (iter_map != m_map_client.end())
					{	
						::closesocket(fd_sock.fd_array[i]);            
						FD_CLR(fd_sock.fd_array[i], &fd_sock);
						m_data->remove_client(iter_map->first);

						m_map_client.erase(iter_map);
					}
					continue;
				}

				std::string str_temp = m_recv_buf; 
				std::vector<std::string> result;
				boost::algorithm::split(result, str_temp, boost::is_any_of("\n"));
				//上次遗留
				if (iter_map->second.uncomplete_last)
				{
					result[0] += iter_map->second.buf;
				}
				//这次遗留
				if (!result[result.size() - 1].empty())
				{
					iter_map->second.uncomplete_last == true;
					memset(iter_map->second.buf, 0, sizeof(iter_map->second.buf));
					strncpy(iter_map->second.buf, result[result.size()-1].c_str(), result[result.size()-1].length());
					
				}
				result.erase(result.end()-1);
				if (result.empty())
				{
					continue;
				}
					
				//判断第一次发送消息
				if (iter_map->second.is_first_msg == false)
				{
					iter_map->second.is_first_msg = true;
					std::string str_temp = result[0];
					m_data->create_log_file(fd_sock.fd_array[i], str_temp);
				}

				//发送给ui
				for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter)
				{
					if ((*iter).size() >= 1024)
					{
						(*iter) = (*iter).substr(0, 1024 - 1);
					}
					m_data->new_log_comming(fd_sock.fd_array[i], *iter);						
				}
			}
		}
	}

	close_socket();

	return 0;
}


