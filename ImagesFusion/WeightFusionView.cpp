// WeightFusionView.cpp : 实现文件
//

#include "stdafx.h"
#include "ImagesFusion.h"
#include "MainFrm.h"
#include "WeightFusionView.h"


// CWeightFusionView

IMPLEMENT_DYNCREATE(CWeightFusionView, CScrollView)

CWeightFusionView::CWeightFusionView()
{

}

CWeightFusionView::~CWeightFusionView()
{
}


BEGIN_MESSAGE_MAP(CWeightFusionView, CScrollView)
END_MESSAGE_MAP()

CImagesFusionDoc* CWeightFusionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagesFusionDoc)));
	return (CImagesFusionDoc*)m_pDocument;
}
// CWeightFusionView 绘图

void CWeightFusionView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView = this;
	SetScrollSizes(MM_TEXT, CSize(100, 100));
}

void CWeightFusionView::OnDraw(CDC* pDC)
{
	CImagesFusionDoc* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	IplImage* pImg = pDoc->GetIplImg(2);
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

	::SetStretchBltMode(MemDc.GetSafeHdc(), COLORONCOLOR);// 设置显示模式		
	::StretchDIBits(MemDc.GetSafeHdc(),					// 设备环境句柄
		0,                 //目标X坐标
		0,                  // 目标Y坐标
		lWidth,            // 目标宽度
		lHeight,           // 目标高度
		0,                // 源X坐标
		0,                 // 源Y坐标
		lWidth,           // 源宽度
		lHeight,          // 源高度
		pDibBites,                      // 指向dib像素的指针
		pInfo,           // 指向位图信息结构的指针
		DIB_RGB_COLORS,                 // 使用的颜色数目
		SRCCOPY);     // 光栅操作类型

	pDC->StretchBlt(0, 0, lWidth, lHeight, &MemDc, 0, 0, lWidth, lHeight, SRCCOPY);

	pDC->SelectObject(pOldBmp);
	SelectObject(MemDc, pOldBmp);
	DeleteDC(MemDc);
}


// CWeightFusionView 诊断

#ifdef _DEBUG
void CWeightFusionView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CWeightFusionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWeightFusionView 消息处理程序
LPBITMAPINFO CWeightFusionView::cv_CreatImgInfo(IplImage* pImg)
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
	memcpy(lpBmInfo, &BIH, 40);                   //  复制位图信息头

	if (bits == 8)
	{                           //  256 色位图
		for (i = 0; i < 256; i++)
		{                                    //  设置灰阶调色板
			ColorTab[i].rgbRed = ColorTab[i].rgbGreen = ColorTab[i].rgbBlue = (BYTE) i;
			ColorTab[i].rgbReserved = 0;
		}
		memcpy(lpBmInfo->bmiColors, ColorTab, 1024);
	}
	return lpBmInfo;
}
