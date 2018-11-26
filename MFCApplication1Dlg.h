#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CvvImage.h"
#include "afxwin.h"
// MFCApplication1Dlg.h : ��� ����

#pragma once


// CMFCApplication1Dlg ��ȭ ����
class CMFCApplication1Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic m_stDisplay;

	// �̹���
	IplImage* m_pImage; // �̹��� ���� ���� 
	CvvImage m_cImage; // ȭ�鿡 �ѷ��� �̹���

	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedButton4();
};

