
// IP_ProgrammingDoc.cpp: CIPProgrammingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IP_Programming.h"
#endif

#include "IP_ProgrammingDoc.h"
#include <vector>

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIPProgrammingDoc

IMPLEMENT_DYNCREATE(CIPProgrammingDoc, CDocument)

BEGIN_MESSAGE_MAP(CIPProgrammingDoc, CDocument)
END_MESSAGE_MAP()


// CIPProgrammingDoc 생성/소멸

CIPProgrammingDoc::CIPProgrammingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CIPProgrammingDoc::~CIPProgrammingDoc()
{
}

BOOL CIPProgrammingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CIPProgrammingDoc serialization

void CIPProgrammingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CIPProgrammingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CIPProgrammingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CIPProgrammingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIPProgrammingDoc 진단

#ifdef _DEBUG
void CIPProgrammingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIPProgrammingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIPProgrammingDoc 명령

BOOL CIPProgrammingDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    CFileOpenDIg myDlg;
    if (myDlg.DoModal() != IDOK)
        return TRUE;

    fpInputImage = nullptr;
    if (_tfopen_s(&fpInputImage, lpszPathName, _T("rb")) != 0 || !fpInputImage)
    {
        AfxMessageBox(_T("파일 열기 실패"));
        return FALSE;
    }

    BITMAPFILEHEADER fileHeader{};
    BITMAPINFOHEADER infoHeader{};
    if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpInputImage) != 1 ||
        fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpInputImage) != 1)
    {
        fclose(fpInputImage);
        AfxMessageBox(_T("BMP 헤더 읽기 실패"));
        return FALSE;
    }

    // BMP 서명 확인 ('BM' == 0x4D42)
    if (fileHeader.bfType != 0x4D42 || infoHeader.biCompression != BI_RGB)
    {
        fclose(fpInputImage);
        AfxMessageBox(_T("지원하지 않는 BMP 포맷입니다 (비압축 BI_RGB만 지원)"));
        return FALSE;
    }

    toolbox.io.m_Width = infoHeader.biWidth;
    toolbox.io.m_Height = (infoHeader.biHeight < 0) ? -infoHeader.biHeight : infoHeader.biHeight;

    // 파일에서 읽을 한 줄 크기(패딩 포함)
    const int srcRowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4;

    // 소스 파일의 픽셀 데이터 위치로 이동
    _fseeki64(fpInputImage, fileHeader.bfOffBits, SEEK_SET);

    // 출력용 RGB 버퍼(패딩 없음) 준비: width * height * 3
    std::vector<UCHAR> rgb(toolbox.io.m_Width * toolbox.io.m_Height * 3);

    std::vector<UCHAR> rowBuf(srcRowSize);

    if (infoHeader.biBitCount == 24)
    {
        // 24bpp: 파일의 BGR을 그대로 읽어 rgb에 채움
        for (int y = 0; y < toolbox.io.m_Height; ++y)
        {
            if (fread(rowBuf.data(), 1, srcRowSize, fpInputImage) != (size_t)srcRowSize)
            {
                fclose(fpInputImage);
                AfxMessageBox(_T("BMP 데이터 읽기 실패"));
                return FALSE;
            }

            UCHAR* dst = rgb.data() + (size_t)y * toolbox.io.m_Width * 3;
            for (int x = 0; x < toolbox.io.m_Width; ++x)
            {
                dst[x * 3 + 0] = rowBuf[x * 3 + 0]; // B
                dst[x * 3 + 1] = rowBuf[x * 3 + 1]; // G
                dst[x * 3 + 2] = rowBuf[x * 3 + 2]; // R
            }
        }
    }
    else if (infoHeader.biBitCount == 8)
    {
        // 8bpp: 팔레트는 bfOffBits가 이미 건너뛰도록 보장
        for (int y = 0; y < toolbox.io.m_Height; ++y)
        {
            if (fread(rowBuf.data(), 1, srcRowSize, fpInputImage) != (size_t)srcRowSize)
            {
                fclose(fpInputImage);
                AfxMessageBox(_T("BMP 데이터 읽기 실패"));
                return FALSE;
            }

            UCHAR* dst = rgb.data() + (size_t)y * toolbox.io.m_Width * 3;
            for (int x = 0; x < toolbox.io.m_Width; ++x)
            {
                UCHAR g = rowBuf[x];
                dst[x * 3 + 0] = g;  // B
                dst[x * 3 + 1] = g;  // G
                dst[x * 3 + 2] = g;  // R
            }
        }
    }
    else
    {
        fclose(fpInputImage);
        AfxMessageBox(_T("현재 8/24비트 BMP만 지원합니다"));
        return FALSE;
    }

    fclose(fpInputImage);

    // 항상 컬러 DIB(24bpp, stride 포함)로 변환
    toolbox.io.IO_MakeColorImagetoBMP(rgb.data());

    // 화면 갱신
    UpdateAllViews(nullptr);
    return TRUE;
}


