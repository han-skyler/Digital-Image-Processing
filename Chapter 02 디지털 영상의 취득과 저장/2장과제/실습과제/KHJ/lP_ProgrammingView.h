
// lP_ProgrammingView.h: ClPProgrammingView 클래스의 인터페이스
//

#pragma once


class ClPProgrammingView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	ClPProgrammingView() noexcept;
	DECLARE_DYNCREATE(ClPProgrammingView)

// 특성입니다.
public:
	ClPProgrammingDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); 
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~ClPProgrammingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpenRaw();   // Raw 파일 열기
	afx_msg void OnFileOpenBmp();   // BMP 파일 열기
	afx_msg void OnCreateTestImage();
};


#ifndef _DEBUG  // lP_ProgrammingView.cpp의 디버그 버전
inline ClPProgrammingDoc* ClPProgrammingView::GetDocument() const
   { return reinterpret_cast<ClPProgrammingDoc*>(m_pDocument); }
#endif

