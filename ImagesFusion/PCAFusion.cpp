#include "StdAfx.h"
#include "PCAFusion.h"

CPCAFusion::CPCAFusion(void)
{
}

CPCAFusion::~CPCAFusion(void)
{
}

void CPCAFusion::PCA_ImageFusion(LPBYTE lpMulspecDIBBits, LPBYTE lpSpotDIBBits, LPBYTE lpFusionDIBBits, int lSpotHeight, int lSpotWidth, int lLineBytesMulspec, int lLineBytesSpot)
{
	int i, j, k, m;
	int nBands = 3; // 多光谱3个波段

	BYTE* lpMulspecBits;       //指向多光谱源图象象素的指针
	BYTE* lpSpotBits;       //指向SPOT源图象象素的指针
	//////////////////////////////////////////////////////////////////////
	//计算多光谱图像每个波段的平均灰度
	//先以零填充各值.  
	double *dMulspecMeanGray, *temp;
	if(((dMulspecMeanGray = new double[nBands]) == NULL)||((temp = new double[nBands]) == NULL))
	{
		AfxMessageBox("Can't alloc enough memory!",MB_OK);
		return ;
	}
	for(i = 0; i < nBands; i++)
	{
		dMulspecMeanGray[i] = 0;
		temp[i] = 0;
	}

	//计算,为了方便用SPOT影像的长和宽来代替多光谱的长和宽。
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{
			for(k = 0; k < nBands; k++)
			{
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				temp[k] += *lpMulspecBits;
			}

		}
	}
	for(i = 0; i < nBands; i++)
	{
		dMulspecMeanGray[i] = temp[i] / ((double)lSpotHeight*lSpotWidth);
	}	

	//////////////////////////////////////////////////////////////////////
	//计算多光谱影像协方差阵。
	double *dCovariance;     //定义协方差阵。
	int nTotal = nBands * nBands;
	//分配内存
	if((dCovariance = new double[nTotal]) == NULL)
	{
		AfxMessageBox("Can't allocate enough memory!");
		return ;
	}
	//清零
	for(i = 0; i < nTotal; i++)		
	{
		dCovariance[i] = 0;
	}	
	double *dPixelX,*dPixelY;
	//分配内存
	if(((dPixelX = new double[nBands]) == NULL)||((dPixelY= new double[nBands]) == NULL))
	{
		AfxMessageBox("Can't allocate enough memory!");
		return ;
	}
	//清零
	for(i = 0; i < nBands; i++)
	{
		dPixelX[i] = 0;
		dPixelY[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////
	//计算协方差矩阵
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{
			for(k = 0; k < nBands; k++)
			{
				lpMulspecBits = (unsigned char*)lpMulspecDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3 + k;
				dPixelX[k] = *lpMulspecBits;

			}
			for(k=0;k<nBands;k++)
			{
				for(m=0;m<nBands;m++)
					dCovariance[k*nBands+m] += (dPixelX[k]-dMulspecMeanGray[k]) * (dPixelX[m]-dMulspecMeanGray[m]);
			}
		}
	}

	for(i = 0; i < nTotal; i++)
	{
		dCovariance[i] /= (double)(lSpotWidth*lSpotHeight);		
	}

	//计算三波段的协方差所形成的矩阵的特征值与特征向量
	double eps = 0.000001;   //控制精度要求
	double *dVector;     //定义特征向量阵。
	//分配内存
	if((dVector = new double[nTotal]) == NULL)
	{
		AfxMessageBox("Can't allocate enough memory!");
		return ;
	}
	//清零
	for(i=0;i<nTotal;i++)
	{
		dVector[i] = 0;
	}
	JacobiCharacterVector(dCovariance,nBands,dVector,eps);

	//按特征值由大到小的顺序排列特征向量。
	for(i = 0; i < nBands-1; i++)
	{
		for(j = i + 1; j < nBands; j++)
		{
			if(dCovariance[j*nBands+j] > dCovariance[i*nBands+i])
			{
				double temp = 0;
				temp = dCovariance[j*nBands+j];
				dCovariance[j*nBands+j] = dCovariance[i*nBands+i];
				dCovariance[i*nBands+i] = temp;
				for(k=0;k<nBands;k++)
				{
					temp = 0;
					temp = dVector[k*nBands+j];
					dVector[k*nBands+j] = dVector[k*nBands+i];
					dVector[k*nBands+i] = temp;
				}
			}
		}			
	}
	//矩阵转置
	TransMatrix(dVector,nBands,nBands);

	//////////////////////////////////////////////////////////////
	double* dImageResult = new double[lLineBytesMulspec * lSpotHeight];
	double* dImageBits;       //指向临时文件数据的指针

	//创建当波段数为3时融合后影像的文件头
	//根据高光谱分辨率图象的头信息写融合生成图象的文件
	///////////////////////////////////////////////////////////////
	//将高光谱分辨率的图像进行主分量变换
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				lpMulspecBits = lpMulspecDIBBits + lLineBytesMulspec * (lSpotHeight-1-i) + j*3 + k;
				dPixelX[k] = *lpMulspecBits;
			}

			//主分量变换
			multimatrix(dVector,dPixelX,nBands,nBands,1,dPixelY);

			//将高分辨率图像替代第一主分量
			lpSpotBits = lpSpotDIBBits + lLineBytesSpot * (lSpotHeight-1-i) + j;
			dPixelY[0] = *lpSpotBits;

			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*nBands+k;
				*dImageBits = dPixelY[k];
			}
		}
	}

	//线性改正，将像素值分波段改到正数的范围内
	double *dMax,*dMin;
	if(((dMax= new double[nBands]) == NULL)||((dMin= new double[nBands]) == NULL))
	{
		AfxMessageBox("Can't alloc enough memory!",MB_OK);
		return ;
	}
	for(i=0;i<nBands;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
	//统计极值
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*nBands+k;
				if(dMax[k]<(*dImageBits))
					dMax[k] = *dImageBits;
				if(dMin[k]>(*dImageBits))
					dMin[k] = *dImageBits;
			}

		}
	}
	//线性拉伸
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*nBands+k;
				if((dMax[k]-dMin[k])<255)
					*dImageBits = (*dImageBits-dMin[k]);
				else
					*dImageBits = (*dImageBits-dMin[k])*255/(dMax[k]-dMin[k]);
			}			
		}
	}

	//////////////////////////////////////////////////////////////////////	
	//进行主分量逆变换。
	InverseMatrix(dVector,nBands);     //求特征向量矩阵的逆矩阵。	

	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*nBands+k;
				dPixelY[k] = *dImageBits;
			}

			//主分量逆变换
			multimatrix(dVector,dPixelY,nBands,nBands,1,dPixelX);

			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*nBands+k;
				*dImageBits = dPixelX[k];				
			}

		}
	}

	for(i=0;i<nBands;i++)
	{
		dMax[i] = -255;
		dMin[i] = 255;
	}
	//统计极值
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec * (lSpotHeight-1-i) + j*nBands+k;
				if(dMax[k]<(*dImageBits))
					dMax[k] = *dImageBits;
				if(dMin[k]>(*dImageBits))
					dMin[k] = *dImageBits;
			}

		}
	}
	//线性拉伸
	for(i=0;i<lSpotHeight;i++)
	{
		for(j=0;j<lSpotWidth;j++)
		{
			for(k=0;k<nBands;k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*nBands+k;
				if((dMax[k]-dMin[k])<255)
					*dImageBits = (*dImageBits-dMin[k]);
				else
					*dImageBits = (*dImageBits-dMin[k])*255/(dMax[k]-dMin[k]);
			}

		}
	}

	BYTE* lpResultBits;       //指向融合源图象象素的指针
	for(i = 0; i < lSpotHeight; i++)
	{
		for(j = 0; j < lSpotWidth; j++)
		{
			for(k = 0; k < 3; k++)
			{
				dImageBits = dImageResult + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				lpResultBits = lpFusionDIBBits + lLineBytesMulspec*(lSpotHeight-1-i) + j*3+k;
				*lpResultBits = (unsigned char)(*dImageBits);
			}		
		}
	}

	delete[] dMulspecMeanGray;
	dMulspecMeanGray = NULL;
	delete[] dCovariance;
	dCovariance = NULL;
	delete[] dVector;
	dVector = NULL;
	delete[] temp;
	temp = NULL;
	delete[] dPixelX;
	dPixelX = NULL;
	delete[] dPixelY;
	dPixelY = NULL;
	delete[] dMin;
	dMin = NULL;
	delete[] dMax;
	dMax = NULL;
}

///////////////////////////////////////////////////////
//求矩阵特征值和特征向量
//参数说明：
//a--双精度实型二维数组指针，存放实对称系数矩阵A；
//n--矩阵的阶数；
//v--双精度实型二维数组指针，存放特征向量
//eps--控制精度要求。
void CPCAFusion::JacobiCharacterVector(double *a, int n, double *v, double eps)
{

	int i,j,p,q,u,w,t,s;
	double ff,fm,cn,sn,omega,x,y,d;

	for (i=0; i<=n-1; i++)
	{ 
		v[i*n+i]=1.0;
		for (j=0; j<=n-1; j++)
			if (i!=j)
				v[i*n+j]=0.0;
	}
	ff=0.0;
	for (i=1; i<=n-1; i++)
		for (j=0; j<=i-1; j++)
		{ 
			d=a[i*n+j]; ff=ff+d*d; 
		}
		ff=sqrt(2.0*ff);
loop0:
		ff=ff/(1.0*n);
loop1:
		for (i=1; i<=n-1; i++)
			for (j=0; j<=i-1; j++)
			{ 
				d=fabs(a[i*n+j]);
				if (d>ff)
				{ 
					p=i; 
					q=j;
					goto loop;
				}
			}
			if (ff<eps) return;
			goto loop0;
loop:
			u=p*n+q; 
			w=p*n+p; 
			t=q*n+p; 
			s=q*n+q;
			x=-a[u];
			y=(a[s]-a[w])/2.0;
			omega=x/sqrt(x*x+y*y);
			if (y<0.0) omega=-omega;
			sn=1.0+sqrt(1.0-omega*omega);
			sn=omega/sqrt(2.0*sn);
			cn=sqrt(1.0-sn*sn);
			fm=a[w];
			a[w]=fm*cn*cn+a[s]*sn*sn+a[u]*omega;
			a[s]=fm*sn*sn+a[s]*cn*cn-a[u]*omega;
			a[u]=0.0; a[t]=0.0;
			for (j=0; j<=n-1; j++)
				if ((j!=p)&&(j!=q))
				{
					u=p*n+j;
					w=q*n+j;
					fm=a[u];
					a[u]=fm*cn+a[w]*sn;
					a[w]=-fm*sn+a[w]*cn;
				}
				for (i=0; i<=n-1; i++)
					if ((i!=p)&&(i!=q))
					{
						u=i*n+p; 
						w=i*n+q;
						fm=a[u];
						a[u]=fm*cn+a[w]*sn;
						a[w]=-fm*sn+a[w]*cn;
					}
					for (i=0; i<=n-1; i++)
					{
						u=i*n+p;
						w=i*n+q;
						fm=v[u];
						v[u]=fm*cn+v[w]*sn;
						v[w]=-fm*sn+v[w]*cn;
					}
					goto loop1;

}

//矩阵转置
void CPCAFusion::TransMatrix(double *a, int m, int n)
{
	double*p;
	if((p=new double[m*n])==NULL)
		return;
	double temp =0;
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{			
			*(p+i*n+j) = *(a+j*m+i);
		}
		for(int i=0;i<m;i++)
			for(int j=0;j<n;j++)
			{			
				*(a+i*n+j) = *(p+i*n+j);
			}
			delete []p;
}

void CPCAFusion::multimatrix(double *a, double *b, int m, int n, int k, double *c)
{
	int i,j,l,u;
	for (i=0; i<=m-1; i++)
		for (j=0; j<=k-1; j++)
		{ 
			u=i*k+j; 
			c[u]=0.0;
			for (l=0; l<=n-1; l++)
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
		}
		return;
}


//矩阵求逆
void CPCAFusion::InverseMatrix(double *a, int n)
{
	int *is,*js,i,j,k,l,u,v;
	double d,p;
	is=new int[n*sizeof(int)];
	js=new int[n*sizeof(int)];
	for (k=0; k<=n-1; k++)
	{ d=0.0;
	for (i=k; i<=n-1; i++)
		for (j=k; j<=n-1; j++)
		{ l=i*n+j; p=fabs(a[l]);
	if (p>d) { d=p; is[k]=i; js[k]=j;}
	}
	if (d+1.0==1.0)
	{ 
		delete []is; 
		delete []js;
		printf("err**not inv\n");
		return;
	}
	if (is[k]!=k)
		for (j=0; j<=n-1; j++)
		{ u=k*n+j; v=is[k]*n+j;
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	if (js[k]!=k)
		for (i=0; i<=n-1; i++)
		{ u=i*n+k; v=i*n+js[k];
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	l=k*n+k;
	a[l]=1.0/a[l];
	for (j=0; j<=n-1; j++)
		if (j!=k)
		{ u=k*n+j; a[u]=a[u]*a[l];}
		for (i=0; i<=n-1; i++)
			if (i!=k)
				for (j=0; j<=n-1; j++)
					if (j!=k)
					{ u=i*n+j;
		a[u]=a[u]-a[i*n+k]*a[k*n+j];
		}
		for (i=0; i<=n-1; i++)
			if (i!=k)
			{ u=i*n+k; a[u]=-a[u]*a[l];}
	}
	for (k=n-1; k>=0; k--)
	{ if (js[k]!=k)
	for (j=0; j<=n-1; j++)
	{ u=k*n+j; v=js[k]*n+j;
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	if (is[k]!=k)
		for (i=0; i<=n-1; i++)
		{ u=i*n+k; v=i*n+is[k];
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	}
	delete []is; 
	delete []js;
	return;
}
