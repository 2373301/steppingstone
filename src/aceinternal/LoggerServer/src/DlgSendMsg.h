#pragma once
#include "..\resource.h"

// CDlgSendMsg �Ի���

class CDlgSendMsg : public CDialog
{
	DECLARE_DYNAMIC(CDlgSendMsg)

public:
	CDlgSendMsg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSendMsg();

// �Ի�������
	enum { IDD = IDD_DLG_SEND_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_str_msg;
};
