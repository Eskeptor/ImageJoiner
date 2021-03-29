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
	ON_BN_CLICKED(IDC_BTN_CREATOR, &CImageJoinerDlg::OnBnClickedBtnCreator)
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
	if (m_lBoxImg.m_hWnd == NULL)
		m_lBoxImg.SubclassDlgItem(IDC_LIST_IMAGE, this);

	if (m_cboxGrid.m_hWnd == NULL)
		m_cboxGrid.SubclassDlgItem(IDC_CBOX_GRID, this);

	CreateButtonEnabler(FALSE);
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

		m_lBoxImg.ResetContent();
		m_cboxGrid.ResetContent();
		CreateButtonEnabler(FALSE);
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
	strFolderFindPath.Format(_T("%s\\*.*"), m_strFolderPath.GetString());

	CFileFind fFind;
	BOOL bIsFindPath = fFind.FindFile(strFolderFindPath);

	if (bIsFindPath)
	{
		CString strCurFile = _T("");
		CString strCurFileEx = _T("");

		if (m_vImgList.empty() == false)
			m_vImgList.clear();

		m_lBoxImg.ResetContent();
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
				{
					m_lBoxImg.AddString(strCurFile);
					m_vImgList.push_back(fFind.GetFilePath());
				}
			}
		}

		CreateButtonEnabler(TRUE);
		MakeComboList(m_vImgList.size());
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
	if (ImageMerge(eMergeType::MergeHorizon))
		MessageBox(_T("이미지를 저장하였습니다."), _T("저장 완료"));
	else
		MessageBox(_T("이미지 저장에 실패하였습니다."), _T("저장 실패"));
}


/**
버튼 - 세로 붙이기
*/
void CImageJoinerDlg::OnBnClickedBtnCreate2()
{
	if (ImageMerge(eMergeType::MergeVertical))
		MessageBox(_T("이미지를 저장하였습니다."), _T("저장 완료"));
	else
		MessageBox(_T("이미지 저장에 실패하였습니다."), _T("저장 실패"));
}


/**
버튼 - 격자 붙이기
*/
void CImageJoinerDlg::OnBnClickedBtnCreate3()
{
	if (ImageMerge(eMergeType::MergeGrid))
		MessageBox(_T("이미지를 저장하였습니다."), _T("저장 완료"));
	else
		MessageBox(_T("이미지 저장에 실패하였습니다."), _T("저장 실패"));
}


/**
이미지 합치는 함수
@param		eType			Merge 타입
*/
bool CImageJoinerDlg::ImageMerge(eMergeType eType)
{
	if (eType <= eMergeType::MergeNone ||
		eType >= eMergeType::MergeMax)
		return false;

	if (m_vImgList.empty() == false)
	{
		const int SIZE = m_vImgList.size();
		CImage* arrImages = new CImage[SIZE];
		HRESULT hResult = 0;
		int nWidth = 0;
		int nHeight = 0;
		int nTempHeight = 0;

		if (eType == eMergeType::MergeGrid)					// 격자 붙이기
		{
			int nTotalWidth = 0;
			int nTotalHeight = 0;
			int nCurSelGrid = m_cboxGrid.GetCurSel();
			int nIdx = 0;
			for (int nY = 0; nY < m_vGridType.at(nCurSelGrid).nY; nY++)
			{
				for (int nX = 0; nX < m_vGridType.at(nCurSelGrid).nX; nX++)
				{
					nIdx = nY * m_vGridType.at(nCurSelGrid).nX + nX;
					if (nIdx >= SIZE)
						break;

					hResult = arrImages[nIdx].Load(m_vImgList.at(nIdx));

					if (FAILED(hResult))	
						break;

					nTotalWidth += arrImages[nIdx].GetWidth();
					nTotalHeight = arrImages[nIdx].GetHeight();

					if (nTempHeight < nTotalHeight)
						nTempHeight = nTotalHeight;
				}

				if (nWidth < nTotalWidth)
					nWidth = nTotalWidth;
				nTotalWidth = 0;

				nHeight += nTempHeight;
				nTempHeight = 0;
			}
		}
		else
		{
			for (int i = 0; i < SIZE; i++)
			{
				hResult = arrImages[i].Load(m_vImgList.at(i));

				if (FAILED(hResult))
					break;

				if (eType == eMergeType::MergeHorizon)				// 가로 붙이기
				{
					nWidth += arrImages[i].GetWidth();
					if (nHeight < arrImages[i].GetHeight())
						nHeight = arrImages[i].GetHeight();
				}
				else if (eType == eMergeType::MergeVertical)		// 세로 붙이기
				{
					nHeight += arrImages[i].GetHeight();
					if (nWidth < arrImages[i].GetWidth())
						nWidth = arrImages[i].GetWidth();
				}
			}
		}

		if (SUCCEEDED(hResult))
		{
			CImage newImg;
			newImg.Create(nWidth, nHeight, arrImages[0].GetBPP());

			int nCurAdder = 0;
			HDC hNewImgHDC = newImg.GetDC();
			HDC hImgHDC = NULL;

			if (eType == eMergeType::MergeGrid)				// 격자 붙이기
			{
				int nCurSelGrid = m_cboxGrid.GetCurSel();
				int nIdx = 0;
				int nCurXAdder = 0;
				int nCurYAdder = 0;
				int nBigAdder = 0;
				for (int nY = 0; nY < m_vGridType.at(nCurSelGrid).nY; nY++)
				{
					for (int nX = 0; nX < m_vGridType.at(nCurSelGrid).nX; nX++)
					{
						nIdx = nY * m_vGridType.at(nCurSelGrid).nX + nX;
						if (nIdx >= SIZE)
							break;

						hImgHDC = arrImages[nIdx].GetDC();

						::BitBlt(hNewImgHDC, nCurXAdder, nCurYAdder, arrImages[nIdx].GetWidth(), arrImages[nIdx].GetHeight(), hImgHDC, 0, 0, SRCCOPY);
						nCurXAdder += arrImages[nIdx].GetWidth();

						if (nBigAdder < arrImages[nIdx].GetHeight())
							nBigAdder = arrImages[nIdx].GetHeight();

						arrImages[nIdx].ReleaseDC();
					}

					nCurXAdder = 0;
					nCurYAdder += nBigAdder;
					nBigAdder = 0;
				}
			}
			else
			{
				for (int i = 0; i < SIZE; i++)
				{
					hImgHDC = arrImages[i].GetDC();

					if (eType == eMergeType::MergeHorizon)			// 가로 붙이기
					{
						::BitBlt(hNewImgHDC, nCurAdder, 0, arrImages[i].GetWidth(), nHeight, hImgHDC, 0, 0, SRCCOPY);
						nCurAdder += arrImages[i].GetWidth();
					}
					else if (eType == eMergeType::MergeVertical)	// 세로 붙이기
					{
						::BitBlt(hNewImgHDC, 0, nCurAdder, nWidth, arrImages[i].GetHeight(), hImgHDC, 0, 0, SRCCOPY);
						nCurAdder += arrImages[i].GetHeight();
					}
					arrImages[i].ReleaseDC();
				}
			}

			newImg.Save(_T("Test.png"), Gdiplus::ImageFormatPNG);
			newImg.ReleaseDC();
		}

		delete[] arrImages;
		arrImages = nullptr;

		if (SUCCEEDED(hResult))
			return true;
		else
			return false;
	}

	return false;
}


/**
붙이기 버튼을 활성화 또는 비활성화 하는 함수
@param		bIsEnable		활성화 혹은 비활성화
*/
void CImageJoinerDlg::CreateButtonEnabler(BOOL bIsEnable)
{
	GetDlgItem(IDC_BTN_CREATE_1)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_BTN_CREATE_2)->EnableWindow(bIsEnable);
	GetDlgItem(IDC_BTN_CREATE_3)->EnableWindow(bIsEnable);
}


/**
이미지 개수에 맞게 Grid 타입을 ComboBox 목록에 추가하는 함수
@param		nImgCnt			이미지 개수
*/
void CImageJoinerDlg::MakeComboList(int nImgCnt)
{
	if (nImgCnt <= 0)
		return;

	if (m_vGridType.empty() == false)
		m_vGridType.clear();

	m_cboxGrid.ResetContent();

	int nX = 0;
	int nY = 0;
	stGridType stGrid;
	CString strTemp = _T("");
	for (int i = 0; i < nImgCnt; i++)
	{
		nX = i + 1;
		nY = nImgCnt / nX;
		if (nX * nY < nImgCnt)
			nY += 1;

		strTemp.Format(_T("%d x %d"), nX, nY);
		m_cboxGrid.AddString(strTemp);

		stGrid.nX = nX;
		stGrid.nY = nY;
		m_vGridType.push_back(stGrid);
	}

	m_cboxGrid.SetCurSel(0);
}


/**
버튼 - 개발자
*/
void CImageJoinerDlg::OnBnClickedBtnCreator()
{
	TCHAR chBrowser[MAX_PATH] = { 0, };
	HFILE h = _lcreat("dummy.html", 0);
	_lclose(h);
	FindExecutable(_T("dummy.html"), NULL, chBrowser);
	DeleteFile(_T("dummy.html"));

	ShellExecute(this->m_hWnd, _T("open"), chBrowser, _T("https://github.com/eskeptor"), NULL, SW_SHOW);
}
