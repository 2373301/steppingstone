#include "StdAfx.h"
#include "Data.h"
#include "type_def.h"

const int FLUSH_EVERY_ROUNDS = 10;	//没写入十次日志刷新一会到磁盘
const int LOG_NUM_MAX = 10000;		//日志备份10w条

const int SOCK_TO_UI_CACHE = 2048;	//从socket发往ui的缓存信息

CData::CData( )
:m_log_dir("")
{
	m_hwnd = 0;
	m_cache_init = 0;
	m_cache_reallocate = 0;
	m_cache_max = 0;

}
CData::~CData(void)
{	
	for (std::vector<Node_Info*>::iterator iter = m_vec_node_info.begin(); iter != m_vec_node_info.end(); ++iter)
	{
		delete *iter;
	}
}
//初始化
int CData::init(HWND hwnd, int cache_init, int cache_reallocate, int cache_max, const std::string& log_dir)
{
	m_hwnd = hwnd;
	m_cache_init = cache_init;
	m_cache_reallocate = cache_reallocate;
	m_cache_max = cache_max;
	m_log_dir = log_dir;

	if (!InitializeCriticalSectionAndSpinCount(&m_crit_data_modify, 0) 
		|| !InitializeCriticalSectionAndSpinCount(&m_crit_data_backup, 0)
		/*|| !InitializeCriticalSectionAndSpinCount(&m_crit_client_info, 0)*/)
		return -1;
	
	for (int i=0; i<SOCK_TO_UI_CACHE; ++i)
	{
		Node_Msg node_msg;
		m_list_msg.push_back(node_msg);
	}

	Node_Info* pNodeInfo = new Node_Info[m_cache_init];

	m_vec_node_info.reserve(m_cache_init);
	for (int i=0; i<m_cache_init; ++i)
	{
		m_vec_node_info.push_back(pNodeInfo++);
	}

	return 0;

}
Node_Info* CData::get_unused_node_info()
{
	if (m_vec_node_info.empty())
	{
		allocate_node_info();
	}
	if(m_vec_node_info.size() > m_cache_max)
	{
		int size = m_vec_node_info.size();
		for (int i=m_cache_init; i<size; ++i)
		{
			delete m_vec_node_info[i];
		}
		m_vec_node_info.reserve(m_cache_init);
	}

	Node_Info* pNode = m_vec_node_info.back();
	pNode->reset();
	m_vec_node_info.pop_back();
	return pNode;
}
void CData::set_unused_node_info(Node_Info* pNode)
{
	m_vec_node_info.push_back(pNode);
}
void CData::allocate_node_info()
{
	Node_Info* pNodeInfo = new Node_Info[m_cache_reallocate];
	m_vec_node_info.reserve(m_vec_node_info.size()+m_cache_reallocate);
	for (int i=0; i<m_cache_reallocate; ++i)
	{
		m_vec_node_info.push_back(pNodeInfo++);
	}
}
void CData::close()
{
	DeleteCriticalSection(&m_crit_data_modify);
	DeleteCriticalSection(&m_crit_data_backup);
//	DeleteCriticalSection(&m_crit_client_info);
}
//新建一个客户端	
int CData::new_client_comming(SOCKET sock)
{

	Client_Info* client = new Client_Info;
	client->socket = sock;
	m_map_client_info.insert(std::make_pair(sock, client));

	LogOneClient* log_backup = new LogOneClient;
	log_backup->reserve(10000);
	m_map_log_backup.insert(make_pair(sock, log_backup));

	return 0;
}

void CData::remove_client(SOCKET sock)
{
	remove_client_info(sock);
	remove_log_backup(sock);

	::PostMessage(m_hwnd, WM_SOCKET_TO_UI_REMOVE_CLIENT, sock, 0);
}
//void CData::stop_client(SOCKET sock)
//{
//	remove_log_info_backup(sock);
//}

//新来消息
int CData::new_log_comming(SOCKET sock, const std::string& str_log)
{
	Client_Info* client_info = get_client_info(sock);
	if (client_info == NULL)
	{
		return -1;
	}


	//1backup
//	Node_Info node_info;
	Node_Info* node_info = get_unused_node_info();
	node_info->color = get_keyword_color(str_log);
	memset(node_info->data, 0, sizeof(node_info->data));
	strncpy(node_info->data, str_log.c_str(), str_log.length());

	EnterCriticalSection(&m_crit_data_backup);
	LogOneClient* c_back = m_map_log_backup[client_info->socket];
	if (c_back != NULL)
	{
		if (c_back->size() > LOG_NUM_MAX)
		{
			remove_log_info_backup(client_info->socket);
			::PostMessage(m_hwnd, WM_SOCKET_TO_UI_CLEAN_LIST, client_info->socket, 0);
		}
		c_back->push_back(node_info);
	}
	LeaveCriticalSection(&m_crit_data_backup);	


	//2send to ui
	if (client_info->need_show
		&& (strlen(client_info->filter) == 0 || str_log.find(client_info->filter) != std::string::npos))
	{

		EnterCriticalSection(&m_crit_data_modify);
		int index_data = -1;
		if (client_info->need_show)
		{
			index_data = get_unused_node_msg();
			if (index_data != -1)
			{
				m_list_msg[index_data].node_info.color = node_info->color;
				m_list_msg[index_data].is_used = true;
				m_list_msg[index_data].socket = sock;
				memset(m_list_msg[index_data].node_info.data, 0, sizeof(m_list_msg[index_data].node_info.data));
				strncpy(m_list_msg[index_data].node_info.data, str_log.c_str(),str_log.length());
				::PostMessage(m_hwnd, WM_SOCKET_TO_UI_LOG_COMMING, client_info->socket, index_data);
			}
		}
		LeaveCriticalSection(&m_crit_data_modify);	

	}

	write_to_file(sock, str_log.c_str());

	return 0;
}
int CData::set_client_show(SOCKET socket, bool need_show)
{
	Client_Info* client_info = get_client_info(socket);
	if (client_info == NULL)
	{
		return -1;
	}
	client_info->need_show = need_show;
}
int CData::get_keyword_color(const std::string& str_log)
{
	for (std::map<std::string, COLORREF>::iterator rev_iter = m_map_keyword_color.begin(); rev_iter != m_map_keyword_color.end(); ++rev_iter)
	{
		if (str_log.find(rev_iter->first) != std::string::npos)
		{
			return  rev_iter->second;
			break;
		}
	}
	return RGB(255,255,255);
}
int CData::set_filter(SOCKET sock, const std::string& str_filter)
{
	Client_Info* client_info = get_client_info(sock);
	if (client_info == NULL)
	{
		return -1;
	}
	if (str_filter.empty())
	{
		memset(client_info->filter, 0, sizeof(client_info->filter));
	}
	else
	{
		strncpy(client_info->filter, str_filter.c_str(), str_filter.length());
	}


	//LogOneClient* client = get_log_info_backup(sock);
	//if (client != NULL)
	//{
	//	std::vector<std::string> vecotr
	//	for (LogOneClient::iterator iter = client->begin(); iter != client->end(); ++iter)
	//	{
	//		if (strlen(client_info->filter) == 0 || strstr(iter->data, client_info->filter) != NULL)
	//		{
	//			::PostMessage(m_hwnd, WM_SOCKET_TO_UI_LOG_COMMING, client_info->socket, index_data);
	//		}
	//	}
	//}

	return 0;
}

//获取指定消息内容
int CData::get_msg_info(int index, Node_Info& content)
{
	if (index < 0)
	{
		return -1;
	}

	EnterCriticalSection(&m_crit_data_modify);

	if (index >= m_list_msg.size())
		return -1;
//	strncpy(des, m_list_msg[index].node_info.data, len);
//	content = m_list_msg[index].node_info.data;
	memcpy(&content, &m_list_msg[index].node_info, sizeof(m_list_msg[index].node_info));

	LeaveCriticalSection(&m_crit_data_modify);

	return 0;
}
//取得一个未使用节点
int CData::get_unused_node_msg()
{
	int index = -1;

	int size = m_list_msg.size();
	for (int i = 0; i < size; ++i)
	{
		if (!m_list_msg[i].is_used)
		{
			index = i;
			break;
		}
	}

	return index;
}
//清楚一个指定节点
int CData::remove_single_data(int index)
{
	if (index < 0)
	{
		return -1;
	}

	EnterCriticalSection(&m_crit_data_modify);
	if (index >= m_list_msg.size())
	{
		return -1;
	}
	m_list_msg[index].reset();

	LeaveCriticalSection(&m_crit_data_modify);
	return 0;
}

//追加关键字对应的颜色，如果原已存在则用心的参数覆盖
int CData::add_keyword_color(std::string str_keyword, COLORREF str_color)
{
	m_map_keyword_color[str_keyword] = str_color;
	return 0;
}

//返回消息内存备份
LogOneClient* CData::get_log_info_backup(SOCKET sock)
{
	LogOneClient* client = NULL;
	EnterCriticalSection(&m_crit_data_backup);
	LogAllClientBackup::iterator iter = m_map_log_backup.find(sock);
	if (iter != m_map_log_backup.end())
	{
		client = iter->second;
	}
	LeaveCriticalSection(&m_crit_data_backup);

	return client;
}
//清空消息内存备份数据
void CData::remove_log_info_backup(SOCKET sock)
{
	EnterCriticalSection(&m_crit_data_backup);
	LogAllClientBackup::iterator iter = m_map_log_backup.find(sock);
	if (iter != m_map_log_backup.end() && iter->second != NULL)
	{
		for (LogOneClient::iterator iter_node = iter->second->begin();
			iter_node != iter->second->end();
			++iter_node)
		{
			set_unused_node_info(*iter_node);
		}
		iter->second->clear();
	}
	LeaveCriticalSection(&m_crit_data_backup);
}
void CData::remove_log_backup(SOCKET sock)
{
	EnterCriticalSection(&m_crit_data_backup);
	LogAllClientBackup::iterator iter = m_map_log_backup.find(sock);
	if (iter != m_map_log_backup.end() && iter->second != NULL)
	{
		for (LogOneClient::iterator iter_node = iter->second->begin();
			iter_node != iter->second->end();
			++iter_node)
		{
			set_unused_node_info(*iter_node);
		}
		iter->second->clear();
		delete iter->second;
		m_map_log_backup.erase(iter);
	}
	LeaveCriticalSection(&m_crit_data_backup);
}
//刷新缓存进文件
void CData::write_to_file(SOCKET sock, std::string str_content)
{
	str_content += '\n';
	Client_Info* client_info = get_client_info(sock);
	if (client_info == NULL || client_info->file == NULL)
	{
		return;
	}

	fputs(str_content.c_str(), client_info->file);

	if (client_info->num_unflush > FLUSH_EVERY_ROUNDS)
	{
		fflush(client_info->file);
		client_info->num_unflush = 0;
	}

}
	
//创建socket对应日志文件
FILE* CData::create_log_file(SOCKET sock, std::string name)
{

	//创建备份及缓冲,备份资格保存hfile，缓冲自个保存hwnd
	//		LogOneClient* log_backup = new LogOneClient;
	//		LogOneClient* log_cache = new LogOneClient;
	//		m_map_log_backup.insert(make_pair(index, log_backup));
	//		m_vec_log_cache.insert(make_pair(index, log_cache));

	std::string str_path = m_log_dir;
	str_path += "\\";
	//创建log文件
	time_t time_local = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y_%m_%d_%H_%M_%S_", localtime(&time_local));
	str_path += tmp;
	str_path += name;
	str_path += ".txt";
	FILE* file = create_file(str_path);


	Client_Info* client_info = get_client_info(sock);
	if (client_info == NULL)
	{
		return NULL;
	}
	client_info->file = file;


//	memset(m_ach_client_name, 0, sizeof(m_ach_client_name));
//	strncpy(m_ach_client_name, name.c_str(), name.length());

	char* client_name = new char[name.length() + 1];
	strncpy(client_name, name.c_str(), name.length());
	client_name[name.length()] = 0;

	PostMessage(m_hwnd, WM_SOCKET_TO_UI_NEW_CLIENT, sock, (LPARAM)client_name);

	return file;
}
	
FILE* CData::create_file(std::string str_path)
{

	//
	FILE* file = fopen(str_path.c_str(), "w+");

	return file;

}

Client_Info* CData::get_client_info(SOCKET sock)
{
	
//	EnterCriticalSection(&m_crit_client_info);

	std::map<SOCKET, Client_Info*>::iterator iter_hwnd = m_map_client_info.find(sock);
	if (iter_hwnd == m_map_client_info.end())
	{	
		return NULL;
	}
	return iter_hwnd->second;
}

void CData::remove_client_info(SOCKET sock)
{	
//	EnterCriticalSection(&m_crit_client_info);

	std::map<SOCKET, Client_Info*>::iterator iter_hwnd = m_map_client_info.find(sock);
	if (iter_hwnd == m_map_client_info.end())
	{	
		return;
	}
	if (iter_hwnd->second != NULL)
	{
		if (iter_hwnd->second->file != NULL)
		{
			fclose(iter_hwnd->second->file);
		}
		delete iter_hwnd->second;
	}
	m_map_client_info.erase(iter_hwnd);

//	LeaveCriticalSection(&m_crit_client_info);	
}