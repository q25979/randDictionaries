// 随机字典生成器Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "随机字典生成器.h"
#include "随机字典生成器Dlg.h"
#include "io.h"
#include "fcntl.h"
#include "stdlib.h"
#include "ctime"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool bStart = TRUE;	// 全局变量，定义判断是否开启
CString sBuild;		// 初始化最终使用的字符串
long iTotal;		// 总条数

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	m_edit_length_min = 0;
	m_edit_length_max = 0;
	m_edit_number = 0;
	m_edit_custom = _T("");
	m_edit_result = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_LIST_RESULT, m_list_result);
	DDX_Text(pDX, IDC_EDIT_LENGTH_MIN, m_edit_length_min);
	DDX_Text(pDX, IDC_EDIT_LENGTH_MAX, m_edit_length_max);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_edit_number);
	DDX_Text(pDX, IDC_EDIT_CUSTOM, m_edit_custom);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnButtonBuild)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_RADIO_MULTI, OnRadioMulti)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_RADIO_FORM, OnRadioForm)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

// 打开控制台
void InitConsoleWindow() 
{ 
    AllocConsole(); 
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); 
    int hCrt = _open_osfhandle((long)handle,_O_TEXT); 
    FILE * hf = _fdopen( hCrt, "w" ); 
    *stdout = *hf; 
}

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// 数据初始化
	// InitConsoleWindow();	// 添加控制台
	CButton* aLetters = (CButton*)GetDlgItem(IDC_CHECK_ALETTERS);
	CButton* bLetters = (CButton*)GetDlgItem(IDC_CHECK_BLETTERS);
	CButton* iNumber = (CButton*)GetDlgItem(IDC_CHECK_NUMBER);
	CButton* rMulti  = (CButton*)GetDlgItem(IDC_RADIO_MULTI);
	m_edit_length_min = 8;		// 初始化最小长度
	m_edit_length_max = 10;		// 初始化最大长度
	m_edit_number = 1000;		// 初始化条数
	aLetters->SetCheck(1);	// 小写字母默认打开
	iNumber->SetCheck(1);	// 数字默认打开
	rMulti->SetCheck(1);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 生成按钮事件
void CMyDlg::OnButtonBuild() 
{
	// 初始化定义
	CString sMFstring;	// 初始化多项和组合字符串
	CString sSpecial = "~!@#$%^&*()_+{}|:<>?`-=/[]\.";	// 特殊字符定义
	CString sBLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";	// 大写字母
	CString sALetter = "abcdefghijklmnopqrstuvwxyz";	// 小写字母
	CString sNumber  = "0123456789";	// 纯数字
	// 单选框
	UINT iStatus = GetCheckedRadioButton(IDC_RADIO_MULTI, IDC_RADIO_FORM);
	// 多选框
	int iALetter = ((CButton*)GetDlgItem(IDC_CHECK_ALETTERS))->GetCheck();	// 小写字母
	int iBLetter = ((CButton*)GetDlgItem(IDC_CHECK_BLETTERS))->GetCheck();	// 大写字母
	int iNumber  = ((CButton*)GetDlgItem(IDC_CHECK_NUMBER))->GetCheck();	// 数字
	int iSpecial = ((CButton*)GetDlgItem(IDC_CHECK_SPECIAL))->GetCheck();	// 特殊字符
	
	bStart = TRUE;	// 开启字典生成
	// 编辑框	
	UpdateData(TRUE);	// 更新
	int iCustomLength = m_edit_custom.GetLength();	// 自定义编辑框长度
	// 判断选择的长度必须是大于0
	int iLength = m_edit_length_max - m_edit_length_min;
	if (iLength < 0 || m_edit_length_min <= 0) {
		MessageBox("长度必须大于0,并且最大长度大于最小长度！", "提示", MB_OK);
		return ;
	}
	// 条数
	if (m_edit_number < 1) {
		MessageBox("生成字典条数不能少于1", "提示", MB_OK);
		return ;
	}
	// 判断自定义不能没有输入
	if (iStatus == 1028 || iStatus == 1029) {
		if (iCustomLength < 1) {
			MessageBox("自定义输入框不能为空！", "提示", MB_OK);
			return ;
		}
	}
	
	// 判断列表框是否有数据
	if (m_list_result.GetCount() > 0) {
		int iBox = MessageBox("是否清空列表框数据？", "提示", MB_YESNO);
		if (iBox == 6) {
			cth_clear = AfxBeginThread(ClearList, this);
		}
		if (iBox == 7) return ;
	}
	GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(FALSE);	// 禁用生成按钮
	m_progress.SetPos(0);
	
	// 获取多选框的数据
	if (iALetter == 1) sMFstring += sALetter;
	if (iBLetter == 1) sMFstring += sBLetter;
	if (iSpecial == 1) sMFstring += sSpecial;
	if (iNumber  == 1) sMFstring += sNumber;
	
	// 1027 1028 1029 单选事件
	if (iStatus == 1027) {
		sBuild = sMFstring;
		// 判断四个选框不能都为空
		if (iALetter == 0 && iBLetter == 0 && iSpecial == 0 && iNumber == 0) {
			MessageBox("多选框不能都为空");
			return ;
		}
	}
	if (iStatus == 1028) sBuild = m_edit_custom;
	if (iStatus == 1029) sBuild = sMFstring + m_edit_custom;
	
	// 开始生成字符串
	cth_start = AfxBeginThread(StartThread, this);
}

// 导出按钮事件
void CMyDlg::OnButtonExport()
{
	// 获取文件路径
	CString sFilePath;
	CFileDialog fileDlg(FALSE, "txt", _T("rand"));
	fileDlg.DoModal();
	sFilePath = fileDlg.GetPathName();
	CString sResult="";	// 生成字符串
	CString sTemp;		// 临时存储字符串
	
	// 保存文件
	if (sFilePath != "rand") {
		CFile file;

		// 读取数据
		for (int i=0; i<m_list_result.GetCount(); i++) {
			m_list_result.GetText(i, sTemp);
			sResult +=  sTemp;
			sResult += i == m_list_result.GetCount()-1 ? "" : "\r\n";
		}
		file.Open(sFilePath, CFile::modeCreate|CFile::modeWrite, NULL);
		file.Write(sResult, strlen(sResult));
		file.Close();
	}	
}

// 停止按钮事假
void CMyDlg::OnButtonStop()
{
	// TODO: Add your control notification handler code here
	bStart = FALSE;
	GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
}

// 多选单选事件
void CMyDlg::OnRadioMulti()
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(1);
}

// 自定义单选事件
void CMyDlg::OnRadioCustom() 
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(2);
}

// 组合单选事件
void CMyDlg::OnRadioForm() 
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(3);
	
}

// 单选事件方法 1-多选 2-自定义 3-组合
void CMyDlg::CmRadioEvent(int type)
{
	bool bMulti  = false;	// 多选标记
	bool bCustom = false;	// 自定义标记

	if (type == 1) bMulti = true;
	if (type == 2) bCustom = true;
	if (type == 3) {
		bMulti = true;
		bCustom = true;
	}
	
	// 单选后设置样式
	GetDlgItem(IDC_CHECK_ALETTERS)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_BLETTERS)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_SPECIAL)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_NUMBER)->EnableWindow(bMulti);

	// 自定义
	GetDlgItem(IDC_EDIT_CUSTOM)->EnableWindow(bCustom);
}

// 创建线程，开始生成字符串
UINT CMyDlg::StartThread(LPVOID param) {
	CMyDlg* dlg = (CMyDlg*)param;
	int iIndex;			// 随机生成的下标
	CString sResult;	// 最终得到的结果
	
	// 设置进度条
	long  iProgress = dlg->m_edit_number;
	short iStep = 1;
	iTotal = 0;			// 初始化总条数
	
	srand(time(NULL));	// 刷新随机数
	if (iProgress > 100) {
		iStep = iProgress / 100;
		iProgress = 100;
	}
	dlg->m_progress.SetRange(0, iProgress);	// 设置进度条范围
	dlg->m_progress.SetStep(1);				// 设置进度条步长
	for (int i=0; i<dlg->m_edit_number; i++) {
		int temp = rand() % (dlg->m_edit_length_max - dlg->m_edit_length_min + 1) + dlg->m_edit_length_min; // 随机长度
		if (bStart != TRUE) return 0;
		// 生成字符串
		for (int j=0; j<temp; j++) {
			iIndex = rand() % sBuild.GetLength(); // 根据最近用户选定的字符进行随机搜索
			sResult += sBuild[iIndex];
			if (temp-1 == j) {
				dlg->m_list_result.AddString(_T(sResult));
				dlg->m_list_result.SetTopIndex(dlg->m_list_result.GetCount()-1);
				sResult = "";
				iTotal++;
				if (iTotal % iStep == 0) dlg->m_progress.StepIt();
			}	
		}	
	}
	
	// 执行完成按钮返回原样
	dlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
	return 0;
}

// 清除数据
UINT CMyDlg::ClearList(LPVOID param)
{
	CMyDlg* dlg = (CMyDlg*)param;
	dlg->m_list_result.ResetContent();
	return 0;
}
