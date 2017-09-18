// MainFrm.h : CMainFrame ��Ľӿ�
//

#include "PCAFusionView.h"
#include "WaveletFusionView.h"
#include "WeightFusionView.h"
#include "ImagesFusionView.h"
#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CSplitterWnd m_wndSplitter;

	CPCAFusionView* m_pPCAFusionView;
	CWaveletFusionView* m_pWaveletFusionView;
	CWeightFusionView* m_pWeightFusionView;

	CImagesFusionView* m_pImagesFusionView;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


