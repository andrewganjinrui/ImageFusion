#pragma once

class CPCAFusion
{
public:
	CPCAFusion(void);
	~CPCAFusion(void);

public:
	void InverseMatrix(double* a,int n);
	void multimatrix(double *a,double*b,int m,int n,int k,double *c);
	void TransMatrix(double *a,int m,int n);
	void JacobiCharacterVector(double* a,int n,double* v,double eps);
	void PCA_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE DstImgData,
		                 int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot);
};
