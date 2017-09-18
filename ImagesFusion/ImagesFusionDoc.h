// ImagesFusionDoc.h : CImagesFusionDoc 类的接口
//

#pragma once


class CImagesFusionDoc : public CDocument
{
protected: // 仅从序列化创建
	CImagesFusionDoc();
	DECLARE_DYNCREATE(CImagesFusionDoc)

// 属性
public:
	ParameterVect m_ParameterVect;
	Parameter m_Parameter;
private:
	IplImage* m_pAImage;
	IplImage* m_pBImage;
	IplImage* m_pPcaFusionImage;
	IplImage* m_pFusionImage;
	IplImage* m_pWaveletFusionImage;

// 操作
public:
	void FreeImage(int K);
	IplImage* GetIplImg(int K);

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CImagesFusionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenAimage();
	afx_msg void OnOpenbimage();
	afx_msg void OnWaveletfusion();
	afx_msg void OnPcafusion();
	afx_msg void OnWeightfusion();
	afx_msg void OnEvaluatealgorithm();
	afx_msg void OnCorrelationfusion();
	afx_msg void OnMulfusion();
	afx_msg void OnLinearmulfusion();
	afx_msg void OnBrovryfusion();

public:
	void CaculateParameter(LPBYTE lpSrcDIBBits, LPBYTE lpFusionDIBBits, int lHeight, int lWidth, int lLineBytes,
		double& MI, double& UE);
	void SetParameter(CString AlgorithmName, float t, LPBYTE lpSrcDIBBits1, LPBYTE lpSrcDIBBits2, LPBYTE lpFusionDIBBits, 
		int lHeight, int lWidth, int lLineBytes);
};


