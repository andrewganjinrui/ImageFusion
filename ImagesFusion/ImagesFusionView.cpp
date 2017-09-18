// ImagesFusionView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImagesFusion.h"
#include "MainFrm.h"
#include "ImagesFusionView.h"


// CImagesFusionView

IMPLEMENT_DYNCREATE(CImagesFusionView, CListView)

CImagesFusionView::CImagesFusionView()
{

}

CImagesFusionView::~CImagesFusionView()
{
}

BEGIN_MESSAGE_MAP(CImagesFusionView, CListView)
END_MESSAGE_MAP()


// CImagesFusionView ���

#ifdef _DEBUG
void CImagesFusionView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CImagesFusionView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImagesFusionView ��Ϣ�������
CImagesFusionDoc* CImagesFusionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagesFusionDoc)));
	return (CImagesFusionDoc*)m_pDocument;
}

BOOL CImagesFusionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CImagesFusionView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView = this;
	InitializeListView();
}

void CImagesFusionView::InitializeListView()
{
	CListCtrl& theListCtrl = GetListCtrl();
	theListCtrl.SetRedraw(FALSE);

	DWORD dwExStyle = LVS_EX_FULLROWSELECT;
	theListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)dwExStyle, (LPARAM)dwExStyle);
	COLORREF crYellow = RGB(240, 240, 2); //light yellow
	theListCtrl.SetBkColor(crYellow);
	theListCtrl.SetTextBkColor(crYellow);

	CString str;
	for (int i = 0; i <= 3; i++)
	{
		if (i == 0)
		{
			str = _T("                        ");
			theListCtrl.InsertColumn(i, str);
		}
		if (i == 1)
		{
			str = _T("       ʱ��      ");
			theListCtrl.InsertColumn(i, str);
		}
		if (i == 2)
		{
			str = _T("          ����Ϣ         ");
			theListCtrl.InsertColumn(i, str);
		}
		if (i == 3)
		{
			str = _T("          ������         ");
			theListCtrl.InsertColumn(i, str);
		}
	}

	for(UINT i = 0; i<= 3; i++)
		theListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
}

void CImagesFusionView::DisplayParameters()
{
	//The functions GetDocument() and GetParentFrame()->GetActiveDocument() won't always work 
	//here, so we will use the app's doc pointer:
	CImagesFusionDoc* pDoc = GetDocument();
	ParameterVect MyVect = pDoc->m_ParameterVect;

	CListCtrl& theListCtrl = GetListCtrl();
	theListCtrl.SetRedraw(FALSE);
	theListCtrl.DeleteAllItems();

	CString str;
	for (int k = 0; k < MyVect.size(); k++)
	{
		str = MyVect[k].AlgorithmName;
		theListCtrl.InsertItem(k, str);

		for (int j = 1; j <= 3; j++)
		{
			if (j == 1)
			{
				str = MyVect[k].TimeStr;
				theListCtrl.SetItemText(k, j, str);
			}
			if (j == 2)
			{
				str.Format(_T("%.3f"), MyVect[k].MI);
				theListCtrl.SetItemText(k, j, str);
			}
			if (j == 3)
			{
				str.Format(_T("%.3f"), MyVect[k].UE);
				theListCtrl.SetItemText(k, j, str);
			}
		}
	} 
	theListCtrl.SetRedraw(TRUE);
	theListCtrl.Invalidate();
}
