#pragma once



// CCameraCalibrationView ������ͼ

class CCameraCalibrationView : public CFormView
{
	DECLARE_DYNCREATE(CCameraCalibrationView)

protected:
	CCameraCalibrationView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCameraCalibrationView();

public:
	enum { IDD = IDD_DIALOG_CALIBRATION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


