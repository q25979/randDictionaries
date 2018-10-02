// 随机字典生成器Dlg.h : header file
//

#if !defined(AFX_DLG_H__94BA3A34_BD49_4CB8_919E_4ABED26ED8C7__INCLUDED_)
#define AFX_DLG_H__94BA3A34_BD49_4CB8_919E_4ABED26ED8C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor
	CWinThread* pThread;

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	CProgressCtrl	m_progress;
	CListBox	m_list_result;
	int		m_edit_length_min;
	int		m_edit_length_max;
	long	m_edit_number;
	CString	m_edit_custom;
	CString	m_edit_result;
	CWinThread* cth_start;	// 声明线程
	CWinThread* cth_clear;	// 清空数据
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonBuild();
	afx_msg void OnButtonExport();
	afx_msg void OnRadioMulti();
	afx_msg void OnRadioCustom();
	afx_msg void OnRadioForm();
	afx_msg void OnButtonStop();
	afx_msg void CmRadioEvent(int);
	static afx_msg UINT StartThread(LPVOID param);	// 生成线程函数
	static afx_msg UINT ClearList(LPVOID param);	// 清除数据
	static afx_msg UINT SuccessCount(LPVOID param);	// 已完成数量统计
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__94BA3A34_BD49_4CB8_919E_4ABED26ED8C7__INCLUDED_)
