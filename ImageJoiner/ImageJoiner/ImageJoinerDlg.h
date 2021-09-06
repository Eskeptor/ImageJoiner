#pragma once
#include <vector>
#include <algorithm>

using namespace std;


class CImageJoinerDlg : public CDialogEx
{
// Enum and Struct ===========================================================================
private:
	enum class eMergeType
	{
		MergeNone = -1,
		MergeHorizon,
		MergeVertical,
		MergeGrid,

		MergeMax
	};

	struct stGridType		// Grid Attach Struct
	{
		int nX;		// Horiziontal Length
		int nY;		// Vertical Length
		stGridType(int nX = 0, int nY = 0) : nX(nX), nY(nY) {}
	};

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEJOINER_DIALOG };
#endif

// ==========================================================================================


// Member ===================================================================================
private:
	CString m_strFolderPath;			// Path to the folder where the image resides
	vector<CString> m_vImgList;			// Vectors that store the list of images

	CListBox m_lBoxImg;					// File list

	vector<stGridType> m_vGridType;		// Vectors used by Grid attachment
	CComboBox m_cboxGrid;				// Grid attach ComboBox

protected:
	HICON m_hIcon;

// ==========================================================================================


// Function =================================================================================
private:
	// MFC Control Init
	void InitControls();
	// Function that enables or disables the create button
	void CreateButtonEnabler(BOOL bIsEnable);
	// Function to add Grid type to ComboBox list for image count
	void MakeComboList(int nImgCnt);

	// Get Program Version
	CString GetProgVersion();
	
	// Image Save
	void ImageSave(eMergeType eType);
	// Image Merge
	bool ImageMerge(eMergeType eType, CString strSavePath, GUID imgFormat);

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
	afx_msg void OnBnClickedBtnCreator();
	afx_msg void OnDropFiles(HDROP hDropInfo);

// ==========================================================================================

};
