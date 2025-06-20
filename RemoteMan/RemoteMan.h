
// RemoteMan.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

//#define SHOW_HOST_PASSWORD		


#define PASSWORD_MAXLEN			31

#define CTRL_MODE_RDP_NAME			"RDP"
#define CTRL_MODE_RADMIN_NAME		"Radmin"
#define CTRL_MODE_SSH_NAME			"SSH"
#define CTRL_MODE_TIGHTVNC_NAME		"TightVNC"
#define CTRL_MODE_REALVNC_NAME		"RealVNC"
extern char const CTRL_MODE[5][9];

// CRemoteManApp:
// 有关此类的实现，请参阅 RemoteMan.cpp
//

class CRemoteManApp : public CWinApp
{
public:
	CRemoteManApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CRemoteManApp theApp;