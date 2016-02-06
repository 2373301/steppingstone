// log_collectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "log_collect.h"
#include "log_collectDlg.h"
#include <boost/foreach.hpp>
#include "type_def.h"
#include "Data.h"
#include "MySocket.h"
#include <process.h>
#include "SimpleXML.h"
#include <boost/algorithm/string.hpp>
#include "DlgSendMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum
{
	REPORTCOLUMN_TYPE,
	REPORTCOLUMN_CONTENT,
};


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Clog_collectDlg 对话框




Clog_collectDlg::Clog_collectDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(Clog_collectDlg::IDD, pParent)
	,m_tab_index_top(0)
	,m_scroll(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_data = new CData();
	CMySocket::m_data = m_data;
	m_socket = new CMySocket();

}
Clog_collectDlg::~Clog_collectDlg()
{
	m_data->close();
	if (m_data)
	{
		delete m_data;
	}
	if (m_socket)
	{
		delete m_socket;
	}
}

void Clog_collectDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(Clog_collectDlg, CXTResizeDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET_TO_UI_NEW_CLIENT, OnNewClient)
	ON_MESSAGE(WM_SOCKET_TO_UI_LOG_COMMING, OnNewLogMsg)
	ON_MESSAGE(WM_SOCKET_TO_UI_REMOVE_CLIENT, OnRemoveClient)
	ON_MESSAGE(WM_SOCKET_TO_UI_CLEAN_LIST, OnCleanList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONFIG, &Clog_collectDlg::OnBnClickedBtnConfig)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &Clog_collectDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SCROLL, &Clog_collectDlg::OnBnClickedBtnScroll)
	ON_BN_CLICKED(IDC_BTN_CLEAN, &Clog_collectDlg::OnBnClickedBtnClean)
	ON_BN_CLICKED(IDC_BTN_DISPLAY, &Clog_collectDlg::OnBnClickedBtnDisplay)
	ON_COMMAND(ID_ALL, &Clog_collectDlg::OnAll)
	ON_UPDATE_COMMAND_UI(ID_ALL, &Clog_collectDlg::OnUpdateAll)
	ON_COMMAND(ID_32788, &Clog_collectDlg::OnSendMsg)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL, OnSelChangeTab)

END_MESSAGE_MAP()


// Clog_collectDlg 消息处理程序

BOOL Clog_collectDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//加载菜单
	m_menu.LoadMenu(IDR_MAIN_MENU);
	m_menu.CheckMenuItem(ID_ALL, MF_CHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_DEBUG, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_INFO, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_WARNING, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_TRACE, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_ERROR, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_FATAL, MF_UNCHECKED | MF_BYCOMMAND);
	m_menu.CheckMenuItem(ID_CUSTOM, MF_UNCHECKED | MF_BYCOMMAND);

	//设置当前菜单
	SetMenu(&m_menu);
	CWnd::DragAcceptFiles();

	//
	VERIFY(CXTPReportControl::UseReportCustomHeap());
	CXTPReportControl::UseRowBatchAllocation();
	InitTabCtrl();


	SetResize(IDC_TAB_CTRL,     SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_SCROLL,             SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BTN_DISPLAY,             SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BTN_CLEAN,             SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BTN_CONFIG,             SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_BTN_SEARCH,             SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_SEARCH,             SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	// Load window placement
	LoadPlacement(_T("CTabCtrlDlg"));

	::SetWindowPos(m_tabCtrl.m_hWnd, NULL, 0,0,0,0,
		/*SWP_NOMOVE|SWP_NOSIZE|*/SWP_FRAMECHANGED);
	m_tabCtrl.Invalidate(FALSE);

	//读取配置文件
	Document * pDocument = XMLFactory::create_document();
	pDocument->load_file("log_collect_cfg.xml");

	Element * elem_root = pDocument->get_root();
	Element* elem_keys = elem_root->get_element("keys");
	Elements elem_list = elem_keys->get_elements();
	for (Elements::iterator iter = elem_list.begin(); iter != elem_list.end(); ++iter)
	{
		Attribute* attr_key = (*iter)->get_attribute("content");
		Attribute* attr_color = (*iter)->get_attribute("color");

		m_data->add_keyword_color(attr_key->get_value(), parse_color(attr_color->get_value()));
	}
	//创建文件夹
	std::string log_directory = elem_root->get_element("log_dir")->get_attribute("directory")->get_value();
	create_directory(log_directory);

	//新建线程
	Element* elem_addr = elem_root->get_element("addr");
	Element* elem_cache = elem_root->get_element("cache");
	m_data->init(m_hWnd
		,atoi((elem_cache->get_attribute("init")->get_value()).c_str())
		,atoi((elem_cache->get_attribute("reallocate")->get_value()).c_str())
		,atoi((elem_cache->get_attribute("max")->get_value()).c_str())
		, log_directory);
	m_socket->init(elem_addr->get_attribute("listen_ip")->get_value()
		, atoi((elem_addr->get_attribute("listen_port")->get_value()).c_str()));
	_beginthreadex(0,0,CMySocket::read_proc, 0,0,0);

	ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
bool Clog_collectDlg::create_directory(const std::string& strPath)
{
	bool bret = CreateDirectory(strPath.c_str(), NULL);
	if (bret
		|| GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return true;
	}
	return false;
}
COLORREF Clog_collectDlg::parse_color(const std::string& str_color)
{
	std::vector<std::string> result;
	boost::algorithm::split(result, str_color, boost::is_any_of(","));
	if (result.size() != 3)
	{
		return RGB(255,255,255);
	}
	return RGB(atoi(result[0].c_str()), atoi(result[1].c_str()), atoi(result[2].c_str()));
}

void Clog_collectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CXTResizeDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Clog_collectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR Clog_collectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int Clog_collectDlg::InitTabCtrl()
{
	// you can use this to change the tab orientation

	/*
	m_tabCtrl.ModifyTabStyle(NULL, TCS_BOTTOM);
	*/

	// Have the tab control send WM_INITIALUPDATE to views.
	m_tabCtrl.SendInitialUpdate(TRUE);

	// you can use something like the following snippet to remove an image
	// from a tab that's already been created...

	/*
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	tci.iImage = -1;
	m_tabCtrl.SetItem(1, &tci);
	*/

	m_tabCtrl.SetPadding(CSize(6, 3));

	//// Create the image list used by the tab control.
	//if (!m_imageList.Create( IDB_IMAGELIST, 16, 1, RGB( 0x00,0xFF,0x00 )))
	//{
	//	TRACE0("Failed to create image list.\n");
	//	return -1;
	//}

	//// Set the tab controls image list.
	//m_tabCtrl.SetImageList(&m_imageList);

	// Set the tab to auto condense (shrink) its tabs when sized.
	m_tabCtrl.SetAutoCondense(true);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_tabCtrl.ModifyStyle(0L, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

//	ForceRefreshTabCtrl();

	return 0;
}
CXTPReportControl* Clog_collectDlg::new_report_list()
{
//	for (int i=0; i<4; ++i)
//	{
		CXTPReportControl* plist = new CXTPReportControl;
		//if (!plist->CreateEx(WS_EX_CLIENTEDGE, _T("XTPReport"), NULL,
		//	WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL,
		//	CRect(0,0,0,0), &m_tabCtrl, IDC_EDIT_LIST_BEGIN + m_tab_index_top))
		//{
		//	return NULL;
		//}

		plist->Create(0, CRect(0,0,0,0), &m_tabCtrl, IDC_EDIT_LIST_BEGIN + m_tab_index_top);

		++m_tab_index_top;


//		plist->AddColumn(new CXTPReportColumn(0, _T("Type"), 50));
		plist->AddColumn(new CXTPReportColumn(0, _T("Content"), 200));

		plist->GetReportHeader()->AllowColumnSort(FALSE);
		CXTPReportColumn* column_group = plist->GetColumns()->GetAt(0);
		column_group->SetAutoSortWhenGrouped(FALSE);
		column_group->SetSortable(FALSE);

		// initialize list box.
		m_vec_list.push_back(plist);
//	}


	return plist;
}

int Clog_collectDlg::add_tab(CXTPReportControl* client, std::string str_name)
{

//	for (std::vector<CXTPReportControl*>::iterator iter_list = m_vec_list.begin(); iter_list != m_vec_list.end(); ++iter_list)
	{
		if (!m_tabCtrl.AddControl(_T(str_name.c_str()), client/* *iter_list*/, -1))
		{
			TRACE0("Failed to create edit list.\n");
			return -1;
		}
	}


	return 0;
}
void Clog_collectDlg::OnBnClickedBtnConfig()
{
	//

}
//新客户端
LRESULT Clog_collectDlg::OnNewClient(WPARAM wParam, LPARAM lParam)
{
	if ((char*)lParam == NULL)
	{
		return 0;
	}
//	m_report_list.SubclassDlgItem( IDC_REPORTCTRL, this);

	CXTPReportControl* report_list = new_report_list();

//	report_list = &m_report_list;
//	report_list->AddColumn(new CXTPReportColumn(0, _T("Content"), 200));

	add_tab(report_list, (char*)lParam);

	ClientTab tab_client;
	tab_client.report_list = report_list;
	m_map_sock_tab.insert(std::make_pair((SOCKET)wParam, tab_client));

	//
	CString str_log;
	str_log.Format("Client connect,socket is:",(SOCKET)wParam);
	m_vec_log.push_back(str_log);

	delete[] (char*)(lParam);
	return 0;
}
//新消息
LRESULT Clog_collectDlg::OnNewLogMsg(WPARAM wParam, LPARAM lParam)
{
	CXTPReportControl* client = get_tab_from_socket((SOCKET)wParam);

	if (client == NULL)
	{
		return -1;
	}

	CXTPReportRow* top_row = NULL;
	if(client->GetRows())
	{
		top_row = client->GetRows()->GetAt(client->GetTopRowIndex());
	}

	Node_Info node_log;
	m_data->get_msg_info((int)lParam, node_log);
	m_data->remove_single_data((int)lParam);
	//插入数据
	CXTPReportRecord* record = new CXTPReportRecord;
	CXTPReportRecordItemText* item_text = new CXTPReportRecordItemText(node_log.data);
	item_text->SetBackgroundColor(node_log.color);
	record->AddItem(item_text);


	if (client == (CXTPReportControl*)m_tabCtrl.GetActiveView())
	{
		client->AddRecordEx(record, NULL, 0);
		if (get_current_client_tab()->is_scroll)
		{
			int count = client->GetRows()->GetCount();
			count = count - 1 > 0 ? count - 1 : 0;
			client->SetTopRow(count);
		}
		else
		{
			if(top_row)
			{
				client->SetTopRow(top_row->GetIndex());
			}
		}
	}
	else
	{
		client->AddRecord(record);
	}
//	client->RedrawControl();
	
//	client->Populate();	

//	client->EnableDragDrop(_T("CustomDrawReport"), xtpReportAllowDrag | xtpReportAllowDrop);


	return 0;
}


	
LRESULT Clog_collectDlg::OnRemoveClient(WPARAM wParam, LPARAM lParam)
{
	CXTPReportControl* client = get_tab_from_socket((SOCKET)wParam);
	if (client == NULL)
	{
		return 0;
	}
	client->ResetContent();

	m_tabCtrl.DeleteView(client);

	erase_tab_from_socket((SOCKET)wParam);

	
	CString str_log;
	str_log.Format("Client disconnect, socket is:",(SOCKET)wParam);
	m_vec_log.push_back(str_log);

	return 0;
}
LRESULT Clog_collectDlg::OnCleanList(WPARAM wParam, LPARAM lParam)
{
	CXTPReportControl* client = get_tab_from_socket((SOCKET)wParam);
	if (client != NULL)
	{
		client->ResetContent();
	}
	return 0;
}
void Clog_collectDlg::erase_tab_from_socket(SOCKET sock)
{
	std::map<SOCKET, ClientTab>::iterator iter = m_map_sock_tab.find(sock);
	if (iter != m_map_sock_tab.end())
	{
		m_map_sock_tab.erase(iter);
	}
}
CXTPReportControl* Clog_collectDlg::get_tab_from_socket(SOCKET sock)
{

	std::map<SOCKET, ClientTab>::iterator iter = m_map_sock_tab.find(sock);
	if (iter == m_map_sock_tab.end())
	{
		return NULL;
	}
	return iter->second.report_list;

}
SOCKET Clog_collectDlg::get_socket_form_tab(CXTPReportControl* tab)
{
	if (tab == NULL)
	{
		return 0;
	}
	for (std::map<SOCKET, ClientTab> ::iterator iter = m_map_sock_tab.begin(); iter != m_map_sock_tab.end(); ++iter)
	{
		if (tab == iter->second.report_list)
		{
			return iter->first;
		}
	}
	return 0;
}
void Clog_collectDlg::OnBnClickedBtnSearch()
{
	//

	CString str_search;
	GetDlgItemText(IDC_EDIT_SEARCH, str_search);
	std::string str_filter = str_search;

	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();

	SOCKET socket = get_socket_form_tab(tab);
	if (socket != 0)
	{
		m_data->set_filter(socket, str_filter);
		((CXTPReportControl*)m_tabCtrl.GetActiveView())->ResetContent();
	}

	LogOneClient* client = m_data->get_log_info_backup(socket);
	if (client != NULL)
	{
		for (LogOneClient::iterator iter = client->begin(); iter != client->end(); ++iter)
		{	
			if (str_filter.empty() || strstr((*iter)->data, str_filter.c_str()) != NULL)
			{
				//显示
				CXTPReportRecord* record = new CXTPReportRecord;
				CXTPReportRecordItemText* item_text = new CXTPReportRecordItemText((*iter)->data);
				item_text->SetBackgroundColor((*iter)->color);
				record->AddItem(item_text);
				tab->AddRecord(record);
			}
		}
		tab->Populate();
	}

}

void Clog_collectDlg::OnBnClickedBtnScroll()
{

	ClientTab* client_tab = get_current_client_tab();
	client_tab->is_scroll = !client_tab->is_scroll;
	if (client_tab->is_scroll)
	{
		SetDlgItemText(IDC_BTN_SCROLL, "停止滚动");
	}
	else
	{
		SetDlgItemText(IDC_BTN_SCROLL, "开始滚动");
	}
}
ClientTab* Clog_collectDlg::get_current_client_tab()
{
	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();
	if (tab != NULL)
	{
		SOCKET socket = get_socket_form_tab(tab);
		std::map<SOCKET, ClientTab>::iterator iter = m_map_sock_tab.find(socket);
		if (iter == m_map_sock_tab.end())
		{
			return NULL;
		}
		return &iter->second;
	}
}
//清空
void Clog_collectDlg::OnBnClickedBtnClean()
{
	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();
	if (tab != NULL)
	{
		tab->ResetContent();
		SOCKET socket = get_socket_form_tab(tab);
		m_data->remove_log_info_backup(socket);
	}
}
//显示
void Clog_collectDlg::OnBnClickedBtnDisplay()
{
	get_current_client_tab()->is_show = !get_current_client_tab()->is_show;

	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();
	if (tab != NULL)
	{		
		SOCKET socket = get_socket_form_tab(tab);
		if (!get_current_client_tab()->is_show)
		{
			m_data->set_client_show(socket, false);
			m_data->remove_log_info_backup(socket);
			tab->ResetContent();
			SetDlgItemText(IDC_BTN_DISPLAY, "重新显示");
		}
		else
		{
			m_data->set_client_show(socket, true);
			SetDlgItemText(IDC_BTN_DISPLAY, "停止显示");
		}
	}

}

void Clog_collectDlg::OnAll()
{
	CMenu* sub_menu = m_menu.GetSubMenu(1);

	 
	sub_menu->CheckMenuItem(ID_ALL, !sub_menu->GetMenuState(ID_ALL, MF_BYCOMMAND));
}

void Clog_collectDlg::OnUpdateAll(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void Clog_collectDlg::OnSendMsg()
{
	// TODO: 在此添加命令处理程序代码

	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();
	if (tab != NULL)
	{		
		SOCKET socket = get_socket_form_tab(tab);
		try
		{
			CDlgSendMsg dlg;
			if (dlg.DoModal() == IDOK && !dlg.m_str_msg.IsEmpty())
			{
				::send(socket, dlg.m_str_msg, dlg.m_str_msg.GetLength(), 0);
			}
		}
		catch (...)
		{
			MessageBox("Send error!");
		}
	}
	else
	{
			MessageBox("No client!");
	}
}
void Clog_collectDlg::OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// This is for demonstration purposes, you could 
	// also use the m_tabCtrl member.

	CXTTabCtrl* pTabCtrl = DYNAMIC_DOWNCAST(CXTTabCtrl, CWnd::FromHandle(pNMHDR->hwndFrom));
	ASSERT_VALID(pTabCtrl);

	// This is for demonstration purposes, you could 
	// also use IDC_TAB_CTRL or m_tabCtrl.GetDlgCtrlID().

	int nID = (int)pNMHDR->idFrom;
//	TRACE1("TCN_SELCHANGE called for ID %d.\n", nID);

	*pResult = 0;


	//刷新
	CXTPReportControl* tab = (CXTPReportControl*)m_tabCtrl.GetActiveView();
	tab->Populate();

	ClientTab* client_tab = get_current_client_tab();

	if (!client_tab->is_show)
	{
		SetDlgItemText(IDC_BTN_DISPLAY, "重新显示");
	}
	else
	{
		SetDlgItemText(IDC_BTN_DISPLAY, "停止显示");
	}
	if (client_tab->is_scroll)
	{
		SetDlgItemText(IDC_BTN_SCROLL, "停止滚动");
	}
	else
	{
		SetDlgItemText(IDC_BTN_SCROLL, "开始滚动");
	}
}
