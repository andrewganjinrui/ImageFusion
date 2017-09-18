#include "StdAfx.h"
#include "WvltFusion.h"

CWvltFusion::CWvltFusion(void)
{
}

CWvltFusion::~CWvltFusion(void)
{
}

/********************************************************************************
*函数描述：Hori_Transform完成一次图像水平方向的小波变换							*	
*函数参数：BYTE **spOriginData：二维指针，指向原始的图像数据					*
*		  BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数		*
*		  int   nHeight		  ：图像属性参数，数值为原始图像的高				*
*		  int	nWidth_H	  ：图像属性参数，数值为原始图像宽度值的一半		*
*		  float fRadius		  ：小波变换因子，在调用时候已指定数值为1			*
********************************************************************************/

void CWvltFusion::Hori_Transform(BYTE** spOriginData, BYTE** spTransData0, int nHeight, int nWidth_H, float fRadius)
{
	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	int iWidth = nWidth_H * 2;	//原始图像的宽度值
	BYTE Trans_Coeff0;			//小波变换系数
	BYTE Trans_Coeff1;
	//本模块完成变换系数的赋值采样
	for(Trans_H = 0; Trans_H < nHeight; Trans_H ++)            
	{
		for(Trans_N = 0; Trans_N < nWidth_H; Trans_N ++)           
		{
			Trans_W = Trans_N << 1;
			if (fRadius == 2)
			{
				spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
				spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
			}
			else
			{
				spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		//even
				spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	//odd
			}
		}
	}
	//通过图像的差分，完成小波变换
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//奇偶数值和的一半
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//逻辑非操作后数值加1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//系数预测
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//完成一个偶系数的边界处理
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][iWidth-1] = spTransData0[Trans_H][iWidth-1]+Trans_Coeff1;
		//完成一个奇系数的边界处理
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//提升，整数到整数的变换
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}
}

/********************************************************************************
*函数描述：	Vert_Transform完成一次图像竖直方向的小波变换						*	
*函数参数：	BYTE **spOriginData：二维指针，指向原始的图像数据					*
*			BYTE **spTransData1：小波变换系数，存放一次竖直变换后的小波系数	*
*			int   nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int	  nWidth		：图像属性参数，数值为原始图像宽度				*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1		*
********************************************************************************/
void CWvltFusion::Vert_Transform(BYTE** spOriginData, BYTE** spTransData1, int nHeight_H, int nWidth, float fRadius)
{
	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	int iHeight = nHeight_H * 2;//原始图像的宽度值
	BYTE Trans_Coeff0;			//小波变换系数
	BYTE Trans_Coeff1;
	//本模块完成变换系数的赋值采样
	for(Trans_W = 0; Trans_W < nWidth; Trans_W ++)            
	{
		for(Trans_M = 0; Trans_M < nHeight_H; Trans_M ++)           
		{
			Trans_H = Trans_M << 1;
			if (fRadius == 2)
			{
				spTransData1[Trans_M][Trans_W] = (spOriginData[Trans_H][Trans_W]);
				spTransData1[nHeight_H + Trans_M][Trans_W] = (spOriginData[Trans_H+1][Trans_W]);
			}
			else
			{
				spTransData1[Trans_M][Trans_W] = (spOriginData[Trans_H][Trans_W]-128);		//even
				spTransData1[nHeight_H + Trans_M][Trans_W] = (spOriginData[Trans_H+1][Trans_W]-128);	//odd
			}
		}
	}
	//通过图像的差分，完成小波变换
	for(Trans_W=0; Trans_W<nWidth; Trans_W++)
	{
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//奇偶数值和的一半
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_W]+spTransData1[Trans_M+1][Trans_W])>>1);	
			//逻辑非操作后数值加1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//系数预测
			spTransData1[nHeight_H + Trans_M][Trans_W] = spTransData1[nHeight_H + Trans_M][Trans_W]+Trans_Coeff1;
		}
		//完成一个偶系数的边界处理
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_W]+spTransData1[nHeight_H-2][Trans_W])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[iHeight-1][Trans_W] = spTransData1[iHeight-1][Trans_W]+Trans_Coeff1;
		//完成一个奇系数的边界处理
		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_W]+spTransData1[nHeight_H+1][Trans_W])>>2);
		spTransData1[0][Trans_W] = spTransData1[0][Trans_W]+Trans_Coeff0;
		//提升，整数到整数的变换
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H + Trans_M][Trans_W]+spTransData1[nHeight_H + Trans_M -1][Trans_W])>>2);
			spTransData1[Trans_M][Trans_W] = spTransData1[Trans_M][Trans_W]+Trans_Coeff0;
		}

	}
}

/********************************************************************************
*函数描述：	DWT_Once完成一次图像的小波变换										*
*函数参数：	BYTE **spOriginData：二维指针，指向原始的图像数据					*
*			BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			BYTE **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数，数值为1层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltFusion::DWT_Once(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	BYTE Trans_Coeff0;			//小波变换系数
	BYTE Trans_Coeff1;
	fRadius=1.414;				//变换滤波系数
	//本模块完成变换系数的赋值采样
	//行变换,第一次（layer=1时）时nHeight即为原始图像的高度值
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			//layer=1时，nWidth_H为原始图像宽度值的一半
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)          
			{
				Trans_W=Trans_N<<1;
				if (fRadius==2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
				}
				else
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
			}
			//若变换层数大于1,则仅采样低频的小波系数
			if(layer > 1)
				for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
				{
					Trans_W=Trans_N<<1;
					spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
					spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_H][Trans_W+1];
				}
	}
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//奇偶数值和的一半
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//逻辑非操作后数值加1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//系数预测
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//完成一个偶系数的边界处理
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//完成一个奇系数的边界处理
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//提升，整数到整数的变换
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//水平方向的变换结束
	//竖直方向的变换开始，数据源未水平变换后的小波系数
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//行提升后的数据在spTransData0中，spTransData0中的数据自然奇偶有序
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//列变换
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//频带LL部分
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//频带HL部分
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//频带LH部分
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//频带HH部分
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//第一次提升奇数坐标系数
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//竖直方向的变换 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		//边界处理
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//边界处理
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//第一次提升偶数坐标系数
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//存放小波系数，LL频带的系数进行幅值增强处理，其它高频频带的系数则削弱其幅值
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}
}

/********************************************************************************
*函数描述：	DWT_TwoLayers完成两次图像的小波变换									*	
*函数参数：	BYTE **spOriginData：二维指针，指向原始的图像数据					*
*			BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			BYTE **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数，数值为2层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltFusion::DWT_TwoLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	BYTE **pData, **pTran0, **pTran1;
	//图像的属性参数
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//获得数据空间的指针
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//图像属性参数赋值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//利用循环完成两次小波变换
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}

}

/********************************************************************************
*函数描述：	DWT_TriLayers完成三次图像的小波变换									*	
*函数参数：	BYTE **spOriginData：二维指针，指向原始的图像数据					*
*			BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			BYTE **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数,数值为3层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltFusion::DWT_TriLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	BYTE **pData, **pTran0, **pTran1;
	//图像的属性参数
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//获得数据空间的指针
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//图像属性参数赋值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//利用循环完成两次小波变换
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

/********************************************************************************
*函数描述：	DWTi_Once完成一次图像小波变换的逆变换								*	
*函数参数：	BYTE **spData		：二维指针，其数据空间存放小波逆变换后的图像数据*
*			BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			BYTE **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数,数值为3层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltFusion::DWTi_Once(BYTE **spData, BYTE **spTransData0, BYTE **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{

	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	BYTE WvltCoeff0;			//小波变换系数
	BYTE WvltCoeff1;

	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]/=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]*=fRadius;
		}
	}
	//逆变换是一个逆过程，所以现从竖直方向开始
	//竖直方向的逆变换（此时自然奇偶有序排列），Trans_N为图像矩阵的纵坐标
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//偶数坐标的小波系数逆变换时的边界处理
		WvltCoeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]-WvltCoeff0;
		WvltCoeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]-WvltCoeff0;
		//竖直方向逆变换的第二次偶数坐标小波系数的逆变换
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]-WvltCoeff0;
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]-WvltCoeff0;
		}
		//第二次奇数坐标小波系数的逆变换
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			WvltCoeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]-WvltCoeff1;
			WvltCoeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//奇数坐标小波系数逆变换时的边界处理
		WvltCoeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]-WvltCoeff1;
		WvltCoeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]-WvltCoeff1;
		//本模块完成变换系数的赋值采样的逆操作
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_M][Trans_N];
			spTransData0[Trans_H+1][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N];
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N];
			spTransData0[Trans_H+1][nWidth_H+Trans_N]= spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N];
		}
	}

	//去除小波变换中频带系数的滤波影像
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]/=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]*=fRadius;
		}
	}
	//下面进行水平方向的逆变换
	//水平方向的逆变换
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		//偶数坐标小波系数逆变换时的边界处理
		WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]-WvltCoeff0;
		//第二次偶数坐标小波系数的逆变换
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]-WvltCoeff0;
		}
		//第二次奇数坐标小波系数的逆变换
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			WvltCoeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//奇数坐标小波系数逆变换时的边界处理
		WvltCoeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]-WvltCoeff1;

		if(layer > 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				Trans_W =Trans_N<<1;
				spTransData1[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
				spTransData1[Trans_H][Trans_W+1] = spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}
		if(layer == 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				Trans_W =Trans_N<<1;
				if(fRadius!=2)
				{
					spTransData0[Trans_H][Trans_N]=spTransData0[Trans_H][Trans_N]+128;
					spTransData0[Trans_H][nWidth_H+Trans_N]=spTransData0[Trans_H][nWidth_H+Trans_N]+128;
				}
				if(spTransData0[Trans_H][Trans_N]>255) spTransData0[Trans_H][Trans_N]=255;
				if(spTransData0[Trans_H][Trans_N]<0)   spTransData0[Trans_H][Trans_N]=0;
				if(spTransData0[Trans_H][nWidth_H+Trans_N]>255) spTransData0[Trans_H][nWidth_H+Trans_N]=255;
				if(spTransData0[Trans_H][nWidth_H+Trans_N]<0)   spTransData0[Trans_H][nWidth_H+Trans_N]=0;
				//   spData[Trans_H][Trans_W] = (unsigned char)spTransData0[Trans_H][Trans_N];
				spData[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
				//    spData[Trans_H][Trans_W+1] = (unsigned char)spTransData0[Trans_H][nWidth_H+Trans_N];
				spData[Trans_H][Trans_W+1] =spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}

	}

}

/********************************************************************************
*函数描述：	DIP_WvltRevers完成图像小波系数的复原，恢复出原始的图像数据			*
*函数参数：	BYTE **spData		：二维指针，指向原始的图像数据					*
*			BYTE **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			BYTE **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数,数值为3层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltFusion::DIP_WvltRevers(BYTE **spData, BYTE **spTransData0, BYTE **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	BYTE **spOriginData, **spTransData, **spWvltData;
	int iHeight = (int)nHeight /pow(2.0,layer-1), iWidth =(int)nWidth / pow(2.0,layer-1);
	int iHeight_H =(int) nHeight_H / pow(2.0,layer-1), iWidth_H = (int)nWidth_H/ pow(2.0,layer-1);
	//分配图像复原所需的内存空间
	spOriginData = spData;
	spTransData = spTransData0;
	spWvltData = spTransData1;

	for(int i = layer; i >= 1; i--)
	{
		DWTi_Once(spOriginData, spTransData, spWvltData, iHeight, iHeight_H, iWidth, iWidth_H, i, 1.414);
		iHeight <<= 1;		iWidth <<= 1;
		iHeight_H <<= 1;	iWidth_H <<= 1;
	}

}

/********************************************************************************
*函数描述：	DIP_ConsEnhance完成图像对比度信息的增强，获取隐藏的图像信息			*
*函数参数：	BYTE **spData		：二维指针，指向原始的图像数据					*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			float *NormWvltRng	：存放图像小波的正则化参数以及逆变换的正则化参数*
********************************************************************************/

void CWvltFusion::DIP_ConsEnhance(BYTE **spData, int nHeight, int nWidth, float *NormWvltRng)
{
	BYTE **spOriginData, **spTransData, **spWvltData;
	float **fpNormGradient , filtCoeff[10],   *fWvltRng;
	int iHeight = nHeight, iWidth = nWidth;
	int iHeight_H = nHeight / 2, iWidth_H = nWidth / 2;
	int x, y;
	fWvltRng = NormWvltRng;
	//为图像处理分配内存空间
	spOriginData = spData;
	spTransData = new BYTE * [nHeight];
	spWvltData = new BYTE * [nHeight];
	fpNormGradient = new float * [nHeight];
	for(int i = 0; i < nHeight; i ++)
	{
		spTransData[i] = new BYTE [nWidth];
		spWvltData[i] = new BYTE [nWidth];
		fpNormGradient[i] = new float [nWidth];
	}
	DWT_TriLayers(spOriginData, spTransData, spWvltData, iHeight, iHeight_H, iWidth, iWidth_H, 3, 1.414);
	//小波系数的正则化处理
	//正则化处理后spData存放非正则化的小波系数，spWvltData存放正则化后的小波系数
	Wvlt_Normalize(spWvltData, iHeight, iWidth, fWvltRng);
	//计算小波系数的梯度信息，将其存放在spTransData中
	//设定显示设备的颜色灰度范围是0~255
	for(y = 0; y < nHeight; y ++)
	{
		for(x = 0; x < nWidth; x ++)
		{
			fpNormGradient[y][x] = (float) spWvltData[y][x] / 255;
		}
	}
	//选择线性滤波器gj(x) = x, 且v =kj gj(u)
	//统计出不同频带小波系数的极大值
	for(y = 0; y < 3; y ++)
	{
		if(y == 0)
		{
			filtCoeff[4*y] = Search_BandMax(spWvltData, 0, 0, iHeight / 8, iWidth / 8);
		}
		filtCoeff[3*y + 1] = Search_BandMax(spWvltData, 0, (int)(pow(2.0,y) * iWidth / 8), (int)(pow(2.0,y)* iHeight / 8), (int)(pow(2.0,y)*iWidth / 4));
		filtCoeff[3*y + 2] = Search_BandMax(spWvltData, (int)(pow(2.0,y) * iHeight / 8), 0, (int)(pow(2.0,y)* iHeight / 4), (int)(pow(2.0,y)*iWidth / 8));
		filtCoeff[3*y + 3] = Search_BandMax(spWvltData, (int)(pow(2.0,y) * iHeight / 8), (int)(pow(2.0,y) * iWidth / 8), (int)(pow(2.0,y)* iHeight / 4), (int)(pow(2.0,y)*iWidth / 4));
	}
	//计算得到各频带滤波器的滤波系数
	for(y = 0; y < 10; y++)
	{
		filtCoeff[y] = (float) 255.0 / filtCoeff[y];
		filtCoeff[0] += (float)sqrt(filtCoeff[y]);
	}
	filtCoeff[0] /= 10;
	//正则化后小波信息的梯度信息滤波处理
	for(y = 0; y < 3; y ++)
	{
		if(y == 0)
		{
			Band_Enhance(fpNormGradient, filtCoeff[3*y], 0, 0, iHeight / 8, iWidth / 8);
		}
		Band_Enhance(fpNormGradient, (float)sqrt(filtCoeff[3*y + 1]) /2, 0, (int)(pow(2.0,y) * iWidth / 8), (int)(pow(2.0,y)* iHeight / 8), (int)(pow(2.0,y)*iWidth / 4));
		Band_Enhance(fpNormGradient, (float)sqrt(filtCoeff[3*y + 2]) /2, (int)(pow(2.0,y) * iHeight / 8), 0, (int)(pow(2.0,y)* iHeight / 4), (int)(pow(2.0,y)*iWidth / 8));
		Band_Enhance(fpNormGradient, (float)sqrt(filtCoeff[3*y + 3]) /2, (int)(pow(2.0,y) * iHeight / 8), (int)(pow(2.0,y) * iWidth / 8), (int)(pow(2.0,y)* iHeight / 4), (int)(pow(2.0,y)*iWidth / 4));
	}
	//还原出滤波增强后的小波系数
	for(y = 0; y< iHeight; y ++)
	{
		for(x = 0; x < iWidth; x++)
		{
			fpNormGradient[y][x] *= (float) (fWvltRng[1] - fWvltRng[0]);
			fpNormGradient[y][x] /= 255.0;
			spWvltData[y][x] = (BYTE) fpNormGradient[y][x] + fWvltRng[0];
		}
	}

	//复原增强后的小波系数
	DIP_WvltRevers(spOriginData, spTransData, spWvltData, iHeight, iHeight_H, iWidth, iWidth_H, 3, 1.414);
	//将复原的图像数据进行正则化
	Wvlt_Normalize(spOriginData, iHeight, iWidth, fWvltRng);
	//释放临时的数据空间
	delete spTransData;
	delete spWvltData;
	delete fpNormGradient;
}

/********************************************************************************
*函数描述：	DIP_ImageFusion完成两幅图像的融合，恢复出原始的图像					*
*函数参数：	BYTE **spImgData0	：二维指针，存放其中一幅原始图像的数据			*
*			BYTE **spImgData1	：二维指针，存放其中另外一幅原始图像的数据		*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
********************************************************************************/
void CWvltFusion::DIP_ImageFusion(LPBYTE ImgData0, LPBYTE ImgData1, LPBYTE DstImgData, int nHeight, int nWidth, int nLinebyte)
{
	BYTE **spImgData0, **spImgData1, **spDstImgData;
	spImgData0 = new BYTE * [nHeight];
	spImgData1 = new BYTE * [nHeight];
	spDstImgData = new BYTE * [nHeight];
	for(int i = 0; i < nHeight; i ++)
	{
		spImgData0[i] = new BYTE [nWidth];
		spImgData1[i] = new BYTE [nWidth];
		spDstImgData[i] = new BYTE [nWidth];
	}
	for(int i = 0; i < nHeight; i ++)
	{
		for(int j = 0; j < nWidth; j ++)
		{
			spImgData0[nHeight- 1 - i][j] = *(ImgData0 + i * nLinebyte + j);
			spImgData1[nHeight- 1 - i][j] = *(ImgData1 + i * nLinebyte + j);
			spDstImgData[nHeight- 1 - i][j] = 0;
		}
	}
	//获取图像的属性参数
	int iHeight = nHeight, iWidth = nWidth;
	//图像融合所用到的数据空间及数据指针
	BYTE **spOriginData, **spTransData, **spWvltData0, **spWvltData1;
	//分配数据空间
	spTransData = new BYTE *[iHeight];
	spWvltData0 = new BYTE *[iHeight];
	spWvltData1 = new BYTE *[iHeight];
	for(int i = 0; i < iHeight; i ++)
	{
		spTransData[i] = new BYTE [iWidth];
		spWvltData0[i] = new BYTE [iWidth];
		spWvltData1[i] = new BYTE [iWidth];
	}

	//获得图像数据空间的指针，完成小波变换
	spOriginData = spImgData0;
	//三层小波变换
	DWT_TriLayers(spOriginData, spTransData, spWvltData0, iHeight, iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	//获得图像数据空间的指针，完成另一幅图像的小波变换
	spOriginData = spImgData1;
	//三层小波变换
	DWT_TriLayers(spOriginData, spTransData, spWvltData1, iHeight,iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	//小波系数的融合处理：频带有LL3,LH3,HL3,HH3,LH2,HL2,HH2,LH1,HL1,HH1
	//融合处理将分频带进行，处理方法采用的是3*3的窗口
	//LL3频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, 0, 0, iHeight / 8, iWidth / 8);
	//HL3频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 8, iHeight / 8, iWidth / 4);
	//LH3频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 8, 0, iHeight / 4, iWidth / 8);
	//HH3频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 8, iWidth / 8, iHeight / 4, iWidth / 4);
	//HL2频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 4, iHeight / 4, iWidth / 2);
	//LH2频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 4, 0, iHeight / 2, iWidth / 4);
	//HH2频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 4, iWidth / 4, iHeight / 2, iWidth / 2);
	//HL1频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 2, iHeight / 2, iWidth);
	//LH1频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 2, 0, iHeight, iWidth / 2);
	//HH1频带小波系数的融合
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 2, iWidth / 2, iHeight, iWidth);
	//将融合后的小波系数复原，完成图像的融合
	DIP_WvltRevers(spDstImgData, spTransData, spWvltData0, iHeight, iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	for(int i = 0; i < nHeight; i ++)
	{
		for(int j = 0; j < nWidth; j ++)
		{
			*(DstImgData + (nHeight- 1 - i) * nLinebyte + j) = spDstImgData[i][j];		
		}
	}
	//释放临时的数据空间
	for(int i = 0; i < nHeight; ++i)
	{
		delete []spImgData0[i];
		spImgData0[i] = NULL;

		delete []spImgData1[i];
		spImgData1[i] = NULL;

		delete []spDstImgData[i];
		spDstImgData[i] = NULL;

		delete []spTransData[i];
		spTransData[i] = NULL;

		delete []spWvltData0[i];
		spWvltData0[i] = NULL;

		delete []spWvltData1[i];
		spWvltData1[i] = NULL;
	}
	delete []spImgData0;
	spImgData0 = NULL;
	delete []spImgData1;
	spImgData1 = NULL;
	delete []spDstImgData;
	spDstImgData = NULL;
	delete []spTransData;
	spTransData = NULL;
	delete []spWvltData0;
	spWvltData0 = NULL;
	delete []spWvltData1;
	spWvltData1 = NULL;
}


/********************************************************************************
*函数描述：	Wvlt_Normalize完成图像小波数据与逆变换数据的正则化处理				*
*函数参数：	BYTE **spWvltNormData：二维指针，存放正则化后的小波系数			*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			float *NormWvltRng	：存放图像小波的正则化参数以及逆变换的正则化参数*
********************************************************************************/

void CWvltFusion::Wvlt_Normalize(BYTE **spWvltNormData, int nHeight, int nWidth, float *nWvltRng)
{
	int MaxPixVal, MinPixVal, Diff;
	int x, y;
	float NormCoeff;

	MaxPixVal=spWvltNormData[0][0];
	MinPixVal=spWvltNormData[0][0];
	for( y=0; y < nHeight; y++)
	{
		for( x=0; x < nWidth; x++)
		{
			if(MaxPixVal<spWvltNormData[y][x])
				MaxPixVal=spWvltNormData[y][x];
			if(MinPixVal>spWvltNormData[y][x])
				MinPixVal=spWvltNormData[y][x];
			//spWvltData[y][x] = spWvltNormData[y][x];
		}
	}
	Diff=MaxPixVal-MinPixVal;
	nWvltRng[1] = MaxPixVal;
	nWvltRng[0] = MinPixVal;
	for(y=0; y < nHeight; y++)
	{
		for(x=0; x < nWidth; x++)
		{
			//因为小波变换后的小波系数有可能超过255甚至更多，那么就将
			//小波系数的范围映射到0~255区间内，以后出现类似的处理，目的都是一样的
			NormCoeff = spWvltNormData[y][x];
			NormCoeff -= MinPixVal;
			NormCoeff *= 255;
			NormCoeff /= (float) Diff;
			spWvltNormData[y][x] = NormCoeff;

		}
	}
}

/*********************************************************************************
*函数描述：	Window_WvltFusion完成图像小波系数的融合操作，得到各频带的小波融合数据*
*函数参数：	BYTE **spWvltData0 ：二维指针，存放其中一幅图像的原始小波系数		 *
*			BYTE **spWvltData1 ：二维指针，存放其中另外一幅图像的原始小波系数	 *
*			int   Scan_y		：扫描线起始横坐标								 *
*			int   Scan_x		：扫描线起始纵坐标								 *
*			int   End_y			：扫描线终止横坐标								 *
*			int   End_x			：扫描线终止纵坐标								 *
*********************************************************************************/

void CWvltFusion::Window_WvltFusion(BYTE **spWvltData0, BYTE **spWvltData1, int Scan_y, int Scan_x, int End_y, int End_x)
{
	int y,x;
	BYTE WndSum0, WndSum1;
	for(y = Scan_y; y < End_y; y ++)
	{
		for(x = Scan_x; x < End_x; x ++)
		{
			//初始化窗口中小波系数的和
			WndSum0 = 0;	WndSum1 = 0;
			//计算窗口中小波系数的和
			for(int i = -1; i <= 1; i++)
			{
				for(int j = -1; j <= 1; j++)
				{
					if( (y+i) < Scan_y || (x+j) < Scan_x || (y+i) >= End_y || (x+j) >= End_x)
					{
						WndSum0 += 0;
						WndSum1 += 0;
					}
					else
					{
						WndSum0 += abs((int)spWvltData0[y + i][x + j]);
						WndSum1 += abs((int)spWvltData1[y + i][x + j]);
					}
				}
			}
			if(WndSum0 < WndSum1)
				spWvltData0[y][x] = spWvltData1[y][x];
		}
	}
}

float CWvltFusion::Search_BandMax(BYTE **spWvltData, int Scan_y, int Scan_x, int End_y, int End_x)
{
	int x, y;
	float Band_max;
	Band_max = 0;
	for(y = Scan_y; y < End_y; y ++)
	{
		for(x = Scan_x; x < End_x; x ++)
		{
			if(Band_max < spWvltData[y][x])
				Band_max = spWvltData[y][x];
		}
	}
	return Band_max;
}

void CWvltFusion::Band_Enhance(float **fpNormGradient, float FilterCoeff, int Scan_y, int Scan_x, int End_y, int End_x)
{
	for(int y = Scan_y; y < End_y; y ++)
		for(int x = Scan_x; x < End_x; x ++)
			fpNormGradient[y][x] *= (float)(255.0 * FilterCoeff);	
}
