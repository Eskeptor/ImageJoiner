#pragma once
#include <vector>
#include <algorithm>
using namespace std;


class CImageJoinerDlg : public CDialogEx
{
// 열거형 및 구조체 ===========================================================================
private:
	enum eMergeType
	{
		MergeNone = -1,
		MergeHorizon,
		MergeVertical,
		MergeGrid,

		MergeMax
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEJOINER_DIALOG };
#endif

// ==========================================================================================


// 멤버 변수 =================================================================================
private:
	CString m_strFolderPath;
	vector<CString> m_vImgList;

	CListBox m_lBoxImg;

protected:
	HICON m_hIcon;
// ==========================================================================================


// 함수 =====================================================================================
private:
	void InitControls();
	
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

};
