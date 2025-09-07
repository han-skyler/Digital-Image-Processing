
// lP_ProgrammingView.cpp: ClPProgrammingView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "lP_Programming.h"
#endif

#include "lP_ProgrammingDoc.h"
#include "lP_ProgrammingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ClPProgrammingView

IMPLEMENT_DYNCREATE(ClPProgrammingView, CScrollView)

BEGIN_MESSAGE_MAP(ClPProgrammingView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN_RAW, &ClPProgrammingView::OnFileOpenRaw)
	ON_COMMAND(ID_FILE_OPEN_BMP, &ClPProgrammingView::OnFileOpenBmp)
	ON_COMMAND(ID_CREATE_TEST_IMAGE, &ClPProgrammingView::OnCreateTestImage)
END_MESSAGE_MAP()

// 메시지 핸들러 함수
void ClPProgrammingView::OnFileOpenBmp()
{
	CFileDialog dlg(TRUE, _T("bmp"), NULL,
		OFN_FILEMUSTEXIST, _T("BMP Files (*.bmp)|*.bmp||"));

	if (dlg.DoModal() == IDOK) {
		GetDocument()->LoadImageFile(dlg.GetPathName());
	}
}

void ClPProgrammingView::OnCreateTestImage()
{
	GetDocument()->CreateTestImage();
}

void ClPProgrammingView::OnFileOpenRaw()
{
	CFileDialog dlg(TRUE, _T("raw"), NULL,
		OFN_FILEMUSTEXIST, _T("Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK) {
		// Raw 파일은 크기 정보가 없으므로 사용자 입력 필요
		// 간단히 512x512 흑백으로 가정
		GetDocument()->LoadRawData(dlg.GetPathName(), 512, 512, 1);
	}
}

// ClPProgrammingView 생성/소멸

ClPProgrammingView::ClPProgrammingView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

ClPProgrammingView::~ClPProgrammingView()
{
}

BOOL ClPProgrammingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// ClPProgrammingView 그리기

void ClPProgrammingView::OnDraw(CDC* pDC)
{
	ClPProgrammingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// 영상 데이터가 있는 경우에만 출력
	if (pDoc->m_InputImage != NULL) {
		// 동적으로 BITMAPINFO 할당 (팔레트 공간 포함)
		int paletteSize = sizeof(RGBQUAD) * 256;
		BITMAPINFO* pBmpInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + paletteSize);

		memset(pBmpInfo, 0, sizeof(BITMAPINFOHEADER) + paletteSize);
		pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBmpInfo->bmiHeader.biWidth = pDoc->m_Width;
		pBmpInfo->bmiHeader.biHeight = -pDoc->m_Height;
		pBmpInfo->bmiHeader.biPlanes = 1;
		pBmpInfo->bmiHeader.biBitCount = 8;
		pBmpInfo->bmiHeader.biCompression = BI_RGB;

		// 그레이스케일 팔레트 생성
		for (int i = 0; i < 256; i++) {
			pBmpInfo->bmiColors[i].rgbRed = i;
			pBmpInfo->bmiColors[i].rgbGreen = i;
			pBmpInfo->bmiColors[i].rgbBlue = i;
			pBmpInfo->bmiColors[i].rgbReserved = 0;
		}

		// 영상 출력
		SetDIBitsToDevice(pDC->GetSafeHdc(),
			10, 10,
			pDoc->m_Width, pDoc->m_Height,
			0, 0,
			0, pDoc->m_Height,
			pDoc->m_InputImage,
			pBmpInfo,
			DIB_RGB_COLORS);

		// 메모리 해제
		free(pBmpInfo);
	}
}


// ClPProgrammingView 인쇄

BOOL ClPProgrammingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void ClPProgrammingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void ClPProgrammingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// ClPProgrammingView 진단

#ifdef _DEBUG
void ClPProgrammingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void ClPProgrammingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

ClPProgrammingDoc* ClPProgrammingView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ClPProgrammingDoc)));
	return (ClPProgrammingDoc*)m_pDocument;
}
#endif //_DEBUG


// ClPProgrammingView 메시지 처리기


// 창 크기 변경하기
void ClPProgrammingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// 스크롤 크기 설정 (가로 800, 세로 600 픽셀)
	CSize sizeTotal(800, 600);
	CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
	CSize sizeLine(20, 20);

	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

	// 창 크기 조정 (선택사항)
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

