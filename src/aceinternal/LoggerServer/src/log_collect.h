// log_collect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "..\resource.h"		// ������


// Clog_collectApp:
// �йش����ʵ�֣������ log_collect.cpp
//

class Clog_collectApp : public CWinApp
{
public:
	Clog_collectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Clog_collectApp theApp;