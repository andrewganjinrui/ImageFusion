// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!m_wndSplitter.CreateStatic(this, 2, 3, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//在左边客户区新建一个命令视图窗口
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

	return TRUE; //重要
}
