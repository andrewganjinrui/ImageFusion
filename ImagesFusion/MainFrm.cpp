// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ImagesFusion.h"

#include "MainFrm.h"

#include "ImagesFusionView.h"
#include "AImageView.h"
#include "BImageView.h"
#include "CameraCalibrationView.h"
#include "PCAFusionView.h"
#include "WaveletFusionView.h"
#include "WeightFusionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_pPCAFusionView = NULL;
	m_pWaveletFusionView = NULL;
	m_pWeightFusionView = NULL;

	m_pImagesFusionView = NULL;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(!m_wndSplitter.CreateStatic(this, 2, 3, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//����߿ͻ����½�һ��������ͼ����
	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CImagesFusionView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	if(!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CAImageView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	if(!m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CBImageView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	if(!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CWeightFusionView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	if(!m_wndSplitter.CreateView(1, 1, RUNTIME_CLASS(CPCAFusionView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	if(!m_wndSplitter.CreateView(1, 2, RUNTIME_CLASS(CWaveletFusionView), CSize(0,0), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	m_wndSplitter.SetRowInfo( 0, 350, 50 );
	m_wndSplitter.SetColumnInfo( 0, 450, 50 );
	m_wndSplitter.SetColumnInfo( 1, 450, 50 );

	return TRUE; //��Ҫ
}
