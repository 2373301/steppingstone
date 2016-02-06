// src\DlgSendMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSendMsg.h"


// CDlgSendMsg 对话框

IMPLEMENT_DYNAMIC(CDlgSendMsg, CDialog)

CDlgSendMsg::CDlgSendMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSendMsg::IDD, pParent)
	, m_str_msg(_T(""))
{

}

CDlgSendMsg::~CDlgSendMsg()
{
}

void CDlgSendMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MSG_SEND, m_str_msg);
}


BEGIN_MESSAGE_MAP(CDlgSendMsg, CDialog)
END_MESSAGE_MAP()


// CDlgSendMsg 消息处理程序
