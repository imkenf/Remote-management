// AddHostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteMan.h"
#include "AddHostDlg.h"
#include "afxdialogex.h"


// CAddHostDlg �Ի���

IMPLEMENT_DYNAMIC(CAddHostDlg, CDialogEx)

CAddHostDlg::CAddHostDlg(HOST_STRUCT const *pHost,  HTREEITEM hItem, CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddHostDlg::IDD, pParent)
	, m_hParentItem(hItem)
	, IsPasswordChange(false)
	, m_Password(_T(""))
{
	if (pHost)
	{
		bAddHost=false;
		m_Password=pHost->Password;
		memcpy(&m_Host, pHost, sizeof(m_Host));
	}
	else
	{
		bAddHost=true;
		memset(&m_Host, 0, sizeof(m_Host));
	}
}

CAddHostDlg::~CAddHostDlg()
{
}

void CAddHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
}


BEGIN_MESSAGE_MAP(CAddHostDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddHostDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRLMODE, &CAddHostDlg::OnCbnSelchangeComboCtrlmode)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CAddHostDlg::OnEnChangeEditPassword)
END_MESSAGE_MAP()


// CAddHostDlg ��Ϣ�������


BOOL CAddHostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_CTRLMODE);
	for(int i=0; i<_countof(CTRL_MODE);i++)
	{
		pBox->AddString(CTRL_MODE[i]);
	}
	if (bAddHost)
	{
		pBox->SetCurSel(0);
		SetCtrlModeDefPort(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_USER))->SetCurSel(2);
	}
	else
	{
		pBox->SetCurSel(m_Host.CtrlMode);
		SetDlgItemInt(IDC_EDIT_HOSTPORT,m_Host.HostPort);
		SetDlgItemText(IDC_EDIT_HOSTADDR,m_Host.HostAddress);
		SetDlgItemText(IDC_EDIT_HOSTNAME, m_Host.Name);
		SetDlgItemText(IDC_EDIT_MAC, m_Host.MacAddr);
		SetDlgItemText(IDC_COMBO_USER,m_Host.Account);
		SetDlgItemText(IDC_EDIT_README, m_Host.ReadMe);
		SetWindowText("�༭����");
		SetDlgItemText(IDOK,"ȷ��");
		SetDlgItemText(IDCANCEL,"ȡ��");
	}

#ifdef SHOW_HOST_PASSWORD
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
	pEdit->SetPasswordChar(0);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

bool FormatMac(char const *src, char *dst)
{
	UCHAR buff[24], n = 0;
	int len = strlen(src);
	*dst = 0;
	if (len != 12 && len != 17)
		return false;
	for (int i = 0; i < len;)
	{
		char ch = src[i++];
		char cl = src[i++];
		if (len == 17 && i != 17)
		{
			char c = src[i++];
			if (c != ':' && c != '-') return false;
		}
		if (ch >= '0' && ch <= '9')
			ch -= '0';
		else if (ch >= 'A' && ch <= 'F')
			ch -= 'A' - 10;
		else if (ch >= 'a' && ch <= 'f')
			ch -= 'a' - 10;
		else
			return false;
		if (cl >= '0' && cl <= '9')
			cl -= '0';
		else if (cl >= 'A' && cl <= 'F')
			cl -= 'A' - 10;
		else if (cl >= 'a' && cl <= 'f')
			cl -= 'a' - 10;
		else
			return false;
		buff[n++] = ch * 16 + cl;
	}
	sprintf_s(dst, 18, "%02X:%02X:%02X:%02X:%02X:%02X", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
	return true;
}

void CAddHostDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	//����������
	GetDlgItemText(IDC_EDIT_HOSTNAME,str);
	str.Trim();
	if (str.GetLength()>=sizeof(m_Host.Name))
	{
		str.Format("�����������ô���,����%d�ֽڡ�",sizeof(m_Host.Name)-1);
		MessageBox(str,"����",MB_ICONERROR);
		return;
	}
	strcpy_s(m_Host.Name,sizeof(m_Host.Name), str);
	//��������ַ
	GetDlgItemText(IDC_EDIT_HOSTADDR,str);
	str.Trim();
	if (str.GetLength()>=sizeof(m_Host.HostAddress) || str.GetLength()<2)
	{
		str.Format("������ַ���ô���,����%d�ֽڡ�",sizeof(m_Host.HostAddress)-1);
		MessageBox(str,"����",MB_ICONERROR);
		return;
	}
	strcpy_s(m_Host.HostAddress, sizeof(m_Host.HostAddress), str);
	if (m_Host.Name[0]==0) strcpy_s(m_Host.Name,sizeof(m_Host.Name),str);
	//�˿�
	m_Host.HostPort=GetDlgItemInt(IDC_EDIT_HOSTPORT);
	if (m_Host.HostPort==0 || m_Host.HostPort>=0x10000)
	{
		MessageBox("�����˿����ô���","����",MB_ICONERROR);
		return;
	}
	//MAC��ַ
	GetDlgItemText(IDC_EDIT_MAC, str);
	str.Trim();
	if (!FormatMac(str, m_Host.MacAddr))
		SetDlgItemText(IDC_EDIT_MAC, "");
	//����ģʽ
	m_Host.CtrlMode=((CComboBox*)GetDlgItem(IDC_COMBO_CTRLMODE))->GetCurSel();
	//�û���
	GetDlgItemText(IDC_COMBO_USER,str);
	str.Trim();
	if (str.GetLength()>=sizeof(m_Host.Account))
	{
		str.Format("�û������ô���,����%d�ֽڡ�",sizeof(m_Host.Account)-1);
		MessageBox(str,"����",MB_ICONERROR);
		return;
	}
	strcpy_s(m_Host.Account,sizeof(m_Host.Account),str);
	//����
	GetDlgItemText(IDC_EDIT_PASSWORD,str);
	if (str.GetLength()>PASSWORD_MAXLEN)
	{
		str.Format("�û��������,����%d�ֽڡ�",PASSWORD_MAXLEN);
		MessageBox(str,"����",MB_ICONERROR);
		return;
	}
	strcpy_s(m_Host.Password,sizeof(m_Host.Password),str);
	//˵��
	GetDlgItemText(IDC_EDIT_README,str);
	if (str.GetLength()>=sizeof(m_Host.ReadMe))
	{
		str.Format("����˵�����ô���,����%d�ֽڡ�",sizeof(m_Host.ReadMe)-1);
		MessageBox(str,"����",MB_ICONERROR);
		return;
	}
	strcpy_s(m_Host.ReadMe,sizeof(m_Host.ReadMe),str);

	//��������
	if (bAddHost)
		GetParent()->SendMessage(WM_ADDHOST_MESSAGE, WPARAM(&m_Host), (LPARAM)m_hParentItem);
	else
		CDialogEx::OnOK();
}


void CAddHostDlg::OnCbnSelchangeComboCtrlmode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCtrlModeDefPort(((CComboBox*)GetDlgItem(IDC_COMBO_CTRLMODE))->GetCurSel());
}


void CAddHostDlg::SetCtrlModeDefPort(int CtrlMode)
{
	if (strcmp(CTRL_MODE[CtrlMode],CTRL_MODE_RDP_NAME)==0)
		SetDlgItemInt(IDC_EDIT_HOSTPORT,3389);
	else if (strcmp(CTRL_MODE[CtrlMode],CTRL_MODE_RADMIN_NAME)==0)
		SetDlgItemInt(IDC_EDIT_HOSTPORT,4899);
	else if (strcmp(CTRL_MODE[CtrlMode],CTRL_MODE_SSH_NAME)==0)
		SetDlgItemInt(IDC_EDIT_HOSTPORT,22);
	else if (strcmp(CTRL_MODE[CtrlMode],CTRL_MODE_TIGHTVNC_NAME)==0)
		SetDlgItemInt(IDC_EDIT_HOSTPORT,5900);
}


void CAddHostDlg::OnEnChangeEditPassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IsPasswordChange=true;
}
