// DebugCSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <string>
#include "SimpleXML.h"
#include "DebugCS.h"
#include "DebugCSDlg.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/asio.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

//#include <time.h>

using namespace boost::asio; 
using namespace std;

#define BLOCK_SIZE 3000

bool UTF8ToMB(string & pmb, const char* pu8, int utf8Len)  
{  
	// convert an UTF8 string to widechar   
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(bad_alloc &memExp)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  

	// convert an widechar string to Multibyte   
	int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return false;  
	}  
	pmb.resize(MBLen, 0);  
	nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, (char *)pmb.data(), MBLen, NULL, NULL);  
	delete[] lpszW;  

	if(nRtn != MBLen)  
	{  
		pmb.clear();  
		return false;  
	}  
	return true;  
} 

// 多字节编码转为UTF8编码  
bool MBToUTF8(string & pu8, const char* pmb, int mLen)  
{  
	// convert an MBCS string to widechar   
	int nLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(bad_alloc &memExp)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  
	// convert an widechar string to utf8  
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (utf8Len <= 0)  
	{  
		return false;  
	}  
	pu8.resize(utf8Len);  
	nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, (char *)pu8.data(), utf8Len, NULL, NULL);  
	delete[] lpszW;  

	if (nRtn != utf8Len)  
	{  
		pu8.clear();  
		return false;  
	}  
	return true;  
}  

std::string GetMessageName( Message* msg )	
{
	std::string msg_name = msg->GetTypeName(); 
	size_t pos2 = msg_name.find_last_of("."); 
	if ( pos2 != std::string::npos ) 
	msg_name = msg_name.erase( 0, pos2 + 1 ); 
	return msg_name;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDebugCSDlg 对话框
CDebugCSDlg::CDebugCSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugCSDlg::IDD, pParent),
	m_pimporter(NULL),m_pcfont(NULL),m_login_info(NULL),m_pclient(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_login_info = new LoginInfo();
	m_pcfont = new CFont();
	m_pclient = sConnector.build();
	m_pcfont->CreateFont(15,0,0,0,500,0,0,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		"arial");
}

CDebugCSDlg::~CDebugCSDlg()
{
	m_opcode_kn_map.clear();
	DEL_POINTER(m_pimporter);
	DEL_POINTER(m_pcfont);
	DEL_POINTER(m_login_info);
}

void CDebugCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ClientMsg, m_cb_clientmsg);
	DDX_Control(pDX, IDC_UserName, m_ce_username);
	DDX_Control(pDX, IDC_Login, m_cb_login);
	DDX_Control(pDX, IDC_SendMsg, m_cb_sendmsg);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_STATIC_N, m_static_name);
	DDX_Control(pDX, IDC_STATIC_S, m_static_s);
	DDX_Control(pDX, IDC_STATIC_L, m_static_l);
}

BEGIN_MESSAGE_MAP(CDebugCSDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_ClientMsg, &CDebugCSDlg::OnCbnSelchangeClientmsg)
	ON_BN_CLICKED(IDC_Login, &CDebugCSDlg::OnBnClickedLogin)
	ON_EN_CHANGE(IDC_UserName, &CDebugCSDlg::OnEnChangeUsername)
	ON_BN_CLICKED(IDC_SendMsg, &CDebugCSDlg::OnBnClickedSendmsg)
	ON_MESSAGE(WM_READ_STREAM, &CDebugCSDlg::OnReadStream)
	ON_CBN_EDITCHANGE(IDC_ClientMsg, &CDebugCSDlg::OnCbnEditchangeClientmsg)
	ON_BN_CLICKED(IDC_ClearMsg, &CDebugCSDlg::OnBnClickedClearmsg)
END_MESSAGE_MAP()


// CDebugCSDlg 消息处理程序

BOOL CDebugCSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	m_menu.LoadMenu(IDR_MENU);
	SetMenu(&m_menu);
	
	m_tree.ModifyStyle( 0, TVS_EDITLABELS );

	LoadXmlInfo();
	GetEntityMsg();
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CDebugCSDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDebugCSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDebugCSDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//RECT rect;
	//GetClientRect(&rect);
	//rect.left = 303;
	//rect.right = 49;
	//rect.top = 259;
	//rect.bottom = 306;


	//m_tree.MoveWindow(&rect);
}

bool CDebugCSDlg::LoadXmlInfo()
{
	//读取config.xml文件
	try
	{
		Document* pDocument = XMLFactory::create_document();
		if ( pDocument->load_file("debugcs_config.xml") )
		{
			Element* elem_root = pDocument->get_root();
			Attributes attr_lit = elem_root->get_element("http")->get_attributes();
			for( Attributes::iterator iter = attr_lit.begin(); iter != attr_lit.end(); ++iter )
			{
				if( (*iter)->get_name() == "http_path")
					m_login_info->http_path = (*iter)->get_value();
				else if( (*iter)->get_name() == "game_server_name")
					m_login_info->game_server_name = (*iter)->get_value();
				else if( (*iter)->get_name() == "http_server_id")
					m_login_info->http_server_id = (*iter)->get_value();
				else if( (*iter)->get_name() == "platform_id")
					m_login_info->platform_id = (*iter)->get_value();
				else if( (*iter)->get_name() == "cmflag")
					m_login_info->cmflag = (*iter)->get_value();
				else if( (*iter)->get_name() == "http_server_ip")
					m_login_info->http_server_ip = (*iter)->get_value();
				else if( (*iter)->get_name() == "http_server_port")
					m_login_info->http_server_port = boost::lexical_cast<unsigned short>((*iter)->get_value());
				else if( (*iter)->get_name() == "key")
					m_login_info->key = (*iter)->get_value();
			}
			Attribute* entity_path = elem_root->get_element("path")->get_attribute("entity_path");
			if (NULL != entity_path)
				m_entity_path = entity_path->get_value();

			Attribute* opcode_path = elem_root->get_element("path")->get_attribute("opcode_path");
			if (NULL != opcode_path)
				m_opcode_path = opcode_path->get_value();
			
			pDocument->clear();
		}

		//读取opcode.xml文件
		if ( pDocument->load_file(m_opcode_path) )
		{
			Element* elem_root = pDocument->get_root();
			Elements elem_list = elem_root->get_elements();
			for( Elements::iterator iter = elem_list.begin(); iter != elem_list.end(); ++iter )
			{
				Attribute* attr_opcode = (*iter)->get_attribute("opcode");
				Attribute* attr_name = (*iter)->get_attribute("name");
				if ( !attr_opcode || ! attr_name ) 
				{
					MessageBox( "opcode.xml属性只能是opcode和name。", "opcode.xml属性错误" );
					continue;
				}

				int opcode_id = atoi(attr_opcode->get_value().c_str());
				std::string opcode_name = attr_name->get_value();
				m_opcode_kn_map.insert( make_pair( opcode_name, opcode_id ) );
				m_opcode_ki_map.insert( make_pair( opcode_id, opcode_name ) );

				//初始化下拉框
				//size_t found = opcode_name.find_first_of("c");
				//if ( found != std::string::npos  && found < 1)
				//{
					m_cmsg_vector.push_back( opcode_name );
					m_cb_clientmsg.AddString( opcode_name.c_str() );
				//}
			}
			return true;
		}
	}
	catch ( const ::boost::bad_lexical_cast& e )
	{
		MessageBox( e.what(),"config.xml端口配置错误" );
	}
	return false;
}

std::string GetMseeageType( FieldDescriptor::Type types, const std::string& str)
{
	switch ( types )
	{
	case FieldDescriptor::TYPE_ENUM:
	case FieldDescriptor::TYPE_INT32:
		return "(" + str +"int32)";
	case FieldDescriptor::TYPE_UINT32:
		return"(" + str +"uint32)";
	case FieldDescriptor::TYPE_INT64:
		return "(" + str +"int64)";
	case FieldDescriptor::TYPE_UINT64:
		return "(" + str +"uint64)";
	case FieldDescriptor::TYPE_STRING:
	case FieldDescriptor::TYPE_BYTES:
		return "(" + str +"string)";
	case FieldDescriptor::TYPE_BOOL:
		return "(" + str +"bool)";
	case FieldDescriptor::TYPE_FLOAT:
		return "(" + str +"float)";
	default:
		return "(" + str +"none)";
	}
}

//选择消息体
void CDebugCSDlg::OnCbnSelchangeClientmsg()
{
	if ( !m_cb_clientmsg.GetCount() )
	{
		for( CMsg_Vector::iterator iter = m_cmsg_vector.begin(); iter != m_cmsg_vector.end(); ++iter )
		{
			m_cb_clientmsg.AddString( (*iter).c_str() );
		}
	}
	CString msg_name;
	int nindex = m_cb_clientmsg.GetCurSel();
	if ( nindex < 0 ) return;
	m_cb_clientmsg.GetLBText( nindex, msg_name );

	if ( GetDynamicResourcesByMsgName( (string)msg_name) )
	{
		SetShowWindowByMsgName( m_show_window_msg_name, false );
		SetShowWindowByMsgName( (string)msg_name, true );
		m_show_window_msg_name = msg_name;
		return;
	}
	if ( m_show_window_msg_name != (string)msg_name )
		SetShowWindowByMsgName( m_show_window_msg_name, false );
	m_show_window_msg_name = msg_name;
	DynamicResources* pdr = new DynamicResources();
	Message_Map::iterator found = m_message_map.find( (string)msg_name );
	if ( found == m_message_map.end() ) return;
	
	int dis = (int)distance( m_message_map.begin(), found );
	const Descriptor* des = found->second->GetDescriptor();
	const Reflection* ref = found->second->GetReflection();

	int count = des->field_count();
	for ( int indes = 1; indes <= count; ++indes )
	{
		const FieldDescriptor* field = des->FindFieldByNumber( indes );
		if ( !field ) 
			continue;

		CStatic* p_static_text = new CStatic();
		std::string field_name= field->name();
		std::string str = "";
		if (field->is_repeated())
			str = "Rep";
		std::string types = GetMseeageType( field->type(), str );

		field_name += types + ":";
		int static_start_x = 20;
		int static_start_y = 95 + ( 30 * indes );
		int static_end_x = static_start_x + 140;
		int static_end_y = static_start_y + 20;
		unsigned static_id = 10000 + dis * 100 + 2*indes;
		p_static_text->Create( field_name.c_str(),WS_CHILD|WS_VISIBLE|SS_RIGHT, CRect(static_start_x,static_start_y,static_end_x,static_end_y),this, static_id );
		p_static_text->SetFont( m_pcfont );
		pdr->statoc_vector.push_back( p_static_text );

		CEdit* p_edit_text = new CEdit();
		int edit_start_x = static_end_x + 5;
		int edit_start_y = static_start_y;
		int edit_end_x = edit_start_x + 220;
		int edit_end_y = static_end_y;
		unsigned edit_id = static_id - 1;
		p_edit_text->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_AUTOVSCROLL|WS_TABSTOP, CRect(edit_start_x,edit_start_y,edit_end_x,edit_end_y),this, edit_id );
		p_edit_text->SetFont( m_pcfont );
		pdr->edit_map.insert( std::make_pair( field->name(), p_edit_text ) );
	}
	m_dynamic_resources_map.insert( std::make_pair( m_show_window_msg_name, pdr ) );
}

void CDebugCSDlg::GetEntityMsg()
{
	PError errorCollector;
	compiler::DiskSourceTree sourceTree;
	m_pimporter = new compiler::Importer(&sourceTree, &errorCollector); 
	//std::string disk_path = "protocol";
	sourceTree.MapPath( "", m_entity_path ); 

	boost::filesystem::path path( m_entity_path );
	if ( !boost::filesystem::is_directory( path ) )
	{
		MessageBox("没有protocol文件夹,请添加protocol文件夹及protobuf文件", "路径错误");
		return;
	}
	boost::filesystem::directory_iterator beg_iter(path);
	boost::filesystem::directory_iterator end_iter;
	for (; beg_iter != end_iter; ++beg_iter)
	{
		if (boost::filesystem::is_directory(*beg_iter))
			continue;
		else
		{
			string protofile_name = beg_iter->path().filename().generic_string();
			m_pimporter->Import(protofile_name);

			DynamicMessageFactory* pDMF = new DynamicMessageFactory();
			Message* p_message = NULL;
			const DescriptorPool* p_pool = m_pimporter->pool();
			const FileDescriptor* p_file = p_pool->FindFileByName( protofile_name );
			if(NULL == p_file)
			{
				MessageBox(CString(protofile_name.c_str()) + "读取异常。", "文件错误");
				continue;
			}
			int msg_count = p_file->message_type_count();
			for ( int i = 0; i < msg_count; ++i )
			{
				const Descriptor* descriptor = p_file->message_type( i );
				if(descriptor)
				{
					p_message = pDMF->GetPrototype(descriptor)->New();
					std::string msg_name = GetMessageName(p_message);
					m_message_map.insert( std::make_pair( msg_name, p_message ) );
				}
			}
		}
	}
}

DynamicResources* CDebugCSDlg::GetDynamicResourcesByMsgName( std::string msg_name )
{
	DynamicResources_Map::iterator found = m_dynamic_resources_map.find( msg_name );
	if ( found != m_dynamic_resources_map.end() )
		return found->second;
	return NULL;
}

void CDebugCSDlg::SetShowWindowByMsgName( std::string msg_name, bool is_show )
{
	DynamicResources_Map::iterator found = m_dynamic_resources_map.find( msg_name );
	if ( found != m_dynamic_resources_map.end() )
	{
		for( CEdit_Map::iterator iter = found->second->edit_map.begin(); iter != found->second->edit_map.end(); ++iter )
			iter->second->ShowWindow( is_show );
		for ( CStatic_Vector::iterator iter = found->second->statoc_vector.begin(); iter != found->second->statoc_vector.end(); ++iter )
			(*iter)->ShowWindow( is_show );
	}
}

void CDebugCSDlg::OnBnClickedLogin()
{
	m_cb_login.EnableWindow( false );
	CString user_name;
	m_ce_username.GetWindowText( user_name );
	if( user_name.IsEmpty() )
	{
		m_cb_login.EnableWindow( true );
		MessageBox("用户名不能为空！","登陆失败");
		return;
	}
	m_login_info->user_name = user_name;
	m_static_name.SetWindowText(user_name);


	// 连接服务器
	m_pclient->msg_handler(this->m_hWnd, WM_READ_STREAM);

	// 链接gate
	if (m_pclient->open(m_login_info->http_server_ip, m_login_info->http_server_port))
	{
		m_cb_sendmsg.EnableWindow( true );
		m_cb_clientmsg.SetFocus();
		m_static_s.SetWindowText("连接正常");
		MessageBox("登陆成功！","登陆成功");
	}
	else
	{
		m_static_s.SetWindowText("连接失败");
		m_cb_login.EnableWindow( true );
	}
	

// 	if ( LoginCheck() && GateCheck() )
// 	{
// 		m_cb_sendmsg.EnableWindow( true );
// 		m_cb_clientmsg.SetFocus();
// 		m_static_s.SetWindowText("连接正常");
// 		MessageBox("登陆成功！","登陆成功");
// 	}
// 	else
// 	{
// 		m_static_s.SetWindowText("连接失败");
// 		m_cb_login.EnableWindow( true );
// 	}
}

std::string ToString( unsigned char digest[], int len )
{
	char output[33] = {0};
	for ( int i = 0; i < len; ++i)
		sprintf( output + i * 2, "%02x", ((unsigned char*)digest)[i]);
	return std::string(output);
}

bool CDebugCSDlg::LoginCheck()
{
	// 所有asio类都需要io_service对象 
	io_service iosev; 
	ip::tcp::socket socket(iosev); 
	ip::tcp::endpoint ep(ip::address_v4::from_string( m_login_info->http_server_ip ), m_login_info->http_server_port); 
	boost::system::error_code ec; 
	socket.connect(ep,ec); 
	if(ec) 
	{
		MessageBox("http_server_ip或http_server_port配置错误，请检查config.xml。","配置失败");
		return false;
	}

	//md5_append(gamename+userid+serverid+timestamp+key);
	std::string strnowtime = boost::lexical_cast<std::string>( static_cast<unsigned int>(time(NULL)) );
	md5_state_t state;
	unsigned char digest[16] = {0};
	md5_init( &state );
	std::string src_append=	m_login_info->game_server_name + 
							m_login_info->user_name + 
							m_login_info->http_server_id + 
							strnowtime + 
							m_login_info->key;
	md5_append(&state, (unsigned char*)src_append.c_str(), src_append.length());
	md5_finish( &state, digest );
	std::string sign = ToString( digest,16 );

	std::string  temp_buf = "post ";
	temp_buf += m_login_info->http_path + "?";
	temp_buf += "game=" + m_login_info->game_server_name;
	temp_buf += "&userid=" + m_login_info->user_name;
	temp_buf += "&serverid=" + m_login_info->http_server_id;
	temp_buf += "&timestamp=" + strnowtime;
	temp_buf += "&who=" + m_login_info->platform_id;
	temp_buf += "&sign=" + sign;
	temp_buf += "&cmflag=" + m_login_info->cmflag;
	temp_buf += "\r\n";

	// 发送数据
	char buf[BLOCK_SIZE];
	size_t len = socket.write_some(buffer(temp_buf), ec);
	std::cout << "writed " << len << std::endl;

	// 接收数据 
	memset(buf, 0, BLOCK_SIZE);
	len=socket.read_some(buffer(buf), ec); 
	std::cout.write(buf, len); 
	std::cout << len << std::endl;

	// 截取数据
	std::string msg = buf;
	size_t a = msg.find("param_gate_host");
	size_t b = msg.find("param_session_id");
	size_t c = msg.find("param_account_id");
	size_t d = msg.find("param_vip");
	if ( a == std::string::npos ||
		 b == std::string::npos ||
		 c == std::string::npos ||
		 d == std::string::npos  )
	{
		CString buf;
		buf.Format("config.xml配置为http_server_id=[%s]的GS链接失败，检查GS是否正常开启。", m_login_info->http_server_id.c_str());
		MessageBox(buf,"GS链接失败");
		return false;
	}
	std::string temphost = msg.substr( a+17,b-a-17-2);
	size_t pos2 = temphost.find_last_of(":");
	if ( pos2 != std::string::npos )
	{
		m_login_info->gate_host = temphost.substr( 0, pos2);
		m_login_info->gate_port = boost::lexical_cast<unsigned short>( temphost.erase( 0, pos2 + 1));
	}
	m_login_info->session_id = msg.substr( b+18,c-b-18-2);
	m_login_info->account_id = msg.substr( c+18,d-c-18-2);

	return true;
}

bool CDebugCSDlg::GateCheck()
{
	Message* pmsg = GetMessageByMsgName("cmsg_validate_account");
	if ( !pmsg ) 
	{
		MessageBox("cmsg_validate_account在opcode.xml找不到。","登陆失败");
		return false;
	}

	const Descriptor* des  = pmsg->GetDescriptor();
	const Reflection* refl = pmsg->GetReflection();
	const FieldDescriptor* field_1 = des->FindFieldByName("account_id");
	const FieldDescriptor* field_2 = des->FindFieldByName("session_id");
	refl->SetString( pmsg, field_1, m_login_info->account_id );
	refl->SetString( pmsg, field_2, m_login_info->session_id );
	
	m_pclient->msg_handler(this->m_hWnd, WM_READ_STREAM);

	// 链接gate
	if ( m_pclient->open( m_login_info->gate_host, m_login_info->gate_port ) )
	{
		// 发送
		boost::uint32_t opcodeid = GetOpcodeIDByMsgName("cmsg_validate_account");
		if ( !opcodeid )
		{
			MessageBox("cmsg_validate_account在opcode.xml找不到。","登陆失败");
			return false;
		}
		std::string strbuff = pmsg->SerializeAsString();
		m_pclient->write( opcodeid, strbuff );

		::Sleep(1000);
		
		// 接收
		std::deque<PacketStream*> read_queue;
		m_pclient->read( read_queue );
		for ( std::deque<PacketStream*>::iterator iter = read_queue.begin(); iter != read_queue.end(); ++iter )
		{
			std::string msg_name = GetMessageNameByOpcodeId( (*iter)->opcode() );
			if( msg_name == "smsg_request_player_list" )
			{
				Message* smsg = GetMessageByMsgName( msg_name );
				if ( !smsg ) 
				{
					MessageBox("smsg_request_player_list消息实体不存在。","登陆失败");
					return false;
				}
				
				if( !((*iter)->parse_protocol( *smsg )) )
				{
					MessageBox("smsg_request_player_list数据填充失败。","登陆失败");
					return false;
				}
				const Descriptor* des  = smsg->GetDescriptor();
				const Reflection* refl = smsg->GetReflection();
				const FieldDescriptor* field_des = des->FindFieldByName("player_num");
				boost::int32_t num = refl->GetInt32( *smsg, field_des );
				ReadDesAndInsert( *smsg, m_tree.InsertItem( msg_name.c_str(), TVI_ROOT) );

				boost::uint64_t palyer_guid = 0;
				if ( !num )
				{
					// 创建角色
					Message* pmsg_make = GetMessageByMsgName("cmsg_player_make");
					if ( !pmsg_make ) return false;
					const Descriptor* des_make  = pmsg_make->GetDescriptor();
					const Reflection* refl_make = pmsg_make->GetReflection();

					const FieldDescriptor* field_account = des_make->FindFieldByName("account");
					refl_make->SetString( pmsg_make, field_account, m_login_info->user_name );
					std::string name = boost::posix_time::to_iso_string( boost::posix_time::second_clock::local_time() );
					name = "m" + name.erase(0,6);
					const FieldDescriptor* field_name = des_make->FindFieldByName("name");
					refl_make->SetString( pmsg_make, field_name, name );
					const FieldDescriptor* field_sex = des_make->FindFieldByName("sex");
					refl_make->SetInt32( pmsg_make, field_sex, 1 );
					const FieldDescriptor* field_profession = des_make->FindFieldByName("profession");
					refl_make->SetInt32( pmsg_make, field_profession, 1 );
					const FieldDescriptor* field_gm_level = des_make->FindFieldByName("gm_level");
					refl_make->SetUInt32( pmsg_make, field_gm_level, 1 );

					opcodeid = GetOpcodeIDByMsgName("cmsg_player_make");
					std::string strbuff = pmsg_make->SerializeAsString();
					m_pclient->write( opcodeid, strbuff );

					::Sleep(2000);

					std::deque<PacketStream*> read_make;
					m_pclient->read( read_make );
					for ( std::deque<PacketStream*>::iterator iter_make = read_make.begin(); iter_make != read_make.end(); ++iter_make )
					{
						boost::uint32_t code = (*iter_make)->opcode();
						std::string msg_name = GetMessageNameByOpcodeId( (*iter_make)->opcode() );
						if( msg_name == "smsg_player_make" )
						{
							Message* smsg = GetMessageByMsgName( msg_name );
							if ( !smsg ) 
							{
								MessageBox("smsg_player_make消息实体不存在。","登陆失败");
								return false;
							}

							if( !((*iter_make)->parse_protocol( *smsg )) )
							{
								MessageBox("smsg_player_make数据填充失败。","登陆失败");
								return false;
							}
							const Descriptor* des  = smsg->GetDescriptor();
							const Reflection* refl = smsg->GetReflection();
							const FieldDescriptor* field_des = des->FindFieldByName("guid");
							palyer_guid = refl->GetUInt64( *smsg, field_des );
							ReadDesAndInsert( *smsg, m_tree.InsertItem( msg_name.c_str(), TVI_ROOT) );
							m_static_l.SetWindowText("新账号");
							break;
						}
						m_static_l.SetWindowText("未创建账号");
						MessageBox("账号自动创建角色失败。","登陆失败");
						return false;
					}
				}
				else
				{
					m_static_l.SetWindowText("老账号");
					const FieldDescriptor* field_list = des->FindFieldByName("player_list");
					int field_count = refl->FieldSize( *smsg, field_list );
					for ( int i = 0; i < field_count; ++i )
					{
						const google::protobuf::Message& palyer_msg = refl->GetRepeatedMessage( *smsg, field_list, i );
						const Descriptor* des_palyer  = palyer_msg.GetDescriptor();
						const Reflection* refl_palyer = palyer_msg.GetReflection();
						const FieldDescriptor* field_guid = des_palyer->FindFieldByName("guid");
						palyer_guid = refl_palyer->GetUInt64( palyer_msg, field_guid );
						break;
					}
				}
				Message* pmsg_enter_map = GetMessageByMsgName("cmsg_player_enter_map");
				if ( !pmsg_enter_map ) return false;
				const Descriptor* des_enter_map  = pmsg_enter_map->GetDescriptor();
				const Reflection* refl_enter_map = pmsg_enter_map->GetReflection();
				const FieldDescriptor* field_guid2 = des_enter_map->FindFieldByName("guid");
				refl_enter_map->SetUInt64( pmsg_enter_map, field_guid2, palyer_guid );

				opcodeid = GetOpcodeIDByMsgName("cmsg_player_enter_map");
				std::string strbuff = pmsg_enter_map->SerializeAsString();
				m_pclient->write( opcodeid, strbuff);
				return true;
			}
		}
	}
	CString buf;
	buf.Format("gate_host=[%s],gate_port=[%u]", m_login_info->gate_host.c_str(), m_login_info->gate_port);
	MessageBox("网关链接失败:" + buf ,"登陆失败");
	return false;
}

void CDebugCSDlg::OnEnChangeUsername()
{
	m_cb_login.EnableWindow( true );
	m_cb_sendmsg.EnableWindow( false );
}

void CDebugCSDlg::OnBnClickedSendmsg()
{
	Message_Map::iterator found_msg = m_message_map.find( m_show_window_msg_name );
	boost::uint32_t opcodeid = GetOpcodeIDByMsgName(m_show_window_msg_name);
	if ( !opcodeid ) return ;

	if ( found_msg != m_message_map.end() )
	{
		Message* pmsg = found_msg->second;
		pmsg->Clear();
		const Descriptor* des  = pmsg->GetDescriptor();
		const Reflection* refl = pmsg->GetReflection();

		int field_count = des->field_count();
		DynamicResources* pdr = GetDynamicResourcesByMsgName( m_show_window_msg_name );
		if ( field_count && !pdr ) return ;
		for ( int i = 1; i <= field_count; ++i )
		{
			const FieldDescriptor* field_des = des->FindFieldByNumber( i );
			if ( !field_des ) continue;

			CEdit* pcedit = pdr->GetCEditByFiledName( field_des->name() );
			CString cedit_values;
			pcedit->GetWindowText( cedit_values );
			if( cedit_values.IsEmpty() )
			{
				MessageBox("消息数据不能为空！","数据错误");
				pcedit->SetFocus();
				return;
			}

			if (field_des->is_repeated())
			{
				std::vector<CString> str_v;
				while(true)
				{
					CString str = cedit_values.SpanExcluding(";");
					str_v.push_back(str);
					cedit_values = cedit_values.Right(cedit_values.GetLength() - str.GetLength() - 1);
					if (cedit_values.IsEmpty()) break;
				}

				for ( int i = 0, n = str_v.size(); i < n; ++i )
				{
					try
					{
						switch (field_des->type())
						{
						case FieldDescriptor::TYPE_INT32:
							{
								boost::int32_t values =  boost::lexical_cast<boost::int32_t>(str_v[i]);
								refl->AddInt32( pmsg, field_des, values );
								break;
							}
						case FieldDescriptor::TYPE_UINT32:
							{
								boost::uint32_t values = boost::lexical_cast<boost::uint32_t>(str_v[i]);
								refl->AddUInt32( pmsg, field_des, values );
								break;
							}
						case FieldDescriptor::TYPE_INT64:
							{
								boost::int64_t values = boost::lexical_cast<boost::int64_t>(str_v[i]);
								refl->AddInt64( pmsg, field_des, values );
								break;
							}
						case FieldDescriptor::TYPE_UINT64:
							{
								boost::uint64_t values = boost::lexical_cast<boost::uint64_t>(str_v[i]);
								refl->AddUInt64( pmsg, field_des, values );
								break;
							}
						}
					}
					catch ( const ::boost::bad_lexical_cast& e )
					{
						MessageBox("数据类型错误，请核查消息体对应的数据型！","数据类型错误" );
						return ;
					}
				}
			}
			else
			{
				try
				{
					switch ( field_des->type() )
					{
					case FieldDescriptor::TYPE_INT32:
						{
							boost::int32_t values = boost::lexical_cast<boost::int32_t>( cedit_values );
							refl->SetInt32( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_ENUM:
						{
							boost::int32_t values = boost::lexical_cast<boost::int32_t>( cedit_values );
							const EnumDescriptor* enum_des = field_des->enum_type();
							if (NULL == enum_des)
							{
								MessageBox("无法获取枚举描述符！" + CString(field_des->name().c_str()) ,"异常消息类型" );
							}
							else
							{
								const EnumValueDescriptor * enum_value = enum_des->FindValueByNumber(values);
								if (NULL == enum_value)
								{
									MessageBox("非法的枚举类型值！" + CString(field_des->name().c_str()) ,"异常消息类型" );
								}
								else
								{
									refl->SetEnum( pmsg, field_des, enum_value);
								}
							}
							break;
						}
					case FieldDescriptor::TYPE_UINT32:
						{
							boost::uint32_t values = boost::lexical_cast<boost::uint32_t>( cedit_values );
							refl->SetUInt32( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_INT64:
						{
							boost::int64_t values = boost::lexical_cast<boost::int64_t>( cedit_values );
							refl->SetInt64( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_UINT64:
						{
							boost::uint64_t values = boost::lexical_cast<boost::uint64_t>( cedit_values );
							refl->SetUInt64( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_STRING:
					case FieldDescriptor::TYPE_BYTES:
						{		
							std::string values = boost::lexical_cast<std::string>( cedit_values );
							std::string utf_str;
							MBToUTF8(utf_str, values.c_str(), values.size());
							refl->SetString( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_BOOL:
						{
							bool values = boost::lexical_cast<bool>( cedit_values );
							refl->SetBool( pmsg, field_des, values );
							break;
						}
					case FieldDescriptor::TYPE_FLOAT:
						{
							float values = boost::lexical_cast<float>( cedit_values );
							refl->SetFloat( pmsg, field_des, values );
							break;
						}
					default:
						MessageBox("无法识别的消息类型，请联系工具开发者！","异常消息类型" );
						return;
					}
				}
				catch ( const ::boost::bad_lexical_cast& e )
				{
					MessageBox("数据类型错误，请核查消息体对应的数据型！","数据类型错误" );
					return ;
				}
			}
		}

		// 发送
		std::string strbuff = pmsg->SerializeAsString();
		m_pclient->write( opcodeid, strbuff );
	}
	else
	{
		m_pclient->write( opcodeid, "" );
	}
}

LRESULT CDebugCSDlg::OnReadStream(WPARAM wparam, LPARAM lparam)
{
	// read
	switch (WSAGETSELECTEVENT(lparam))
	{
	case FD_READ:
		{
			// 接收
			std::deque<PacketStream*> read_queue;
			m_pclient->read( read_queue );
			for ( std::deque<PacketStream*>::iterator iter = read_queue.begin(); iter != read_queue.end(); ++iter )
			{
				std::string msg_name = GetMessageNameByOpcodeId( (*iter)->opcode() );
				if( msg_name != "smsg_error" )
				{
					Message* smsg = GetMessageByMsgName( msg_name );
					if ( !smsg ) continue;

					if( (*iter)->parse_protocol( *smsg ) && msg_name != "smsg_player_move" )
					{
						ReadDesAndInsert( *smsg, m_tree.InsertItem( msg_name.c_str(), TVI_ROOT) );
					}
				}
			}
			m_tree.SendMessage( WM_VSCROLL, SB_BOTTOM );

		}
		break;
	case FD_CLOSE:
		{
			m_static_s.SetWindowText("连接断开");
			m_cb_login.EnableWindow( true );
		}
		break;
	}

	return 0;
}

boost::uint32_t CDebugCSDlg::GetOpcodeIDByMsgName( std::string msg_name )
{
	OpcpdeKeyName_Map::iterator found = m_opcode_kn_map.find( msg_name );
	if ( found != m_opcode_kn_map.end() )
		return found->second;
	return 0;
}

std::string CDebugCSDlg::GetMessageNameByOpcodeId( boost::uint32_t opcodeid )
{
	OpcpdeKeyId_Map::iterator found = m_opcode_ki_map.find( opcodeid );
	if ( found != m_opcode_ki_map.end() )
		return found->second;
	return "";
}

Message* CDebugCSDlg::GetMessageByMsgName( std::string msg_name )
{
	Message_Map::iterator found = m_message_map.find( msg_name );
	if ( found != m_message_map.end() )
		return found->second;
	return NULL;
}

void CDebugCSDlg::ReadDesAndInsert(const google::protobuf::Message& message, HTREEITEM hItem)
{
	const google::protobuf::Descriptor* des = message.GetDescriptor();
	const google::protobuf::Reflection* refl = message.GetReflection();
	int count = des->field_count();
	for (int i=0; i<count; ++i)
	{
		const google::protobuf::FieldDescriptor* field_des = des->field(i);
		string str_name = field_des->name();

		//repeated
		if (field_des->is_repeated())
		{
			HTREEITEM hSubItem = m_tree.InsertItem(str_name.c_str(),hItem);
			int field_size = refl->FieldSize(message, field_des);
			for (int j=0; j<field_size; ++j)
			{	
				CString str_show;
				if (field_des->type() == FieldDescriptor::TYPE_ENUM )
				{
					string mb_str = "ENUM_";
					mb_str += refl->GetRepeatedEnum(message, field_des, j)->name();
					mb_str += " = ";
					int number = refl->GetRepeatedEnum(message, field_des, j)->number();
					str_show.Format("[%d]%s%d", j, mb_str.c_str(), number);

					m_tree.InsertItem((LPCSTR)str_show, hSubItem);
				}
				else
				{
					if (field_des->type() == FieldDescriptor::TYPE_STRING 
						|| field_des->type() == FieldDescriptor::TYPE_BYTES)
					{
						string utf8_str = refl->GetRepeatedString(message, field_des, j);
						string mb_str = "";
						if (UTF8ToMB(mb_str, utf8_str.c_str(), utf8_str.size()))
						{
							str_show.Format("[%d]%s", j, mb_str.c_str());
						}
						else
						{
							str_show.Format("[%d]%s", j, utf8_str.c_str());
						}

						m_tree.InsertItem((LPCSTR)str_show, hSubItem);
					}
					else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
					{
						str_show.Format("[%d]", j);
						HTREEITEM hItem_temp = m_tree.InsertItem((LPCSTR)str_show, hSubItem);
						const google::protobuf::Message& msg_sub = refl->GetRepeatedMessage(message, field_des, j);
						ReadDesAndInsert(msg_sub, hItem_temp);
					}
					else
					{

						ULONGLONG temp_value = 0;
						switch (field_des->type())
						{
						case FieldDescriptor::TYPE_INT32:
							temp_value = refl->GetRepeatedInt32(message, field_des, j);
							break;
						case FieldDescriptor::TYPE_UINT32:
							temp_value = refl->GetRepeatedUInt32(message, field_des, j);
							break;
						case FieldDescriptor::TYPE_INT64:
							temp_value = refl->GetRepeatedInt64(message, field_des, j);
							break;
						case FieldDescriptor::TYPE_UINT64:
							temp_value = refl->GetRepeatedUInt64(message, field_des, j);
							break;
						default:
							break;
						}
						str_show.Format("[%d]%I64u", j, temp_value);
						m_tree.InsertItem((LPCSTR)str_show, hSubItem);
					}
				}
			}
		}
		else
		{
			CString str_show;

			if (field_des->type() == FieldDescriptor::TYPE_ENUM )
			{
				string mb_str = "ENUM_";
				mb_str += refl->GetEnum(message, field_des)->name();
				mb_str += " = ";
				int number = refl->GetEnum(message, field_des)->number();
				str_show.Format("[%s]%s%d", str_name.c_str(), mb_str.c_str(), number);

				m_tree.InsertItem((LPCSTR)str_show, hItem);
			}
			else
			{
				if (field_des->type() == FieldDescriptor::TYPE_STRING 
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{
					str_show.Format("[%s]", str_name.c_str());

					string utf8_str = refl->GetString(message, field_des);
					string mb_str = "";
					if (UTF8ToMB(mb_str, utf8_str.c_str(), utf8_str.size()))
					{
						str_show += mb_str.c_str();
					}
					else
					{
						str_show += utf8_str.c_str();
					}

					//str_show += (refl->GetString(message, field_des)).c_str();
					m_tree.InsertItem((LPCSTR)str_show, hItem);
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{
					str_show.Format("%s", str_name.c_str());
					HTREEITEM hSubItem = m_tree.InsertItem((LPCSTR)str_show, hItem);
					const google::protobuf::Message& msg_sub = refl->GetMessage(message, field_des);
					ReadDesAndInsert(msg_sub, hSubItem);
				}
				else
				{
					ULONGLONG temp_value = 0;
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						temp_value = refl->GetInt32(message, field_des);
						break;
					case FieldDescriptor::TYPE_UINT32:
						temp_value = refl->GetUInt32(message, field_des);
						break;
					case FieldDescriptor::TYPE_INT64:
						temp_value = refl->GetInt64(message, field_des);
						break;
					case FieldDescriptor::TYPE_UINT64:
						temp_value = refl->GetUInt64(message, field_des);
						break;
					default:
						break;
					}
					str_show.Format("[%s]%I64u", str_name.c_str(), temp_value);
					m_tree.InsertItem((LPCSTR)str_show, hItem);
				}
			}
		}
	}
}

void CDebugCSDlg::OnCbnEditchangeClientmsg()
{
	for( int i = (m_cb_clientmsg.GetCount()-1); i > -1; --i )
		m_cb_clientmsg.DeleteString( i );

	CString cstr;
	m_cb_clientmsg.GetWindowText(cstr);

	for( CMsg_Vector::iterator iter = m_cmsg_vector.begin(); iter != m_cmsg_vector.end(); ++iter )
	{
		std::string temp_str = (*iter);
		if( temp_str.find( (string)cstr ) != std::string::npos )
			m_cb_clientmsg.AddString( (*iter).c_str() );
	}
	if ( m_cb_clientmsg.GetCount() )
	{
		//m_cb_clientmsg.SetCurSel(0);
		m_cb_clientmsg.ShowDropDown(true);
		SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		m_cb_clientmsg.SetWindowText( cstr );
		::SendMessage(m_cb_clientmsg.m_hWnd,WM_KEYDOWN,VK_END,0);
	}
}
void CDebugCSDlg::OnBnClickedClearmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tree.DeleteAllItems();
}
