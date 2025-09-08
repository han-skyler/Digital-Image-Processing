#include "pch.h"
#include "CIP_IO.h"

CIP_IO::CIP_IO(void)
	: m_BMPbuf(NULL)
	, m_Inputbuf(NULL)
	, m_Outputbuf(NULL)
	, m_Width(0)
	, m_Height(0)
{
	// BMP 헤더 정보 세팅
	m_BMPheader.biSize = sizeof(BITMAPINFOHEADER);
	m_BMPheader.biPlanes = 1;
	m_BMPheader.biBitCount = 24;
	m_BMPheader.biCompression = BI_RGB;
	m_BMPheader.biSizeImage = 0;
	m_BMPheader.biXPelsPerMeter = 0;
	m_BMPheader.biYPelsPerMeter = 0;
	m_BMPheader.biClrUsed = 0;
	m_BMPheader.biClrImportant = 0;
}

CIP_IO::~CIP_IO(void)
{
	if (m_BMPbuf)
		free(m_BMPbuf);

	if (m_Inputbuf)
	{
		free(m_Inputbuf[0]);
		free(m_Inputbuf);
	}

	if (m_Outputbuf)
	{
		free(m_Outputbuf[0]);
		free(m_Outputbuf);
	}
}

void CIP_IO::IO_MakeGrayImagetoBMP(UCHAR** imgbuf)
{
	m_BMPheader.biWidth = m_Width;
	m_BMPheader.biHeight = m_Height;

	int i, j, idx = 0;

	if (!m_BMPbuf)
		m_BMPbuf = (UCHAR*)calloc(sizeof(UCHAR), m_Width * m_Height * 3);

	for (i = m_Height-1; i >= 0; i--)
	{
		for (j = 0; j < m_Width; j++)
		{
			m_BMPbuf[idx++] = imgbuf[i][j];
			m_BMPbuf[idx++] = imgbuf[i][j];
			m_BMPbuf[idx++] = imgbuf[i][j];
		}
	}
}

UCHAR** CIP_IO::memory_alloc2D(int width, int height)
{
	UCHAR** ppMem2D = 0;
	int i;

	// 2차원 배열 메모리 할당
	ppMem2D = (UCHAR**)calloc(sizeof(UCHAR*), height);
	if (ppMem2D == 0) return 0;

	*ppMem2D = (UCHAR*)calloc(sizeof(UCHAR), height * width);
	if ((*ppMem2D) == 0)
	{
		free(ppMem2D);
		return 0;
	}

	for (i = 1; i < height; i++)
	{
		ppMem2D[i] = ppMem2D[i - 1] + width;
	}

	return ppMem2D;
}