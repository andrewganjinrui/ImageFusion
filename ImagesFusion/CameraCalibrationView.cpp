// CameraCalibrationView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImagesFusion.h"
#include "CameraCalibrationView.h"


// CCameraCalibrationView

IMPLEMENT_DYNCREATE(CCameraCalibrationView, CFormView)

CCameraCalibrationView::CCameraCalibrationView()
	: CFormView(CCameraCalibrationView::IDD)
{

}

CCameraCalibrationView::~CCameraCalibrationView()
{
}

void CCameraCalibrationView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraCalibrationView, CFormView)
END_MESSAGE_MAP()


// CCameraCalibrationView ���

#ifdef _DEBUG
void CCameraCalibrationView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCameraCalibrationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCameraCalibrationView ��Ϣ�������
