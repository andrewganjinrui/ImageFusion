// ImagesFusionDoc.h : CImagesFusionDoc ��Ľӿ�
//

#pragma once


class CImagesFusionDoc : public CDocument
{
protected: // �������л�����
	CImagesFusionDoc();
	DECLARE_DYNCREATE(CImagesFusionDoc)

// ����
public:
	ParameterVect m_ParameterVect;
	Parameter m_Parameter;
private:
	IplImage* m_pAImage;
	IplImage* m_pBImage;
	IplImage* m_pPcaFusionImage;
	IplImage* m_pFusionImage;
	IplImage* m_pWaveletFusionImage;

// ����
public:
	void FreeImage(int K);
	IplImage* GetIplImg(int K);

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CImagesFusionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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


