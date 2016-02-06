#pragma once
#include <vector>
#include <map>
// CMySocket 命令目标
typedef  unsigned int uint32;
typedef  unsigned short uint16;

class CData;
class CMySocket
{
public:
	CMySocket();
	virtual ~CMySocket();

public:
	//init();
	//listen();
	//read();
	//write();
	//notify();

static int init(std::string listen_ip, uint16 listen_port);
static	unsigned int _stdcall read_proc(void*);

static 	CData* m_data;
static uint16 m_listen_port;
static std::string m_listen_ip;

private:
	static bool start_socket();
	static int close_socket();

private:
	//std::vector<SOCKET> m_vec_socket;	//socket队列
	//SOCKET m_listen_sock;				//监听socket
	//char m_recv_buf[2048];				//读取缓冲
	//std::map<SOCKET, bool> m_map_first_msg;	//标示是否是第一个消息
};


