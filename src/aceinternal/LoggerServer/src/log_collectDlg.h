// log_collectDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <vector>
#include <map>
// Clog_collectDlg 对话框

typedef struct ClientTab
{
	ClientTab()
		:report_list(NULL)
		,is_scroll(true)
		,is_show(true)
	{
	}
	CXTPReportControl* report_list;
	bool is_scroll;
	bool is_show;
};
class CMySocket;
class CData;
class Clog_collectDlg : public CXTResizeDialog
{
// 构造
public:
	Clog_collectDlg(CWnd* pParent = NULL);	// 标准构造函数
	~Clog_collectDlg();

// 对话框数据
	enum { IDD = IDD_LOG_COLLECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNewClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewLogMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCleanList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnConfig();

	afx_msg void OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()


private:
	CXTTabCtrl m_tabCtrl;

	CMenu m_menu;

	std::vector< CXTPReportControl* > m_vec_list;

	int m_tab_index_top;

	std::map<SOCKET, ClientTab> m_map_sock_tab;

	CMySocket* m_socket;
	CData* m_data;

	std::vector<CString> m_vec_log;

	bool m_scroll;

	CXTPReportControl m_report_list;


private:
	int InitTabCtrl();

	CXTPReportControl* new_report_list();

	int add_tab(CXTPReportControl* client, std::string str_name);

	CXTPReportControl* get_tab_from_socket(SOCKET sock);

	void erase_tab_from_socket(SOCKET sock);

	COLORREF parse_color(const std::string& str_color);

	SOCKET get_socket_form_tab(CXTPReportControl* tab);

	ClientTab* get_current_client_tab();

	bool create_directory(const std::string& strPath);

public:
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnScroll();
	afx_msg void OnBnClickedBtnClean();
	afx_msg void OnBnClickedBtnDisplay();
	afx_msg void OnAll();
	afx_msg void OnUpdateAll(CCmdUI *pCmdUI);
	afx_msg void OnSendMsg();
};
