#pragma once
#include <vector>
#include <algorithm>
using namespace std;


class CImageJoinerDlg : public CDialogEx
{
// 열거형 및 구조체 ===========================================================================
private:
	enum class eMergeType
	{
		MergeNone = -1,
		MergeHorizon,
		MergeVertical,
		MergeGrid,

		MergeMax
	};

	struct stGridType		// 격자 붙이기 구조체
	{
		int nX;		// 가로 개수
		int nY;		// 세로 개수
		stGridType(int nX = 0, int nY = 0) : nX(nX), nY(nY) {}
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEJOINER_DIALOG };
#endif

// ==========================================================================================


// 멤버 변수 =================================================================================
private:
	CString m_strFolderPath;			// 이미지가 있는 폴더의 경로
	vector<CString> m_vImgList;			// 이미지 리스트를 저장하는 벡트

	CListBox m_lBoxImg;					// 파일 리스트

	vector<stGridType> m_vGridType;		// 격자 붙이기에서 사용하는 벡터
	CComboBox m_cboxGrid;

protected:
	HICON m_hIcon;
// ==========================================================================================


// 함수 =====================================================================================
private:
	void InitControls();
	void CreateButtonEnabler(BOOL bIsEnable);
	void MakeComboList(int nImgCnt);
	
	bool ImageMerge(eMergeType eType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CImageJoinerDlg(CWnd* pParent = nullptr);

	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnFolderCheck();
	afx_msg void OnBnClickedBtnCreate1();
	afx_msg void OnBnClickedBtnCreate2();
	afx_msg void OnBnClickedBtnCreate3();

// ==========================================================================================

	afx_msg void OnBnClickedBtnCreator();
};
