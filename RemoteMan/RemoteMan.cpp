
// RemoteMan.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RemoteMan.h"
#include "RemoteManDlg.h"
#include "InputPasswordDlg.h"
#include "Aes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteManApp

BEGIN_MESSAGE_MAP(CRemoteManApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRemoteManApp ����

CRemoteManApp::CRemoteManApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRemoteManApp ����
//����ģʽ���ƣ����������Ҫͬ������ͼ��˳��
char const CTRL_MODE[5][9]={CTRL_MODE_RDP_NAME,CTRL_MODE_RADMIN_NAME,CTRL_MODE_SSH_NAME,CTRL_MODE_REALVNC_NAME,CTRL_MODE_TIGHTVNC_NAME};
CRemoteManApp theApp;

static HANDLE hSem;

// CRemoteManApp ��ʼ��

BOOL CRemoteManApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox("��ʼ���׽��ִ���");
		return FALSE;
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//ֻ����һ��ʵ��
	hSem = CreateSemaphore(NULL, 1, 1, m_pszExeName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// �ر��ź������
//		CloseHandle(hSem);
		// Ѱ����ǰʵ����������
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			// ��鴰���Ƿ���Ԥ��ı��?
			// �У���������Ѱ�ҵ�����
			if (::GetProp(hWndPrevious, m_pszExeName))
			{
				// ��������Ѿ���С���������У���ô�����ȴ�
				if(!::IsWindowVisible(hWndPrevious))
					::PostMessage(hWndPrevious, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				// ����������С������ָ����С
				if (::IsIconic(hWndPrevious))
					::ShowWindow(hWndPrevious,SW_RESTORE);
				// ����������
				::SetForegroundWindow(hWndPrevious);
				// �������ĶԻ��򼤻�
				::SetForegroundWindow(
					::GetLastActivePopup(hWndPrevious));
				// �˳���ʵ��
				return FALSE;
			}
			// ����Ѱ����һ������
			hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT);
		}
		// ǰһʵ���Ѵ��ڣ����Ҳ���������
		AfxMessageBox("����һ��ʵ�������У����Ҳ������������ڣ�");
		// ���ܳ�����
		// �˳���ʵ��
		return FALSE;
	}

	CRemoteManDlg dlg;			//������������ݿⲢ��ȡ�˲���
	//���뿪������
	if (dlg.SysConfig.SysPassword[0]!=0)
	{
		char str[66];
		CInputPasswordDlg pswdlg;
		if (pswdlg.DoModal()!=IDOK) return FALSE;
		if (pswdlg.m_Password.GetLength()>PASSWORD_MAXLEN ||
			strcmp(dlg.SysConfig.SysPassword, AesEnCodeToStr((char const*)pswdlg.m_Password,pswdlg.m_Password.GetLength(),str,AES_KEY))!=0)
		{
			AfxMessageBox("�������");
			return FALSE;
		}
	}

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CRemoteManApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CloseHandle(hSem);
	return CWinApp::ExitInstance();
}
