#include "StdAfx.h"
#include "WaveletFusion.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

CWaveletFusion::CWaveletFusion(void)
{
}

CWaveletFusion::~CWaveletFusion(void)
{
}

void CWaveletFusion::Wavelet_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits,
						 int lSpotHeight, int lSpotWidth, int lLineBytesSpot, int dim)
{
	int i, j;
	int lHeight, lWidth;
	int aa = (int)pow(2.0, dim);

	if (lSpotWidth % aa != 0)
		lWidth = (lSpotWidth / aa + 1) * aa;
	else
		lWidth = lSpotWidth;

	if (lSpotHeight % aa != 0)
		lHeight = (lSpotHeight / aa + 1) * aa;
	else
		lHeight = lSpotHeight;

	double* pMulspecBits = new double[lWidth * lHeight];
	memset(pMulspecBits, 0, lWidth * lHeight * sizeof(double));
	double* pSpotBits = new double[lWidth * lHeight];
	memset(pSpotBits, 0, lWidth * lHeight * sizeof(double));

	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{
			pMulspecBits[i * lWidth + j] = (double)lpMulspecDIBBits[i * lLineBytesSpot + j];
			pSpotBits[i * lWidth + j] = (double)lpSpotDIBBits[i * lLineBytesSpot + j];
		}
	}
	
	double* pFusionBits = new double[lWidth * lHeight];
	memset(pFusionBits, 0, lWidth * lHeight * sizeof(double));
	
	MyWaveDec(pMulspecBits, dim, lHeight, lWidth);
  	MyWaveDec(pSpotBits, dim, lHeight, lWidth);

  	MyFusion(pMulspecBits, pSpotBits, pFusionBits, dim, lHeight, lWidth);

 	MyWaveRec(pFusionBits, dim, lHeight, lWidth);
	
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{
			lpFusionDIBBits[i * lLineBytesSpot + j] = (unsigned char)cvRound(pFusionBits[i * lWidth + j]);
		}
	}

	delete[] pFusionBits;
	pFusionBits = NULL;
	delete[] pMulspecBits;
	pMulspecBits = NULL;
	delete[] pSpotBits;
	pSpotBits = NULL;
}

void CWaveletFusion::MyFusion(double* pMulspecBits, double* pSpotBits, double* pFusionBits, int dim, int lHeight, int lWidth)
{
	int i, j;
	for(i = 0; i < lHeight; i++)
	{
		for(j = 0; j < lWidth; j++)
		{
			if(fabs(pMulspecBits[i * lWidth + j]) > fabs(pSpotBits[i * lWidth + j]))
				pFusionBits[i * lWidth + j] = pMulspecBits[i * lWidth + j];
			else
				pFusionBits[i * lWidth + j] = pSpotBits[i * lWidth + j];
		}
	}	
}

void CWaveletFusion::MyWaveDec(double* pDIBBits, int dim, int lHeight, int lWidth)
{
	int minHeight = 0, maxHeight = lHeight, minWidth = 0, maxWidth = lWidth;
	for (int i = 1; i <= dim; i++)
	{
		My_DWT(pDIBBits, lHeight, lWidth, minHeight, maxHeight, minWidth, maxWidth);
		
		maxHeight = lHeight;
		minHeight = lHeight - lHeight / (int)pow(2.0, i);
		minWidth = 0;
		maxWidth = lWidth / (int)pow(2.0, i);
	}
}

void CWaveletFusion::My_DWT(double* pDIBBits, int lHeight, int lWidth, int minHeight, int maxHeight, int minWidth, int maxWidth)
{
	double lpd[2] = {0.5, 0.5};
	double hpd[2] = {-0.5, 0.5};
	double* pData = NULL;
	//行变换
	for (int i = minHeight; i < maxHeight; i++)
	{
		pData = pDIBBits + i * lWidth;
		MyDWT(pData, maxWidth - minWidth, lpd, hpd);
	}
	//列变换
	double* pColData = new double[maxHeight - minHeight];	
	for (int i = minWidth; i < maxWidth; i++)
	{
		int k = 0;
		for (int j = maxHeight-1; j >= minHeight; j--)
			pColData[k++] = pDIBBits[j * lWidth + i];

		MyDWT(pColData, maxHeight - minHeight, lpd, hpd);

		k = 0;
		for (int j = maxHeight-1; j >= minHeight; j--)
			pDIBBits[j * lWidth + i] = pColData[k++];
	}
	delete[] pColData;
	pColData = NULL;
}

void CWaveletFusion::MyDWT(double* pColData, int lLength, double* lpd, double* hpd)
{
	double* plpdData = new double[lLength+1];
	double* phpdData = new double[lLength+1];

	double* plpdSamplingData = new double[lLength/2];
	double* phpdSamplingData = new double[lLength/2];

	MyConv(pColData, plpdData, lLength, lpd);
	MyConv(pColData, phpdData, lLength, hpd);

	DownSpl(plpdData, lLength+1, plpdSamplingData);
	DownSpl(phpdData, lLength+1, phpdSamplingData);

	for (int i =0; i < lLength; i++)
	{
		if(i < lLength/2)
			pColData[i] = plpdSamplingData[i];
		else
			pColData[i] = phpdSamplingData[i-lLength/2];
	}

	delete[] plpdData;
	delete[] phpdData;
	delete[] plpdSamplingData;
	delete[] phpdSamplingData;
}

void CWaveletFusion::MyConv(double* pColData, double* pData, int lLength, double* pd)
{
	pData[0] = pColData[0] * pd[0];
	pData[lLength] = pColData[lLength-1] * pd[1];

	for (int i = 1; i < lLength; i++)
	{
		pData[i] = pColData[i-1] * pd[1] + pColData[i] * pd[0];
	}
}

void CWaveletFusion::DownSpl(double* pData, int lLength, double* pSamplingData)
{
	for (int i = 0; i < lLength/2; i++)
	{
		pSamplingData[i] = pData[2*i+1];
	}
}

void CWaveletFusion::MyWaveRec(double* pDIBBits, int dim, int lHeight, int lWidth)
{
    int maxHeight, minHeight, minWidth, maxWidth;
	int i = dim - 1;
	do 
	{
		maxHeight = lHeight;
		minHeight = lHeight - lHeight / (int)pow(2.0, i);
		minWidth = 0;
		maxWidth = lWidth / (int)pow(2.0, i);

		My_iDWT(pDIBBits, lHeight, lWidth, minHeight, maxHeight, minWidth, maxWidth);
		i--;
	} while (i >= 0);
}

void CWaveletFusion::My_iDWT(double* pDIBBits, int lHeight, int lWidth, int minHeight, int maxHeight, int minWidth, int maxWidth)
{
	double lpd[2] = {1.0, 1.0};
	double hpd[2] = {1.0, -1.0};

	//列变换
	double* pColData = new double[maxHeight - minHeight];	
	for (int i = minWidth; i < maxWidth; i++)
	{
		int k = 0;
		for (int j = maxHeight-1; j >= minHeight; j--)
			pColData[k++] = pDIBBits[j * lWidth + i];

		MyiDWT(pColData, maxHeight - minHeight, lpd, hpd);

		k = 0;
		for (int j = maxHeight-1; j >= minHeight; j--)
			pDIBBits[j * lWidth + i] = pColData[k++];
	}
	double* pData = NULL;
	//行变换
	for (int i = minHeight; i < maxHeight; i++)
	{
		pData = pDIBBits + i * lWidth;
		MyiDWT(pData, maxWidth - minWidth, lpd, hpd);
	}
	
	delete[] pColData;
	pColData = NULL;
}

void CWaveletFusion::MyiDWT(double* pColData, int lLength, double* lpd, double* hpd)
{
	double* pAData = new double[lLength / 2];
	double* pBData = new double[lLength / 2];

	for (int i = 0; i < lLength; i++)
	{
		if(i < lLength/2)
			pAData[i] = pColData[i];
		else
			pBData[i-lLength/2] = pColData[i];
	}
	
	double* plpdSamplingData = new double[lLength-1];
	double* phpdSamplingData = new double[lLength-1];
	double* plpdData = new double[lLength];
	double* phpdData = new double[lLength];

	UpSpl(pAData, lLength/2, plpdSamplingData);
	MyConv(plpdSamplingData, plpdData, lLength-1, lpd);

	UpSpl(pBData, lLength/2, phpdSamplingData);
	MyConv(phpdSamplingData, phpdData, lLength-1, hpd);

	for (int i = 0; i < lLength; i++)
	{
		pColData[i] = plpdData[i] + phpdData[i];
	}
	
	delete[] pAData;
	pAData = NULL;
	delete[] pBData;
	pBData = NULL;
	delete[] plpdSamplingData;
	plpdSamplingData = NULL;
	delete[] phpdSamplingData;
	phpdSamplingData = NULL;
	delete[] plpdData;
	plpdData = NULL;
	delete[] phpdData;
	phpdData = NULL;
}

void CWaveletFusion::UpSpl(double* pData, int lLength, double* pSamplingData)
{
	for (int i = 0; i < 2*lLength-1; i++)
	{
		if(i % 2 == 0)
			pSamplingData[i] = pData[i/2];
		else
			pSamplingData[i] = 0;
	}
}
