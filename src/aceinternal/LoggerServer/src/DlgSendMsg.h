#pragma once
#include "..\resource.h"

// CDlgSendMsg 对话框

class CDlgSendMsg : public CDialog
{
	DECLARE_DYNAMIC(CDlgSendMsg)

public:
	CDlgSendMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSendMsg();

// 对话框数据
	enum { IDD = IDD_DLG_SEND_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_str_msg;
};
