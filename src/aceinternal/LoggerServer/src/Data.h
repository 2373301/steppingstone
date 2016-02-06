#pragma once
#include <vector>
#include <map>
#include <string>

typedef struct Node_Info{
	Node_Info()
		:type(0)
		,color(RGB(255, 255, 255))
	{

		data[0] = 0;
	}
	void reset()
	{
		type = 0;
		color = RGB(255, 255, 255);
		memset(data, 0, sizeof(data));
	}
	int type;
	COLORREF color;
	char data[1024];
};

//HFILE file;

//消息log节点信息
typedef struct Node_Msg
{
	Node_Msg()
		:is_used(false)
		,socket(NULL)
	{

	}
	void reset()
	{
		is_used = false;
		socket = 0;
		node_info.reset();
	}
	bool is_used;
	SOCKET socket;
//	char data[2048];
	Node_Info node_info;
};

//写入哪个文件
typedef struct Client_Info
{
	Client_Info()
		:socket(0)
		,file(0)
		,need_show(true)
		,num_unflush(0)
	{
		memset(filter, 0, sizeof(filter));
	}
	void reset()
	{
		socket = 0;
		file = 0;
		need_show = true;
		num_unflush = 0;
		memset(filter, 0, sizeof(filter));
	}
	bool need_show;			//是否需要显示
	SOCKET socket;			//客户端SOCKET
	FILE* file;				//文件句柄
	int num_unflush;		//记录上次刷新的时间
	char filter[1024];		//过滤条件
};


typedef std::vector<Node_Info*> LogOneClient;
typedef std::map<SOCKET, LogOneClient* > LogAllClientBackup;
//typedef std::map<HFILE, LogOneClient* > LogAllClientCache;
class CData
{
public:
	CData();
	~CData(void);

public:

	//初始化
	int init(HWND hwnd, int cache_init, int cache_reallocate, int cache_max, const std::string& log_dir);

	//关闭
	void CData::close();

	//新建一个客户端	
	int new_client_comming(SOCKET sock);

	//
	void remove_client(SOCKET sock);

	//发送一个log消息给窗口
	int new_log_comming(SOCKET sock, const std::string& str_log);

	//获取指定消息内容
	int get_msg_info(int index, Node_Info& content);

	//清除一个指定节点
	int remove_single_data(int index);

	//追加关键字对应的颜色，如果原已存在则用心的参数覆盖
	int add_keyword_color(std::string str_keyword, COLORREF str_color);

	//返回消息内存备份
	LogOneClient* get_log_info_backup(SOCKET sock);

	//清空消息备份sock关联数据
	void remove_log_info_backup(SOCKET sock);

	//清楚消息备份sock关联数据及容器（删除客户端相关信息）
	void remove_log_backup(SOCKET sock);

	//刷新缓存进文件
	void write_to_file(SOCKET sock, std::string str_content);

	//创建socket对应日志文件
	FILE* create_log_file(SOCKET sock, std::string name);

	//设置日志过滤条件
	int set_filter(SOCKET sock, const std::string& str_filter);

	//设置界面是否显示日志
	int set_client_show(SOCKET socket, bool need_show);

private:
	//取得一个未使用节点
	int get_unused_node_msg();

	FILE* create_file(std::string str_path);

	Client_Info* get_client_info(SOCKET sock);

	void remove_client_info(SOCKET sock);

	int get_keyword_color(const std::string& str_log);


	Node_Info* get_unused_node_info();
	void set_unused_node_info(Node_Info* pNode);
	void allocate_node_info();

private:
//	std::vector<Node_Info> m_vec_log_info;		

	LogAllClientBackup m_map_log_backup;					//消息内存备份

	std::vector<Node_Info*> m_vec_node_info;				//

//	LogAllClientCache m_vec_log_cache;					//消息cache，每隔一段时间写入文件一次

	std::map<std::string, COLORREF> m_map_keyword_color;	//设置关键字对应的颜色

	std::map<SOCKET, Client_Info*> m_map_client_info;//

	std::vector<Node_Msg> m_list_msg;				// 消息内容队列

	CRITICAL_SECTION m_crit_data_modify;			//消息队列锁

	CRITICAL_SECTION m_crit_data_backup;			//日志备份锁

//	CRITICAL_SECTION m_crit_client_info;			//客户端相关信息锁

	HWND m_hwnd;									//窗口句柄

	int m_cache_init;			//初始分配
	int m_cache_reallocate;		//重新分配
	int m_cache_max;			//最大值，超过就释放多出的部分
	std::string m_log_dir;		//日志文件夹
public:
	char m_ach_client_name[1024];
};
