// ImagesFusion.h : ImagesFusion Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CImagesFusionApp:
// �йش����ʵ�֣������ ImagesFusion.cpp
//

class CImagesFusionApp : public CWinApp
{
public:
	CImagesFusionApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImagesFusionApp theApp;