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

//��Ϣlog�ڵ���Ϣ
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

//д���ĸ��ļ�
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
	bool need_show;			//�Ƿ���Ҫ��ʾ
	SOCKET socket;			//�ͻ���SOCKET
	FILE* file;				//�ļ����
	int num_unflush;		//��¼�ϴ�ˢ�µ�ʱ��
	char filter[1024];		//��������
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

	//��ʼ��
	int init(HWND hwnd, int cache_init, int cache_reallocate, int cache_max, const std::string& log_dir);

	//�ر�
	void CData::close();

	//�½�һ���ͻ���	
	int new_client_comming(SOCKET sock);

	//
	void remove_client(SOCKET sock);

	//����һ��log��Ϣ������
	int new_log_comming(SOCKET sock, const std::string& str_log);

	//��ȡָ����Ϣ����
	int get_msg_info(int index, Node_Info& content);

	//���һ��ָ���ڵ�
	int remove_single_data(int index);

	//׷�ӹؼ��ֶ�Ӧ����ɫ�����ԭ�Ѵ��������ĵĲ�������
	int add_keyword_color(std::string str_keyword, COLORREF str_color);

	//������Ϣ�ڴ汸��
	LogOneClient* get_log_info_backup(SOCKET sock);

	//�����Ϣ����sock��������
	void remove_log_info_backup(SOCKET sock);

	//�����Ϣ����sock�������ݼ�������ɾ���ͻ��������Ϣ��
	void remove_log_backup(SOCKET sock);

	//ˢ�»�����ļ�
	void write_to_file(SOCKET sock, std::string str_content);

	//����socket��Ӧ��־�ļ�
	FILE* create_log_file(SOCKET sock, std::string name);

	//������־��������
	int set_filter(SOCKET sock, const std::string& str_filter);

	//���ý����Ƿ���ʾ��־
	int set_client_show(SOCKET socket, bool need_show);

private:
	//ȡ��һ��δʹ�ýڵ�
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

	LogAllClientBackup m_map_log_backup;					//��Ϣ�ڴ汸��

	std::vector<Node_Info*> m_vec_node_info;				//

//	LogAllClientCache m_vec_log_cache;					//��Ϣcache��ÿ��һ��ʱ��д���ļ�һ��

	std::map<std::string, COLORREF> m_map_keyword_color;	//���ùؼ��ֶ�Ӧ����ɫ

	std::map<SOCKET, Client_Info*> m_map_client_info;//

	std::vector<Node_Msg> m_list_msg;				// ��Ϣ���ݶ���

	CRITICAL_SECTION m_crit_data_modify;			//��Ϣ������

	CRITICAL_SECTION m_crit_data_backup;			//��־������

//	CRITICAL_SECTION m_crit_client_info;			//�ͻ��������Ϣ��

	HWND m_hwnd;									//���ھ��

	int m_cache_init;			//��ʼ����
	int m_cache_reallocate;		//���·���
	int m_cache_max;			//���ֵ���������ͷŶ���Ĳ���
	std::string m_log_dir;		//��־�ļ���
public:
	char m_ach_client_name[1024];
};
