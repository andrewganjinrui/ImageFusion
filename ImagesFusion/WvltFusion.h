#pragma once

class CWvltFusion
{
public:
	CWvltFusion(void);
	~CWvltFusion(void);
public:

	//ͼ��С���任����任
	void DWTi_Once(BYTE** spData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//����С���任
	void DWT_TriLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//����С���任
	void DWT_TwoLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//һ��С���任
	void DWT_Once(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//��ֱ�����С���任
	void Vert_Transform(BYTE** spOriginData, BYTE** spTransData1, int nHeight_H, int nWidth, float fRadius);
	//ˮƽ�����С���任
	void Hori_Transform(BYTE** spOriginData, BYTE** spTransData0, int nHeight, int nWidth_H, float fRadius);

	void Wvlt_Normalize(BYTE **spWvltNormData, int nHeight, int nWidth, float *nWvltRng);
	void DIP_ConsEnhance(BYTE **spData, int nHeight, int nWidth, float * NormWvltRng);
	void DIP_WvltRevers(BYTE **spData, BYTE **spTransData0, BYTE **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	void Band_Enhance(float **fpNormGradient, float FilterCoeff, int Scan_y, int Scan_x, int End_y, int End_x);
	float Search_BandMax(BYTE **spWvltData, int Scan_y, int Scan_x, int End_y, int End_x);
	void Window_WvltFusion(BYTE **spWvltData0, BYTE **spWvltData1, int Scan_y, int Scan_x, int End_y, int End_x);
	void DIP_ImageFusion(LPBYTE ImgData0, LPBYTE ImgData1, LPBYTE DstImgData, int nHeight, int nWidth, int nLinebyte);
};
