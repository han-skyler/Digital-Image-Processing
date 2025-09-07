// FileOpenDIg.cpp: 구현 파일
//

#include "pch.h"
#include "IP_Programming.h"
#include "afxdialogex.h"
#include "FileOpenDIg.h"


// CFileOpenDIg 대화 상자

IMPLEMENT_DYNAMIC(CFileOpenDIg, CDialogEx)

CFileOpenDIg::CFileOpenDIg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_DlgWidth(0)
	, m_DlgHeight(0)
{

}

CFileOpenDIg::~CFileOpenDIg()
{
}

void CFileOpenDIg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DlgWidth);
	DDX_Text(pDX, IDC_EDIT2, m_DlgHeight);
}


BEGIN_MESSAGE_MAP(CFileOpenDIg, CDialogEx)
END_MESSAGE_MAP()


// CFileOpenDIg 메시지 처리기
