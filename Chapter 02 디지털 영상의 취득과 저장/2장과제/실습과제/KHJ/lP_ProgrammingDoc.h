
// lP_ProgrammingDoc.h: ClPProgrammingDoc 클래스의 인터페이스
//


#pragma once


class ClPProgrammingDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	ClPProgrammingDoc() noexcept;
	DECLARE_DYNCREATE(ClPProgrammingDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~ClPProgrammingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 2장 프로젝트 과제
public:
	// 영상 데이터 저장 변수들
	unsigned char* m_InputImage;    // 원본 영상 데이터
	unsigned char* m_OutputImage;   // 처리된 영상 데이터
	int m_Width;                    // 영상 가로 크기
	int m_Height;                   // 영상 세로 크기
	int m_Size;                     // 영상 전체 크기

	// 영상 처리 함수들
	void LoadImageFile(CString filename);  // 이미지 파일 로드
	void LoadRawData(CString filename, int width, int height, int channels = 1);  // Raw 데이터 로드
	void DisplayImage();            // 영상 화면 출력
	void CreateTestImage();
protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
