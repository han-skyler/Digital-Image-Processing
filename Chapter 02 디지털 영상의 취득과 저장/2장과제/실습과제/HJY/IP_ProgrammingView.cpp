
// IP_ProgrammingView.cpp: CIPProgrammingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IP_Programming.h"
#endif

#include "IP_ProgrammingDoc.h"
#include "IP_ProgrammingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIPProgrammingView

IMPLEMENT_DYNCREATE(CIPProgrammingView, CScrollView)

BEGIN_MESSAGE_MAP(CIPProgrammingView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIPProgrammingView 생성/소멸

CIPProgrammingView::CIPProgrammingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CIPProgrammingView::~CIPProgrammingView()
{
}

BOOL CIPProgrammingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CIPProgrammingView 그리기

void CIPProgrammingView::OnDraw(CDC* pDC)
{
	auto* pDoc = static_cast<CIPProgrammingDoc*>(GetDocument());  // ✅ 문서로 캐스팅
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	auto& io = pDoc->toolbox.io;
	if (!io.m_BMPbuf || io.m_Width <= 0 || io.m_Height <= 0) return;

	BITMAPINFO bmi{};
	bmi.bmiHeader = io.m_BMPheader; // 위에서 24bpp + stride 반영된 헤더

	StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, io.m_Width, io.m_Height,            // 화면 출력 크기
		0, 0, io.m_Width, io.m_Height,            // 소스 크기
		io.m_BMPbuf,
		&bmi,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}


void CIPProgrammingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CIPProgrammingView 인쇄

BOOL CIPProgrammingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIPProgrammingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIPProgrammingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CIPProgrammingView 진단

#ifdef _DEBUG
void CIPProgrammingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CIPProgrammingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CIPProgrammingDoc* CIPProgrammingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIPProgrammingDoc)));
	return (CIPProgrammingDoc*)m_pDocument;
}
#endif //_DEBUG


// CIPProgrammingView 메시지 처리기
