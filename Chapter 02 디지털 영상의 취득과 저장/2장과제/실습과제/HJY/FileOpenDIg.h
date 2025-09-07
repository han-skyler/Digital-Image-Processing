#pragma once
#include "afxdialogex.h"


// CFileOpenDIg 대화 상자

class CFileOpenDIg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileOpenDIg)

public:
	CFileOpenDIg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFileOpenDIg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int GetWidth() { return m_DlgWidth; };
	int GetHeight() { return m_DlgHeight; };
private:
	int m_DlgWidth;
	int m_DlgHeight;


};
