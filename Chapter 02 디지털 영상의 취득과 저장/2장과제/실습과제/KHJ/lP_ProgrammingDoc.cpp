
// lP_ProgrammingDoc.cpp: ClPProgrammingDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "lP_Programming.h"
#endif

#include "lP_ProgrammingDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ClPProgrammingDoc

IMPLEMENT_DYNCREATE(ClPProgrammingDoc, CDocument)

BEGIN_MESSAGE_MAP(ClPProgrammingDoc, CDocument)
END_MESSAGE_MAP()


// ClPProgrammingDoc 생성/소멸

ClPProgrammingDoc::ClPProgrammingDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

	// 영상 데이터 초기화
	m_InputImage = NULL;
	m_OutputImage = NULL;
	m_Width = 0;
	m_Height = 0;
	m_Size = 0;
}

ClPProgrammingDoc::~ClPProgrammingDoc()
{
	// 메모리 해제
	if (m_InputImage) {
		delete[] m_InputImage;
		m_InputImage = NULL;
	}
	if (m_OutputImage) {
		delete[] m_OutputImage;
		m_OutputImage = NULL;
	}
}

// BMP 파일 로드 함수
void ClPProgrammingDoc::LoadImageFile(CString filename)
{
	CFile file;
	if (!file.Open(filename, CFile::modeRead)) {
		AfxMessageBox(_T("파일을 열 수 없습니다."));
		return;
	}

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	file.Read(&fileHeader, sizeof(BITMAPFILEHEADER));
	file.Read(&infoHeader, sizeof(BITMAPINFOHEADER));

	m_Width = infoHeader.biWidth;
	m_Height = abs(infoHeader.biHeight);

	if (m_InputImage) delete[] m_InputImage;
	if (m_OutputImage) delete[] m_OutputImage;

	m_Size = m_Width * m_Height;
	m_InputImage = new unsigned char[m_Size];
	m_OutputImage = new unsigned char[m_Size];

	file.Seek(fileHeader.bfOffBits, CFile::begin);

	if (infoHeader.biBitCount == 24) {
		// 24비트 컬러를 그레이스케일로 변환
		unsigned char* tempBuffer = new unsigned char[m_Width * 3];

		for (int i = m_Height - 1; i >= 0; i--) {
			file.Read(tempBuffer, m_Width * 3);
			for (int j = 0; j < m_Width; j++) {
				// RGB를 그레이스케일로 변환 (Y = 0.299*R + 0.587*G + 0.114*B)
				int gray = (int)(0.299 * tempBuffer[j * 3 + 2] + 0.587 * tempBuffer[j * 3 + 1] + 0.114 * tempBuffer[j * 3]);
				m_InputImage[i * m_Width + j] = (unsigned char)gray;
			}
			// BMP는 4바이트 정렬이므로 패딩 건너뛰기
			int padding = (4 - (m_Width * 3) % 4) % 4;
			if (padding > 0) {
				unsigned char dummy[3];
				file.Read(dummy, padding);
			}
		}
		delete[] tempBuffer;
	}
	else if (infoHeader.biBitCount == 8) {
		// 8비트 인덱스 컬러 - 팔레트 건너뛰고 인덱스만 읽기
		for (int i = m_Height - 1; i >= 0; i--) {
			file.Read(&m_InputImage[i * m_Width], m_Width);
			// 4바이트 정렬 패딩
			int padding = (4 - m_Width % 4) % 4;
			if (padding > 0) {
				unsigned char dummy[3];
				file.Read(dummy, padding);
			}
		}
	}
	else if (infoHeader.biBitCount == 1) {
		// 1비트 모노크롬
		int bytesPerRow = (m_Width + 7) / 8;
		int paddedBytesPerRow = (bytesPerRow + 3) & ~3; // 4바이트 정렬
		unsigned char* tempBuffer = new unsigned char[paddedBytesPerRow];

		for (int i = m_Height - 1; i >= 0; i--) {
			file.Read(tempBuffer, paddedBytesPerRow);
			for (int j = 0; j < m_Width; j++) {
				int byteIndex = j / 8;
				int bitIndex = 7 - (j % 8);
				int bit = (tempBuffer[byteIndex] >> bitIndex) & 1;
				m_InputImage[i * m_Width + j] = bit ? 255 : 0;
			}
		}
		delete[] tempBuffer;
	}
	else {
		AfxMessageBox(_T("지원하지 않는 BMP 형식입니다."));
		file.Close();
		return;
	}

	file.Close();
	UpdateAllViews(NULL);
}

// Raw 파일 생성 함수
void ClPProgrammingDoc::CreateTestImage()
{
	m_Width = 256;
	m_Height = 256;
	m_Size = m_Width * m_Height;

	if (m_InputImage) delete[] m_InputImage;
	if (m_OutputImage) delete[] m_OutputImage;

	m_InputImage = new unsigned char[m_Size];
	m_OutputImage = new unsigned char[m_Size];

	// 간단한 그라디언트 패턴 생성
	for (int i = 0; i < m_Height; i++) {
		for (int j = 0; j < m_Width; j++) {
			m_InputImage[i * m_Width + j] = (unsigned char)((i + j) / 2);
		}
	}

	UpdateAllViews(NULL);

}

// Raw 데이터 로드 함수
void ClPProgrammingDoc::LoadRawData(CString filename, int width, int height, int channels)
{
	CFile file;
	if (!file.Open(filename, CFile::modeRead)) {
		AfxMessageBox(_T("Raw 파일을 열 수 없습니다."));
		return;
	}

	m_Width = width;
	m_Height = height;
	m_Size = m_Width * m_Height * channels;

	// 기존 메모리 해제
	if (m_InputImage) delete[] m_InputImage;
	if (m_OutputImage) delete[] m_OutputImage;

	// 새 메모리 할당
	m_InputImage = new unsigned char[m_Size];
	m_OutputImage = new unsigned char[m_Size];

	// Raw 데이터 읽기
	file.Read(m_InputImage, m_Size);
	file.Close();

	// 화면 업데이트
	UpdateAllViews(NULL);
}

BOOL ClPProgrammingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// ClPProgrammingDoc serialization

void ClPProgrammingDoc::Serialize(CArchive& ar)
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
void ClPProgrammingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void ClPProgrammingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void ClPProgrammingDoc::SetSearchContent(const CString& value)
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

// ClPProgrammingDoc 진단

#ifdef _DEBUG
void ClPProgrammingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void ClPProgrammingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



// ClPProgrammingDoc 명령
