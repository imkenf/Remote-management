
// RemoteMan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

//#define SHOW_HOST_PASSWORD		


#define PASSWORD_MAXLEN			31

#define CTRL_MODE_RDP_NAME			"RDP"
#define CTRL_MODE_RADMIN_NAME		"Radmin"
#define CTRL_MODE_SSH_NAME			"SSH"
#define CTRL_MODE_TIGHTVNC_NAME		"TightVNC"
#define CTRL_MODE_REALVNC_NAME		"RealVNC"
extern char const CTRL_MODE[5][9];

// CRemoteManApp:
// �йش����ʵ�֣������ RemoteMan.cpp
//

class CRemoteManApp : public CWinApp
{
public:
	CRemoteManApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CRemoteManApp theApp;