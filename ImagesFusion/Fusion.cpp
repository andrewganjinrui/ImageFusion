#include "StdAfx.h"
#include "Fusion.h"

CFusion::CFusion(void)
{
}

CFusion::~CFusion(void)
{
}

void CFusion::Correlation_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpResultDIBBits, int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot)
{
	int i, j, k;

	BYTE* lpMulspecBits;       //指向多光谱源图象象素的指针
	BYTE* lpSpotBits;       //指向SPOT源图象象素的指针

	//计算SPOT影像的平均灰度
	double SpotMeanGray=0;
	double temp = 0;
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			lpSpotBits = lpSpotDIBBits + lLineBytesSpot*(lSpotHeight-1-i) + j;
			temp += *lpSpotBits;
		}
	}
	SpotMeanGray = temp/((double)lSpotHeight*lSpotWidth);
	temp = 0;

	double dMulspecMeanGrayR=0,dMulspecMeanGrayG=0,dMulspecMeanGrayB=0;
	//R
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3;
			temp += *lpMulspecBits;
		}
	}
	dMulspecMeanGrayR = temp/((double)lSpotHeight*lSpotWidth);

	temp = 0;
	//G
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+1;
			temp += *lpMulspecBits;
		}
	}
	dMulspecMeanGrayG = temp/((double)lSpotHeight*lSpotWidth);

	temp = 0;
	//B
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+2;
			temp += *lpMulspecBits;
		}
	}
	dMulspecMeanGrayB = temp/((double)lSpotHeight*lSpotWidth);

	////////////////////////////////////////////////////////////////////////////
	//计算SPOT全色影像和多光谱影像每个波段的相关系数。
	double dRelation[3];
	for(i=0;i<3;i++)
	{
		dRelation[i]=0;
	}
	double temp1=0,temp2=0,temp3=0;
	double dMulspecMeanGray;
	//计算SPOT全色影像和多光谱影像各个波段的相关系数,
	//dRelation[1]，dRelation[2],dRelation[3]分别代表与红波段，绿波段，兰波段的相关系数。
	for(k=0;k<3;k++)
	{
		if(k == 0)
			dMulspecMeanGray = dMulspecMeanGrayR;
		else if (k == 1)
			dMulspecMeanGray = dMulspecMeanGrayG;
		else if (k == 2)
			dMulspecMeanGray = dMulspecMeanGrayB;

		for(i=0;i<lSpotHeight;i++)
		{
			for(j=0;j<lSpotWidth;j++)
			{

				lpSpotBits = lpSpotDIBBits + lLineBytesSpot*(lSpotHeight-1-i) + j;
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;

				temp1 += (*lpSpotBits - SpotMeanGray) * (*lpMulspecBits - dMulspecMeanGray);

				temp2 += (*lpSpotBits - SpotMeanGray) * (*lpSpotBits - SpotMeanGray);
				temp3 += (*lpMulspecBits - dMulspecMeanGray) * (*lpMulspecBits - dMulspecMeanGray);		
			}
		}

		dRelation[k] = temp1/sqrt(temp2*temp3);
		temp1=temp2=temp3=0;
	}
	/////////////////////////////////////////////////////////
	BYTE* lpResultBits;       //指向融合源图象象素的指针
	//把SPOT全色图像和多光谱影像加权融合生成的像素值填入相应位置的像素。
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<3;k++)
			{
				lpSpotBits = lpSpotDIBBits + lLineBytesSpot*(lSpotHeight-1-i) + j;
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;

				lpResultBits = lpResultDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				*lpResultBits =(unsigned char) ( ( (1+abs(dRelation[k])) * (*lpSpotBits) 
					+ (1-abs(dRelation[k])) * (*lpMulspecBits) ) / 2);		
			}
		}
	}
}

void CFusion::Brovey_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits, int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot)
{
	int i, j, k;

	BYTE* lpMulspecBits;       //指向多光谱源图象象素的指针
	BYTE* lpSpotBits;       //指向SPOT源图象象素的指针

	double RGBNum,*dRGB;
	RGBNum = 0.00;
	if((dRGB = new double[3]) == NULL)

	{
		AfxMessageBox("Can't allocate enough memory!");
		return ;
	}
	//清零
	for(i=0;i<3;i++)
	{
		dRGB[i] = 0;			
	}
	///////////////////////////////////////////////
	double* dImageResult = new double[lLineBytesMulspec * lSpotHeight];
	double* dImageBits;       //指向临时文件数据的指针	
	//////////////////////////////////////////////////////////
	
	//进行比值变换
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				dRGB[k] =  *lpMulspecBits;
				RGBNum += dRGB[k];
				if(RGBNum == 0)
					RGBNum = 1.0;
			}

			for(k = 0; k < 3; k++)
			{
				lpSpotBits = lpSpotDIBBits + lLineBytesSpot*(lSpotHeight-1-i) + j;
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3 + k;

				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3 + k;			
				*dImageBits = (*lpMulspecBits) / RGBNum * (*lpSpotBits);
			}
			RGBNum = 0.0;
		}
	}

	//线性改正，将像素值分波段改到正数的范围内
	double dMax[3],dMin[3];
	for(i=0;i<3;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
	//统计极值
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				if(dMax[k]<(*dImageBits))
					dMax[k] = *dImageBits;
				if(dMin[k]>(*dImageBits))
					dMin[k] = *dImageBits;
			}

		}
	}
	//线性拉伸
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				if((dMax[k]-dMin[k])<255)
					*dImageBits = (*dImageBits-dMin[k]);
				else
					*dImageBits = (*dImageBits-dMin[k])*255/(dMax[k]-dMin[k]);
			}

		}
	}
	BYTE* lpResultBits;
	//将比值变换的结果写入结果图像中去	
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				lpResultBits = lpFusionDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				*lpResultBits = (unsigned char)(*dImageBits);
			}		
		}
	}

	delete[] dImageResult;
	delete[] dRGB;
	dRGB = NULL;
	dImageResult = NULL;
}

void CFusion::Mul_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits, int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot)
{
	int i, j, k;

	BYTE* lpMulspecBits;       //指向多光谱源图象象素的指针
	BYTE* lpSpotBits;       //指向SPOT源图象象素的指针

	double* dImageResult = new double[lLineBytesMulspec * lSpotHeight];
	double* dImageBits;       //指向临时文件数据的指针	

	////////////////////////////////////////////////////////////////	
	//进行比值变换
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				lpSpotBits = lpSpotDIBBits + lLineBytesSpot * (lSpotHeight-1-i) + j;
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;

				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				if(*lpMulspecBits == 0 && *lpSpotBits != 0)
					*dImageBits = (*lpSpotBits);
				else if(*lpSpotBits == 0 && *lpMulspecBits != 0)
					*dImageBits = (*lpMulspecBits);
				else if(*lpSpotBits == 0 && *lpMulspecBits == 0)
					*dImageBits = 0;
				else
					*dImageBits = (*lpMulspecBits) * (*lpSpotBits);				
			}		
		}
	}

	//线性改正，将像素值分波段改到正数的范围内
	double dMax[3],dMin[3];
	for(i=0;i<3;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
	//统计极值
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				if(dMax[k]<(*dImageBits))
					dMax[k] = *dImageBits;
				if(dMin[k]>(*dImageBits))
					dMin[k] = *dImageBits;
			}

		}
	}
	//线性拉伸
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				if((dMax[k]-dMin[k])<255)
					*dImageBits = (*dImageBits-dMin[k]);
				else
					*dImageBits = (*dImageBits-dMin[k])*255/(dMax[k]-dMin[k]);
			}

		}
	}
	BYTE* lpResultBits;
	//将比值变换的结果写入结果图像中去	
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				lpResultBits = lpFusionDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				*lpResultBits = (unsigned char)(*dImageBits);
			}		
		}
	}

	delete[] dImageResult;
	dImageResult = NULL;
}

void CFusion::LiearMul_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits, int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot)
{
	int i, j, k;

	BYTE* lpMulspecBits;       //指向多光谱源图象象素的指针
	BYTE* lpSpotBits;       //指向SPOT源图象象素的指针

	double MultiNum,SpotNum;
	double *m;   

	//变量初始化
	MultiNum = 0;SpotNum=0;
	//分配内存
	if((m = new double[3]) == NULL)

	{
		AfxMessageBox("Can't allocate enough memory!");
		return ;
	}
	//清零
	for(i=0;i<3;i++)
	{
		m[i] = 0;	
	}

	double* dImageResult = new double[lLineBytesMulspec * lSpotHeight];
	double* dImageBits;       //指向临时文件数据的指针	

	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				lpSpotBits = lpSpotDIBBits + lLineBytesSpot * (lSpotHeight-1-i) + j;     
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				//dRGB1[k] =  *lpMulspecBits;
				SpotNum += *lpSpotBits;
				MultiNum += *lpMulspecBits;
				if((MultiNum == 0)||(SpotNum == 0))
				{
					MultiNum = 1.0;
					SpotNum = 1.0;
				}				
			}
		}
	}

	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				m[k] = (MultiNum - SpotNum)/(lSpotHeight* lSpotWidth);

				lpSpotBits = lpSpotDIBBits + lLineBytesSpot*(lSpotHeight-1-i) + j;     
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3 + k;

				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3 + k;
				*dImageBits = m[k] + (sqrtf( ((float)(*lpSpotBits)) * ((float)(*lpMulspecBits)) ))*0.95;

			}
		}
	}
	BYTE* lpResultBits;
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{	
			//读取每一像素的三颜色分量
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				lpResultBits = lpFusionDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;

				if ((*dImageBits)>255)
					(*dImageBits) = 255;
				if ((*dImageBits)<0)
					(*dImageBits) = 0;

				*lpResultBits = (unsigned char)(*dImageBits);
			}		
		}
	}

	delete[] dImageResult;
	dImageResult = NULL;
	delete[] m;
	m = NULL;
}
