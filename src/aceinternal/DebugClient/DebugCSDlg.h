// DebugCSDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include <map>
#include <vector>
#include "md5.h"
#include "ClientTCP.h"

#define WM_READ_STREAM WM_USER + 101

#define DEL_POINTER(p) if(!p){ delete p;p=NULL; }
using namespace google::protobuf;

typedef std::map<std::string/*field_name*/,CEdit*> CEdit_Map;
typedef std::vector<CStatic*> CStatic_Vector;
//��̬�ı��༭��
struct DynamicResources
{
	CStatic_Vector statoc_vector;
	CEdit_Map edit_map;
	
	CEdit* GetCEditByFiledName( std::string field_name ) 
	{
		CEdit_Map::iterator found = edit_map.find( field_name );
		if ( found != edit_map.end() )
			return found->second;
		return NULL;
	}

	~DynamicResources()
	{
		for ( CStatic_Vector::iterator iter = statoc_vector.begin(); iter != statoc_vector.end(); ++iter)
			DEL_POINTER(*iter);
		for ( CEdit_Map::iterator iter = edit_map.begin(); iter != edit_map.end(); ++iter)
			DEL_POINTER( iter->second )
			statoc_vector.clear();
		edit_map.clear();
	}
};

//��½http/gs server��Ϣ
struct LoginInfo
{
	std::string http_server_ip;
	unsigned short http_server_port;
	std::string http_server_id;
	std::string http_path;
	std::string game_server_name;
	std::string user_name;
	std::string timestamp;
	std::string platform_id;
	std::string sign;
	std::string cmflag;
	std::string gate_host;
	unsigned short gate_port;
	std::string session_id;
	std::string account_id;
	std::string key;

	LoginInfo() : http_server_port(80){}
};

// CDebugCSDlg �Ի���
class CDebugCSDlg : public CDialog
{
// ����
public:
	CDebugCSDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDebugCSDlg();

// �Ի�������
	enum { IDD = IDD_DEBUGCS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_menu;
	CComboBox m_cb_clientmsg;
	CEdit m_ce_username;
	CButton m_cb_login;
	CButton m_cb_sendmsg;
	CTreeCtrl m_tree;
	CStatic m_static_name;
	CStatic m_static_s;
	CStatic m_static_l;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeClientmsg();
	afx_msg void OnCbnSelendokClientmsg();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnEnChangeUsername();
	afx_msg void OnBnClickedSendmsg();
	afx_msg void OnCbnEditchangeClientmsg();
	afx_msg void OnBnClickedClearmsg();

public:
	bool LoadXmlInfo();
	void GetEntityMsg();
	DynamicResources* GetDynamicResourcesByMsgName( std::string msg_name);
	void SetShowWindowByMsgName( std::string msg_name, bool is_show );
	bool LoginCheck();
	bool GateCheck();
	boost::uint32_t GetOpcodeIDByMsgName( std::string msg_name );
	std::string GetMessageNameByOpcodeId( boost::uint32_t opcodeid );
	Message* GetMessageByMsgName( std::string msg_name );
	void ReadDesAndInsert(const google::protobuf::Message& message, HTREEITEM hItem);
	LRESULT OnReadStream(WPARAM wparam, LPARAM lparam);
	virtual void OnOK(){}

private:
	typedef std::map<std::string/*msg_name*/, boost::uint32_t/*opcode_id*/> OpcpdeKeyName_Map;
	typedef std::map<boost::uint32_t/*opcode_id*/, std::string/*msg_name*/> OpcpdeKeyId_Map;
	typedef std::map<std::string/*msg_name*/, Message*> Message_Map;
	typedef std::map<std::string/*msg_name*/,DynamicResources*> DynamicResources_Map;
	typedef std::vector<std::string/*cmsg_name*/> CMsg_Vector;
	OpcpdeKeyName_Map m_opcode_kn_map;				// key - value(name - id)
	OpcpdeKeyId_Map m_opcode_ki_map;				// key - value(id - name)
	Message_Map m_message_map;						// opcode.xml�ļ�������Messageʵ��
	DynamicResources_Map m_dynamic_resources_map;	// Messageʵ���ֶζ�̬���ɵ�MFC�ؼ�ʵ��
	CMsg_Vector m_cmsg_vector;
	compiler::Importer * m_pimporter;				// ��̬����protobuf
	std::string m_show_window_msg_name;				// ��ǰ��������Ϣ�������
	CFont* m_pcfont;								// ����
	LoginInfo* m_login_info;						// ��½��֤��Ϣ
	ClientTCP* m_pclient;
	std::string m_entity_path;						// protobuff�ļ�·��
	std::string m_opcode_path;						// opcode.xml�ļ�·��

};

class PError : public compiler::MultiFileErrorCollector
{
	virtual void AddError(const string& filename, int line, int column,
		const string& message)
	{}
};
