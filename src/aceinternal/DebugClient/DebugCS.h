// DebugCS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDebugCSApp:
// �йش����ʵ�֣������ DebugCS.cpp
//

class CDebugCSApp : public CWinApp
{
public:
	CDebugCSApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDebugCSApp theApp;