#include "pch.h"
#include "GFunc.h"

CString GetExePath()
{
	CString strExePath = _T("");

	TCHAR szTemp[_MAX_PATH + 1] = { 0, };
	GetModuleFileName(AfxGetApp()->m_hInstance, szTemp, _MAX_PATH);
	strExePath = szTemp;

	int nLength = strExePath.ReverseFind(_T('\\'));
	if (nLength > 0)
		strExePath = strExePath.Left(nLength + 1);

	return strExePath;
}