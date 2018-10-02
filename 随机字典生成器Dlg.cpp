// ����ֵ�������Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "����ֵ�������.h"
#include "����ֵ�������Dlg.h"
#include "io.h"
#include "fcntl.h"
#include "stdlib.h"
#include "ctime"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool bStart = TRUE;	// ȫ�ֱ����������ж��Ƿ���
CString sBuild;		// ��ʼ������ʹ�õ��ַ���
long iTotal;		// ������

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

// �򿪿���̨
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
	
	// ���ݳ�ʼ��
	// InitConsoleWindow();	// ��ӿ���̨
	CButton* aLetters = (CButton*)GetDlgItem(IDC_CHECK_ALETTERS);
	CButton* bLetters = (CButton*)GetDlgItem(IDC_CHECK_BLETTERS);
	CButton* iNumber = (CButton*)GetDlgItem(IDC_CHECK_NUMBER);
	CButton* rMulti  = (CButton*)GetDlgItem(IDC_RADIO_MULTI);
	m_edit_length_min = 8;		// ��ʼ����С����
	m_edit_length_max = 10;		// ��ʼ����󳤶�
	m_edit_number = 1000;		// ��ʼ������
	aLetters->SetCheck(1);	// Сд��ĸĬ�ϴ�
	iNumber->SetCheck(1);	// ����Ĭ�ϴ�
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

// ���ɰ�ť�¼�
void CMyDlg::OnButtonBuild() 
{
	// ��ʼ������
	CString sMFstring;	// ��ʼ�����������ַ���
	CString sSpecial = "~!@#$%^&*()_+{}|:<>?`-=/[]\.";	// �����ַ�����
	CString sBLetter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";	// ��д��ĸ
	CString sALetter = "abcdefghijklmnopqrstuvwxyz";	// Сд��ĸ
	CString sNumber  = "0123456789";	// ������
	// ��ѡ��
	UINT iStatus = GetCheckedRadioButton(IDC_RADIO_MULTI, IDC_RADIO_FORM);
	// ��ѡ��
	int iALetter = ((CButton*)GetDlgItem(IDC_CHECK_ALETTERS))->GetCheck();	// Сд��ĸ
	int iBLetter = ((CButton*)GetDlgItem(IDC_CHECK_BLETTERS))->GetCheck();	// ��д��ĸ
	int iNumber  = ((CButton*)GetDlgItem(IDC_CHECK_NUMBER))->GetCheck();	// ����
	int iSpecial = ((CButton*)GetDlgItem(IDC_CHECK_SPECIAL))->GetCheck();	// �����ַ�
	
	bStart = TRUE;	// �����ֵ�����
	// �༭��	
	UpdateData(TRUE);	// ����
	int iCustomLength = m_edit_custom.GetLength();	// �Զ���༭�򳤶�
	// �ж�ѡ��ĳ��ȱ����Ǵ���0
	int iLength = m_edit_length_max - m_edit_length_min;
	if (iLength < 0 || m_edit_length_min <= 0) {
		MessageBox("���ȱ������0,������󳤶ȴ�����С���ȣ�", "��ʾ", MB_OK);
		return ;
	}
	// ����
	if (m_edit_number < 1) {
		MessageBox("�����ֵ�������������1", "��ʾ", MB_OK);
		return ;
	}
	// �ж��Զ��岻��û������
	if (iStatus == 1028 || iStatus == 1029) {
		if (iCustomLength < 1) {
			MessageBox("�Զ����������Ϊ�գ�", "��ʾ", MB_OK);
			return ;
		}
	}
	
	// �ж��б���Ƿ�������
	if (m_list_result.GetCount() > 0) {
		int iBox = MessageBox("�Ƿ�����б�����ݣ�", "��ʾ", MB_YESNO);
		if (iBox == 6) {
			cth_clear = AfxBeginThread(ClearList, this);
		}
		if (iBox == 7) return ;
	}
	GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(FALSE);	// �������ɰ�ť
	m_progress.SetPos(0);
	
	// ��ȡ��ѡ�������
	if (iALetter == 1) sMFstring += sALetter;
	if (iBLetter == 1) sMFstring += sBLetter;
	if (iSpecial == 1) sMFstring += sSpecial;
	if (iNumber  == 1) sMFstring += sNumber;
	
	// 1027 1028 1029 ��ѡ�¼�
	if (iStatus == 1027) {
		sBuild = sMFstring;
		// �ж��ĸ�ѡ���ܶ�Ϊ��
		if (iALetter == 0 && iBLetter == 0 && iSpecial == 0 && iNumber == 0) {
			MessageBox("��ѡ���ܶ�Ϊ��");
			return ;
		}
	}
	if (iStatus == 1028) sBuild = m_edit_custom;
	if (iStatus == 1029) sBuild = sMFstring + m_edit_custom;
	
	// ��ʼ�����ַ���
	cth_start = AfxBeginThread(StartThread, this);
}

// ������ť�¼�
void CMyDlg::OnButtonExport()
{
	// ��ȡ�ļ�·��
	CString sFilePath;
	CFileDialog fileDlg(FALSE, "txt", _T("rand"));
	fileDlg.DoModal();
	sFilePath = fileDlg.GetPathName();
	CString sResult="";	// �����ַ���
	CString sTemp;		// ��ʱ�洢�ַ���
	
	// �����ļ�
	if (sFilePath != "rand") {
		CFile file;

		// ��ȡ����
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

// ֹͣ��ť�¼�
void CMyDlg::OnButtonStop()
{
	// TODO: Add your control notification handler code here
	bStart = FALSE;
	GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
}

// ��ѡ��ѡ�¼�
void CMyDlg::OnRadioMulti()
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(1);
}

// �Զ��嵥ѡ�¼�
void CMyDlg::OnRadioCustom() 
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(2);
}

// ��ϵ�ѡ�¼�
void CMyDlg::OnRadioForm() 
{
	// TODO: Add your control notification handler code here
	CMyDlg::CmRadioEvent(3);
	
}

// ��ѡ�¼����� 1-��ѡ 2-�Զ��� 3-���
void CMyDlg::CmRadioEvent(int type)
{
	bool bMulti  = false;	// ��ѡ���
	bool bCustom = false;	// �Զ�����

	if (type == 1) bMulti = true;
	if (type == 2) bCustom = true;
	if (type == 3) {
		bMulti = true;
		bCustom = true;
	}
	
	// ��ѡ��������ʽ
	GetDlgItem(IDC_CHECK_ALETTERS)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_BLETTERS)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_SPECIAL)->EnableWindow(bMulti);
	GetDlgItem(IDC_CHECK_NUMBER)->EnableWindow(bMulti);

	// �Զ���
	GetDlgItem(IDC_EDIT_CUSTOM)->EnableWindow(bCustom);
}

// �����̣߳���ʼ�����ַ���
UINT CMyDlg::StartThread(LPVOID param) {
	CMyDlg* dlg = (CMyDlg*)param;
	int iIndex;			// ������ɵ��±�
	CString sResult;	// ���յõ��Ľ��
	
	// ���ý�����
	long  iProgress = dlg->m_edit_number;
	short iStep = 1;
	iTotal = 0;			// ��ʼ��������
	
	srand(time(NULL));	// ˢ�������
	if (iProgress > 100) {
		iStep = iProgress / 100;
		iProgress = 100;
	}
	dlg->m_progress.SetRange(0, iProgress);	// ���ý�������Χ
	dlg->m_progress.SetStep(1);				// ���ý���������
	for (int i=0; i<dlg->m_edit_number; i++) {
		int temp = rand() % (dlg->m_edit_length_max - dlg->m_edit_length_min + 1) + dlg->m_edit_length_min; // �������
		if (bStart != TRUE) return 0;
		// �����ַ���
		for (int j=0; j<temp; j++) {
			iIndex = rand() % sBuild.GetLength(); // ��������û�ѡ�����ַ������������
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
	
	// ִ����ɰ�ť����ԭ��
	dlg->GetDlgItem(IDC_BUTTON_BUILD)->EnableWindow(TRUE);
	return 0;
}

// �������
UINT CMyDlg::ClearList(LPVOID param)
{
	CMyDlg* dlg = (CMyDlg*)param;
	dlg->m_list_result.ResetContent();
	return 0;
}
