// PCAFusionView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImagesFusion.h"
#include "MainFrm.h"
#include "PCAFusionView.h"


// CPCAFusionView

IMPLEMENT_DYNCREATE(CPCAFusionView, CScrollView)

CPCAFusionView::CPCAFusionView()
{

}

CPCAFusionView::~CPCAFusionView()
{
}


BEGIN_MESSAGE_MAP(CPCAFusionView, CScrollView)
END_MESSAGE_MAP()

CImagesFusionDoc* CPCAFusionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagesFusionDoc)));
	return (CImagesFusionDoc*)m_pDocument;
}
// CPCAFusionView ��ͼ

void CPCAFusionView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	((CMainFrame*)AfxGetMainWnd())->m_pPCAFusionView = this;
	SetScrollSizes(MM_TEXT, CSize(100, 100));
}

void CPCAFusionView::OnDraw(CDC* pDC)
{
	CImagesFusionDoc* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
	IplImage* pImg = pDoc->GetIplImg(3);
	if (pImg == NULL)
		return;

	LPBYTE pDibBites = (LPBYTE)pImg->imageData;
	LPBITMAPINFO pInfo = cv_CreatImgInfo(pImg);
	int lHeight = pImg->height;
	int lWidth = pImg->width;

	CDC MemDc;
	MemDc.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, lWidth, lHeight);
	CBitmap* pOldBmp = MemDc.SelectObject(&bitmap);

	::SetStretchBltMode(MemDc.GetSafeHdc(), COLORONCOLOR);// ������ʾģʽ		
	::StretchDIBits(MemDc.GetSafeHdc(),					// �豸�������
		0,                 //Ŀ��X����
		0,                  // Ŀ��Y����
		lWidth,            // Ŀ����
		lHeight,           // Ŀ��߶�
		0,                // ԴX����
		0,                 // ԴY����
		lWidth,           // Դ���
		lHeight,          // Դ�߶�
		pDibBites,                      // ָ��dib���ص�ָ��
		pInfo,           // ָ��λͼ��Ϣ�ṹ��ָ��
		DIB_RGB_COLORS,                 // ʹ�õ���ɫ��Ŀ
		SRCCOPY);     // ��դ��������

	pDC->StretchBlt(0, 0, lWidth, lHeight, &MemDc, 0, 0, lWidth, lHeight, SRCCOPY);

	pDC->SelectObject(pOldBmp);
	SelectObject(MemDc, pOldBmp);
	DeleteDC(MemDc);
}


// CPCAFusionView ���

#ifdef _DEBUG
void CPCAFusionView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPCAFusionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPCAFusionView ��Ϣ�������
LPBITMAPINFO CPCAFusionView::cv_CreatImgInfo(IplImage* pImg)
{
	if(pImg == NULL)
		return NULL;

	BITMAPINFOHEADER BIH = {40,1,1,1,8,0,0,0,0,0,0};
	LPBITMAPINFO lpBmInfo;
	int          wid, hei, bits, colors, i;
	RGBQUAD  ColorTab[256];
	wid =pImg->width;
	hei =pImg->height;
	bits = pImg->depth * pImg->nChannels;
	if (bits > 8) colors=0;
	else colors = 1<<bits;
	lpBmInfo = (LPBITMAPINFO) malloc(40 + 4*colors);
	BIH.biWidth = wid;
	BIH.biHeight = hei;
	BIH.biBitCount = (BYTE) bits;
	memcpy(lpBmInfo, &BIH, 40);                   //  ����λͼ��Ϣͷ

	if (bits == 8)
	{                           //  256 ɫλͼ
		for (i = 0; i < 256; i++)
		{                                    //  ���ûҽ׵�ɫ��
			ColorTab[i].rgbRed = ColorTab[i].rgbGreen = ColorTab[i].rgbBlue = (BYTE) i;
			ColorTab[i].rgbReserved = 0;
		}
		memcpy(lpBmInfo->bmiColors, ColorTab, 1024);
	}
	return lpBmInfo;
}