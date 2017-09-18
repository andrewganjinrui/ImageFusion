#pragma once


#include "ImagesFusionDoc.h"
// CWeightFusionView 视图

class CWeightFusionView : public CScrollView
{
	DECLARE_DYNCREATE(CWeightFusionView)

protected:
	CWeightFusionView();           // 动态创建所使用的受保护的构造函数
	virtual ~CWeightFusionView();

public:
	CImagesFusionDoc* GetDocument() const;
	LPBITMAPINFO cv_CreatImgInfo(IplImage* pImg);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
};


