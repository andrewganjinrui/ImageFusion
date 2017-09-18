#pragma once
#include "ImagesFusionDoc.h"

// CImagesFusionView ��ͼ

class CImagesFusionView : public CListView
{
	DECLARE_DYNCREATE(CImagesFusionView)

protected:
	CImagesFusionView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CImagesFusionView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
	CImagesFusionDoc* GetDocument() const;

public:
	void InitializeListView();
	void DisplayParameters();
};


