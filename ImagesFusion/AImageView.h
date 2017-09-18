#pragma once

#include "ImagesFusionDoc.h"
// CAImageView ��ͼ

class CAImageView : public CScrollView
{
	DECLARE_DYNCREATE(CAImageView)

protected:
	CAImageView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAImageView();

public:
	CImagesFusionDoc* GetDocument() const;
	LPBITMAPINFO cv_CreatImgInfo(IplImage* pImg);

private:
	IplImage* m_pvImg;

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


