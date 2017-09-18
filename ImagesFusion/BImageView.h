#pragma once

#include "ImagesFusionDoc.h"
// CBImageView ��ͼ

class CBImageView : public CScrollView
{
	DECLARE_DYNCREATE(CBImageView)

protected:
	CBImageView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBImageView();

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


