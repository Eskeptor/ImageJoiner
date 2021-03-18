#include "pch.h"
#include "framework.h"
#include "ImageJoiner.h"
#include "ImageJoinerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CImageJoinerDlg::CImageJoinerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEJOINER_DIALOG, pParent)
	, m_strFolderPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageJoinerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageJoinerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FIND, &CImageJoinerDlg::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_FOLDER_CHECK, &CImageJoinerDlg::OnBnClickedBtnFolderCheck)
	ON_BN_CLICKED(IDC_BTN_CREATE_1, &CImageJoinerDlg::OnBnClickedBtnCreate1)
	ON_BN_CLICKED(IDC_BTN_CREATE_2, &CImageJoinerDlg::OnBnClickedBtnCreate2)
	ON_BN_CLICKED(IDC_BTN_CREATE_3, &CImageJoinerDlg::OnBnClickedBtnCreate3)
END_MESSAGE_MAP()



BOOL CImageJoinerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitControls();

	return TRUE; 
}


/**
MFC 컨트롤 초기화
*/
void CImageJoinerDlg::InitControls()
{
	m_lBoxImg.SubclassDlgItem(IDC_LIST_IMAGE, this);
}


void CImageJoinerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CImageJoinerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CImageJoinerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**
버튼 - 폴더 경로 (...)
*/
void CImageJoinerDlg::OnBnClickedBtnFind()
{
	CFolderPickerDialog dlgPicker(GetExePath(), OFN_FILEMUSTEXIST);
	if (dlgPicker.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_PATH, dlgPicker.GetPathName());
	}
}


/**
버튼 - 폴더 경로 (확인)
*/
void CImageJoinerDlg::OnBnClickedBtnFolderCheck()
{
	GetDlgItemText(IDC_EDIT_PATH, m_strFolderPath);

	if (m_strFolderPath.IsEmpty())
	{
		MessageBox(_T("폴더 경로가 잘못되었습니다."), _T("경고"));
		return;
	}
	
	CString strFolderFindPath = _T("");
	strFolderFindPath.Format(_T("%s\\*.*"), m_strFolderPath);

	CFileFind fFind;
	BOOL bIsFindPath = fFind.FindFile(strFolderFindPath);

	if (bIsFindPath)
	{
		CString strCurFile = _T("");
		CString strCurFileEx = _T("");

		if (m_vImgList.empty() == false)
			m_vImgList.clear();

		while (bIsFindPath)
		{
			bIsFindPath = fFind.FindNextFile();

			if (fFind.IsArchived())
			{
				strCurFile = fFind.GetFileName();
				if (strCurFile.Compare(_T(".")) == 0 ||
					strCurFile.Compare(_T("..")) == 0 ||
					strCurFile.Compare(_T("Thumbs.db")) == 0)
					continue;

				strCurFileEx = strCurFile.Mid(strCurFile.ReverseFind(_T('.')) + 1);
				if (strCurFileEx.Compare(_T("jpg")) == 0 ||
					strCurFileEx.Compare(_T("png")) == 0)
					m_vImgList.push_back(strCurFile);
			}
		}

		if (m_vImgList.empty() == false)
		{
			m_lBoxImg.ResetContent();
			for_each(m_vImgList.begin(), m_vImgList.end(), [&](CString& str) {
				m_lBoxImg.AddString(str);
			});
		}
	}
	else
	{
		MessageBox(_T("폴더가 존재하지 않습니다."), _T("경고"));
		return;
	}
}


/**
버튼 - 가로 붙이기
*/
void CImageJoinerDlg::OnBnClickedBtnCreate1()
{
	// https://m.blog.naver.com/PostView.nhn?blogId=tipsware&logNo=220367065310&proxyReferer=https:%2F%2Fwww.google.com%2F
}


/**
버튼 - 세로 붙이기
*/
void CImageJoinerDlg::OnBnClickedBtnCreate2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


/**
버튼 - 격자 붙이기
*/
void CImageJoinerDlg::OnBnClickedBtnCreate3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
