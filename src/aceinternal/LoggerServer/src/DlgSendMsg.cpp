// src\DlgSendMsg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSendMsg.h"


// CDlgSendMsg �Ի���

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


// CDlgSendMsg ��Ϣ�������
