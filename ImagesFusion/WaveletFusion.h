#pragma once

class CWaveletFusion
{
public:
	CWaveletFusion(void);
	~CWaveletFusion(void);

public:
	void Wavelet_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits,
		int lSpotHeight, int lSpotWidth, int lLineBytesSpot, int dim);

	void MyFusion(double* pMulspecBits, double* pSpotBits, double* pFusionBits, int dim, int lHeight, int lWidth);

	void MyWaveDec(double* pDIBBits, int dim, int lHeight, int lWidth);
	void MyWaveRec(double* pDIBBits, int dim, int lHeight, int lWidth);
	
	void My_DWT(double* pDIBBits, int lHeight, int lWidth, int minHeight, int maxHeight, int minWidth, int maxWidth);
	void MyDWT(double* pColData, int lLength, double* lpd, double* hpd);
	void DownSpl(double* pData, int lLength, double* pSamplingData);

	void MyConv(double* pColData, double* pData, int lLength, double* pd);

	void My_iDWT(double* pDIBBits, int lHeight, int lWidth, int minHeight, int maxHeight, int minWidth, int maxWidth);
	void MyiDWT(double* pColData, int lLength, double* lpd, double* hpd);
	void UpSpl(double* pData, int lLength, double* pSamplingData);
};
