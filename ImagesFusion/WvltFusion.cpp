#include "StdAfx.h"
#include "WvltFusion.h"

CWvltFusion::CWvltFusion(void)
{
}

CWvltFusion::~CWvltFusion(void)
{
}

/********************************************************************************
*����������Hori_Transform���һ��ͼ��ˮƽ�����С���任							*	
*����������BYTE **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*		  BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��		*
*		  int   nHeight		  ��ͼ�����Բ�������ֵΪԭʼͼ��ĸ�				*
*		  int	nWidth_H	  ��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*		  float fRadius		  ��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1			*
********************************************************************************/

void CWvltFusion::Hori_Transform(BYTE** spOriginData, BYTE** spTransData0, int nHeight, int nWidth_H, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	int iWidth = nWidth_H * 2;	//ԭʼͼ��Ŀ��ֵ
	BYTE Trans_Coeff0;			//С���任ϵ��
	BYTE Trans_Coeff1;
	//��ģ����ɱ任ϵ���ĸ�ֵ����
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
	//ͨ��ͼ��Ĳ�֣����С���任
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][iWidth-1] = spTransData0[Trans_H][iWidth-1]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}
}

/********************************************************************************
*����������	Vert_Transform���һ��ͼ����ֱ�����С���任						*	
*����������	BYTE **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			BYTE **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int	  nWidth		��ͼ�����Բ�������ֵΪԭʼͼ����				*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1		*
********************************************************************************/
void CWvltFusion::Vert_Transform(BYTE** spOriginData, BYTE** spTransData1, int nHeight_H, int nWidth, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	int iHeight = nHeight_H * 2;//ԭʼͼ��Ŀ��ֵ
	BYTE Trans_Coeff0;			//С���任ϵ��
	BYTE Trans_Coeff1;
	//��ģ����ɱ任ϵ���ĸ�ֵ����
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
	//ͨ��ͼ��Ĳ�֣����С���任
	for(Trans_W=0; Trans_W<nWidth; Trans_W++)
	{
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_W]+spTransData1[Trans_M+1][Trans_W])>>1);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//ϵ��Ԥ��
			spTransData1[nHeight_H + Trans_M][Trans_W] = spTransData1[nHeight_H + Trans_M][Trans_W]+Trans_Coeff1;
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_W]+spTransData1[nHeight_H-2][Trans_W])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[iHeight-1][Trans_W] = spTransData1[iHeight-1][Trans_W]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_W]+spTransData1[nHeight_H+1][Trans_W])>>2);
		spTransData1[0][Trans_W] = spTransData1[0][Trans_W]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H + Trans_M][Trans_W]+spTransData1[nHeight_H + Trans_M -1][Trans_W])>>2);
			spTransData1[Trans_M][Trans_W] = spTransData1[Trans_M][Trans_W]+Trans_Coeff0;
		}

	}
}

/********************************************************************************
*����������	DWT_Once���һ��ͼ���С���任										*
*����������	BYTE **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			BYTE **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ�������ֵΪ1��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltFusion::DWT_Once(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	BYTE Trans_Coeff0;			//С���任ϵ��
	BYTE Trans_Coeff1;
	fRadius=1.414;				//�任�˲�ϵ��
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	//�б任,��һ�Σ�layer=1ʱ��ʱnHeight��Ϊԭʼͼ��ĸ߶�ֵ
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			//layer=1ʱ��nWidth_HΪԭʼͼ����ֵ��һ��
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
			//���任��������1,���������Ƶ��С��ϵ��
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
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//ˮƽ����ı任����
	//��ֱ����ı任��ʼ������Դδˮƽ�任���С��ϵ��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//���������������spTransData0�У�spTransData0�е�������Ȼ��ż����
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//�б任
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//Ƶ��LL����
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//Ƶ��HL����
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//Ƶ��LH����
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//Ƶ��HH����
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//��һ��������������ϵ��
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//��ֱ����ı任 
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
		//�߽紦��
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//�߽紦��
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//��һ������ż������ϵ��
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
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
*����������	DWT_TwoLayers�������ͼ���С���任									*	
*����������	BYTE **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			BYTE **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ�������ֵΪ2��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltFusion::DWT_TwoLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	BYTE **pData, **pTran0, **pTran1;
	//ͼ������Բ���
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//������ݿռ��ָ��
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//ͼ�����Բ�����ֵ
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//����ѭ���������С���任
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}

}

/********************************************************************************
*����������	DWT_TriLayers�������ͼ���С���任									*	
*����������	BYTE **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			BYTE **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltFusion::DWT_TriLayers(BYTE** spOriginData, BYTE** spTransData0, BYTE** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	BYTE **pData, **pTran0, **pTran1;
	//ͼ������Բ���
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//������ݿռ��ָ��
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//ͼ�����Բ�����ֵ
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//����ѭ���������С���任
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

/********************************************************************************
*����������	DWTi_Once���һ��ͼ��С���任����任								*	
*����������	BYTE **spData		����άָ�룬�����ݿռ���С����任���ͼ������*
*			BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			BYTE **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltFusion::DWTi_Once(BYTE **spData, BYTE **spTransData0, BYTE **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{

	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	BYTE WvltCoeff0;			//С���任ϵ��
	BYTE WvltCoeff1;

	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]/=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]*=fRadius;
		}
	}
	//��任��һ������̣������ִ���ֱ����ʼ
	//��ֱ�������任����ʱ��Ȼ��ż�������У���Trans_NΪͼ������������
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//ż�������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]-WvltCoeff0;
		WvltCoeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]-WvltCoeff0;
		//��ֱ������任�ĵڶ���ż������С��ϵ������任
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]-WvltCoeff0;
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]-WvltCoeff0;
		}
		//�ڶ�����������С��ϵ������任
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			WvltCoeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]-WvltCoeff1;
			WvltCoeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//��������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]-WvltCoeff1;
		WvltCoeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]-WvltCoeff1;
		//��ģ����ɱ任ϵ���ĸ�ֵ�����������
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_M][Trans_N];
			spTransData0[Trans_H+1][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N];
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N];
			spTransData0[Trans_H+1][nWidth_H+Trans_N]= spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N];
		}
	}

	//ȥ��С���任��Ƶ��ϵ�����˲�Ӱ��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]/=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]*=fRadius;
		}
	}
	//�������ˮƽ�������任
	//ˮƽ�������任
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		//ż������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]-WvltCoeff0;
		//�ڶ���ż������С��ϵ������任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]-WvltCoeff0;
		}
		//�ڶ�����������С��ϵ������任
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			WvltCoeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//��������С��ϵ����任ʱ�ı߽紦��
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
*����������	DIP_WvltRevers���ͼ��С��ϵ���ĸ�ԭ���ָ���ԭʼ��ͼ������			*
*����������	BYTE **spData		����άָ�룬ָ��ԭʼ��ͼ������					*
*			BYTE **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			BYTE **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltFusion::DIP_WvltRevers(BYTE **spData, BYTE **spTransData0, BYTE **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	BYTE **spOriginData, **spTransData, **spWvltData;
	int iHeight = (int)nHeight /pow(2.0,layer-1), iWidth =(int)nWidth / pow(2.0,layer-1);
	int iHeight_H =(int) nHeight_H / pow(2.0,layer-1), iWidth_H = (int)nWidth_H/ pow(2.0,layer-1);
	//����ͼ��ԭ������ڴ�ռ�
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
*����������	DIP_ConsEnhance���ͼ��Աȶ���Ϣ����ǿ����ȡ���ص�ͼ����Ϣ			*
*����������	BYTE **spData		����άָ�룬ָ��ԭʼ��ͼ������					*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			float *NormWvltRng	�����ͼ��С�������򻯲����Լ���任�����򻯲���*
********************************************************************************/

void CWvltFusion::DIP_ConsEnhance(BYTE **spData, int nHeight, int nWidth, float *NormWvltRng)
{
	BYTE **spOriginData, **spTransData, **spWvltData;
	float **fpNormGradient , filtCoeff[10],   *fWvltRng;
	int iHeight = nHeight, iWidth = nWidth;
	int iHeight_H = nHeight / 2, iWidth_H = nWidth / 2;
	int x, y;
	fWvltRng = NormWvltRng;
	//Ϊͼ��������ڴ�ռ�
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
	//С��ϵ�������򻯴���
	//���򻯴����spData��ŷ����򻯵�С��ϵ����spWvltData������򻯺��С��ϵ��
	Wvlt_Normalize(spWvltData, iHeight, iWidth, fWvltRng);
	//����С��ϵ�����ݶ���Ϣ����������spTransData��
	//�趨��ʾ�豸����ɫ�Ҷȷ�Χ��0~255
	for(y = 0; y < nHeight; y ++)
	{
		for(x = 0; x < nWidth; x ++)
		{
			fpNormGradient[y][x] = (float) spWvltData[y][x] / 255;
		}
	}
	//ѡ�������˲���gj(x) = x, ��v =kj gj(u)
	//ͳ�Ƴ���ͬƵ��С��ϵ���ļ���ֵ
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
	//����õ���Ƶ���˲������˲�ϵ��
	for(y = 0; y < 10; y++)
	{
		filtCoeff[y] = (float) 255.0 / filtCoeff[y];
		filtCoeff[0] += (float)sqrt(filtCoeff[y]);
	}
	filtCoeff[0] /= 10;
	//���򻯺�С����Ϣ���ݶ���Ϣ�˲�����
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
	//��ԭ���˲���ǿ���С��ϵ��
	for(y = 0; y< iHeight; y ++)
	{
		for(x = 0; x < iWidth; x++)
		{
			fpNormGradient[y][x] *= (float) (fWvltRng[1] - fWvltRng[0]);
			fpNormGradient[y][x] /= 255.0;
			spWvltData[y][x] = (BYTE) fpNormGradient[y][x] + fWvltRng[0];
		}
	}

	//��ԭ��ǿ���С��ϵ��
	DIP_WvltRevers(spOriginData, spTransData, spWvltData, iHeight, iHeight_H, iWidth, iWidth_H, 3, 1.414);
	//����ԭ��ͼ�����ݽ�������
	Wvlt_Normalize(spOriginData, iHeight, iWidth, fWvltRng);
	//�ͷ���ʱ�����ݿռ�
	delete spTransData;
	delete spWvltData;
	delete fpNormGradient;
}

/********************************************************************************
*����������	DIP_ImageFusion�������ͼ����ںϣ��ָ���ԭʼ��ͼ��					*
*����������	BYTE **spImgData0	����άָ�룬�������һ��ԭʼͼ�������			*
*			BYTE **spImgData1	����άָ�룬�����������һ��ԭʼͼ�������		*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
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
	//��ȡͼ������Բ���
	int iHeight = nHeight, iWidth = nWidth;
	//ͼ���ں����õ������ݿռ估����ָ��
	BYTE **spOriginData, **spTransData, **spWvltData0, **spWvltData1;
	//�������ݿռ�
	spTransData = new BYTE *[iHeight];
	spWvltData0 = new BYTE *[iHeight];
	spWvltData1 = new BYTE *[iHeight];
	for(int i = 0; i < iHeight; i ++)
	{
		spTransData[i] = new BYTE [iWidth];
		spWvltData0[i] = new BYTE [iWidth];
		spWvltData1[i] = new BYTE [iWidth];
	}

	//���ͼ�����ݿռ��ָ�룬���С���任
	spOriginData = spImgData0;
	//����С���任
	DWT_TriLayers(spOriginData, spTransData, spWvltData0, iHeight, iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	//���ͼ�����ݿռ��ָ�룬�����һ��ͼ���С���任
	spOriginData = spImgData1;
	//����С���任
	DWT_TriLayers(spOriginData, spTransData, spWvltData1, iHeight,iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	//С��ϵ�����ںϴ���Ƶ����LL3,LH3,HL3,HH3,LH2,HL2,HH2,LH1,HL1,HH1
	//�ںϴ�����Ƶ�����У����������õ���3*3�Ĵ���
	//LL3Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, 0, 0, iHeight / 8, iWidth / 8);
	//HL3Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 8, iHeight / 8, iWidth / 4);
	//LH3Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 8, 0, iHeight / 4, iWidth / 8);
	//HH3Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 8, iWidth / 8, iHeight / 4, iWidth / 4);
	//HL2Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 4, iHeight / 4, iWidth / 2);
	//LH2Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 4, 0, iHeight / 2, iWidth / 4);
	//HH2Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 4, iWidth / 4, iHeight / 2, iWidth / 2);
	//HL1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, 0, iWidth / 2, iHeight / 2, iWidth);
	//LH1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 2, 0, iHeight, iWidth / 2);
	//HH1Ƶ��С��ϵ�����ں�
	Window_WvltFusion(spWvltData0, spWvltData1, iHeight / 2, iWidth / 2, iHeight, iWidth);
	//���ںϺ��С��ϵ����ԭ�����ͼ����ں�
	DIP_WvltRevers(spDstImgData, spTransData, spWvltData0, iHeight, iHeight / 2, iWidth, iWidth / 2, 1, 1.414);
	for(int i = 0; i < nHeight; i ++)
	{
		for(int j = 0; j < nWidth; j ++)
		{
			*(DstImgData + (nHeight- 1 - i) * nLinebyte + j) = spDstImgData[i][j];		
		}
	}
	//�ͷ���ʱ�����ݿռ�
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
*����������	Wvlt_Normalize���ͼ��С����������任���ݵ����򻯴���				*
*����������	BYTE **spWvltNormData����άָ�룬������򻯺��С��ϵ��			*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			float *NormWvltRng	�����ͼ��С�������򻯲����Լ���任�����򻯲���*
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
			//��ΪС���任���С��ϵ���п��ܳ���255�������࣬��ô�ͽ�
			//С��ϵ���ķ�Χӳ�䵽0~255�����ڣ��Ժ�������ƵĴ���Ŀ�Ķ���һ����
			NormCoeff = spWvltNormData[y][x];
			NormCoeff -= MinPixVal;
			NormCoeff *= 255;
			NormCoeff /= (float) Diff;
			spWvltNormData[y][x] = NormCoeff;

		}
	}
}

/*********************************************************************************
*����������	Window_WvltFusion���ͼ��С��ϵ�����ںϲ������õ���Ƶ����С���ں�����*
*����������	BYTE **spWvltData0 ����άָ�룬�������һ��ͼ���ԭʼС��ϵ��		 *
*			BYTE **spWvltData1 ����άָ�룬�����������һ��ͼ���ԭʼС��ϵ��	 *
*			int   Scan_y		��ɨ������ʼ������								 *
*			int   Scan_x		��ɨ������ʼ������								 *
*			int   End_y			��ɨ������ֹ������								 *
*			int   End_x			��ɨ������ֹ������								 *
*********************************************************************************/

void CWvltFusion::Window_WvltFusion(BYTE **spWvltData0, BYTE **spWvltData1, int Scan_y, int Scan_x, int End_y, int End_x)
{
	int y,x;
	BYTE WndSum0, WndSum1;
	for(y = Scan_y; y < End_y; y ++)
	{
		for(x = Scan_x; x < End_x; x ++)
		{
			//��ʼ��������С��ϵ���ĺ�
			WndSum0 = 0;	WndSum1 = 0;
			//���㴰����С��ϵ���ĺ�
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
