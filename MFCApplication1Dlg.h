#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CvvImage.h"
#include "afxwin.h"
// MFCApplication1Dlg.h : 헤더 파일

#pragma once


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CStatic m_stDisplay;

	// 이미지
	IplImage* m_pImage; // 이미지 변수 선언 
	CvvImage m_cImage; // 화면에 뿌려줄 이미지

	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedButton4();
	
	vector<vector<int> > detectFace(Mat frame); // 얼굴 및 눈 영역을 잡아주는 detectFace 함수
	void swap_pix(int x, int y, int width, int height, IplImage* img1); // back_ground 값을 동적으로 바꾸어주는 함수
};

