#pragma once

#define WM_ADDHOST_MESSAGE		(WM_USER+2)


// CAddHostDlg 对话框
struct HOST_STRUCT 
{
	int		Id;
	char	Name[64];
	int		ParentId;
	int		CtrlMode;
	char	HostAddress[64];
	USHORT	HostPort;
	char	MacAddr[18];
	char	Account[32];
	char	Password[66];
	char	ReadMe[256];
};

class CAddHostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddHostDlg)

public:
	bool bAddHost;
	HOST_STRUCT m_Host;
	HTREEITEM m_hParentItem;
	CAddHostDlg(HOST_STRUCT const *pHost, HTREEITEM hItem, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddHostDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDHOST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboCtrlmode();
	void SetCtrlModeDefPort(int CtrlMode);
	afx_msg void OnEnChangeEditPassword();
	bool IsPasswordChange;
	CString m_Password;
};
