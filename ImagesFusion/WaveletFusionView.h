#pragma once


#include "ImagesFusionDoc.h"
// CWaveletFusionView ��ͼ

class CWaveletFusionView : public CScrollView
{
	DECLARE_DYNCREATE(CWaveletFusionView)

protected:
	CWaveletFusionView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWaveletFusionView();

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
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
};


