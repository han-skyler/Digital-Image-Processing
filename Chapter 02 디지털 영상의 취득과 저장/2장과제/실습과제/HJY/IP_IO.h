#pragma once

class CIP_IO
{
public:
    BITMAPINFOHEADER m_BMPheader;   // 입력 영상에 대한 BMP 헤더
    UCHAR* m_BMPbuf;      // BMP 이미지 출력 버퍼
    UCHAR** m_Inputbuf;    // 입력 이미지 버퍼
    UCHAR** m_Outputbuf;   // 출력 이미지 버퍼

    int m_Width;   // 이미지 너비
    int m_Height;  // 이미지 높이

public:
    CIP_IO(void);
    ~CIP_IO(void);

    void IO_MakeGrayImagetoBMP(UCHAR** imgbuf);       // Gray → BMP 변환
    void IO_MakeColorImagetoBMP(UCHAR* rgbbuf);
    UCHAR** memory_alloc2D(int width, int height);    // 2D 메모리 할당
};
