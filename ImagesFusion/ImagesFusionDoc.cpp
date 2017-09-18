// ImagesFusionDoc.cpp : CImagesFusionDoc 类的实现
//

#include "stdafx.h"
#include "ImagesFusion.h"

#include "ImagesFusionDoc.h"
#include "MainFrm.h"
#include "AImageView.h"
#include "BImageView.h"
#include "WvltFusion.h"
#include "PCAFusion.h"
#include "Fusion.h"
#include "WaveletFusion.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImagesFusionDoc

IMPLEMENT_DYNCREATE(CImagesFusionDoc, CDocument)

BEGIN_MESSAGE_MAP(CImagesFusionDoc, CDocument)
	ON_COMMAND(ID_OPEN_AIMAGE, &CImagesFusionDoc::OnOpenAimage)
	ON_COMMAND(ID_OPENBIMAGE, &CImagesFusionDoc::OnOpenbimage)
	ON_COMMAND(ID_WAVELETFUSION, &CImagesFusionDoc::OnWaveletfusion)
	ON_COMMAND(ID_PCAFUSION, &CImagesFusionDoc::OnPcafusion)
	ON_COMMAND(ID_WEIGHTFUSION, &CImagesFusionDoc::OnWeightfusion)
	ON_COMMAND(ID_EvaluateAlgorithm, &CImagesFusionDoc::OnEvaluatealgorithm)
	ON_COMMAND(ID_CORRELATIONFUSION, &CImagesFusionDoc::OnCorrelationfusion)
	ON_COMMAND(ID_MULFUSION, &CImagesFusionDoc::OnMulfusion)
	ON_COMMAND(ID_LINEARMULFUSION, &CImagesFusionDoc::OnLinearmulfusion)
	ON_COMMAND(ID_BROVRYFUSION, &CImagesFusionDoc::OnBrovryfusion)
END_MESSAGE_MAP()


// CImagesFusionDoc 构造/析构

CImagesFusionDoc::CImagesFusionDoc()
{
	// TODO: 在此添加一次性构造代码
	m_pAImage = NULL;
	m_pBImage = NULL;
	m_pPcaFusionImage = NULL;
	m_pFusionImage = NULL;
	m_pWaveletFusionImage = NULL;
}

CImagesFusionDoc::~CImagesFusionDoc()
{
	FreeImage(0);
	FreeImage(1);
	if (m_pPcaFusionImage) 
	{
		cvReleaseImage(&m_pPcaFusionImage); 
		m_pPcaFusionImage = NULL;
	}
	if (m_pFusionImage) 
	{
		cvReleaseImage(&m_pFusionImage); 
		m_pFusionImage = NULL;
	}
	if (m_pWaveletFusionImage) 
	{
		cvReleaseImage(&m_pWaveletFusionImage); 
		m_pWaveletFusionImage = NULL;
	}

	m_ParameterVect.clear();
}

BOOL CImagesFusionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImagesFusionDoc 序列化

void CImagesFusionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CImagesFusionDoc 诊断

#ifdef _DEBUG
void CImagesFusionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImagesFusionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImagesFusionDoc 命令
void CImagesFusionDoc::FreeImage(int K)
{
	if (K == 0)
	{
		if (m_pAImage) 
		{
			cvReleaseImage(&m_pAImage); 
			m_pAImage = NULL;
		}
	}
	else if(K == 1)
	{
		if (m_pBImage) 
		{
			cvReleaseImage(&m_pBImage);
			m_pBImage = NULL;
		}
	}	
}

IplImage* CImagesFusionDoc::GetIplImg(int K)
{
	if(K == 0)
		return m_pAImage;
	else if(K == 1)
		return m_pBImage;
	else if(K == 2)
		return m_pFusionImage;
	else if(K == 3)
		return m_pPcaFusionImage;
	else if(K == 4)
		return m_pWaveletFusionImage;
}

void CImagesFusionDoc::OnOpenAimage()
{
	// TODO: 在此添加命令处理程序代码
    FreeImage(0);

	CFileDialog filedlg(true);
	filedlg.m_ofn.lpstrTitle = _T("请选择所要打开文件A");
	filedlg.m_ofn.lpstrFilter = _T("ALL file(*.*)\0*.*\0\0");
	CString strfile;
	if (filedlg.DoModal() == IDOK)
	{
		strfile = filedlg.GetPathName();
		m_pAImage = cvLoadImage(strfile);
		cvFlip(m_pAImage);

		int lHeight = m_pAImage->height;
		int lWidth = m_pAImage->width;
		CSize sizeDoc = CSize(lWidth, lHeight);

		POSITION pos;
		pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CScrollView* pVeiw = (CScrollView*)GetNextView(pos);
			if (pVeiw->IsKindOf(RUNTIME_CLASS(CAImageView)))
			{
				pVeiw->SetScrollSizes(MM_TEXT, sizeDoc);
				pVeiw->RedrawWindow();
				break;
			}
		}
	}
}

void CImagesFusionDoc::OnOpenbimage()
{
	// TODO: 在此添加命令处理程序代码
	FreeImage(1);

	CFileDialog filedlg(true);
	filedlg.m_ofn.lpstrTitle = _T("请选择所要打开文件B");
	filedlg.m_ofn.lpstrFilter = _T("ALL file(*.*)\0*.*\0\0");
	CString strfile;
	if (filedlg.DoModal() == IDOK)
	{
		strfile = filedlg.GetPathName();

		m_pBImage = cvLoadImage(strfile);
		cvFlip(m_pBImage);

		int lHeight = m_pBImage->height;
		int lWidth = m_pBImage->width;
		CSize sizeDoc = CSize(lWidth, lHeight);

		POSITION pos;
		pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CScrollView* pVeiw = (CScrollView*)GetNextView(pos);
			if (pVeiw->IsKindOf(RUNTIME_CLASS(CBImageView)))
			{
				pVeiw->SetScrollSizes(MM_TEXT, sizeDoc);
				pVeiw->RedrawWindow();
				break;
			}
		}
	}		
}

void CImagesFusionDoc::OnWaveletfusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();

	if (m_pWaveletFusionImage) 
	{
		cvReleaseImage(&m_pWaveletFusionImage); 
		m_pWaveletFusionImage = NULL;
	}
	int lHeight = m_pBImage->height;
	int lWidth = m_pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);

	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pWaveletFusionImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);

	CWaveletFusion* pWvltFusion = new CWaveletFusion;
	pWvltFusion->Wavelet_ImageFusion((LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pWaveletFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep, 3);
//////////////////////////////////////////////////////////////////////////////
	t = (cvGetTickCount() - t)/cvGetTickFrequency();
	CString str;
	str.Format("%.1f", t);

	m_Parameter.AlgorithmName = _T("小波变换");
	m_Parameter.TimeStr = str;

	double MI1, UE1, MI2, UE2;
	MI1 = UE1 = MI2 = UE2 = 0.0;
	CaculateParameter((LPBYTE)pAImage->imageData, (LPBYTE)m_pWaveletFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep, MI1, UE1);
	CaculateParameter((LPBYTE)pBImage->imageData, (LPBYTE)m_pWaveletFusionImage->imageData,
		lHeight, lWidth, pBImage->widthStep, MI1, UE1);

	m_Parameter.MI = MI1 + MI2;
	m_Parameter.UE = UE1 + UE2;
	m_ParameterVect.push_back(m_Parameter);
////////////////////////////////////////////////////////////////////////
	if (pAImage) 
	{
		cvReleaseImage(&pAImage); 
		pAImage = NULL;
	}
	if (pBImage) 
	{
		cvReleaseImage(&pBImage); 
		pBImage = NULL;
	}
	delete pWvltFusion;

	cvSaveImage("D:\\wavelet.png", m_pWaveletFusionImage);

	((CMainFrame*)AfxGetMainWnd())->m_pWaveletFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pWaveletFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnPcafusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();

	if (m_pPcaFusionImage) 
	{
		cvReleaseImage(&m_pPcaFusionImage); 
		m_pPcaFusionImage = NULL;
	}
	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pPcaFusionImage = cvCreateImage(cvGetSize(m_pAImage), m_pAImage->depth, m_pAImage->nChannels);
	int lHeight = pBImage->height;
	int lWidth = pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	CPCAFusion* pPCAFusion = new CPCAFusion;
	pPCAFusion->PCA_ImageFusion((LPBYTE)m_pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pPcaFusionImage->imageData,
		lHeight, lWidth, m_pAImage->widthStep, pBImage->widthStep);

////////////////////////////////////////////////////////////////////////	
	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);
	IplImage* pFusionImage = cvCreateImage(cvGetSize(m_pPcaFusionImage), 8, 1);
	cvCvtColor(m_pPcaFusionImage, pFusionImage, CV_RGB2GRAY);

	SetParameter(_T("PCA变换"), t, (LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)pFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep);

	cvReleaseImage(&pAImage);
	cvReleaseImage(&pFusionImage);

	cvSaveImage("D:\\pca.png", m_pPcaFusionImage);
/////////////////////////////////////////////////////////////////////////////
// 	int Nsum = lWidth * lHeight;
// 	double pixel;
// 
// 	CvMat *VectorOrg;  
// 	CvMat *AvgVector; 
// 	CvMat *EigenValue_Row;  
// 	CvMat *EigenVector; 
// 
// 	VectorOrg = cvCreateMat(Nsum, 3, CV_32FC1);
// 	for (int i = 0; i < Nsum; i++)
// 	{
// 		for(int j = 0; j < 3; j++)
// 		{
// 			pixel = (double)*(m_pAImage->imageData + i * 3 + j);
// 			cvmSet(VectorOrg, i, j, pixel);
// 		}	
// 	}
//  
// 	AvgVector = cvCreateMat(1, 3, CV_32FC1); 
// 	EigenValue_Row = cvCreateMat(3, 1, CV_32FC1); 
// 	EigenVector = cvCreateMat(3, 3, CV_32FC1); 
// 	cvCalcPCA(VectorOrg, AvgVector, EigenValue_Row, EigenVector, CV_PCA_DATA_AS_ROW);
// 	cvProjectPCA(VectorOrg, AvgVector, EigenVector, VectorOrg);
// 	
// 	for (int i = 0; i < Nsum; i++)
// 	{
// 		pixel = (double)*(pBImage->imageData + i * 3);
// 		cvmSet(VectorOrg, i, 0, pixel);	
// 	}
// 	cvBackProjectPCA(VectorOrg, AvgVector, EigenVector, VectorOrg);
// 
// 	for (int i = 0; i < Nsum; i++)
// 	{
// 		for(int j = 0; j < 3; j++)
// 		{
// 			pixel = cvmGet(VectorOrg, i, j);
// 			*(m_pPcaFusionImage->imageData + i * 3 + j) = cvRound(pixel);
// 		}	
// 	}
// 	
// 	cvReleaseMat(&VectorOrg);
// 	cvReleaseMat(&AvgVector);
// 	cvReleaseMat(&EigenValue_Row);
// 	cvReleaseMat(&EigenVector);


	cvReleaseImage(&pBImage);
	delete pPCAFusion;
	pPCAFusion = NULL;

	((CMainFrame*)AfxGetMainWnd())->m_pPCAFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pPCAFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnWeightfusion()
{
	// TODO: 在此添加命令处理程序代码

}

void CImagesFusionDoc::OnCorrelationfusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();
	if (m_pFusionImage) 
	{
		cvReleaseImage(&m_pFusionImage); 
		m_pFusionImage = NULL;
	}
	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pFusionImage = cvCreateImage(cvGetSize(m_pAImage), m_pAImage->depth, m_pAImage->nChannels);
	int lHeight = pBImage->height;
	int lWidth = pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	CFusion* pFusion = new CFusion;
	pFusion->Correlation_ImageFusion((LPBYTE)m_pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pFusionImage->imageData,
		lHeight, lWidth, m_pAImage->widthStep, pBImage->widthStep);
///////////////////////////////////////////////////////////////////////
	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);
	IplImage* pFusionImage = cvCreateImage(cvGetSize(m_pFusionImage), 8, 1);
	cvCvtColor(m_pFusionImage, pFusionImage, CV_RGB2GRAY);

	SetParameter(_T("相关系数融合"), t, (LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)pFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep);

	cvReleaseImage(&pAImage);
	cvReleaseImage(&pFusionImage);
//////////////////////////////////////////////////////////////////
	cvReleaseImage(&pBImage);
	delete pFusion;
	pFusion = NULL;

	cvSaveImage("D:\\相关系数.png", m_pFusionImage);

	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnMulfusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();
	if (m_pFusionImage) 
	{
		cvReleaseImage(&m_pFusionImage); 
		m_pFusionImage = NULL;
	}
	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pFusionImage = cvCreateImage(cvGetSize(m_pAImage), m_pAImage->depth, m_pAImage->nChannels);
	int lHeight = pBImage->height;
	int lWidth = pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	CFusion* pFusion = new CFusion;
	pFusion->Mul_ImageFusion((LPBYTE)m_pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pFusionImage->imageData,
		lHeight, lWidth, m_pAImage->widthStep, pBImage->widthStep);
////////////////////////////////////////////////////////////////////////
	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);
	IplImage* pFusionImage = cvCreateImage(cvGetSize(m_pFusionImage), 8, 1);
	cvCvtColor(m_pFusionImage, pFusionImage, CV_RGB2GRAY);

	SetParameter(_T("乘积变换"), t, (LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)pFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep);

	cvReleaseImage(&pAImage);
	cvReleaseImage(&pFusionImage);
////////////////////////////////////////////////////////////////////
	cvReleaseImage(&pBImage);
	delete pFusion;
	pFusion = NULL;

	cvSaveImage("D:\\乘积变换.png", m_pFusionImage);

	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnLinearmulfusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();
	if (m_pFusionImage) 
	{
		cvReleaseImage(&m_pFusionImage); 
		m_pFusionImage = NULL;
	}
	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pFusionImage = cvCreateImage(cvGetSize(m_pAImage), m_pAImage->depth, m_pAImage->nChannels);
	int lHeight = pBImage->height;
	int lWidth = pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	CFusion* pFusion = new CFusion;
	pFusion->LiearMul_ImageFusion((LPBYTE)m_pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pFusionImage->imageData,
		lHeight, lWidth, m_pAImage->widthStep, pBImage->widthStep);
////////////////////////////////////////////////////////////////////////
	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);
	IplImage* pFusionImage = cvCreateImage(cvGetSize(m_pFusionImage), 8, 1);
	cvCvtColor(m_pFusionImage, pFusionImage, CV_RGB2GRAY);

	SetParameter(_T("线性复合与乘积变换"), t, (LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)pFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep);

	cvReleaseImage(&pAImage);
	cvReleaseImage(&pFusionImage);
//////////////////////////////////////////////////////////////////
	cvReleaseImage(&pBImage);
	delete pFusion;
	pFusion = NULL;

	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnBrovryfusion()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAImage->height != m_pBImage->height || m_pAImage->width != m_pBImage->width)
	{
		AfxMessageBox("两幅图像没有配准！！！");
		return;
	}
	float t = cvGetTickCount();
	if (m_pFusionImage) 
	{
		cvReleaseImage(&m_pFusionImage); 
		m_pFusionImage = NULL;
	}
	IplImage* pBImage = cvCreateImage(cvGetSize(m_pBImage), 8, 1);
	cvCvtColor(m_pBImage, pBImage, CV_RGB2GRAY);

	m_pFusionImage = cvCreateImage(cvGetSize(m_pAImage), m_pAImage->depth, m_pAImage->nChannels);
	int lHeight = pBImage->height;
	int lWidth = pBImage->width;
	CSize sizeDoc = CSize(lWidth, lHeight);

	CFusion* pFusion = new CFusion;
	pFusion->Brovey_ImageFusion((LPBYTE)m_pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)m_pFusionImage->imageData,
		lHeight, lWidth, m_pAImage->widthStep, pBImage->widthStep);

////////////////////////////////////////////////////////
	IplImage* pAImage = cvCreateImage(cvGetSize(m_pAImage), 8, 1);
	cvCvtColor(m_pAImage, pAImage, CV_RGB2GRAY);
	IplImage* pFusionImage = cvCreateImage(cvGetSize(m_pFusionImage), 8, 1);
	cvCvtColor(m_pFusionImage, pFusionImage, CV_RGB2GRAY);

	SetParameter(_T("Brovry变换"), t, (LPBYTE)pAImage->imageData, (LPBYTE)pBImage->imageData, (LPBYTE)pFusionImage->imageData,
		lHeight, lWidth, pAImage->widthStep);

	cvReleaseImage(&pAImage);
	cvReleaseImage(&pFusionImage);
///////////////////////////////////////////////////////

	cvReleaseImage(&pBImage);
	delete pFusion;
	pFusion = NULL;

	cvSaveImage("D:\\Brovry变换.png", m_pFusionImage);

	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->SetScrollSizes(MM_TEXT, sizeDoc);
	((CMainFrame*)AfxGetMainWnd())->m_pWeightFusionView->RedrawWindow();

	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::OnEvaluatealgorithm()
{
	// TODO: 在此添加命令处理程序代码
	((CMainFrame*)AfxGetMainWnd())->m_pImagesFusionView->DisplayParameters();
}

void CImagesFusionDoc::SetParameter(CString AlgorithmName, float t, LPBYTE lpSrcDIBBits1, LPBYTE lpSrcDIBBits2, LPBYTE lpFusionDIBBits, 
									int lHeight, int lWidth, int lLineBytes)
{
	t = (cvGetTickCount() - t)/cvGetTickFrequency();
	CString str;
	str.Format("%.1f", t);

	m_Parameter.AlgorithmName = AlgorithmName;
	m_Parameter.TimeStr = str;

	double MI1, UE1, MI2, UE2;
	MI1 = UE1 = MI2 = UE2 = 0.0;
	CaculateParameter(lpSrcDIBBits1, lpFusionDIBBits,
		lHeight, lWidth, lLineBytes, MI1, UE1);
	CaculateParameter(lpSrcDIBBits2, lpFusionDIBBits,
		lHeight, lWidth, lLineBytes, MI1, UE1);

	m_Parameter.MI = MI1 + MI2;
	m_Parameter.UE = UE1 + UE2;
	m_ParameterVect.push_back(m_Parameter);
}

void CImagesFusionDoc::CaculateParameter(LPBYTE lpSrcDIBBits, LPBYTE lpFusionDIBBits, 
										 int lHeight, int lWidth, int lLineBytes, double& MI, double& UE)
{
	int i, j;
	int N = lHeight * lWidth;
	double dPfs[256][256];
	double dPf[256], dPs[256];
	
	for (i = 0; i < 256; i++)
	{
		dPs[i] = 0.0;
		dPf[i] = 0.0;

		for (j = 0; j < 256; j++)
		{
			dPfs[i][j] = 0.0;
		}
	}

	int pixelS, pixelF;
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			pixelS = lpSrcDIBBits[i * lLineBytes + j];
			dPs[pixelS]++;

			pixelF = lpFusionDIBBits[i * lLineBytes + j];
			dPf[pixelF]++;

			dPfs[pixelF][pixelS]++;
		}
	}
	for (i = 0; i < 256; i++)
	{
		dPs[i] /= N;
		dPf[i] /= N;

		for (j = 0; j < 256; j++)
		{
			dPfs[i][j] /= N;
		}
	}
	
	MI = UE = 0.0;
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			if (dPfs[i][j] == 0 || dPf[i] == 0 || dPs[j] == 0)
			{
				continue;
			}
			UE += dPfs[i][j] * log(dPfs[i][j]) / log(2.0);
			MI += dPfs[i][j] * log(dPfs[i][j] / (dPf[i] * dPs[j])) / log(2.0);
		}
	}
	UE = -1.0 * UE;
}
