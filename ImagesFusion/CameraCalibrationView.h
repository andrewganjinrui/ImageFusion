#pragma once



// CCameraCalibrationView 窗体视图

class CCameraCalibrationView : public CFormView
{
	DECLARE_DYNCREATE(CCameraCalibrationView)

protected:
	CCameraCalibrationView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


