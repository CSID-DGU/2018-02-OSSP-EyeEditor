// ttp://blog.naver.com/PostView.nhn?blogId=koreahu&logNo=220882065913


// MFCApplication1Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent), m_pImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_DISPLAY, m_stDisplay);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_OPEN, &CMFCApplication1Dlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (m_pImage) { // IplImage 형의 이미지를 CvvImage 형으로 복사한 뒤 그 핸들을 이용하여 Static Control 에 이미지를 출력

			CDC* pDC;
			CRect rect;

			pDC = m_stDisplay.GetDC();
			m_stDisplay.GetClientRect(&rect);

			m_cImage.CopyOf(m_pImage);
			m_cImage.DrawToHDC(pDC->m_hDC, rect);

			ReleaseDC(pDC);
		}

		CDialogEx::OnPaint();
	}

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::OnBnClickedButton1() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IplImage * image = cvLoadImage("selfie.jpg", -1);
	cvShowImage("Start OpenCV with MFC", image);
	cvWaitKey(0);
	cvReleaseImage(&image);
}

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pImage != NULL) cvReleaseImage(&m_pImage); // m_pImage 객체가 NULL 이 아니라면 cvReleaseImage 로 소멸

	m_pImage = cvLoadImage("iu.jpg", -1); // cvLoadImage 로 원하는 이미지 파일을 로딩하고 그 리턴값을 m_pImage 에 저장

	// Static 영역을 새로 그리기위해 InvalidateRect 를 호출한다.
    // 그러면 OnPaint 함수가 호출 될것이다.여기에 리턴 받은 m_pImage를 그려 넣는다

	Invalidate(FALSE);
}

BOOL CMFCApplication1Dlg::DestroyWindow() {
	if(m_pImage != NULL) cvReleaseImage(&m_pImage);
	return CDialog::DestroyWindow();
}

void CMFCApplication1Dlg::OnBnClickedOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())

	{
		CString pathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(pathName);

		m_pImage = cvLoadImage(pszConvertedAnsiString, -1); // 이미지 path

		Invalidate(FALSE);    //CFileOpenDlg::OnPaint()에서 Picture Control에 그려주는 함수 추가할 것

	}
}


void CMFCApplication1Dlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pImage) {
		//cvSetImageROI(m_pImage, cvRect(150, 100, 270, 270));
		//cvAddS(m_pImage, cvScalar(200), m_pImage);
		//cvResetImageROI(m_pImage);
		
		Mat img = cvarrToMat(m_pImage);
		IplImage* img2 = new IplImage(img);
		IplImage* background = cvLoadImage("background.jpg"); // 미리 준비한 background 
		int eye_radios[2]; // 타원의 긴 반지름, 작은 반지름


		vector<vector<int> > info(2, vector<int>(3, 0));
		//IplImage* img2 = m_pImage;

		IplImage* gray; // gray 이미지 처리를 위한 iplimage
		IplImage* gray2; // gray 이미지 처리를 위한 iplimag

		info = detectFace(img);

		// info값 출력
		printf("%d %d %d\n", info[0][0], info[0][1], info[0][2]);
		printf("%d %d %d\n", info[1][0], info[1][1], info[1][2]);

		// 두 눈 영역 roi의 center값을 이은 직선
		//cvLine(img2, cvPoint(info[1][0], info[1][1]), cvPoint(info[0][0], info[0][1]), CV_RGB(255, 0, 0), 1, 8);

		// roi의 범위 1/2로 줄이기
		cvSetImageROI(img2, cvRect(info[0][0] - (info[0][2] / 2), info[0][1] - (info[0][2] / 2), info[0][2], info[0][2]));
		cout << "circle1's center_x : " << info[0][0] << " circle1's center_y : " << info[0][1] << endl;

		gray = cvCreateImage(cvGetSize(img2), IPL_DEPTH_8U, 1); // gray instance 생성
		cvCvtColor(img2, gray, CV_RGB2GRAY); // img2를 gray 이미지로 변환
		cvSmooth(gray, gray, CV_MEDIAN, 7, 7, 0, 0); // gray 이미지 노이즈 데이터 처리
		cvThreshold(gray, gray, 70, 255, CV_THRESH_BINARY); // gray 이미지를 이진화 처리,70

		cvMorphologyEx(gray, gray, NULL, NULL, CV_MOP_OPEN, 1); // 모폴로지 연산

		Mat Mat_img = cvarrToMat(gray); // iplimage 구조체 gray 이미지를 mat 구조체로 변환

		int center_x = Mat_img.rows / 2; // 1/2 roi의 중심 x좌표
		int center_y = Mat_img.cols / 2; // 1/2 roi의 중심 y좌표

		printf("center_x %d center_y %d\n", center_x, center_y); // roi's center

																 // 기존 data 출력
		printf("data1\n");
		for (int i = 0; i < Mat_img.rows; i++) {
			for (int j = 0; j < Mat_img.cols; j++) {
				if (i == center_y && j == center_x) {
					printf("7");
				}

				else if (Mat_img.at<uchar>(i, j) == 255) {
					printf("*");
				}

				else if (Mat_img.at<uchar>(i, j) == 0) {
					printf("@");
				}
			}
			printf("\n");
		}

		int flag = 0; // roi 좌우 중심 맞추기
		while (!flag) {
			int right_count = 0;
			int right_count2 = 0;
			int noise = 0;
			for (int i = center_x + 1; i < Mat_img.rows; i++) {
				if (Mat_img.at<uchar>(center_y, i) == 0) {
					if (noise == 0) {
						right_count++;
					}
					right_count2++;
				}
				else {
					if (Mat_img.at<uchar>(center_y, i - 1) == 0) { // @ -> *
						noise++;
					}

					if (noise > 1) {
						break;
					}

					right_count2++;
				}
			}

			right_count = noise == 1 ? right_count : right_count2;

			int left_count = 0;
			int left_count2 = 0;
			noise = 0;
			for (int i = center_x - 1; i > 0; i--) {
				if (Mat_img.at<uchar>(center_y, i) == 0) {
					if (noise == 0) {
						left_count++;
					}
					left_count2++;
				}
				else {
					if (Mat_img.at<uchar>(center_y, i + 1) == 0) { // @ -> *
						noise++;
					}

					if (noise > 1) {
						break;
					}

					left_count2++;
				}
			}

			left_count = (noise == 1) ? left_count : left_count2;

			if (abs(right_count - left_count) <= 1) {
				flag = 1;
				eye_radios[0] = left_count > right_count ? left_count : right_count;
				printf("<break_point1> right_count: %d, left_count %d\n", right_count, left_count);
			}

			else if (right_count > left_count) {
				center_x++;
			}

			else if (right_count < left_count) {
				center_x--;
			}
		}

		flag = 0; // 눈 동공의 center_y값 찾기
		while (!flag) {
			int up_count = 0;
			int up_count2 = 0;
			int noise = 0;
			for (int i = center_y - 1; i > 0; i--) {
				if (Mat_img.at<uchar>(i, center_x) == 0) {
					if (noise == 0) {
						up_count++;
					}
					up_count2++;
				}
				else {
					if (Mat_img.at<uchar>(i + 1, center_x) == 0) {
						noise++;
					}

					if (noise > 1) {
						break;
					}

					up_count2++;
				}
			}

			up_count = (noise == 1) ? up_count : up_count2;

			int down_count = 0;
			int down_count2 = 0;
			noise = 0;
			for (int i = center_y + 1; i < Mat_img.rows; i++) {
				if (Mat_img.at<uchar>(i, center_x) == 0) {
					if (noise == 0) {
						down_count++;
					}
					down_count2++;
				}
				else {
					if (Mat_img.at<uchar>(i - 1, center_x) == 0) {
						noise++;
					}

					if (noise > 1) {
						break;
					}

					down_count2++;
				}
			}

			down_count = (noise == 1) ? down_count : down_count2;

			if (abs(down_count - up_count) <= 1) {
				flag = 1;
				eye_radios[1] = down_count > up_count ? down_count : up_count;
				printf("<break point2> up_count: %d, down_count %d\n", up_count, down_count);
			}

			else if (up_count > down_count) {
				center_y--;
			}

			else if (up_count < down_count) {
				center_y++;
			}
		}



		// 눈동자의 중심 이동 후 출력
		for (int i = 0; i < Mat_img.rows; i++) {
			for (int j = 0; j < Mat_img.cols; j++) {
				if (i == center_y && j == center_x) {
					printf("7");
				}

				else if (Mat_img.at<uchar>(i, j) == 255) {
					printf("*");
				}

				else {
					printf("@");
				}
			}
			printf("\n");
		}

		/* 이미지 출력
		for (int i = 0; i < Mat_img.rows; i++) {
		for (int j = 0; j < Mat_img.cols; j++) {
		printf("%d ", Mat_img.at<uchar>(i, j));
		}
		printf("\n");
		}
		*/


		printf("타원의 짧은 반지름 및 긴 반지름: \n", eye_radios[0], eye_radios[1]); // 타원의 짧은 반지름과 긴 반지름
		int radios = eye_radios[0] > eye_radios[1] ? eye_radios[0] : eye_radios[1];
		//cvCircle(img2, cvPoint(center_x, center_y), radios, CV_RGB(255, 0, 0), 1, 8, 0); // 눈동자 detect


		// 데이터 변형 처리
		cvResetImageROI(img2);
		cvSetImageROI(img2, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios, info[0][1] - (info[0][2] / 2) + center_y - radios, radios * 2, radios * 2));
		//cvAddS(img2, cvScalar(200), img2);

		IplImage* mask_img2 = cvCreateImage(cvSize(radios * 2, radios * 2), IPL_DEPTH_8U, 1); // 3
		cvSet(mask_img2, cvScalarAll(255));

		cvCircle(mask_img2, cvPoint(radios, radios), radios*0.85, CV_RGB(0, 0, 0), -1, 8, 0);
		cvCircle(mask_img2, cvPoint(radios, radios), radios*0.45, CV_RGB(255, 255, 255), -1, 8, 0);

		cout << "radios: " << radios << endl;

		int target_position2[10000][2] = { 0 };
		int t_index2 = 0;
		for (int i = 0; i < radios * 2; i++) {
			for (int j = 0; j < radios * 2; j++) {
				double value = cvGetReal2D(mask_img2, i, j);
				//int a = value > 100 ? 2 : 0;
				//cout << a << " ";

				if (value < 100) {
					target_position2[t_index2][0] = i;
					target_position2[t_index2][1] = j;
					t_index2++;
				}
			}
		}

		cvResetImageROI(img2);

		for (int i = 0; i < t_index2; i++) { // donut모양으로 
			cvSetImageROI(m_pImage, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios + target_position2[i][0], info[0][1] - (info[0][2] / 2) + center_y - radios + target_position2[i][1], 1, 1));
			cvSetImageROI(background, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios + target_position2[i][0], info[0][1] - (info[0][2] / 2) + center_y - radios + target_position2[i][1], 1, 1));
			cvAddS(m_pImage, cvScalar(150), m_pImage);
			//cvAddWeighted(img2, 0.6, background, 0.4, 0.0, img2);
			cvResetImageROI(m_pImage);
			cvResetImageROI(background);
		}


		//cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
		//cvShowImage("Example2", gray);

		cvResetImageROI(img2);

		//  roi의 범위 1/2로 줄이기
		cvSetImageROI(img2, cvRect(info[1][0] - (info[1][2] / 2), info[1][1] - (info[1][2] / 2), info[1][2], info[1][2]));
		cout << "before circle2's center_x : " << info[0][0] << " circle2's center_y : " << info[0][1] << endl;

		gray2 = cvCreateImage(cvGetSize(img2), IPL_DEPTH_8U, 1); // gray instance 생성
		cvCvtColor(img2, gray2, CV_RGB2GRAY); // img2를 gray 이미지로 변환
		cvSmooth(gray2, gray2, CV_MEDIAN, 7, 7, 0, 0); // gray 이미지 노이즈 데이터 처리
		cvThreshold(gray2, gray2, 70, 255, CV_THRESH_BINARY); // gray 이미지를 이진화 처리

		Mat_img = cvarrToMat(gray2); // iplimage 구조체 gray 이미지를 mat 구조체로 변환

		cvMorphologyEx(gray2, gray2, NULL, NULL, CV_MOP_OPEN, 1); // 모폴로지 연산

		center_x = Mat_img.rows / 2; // 1/2 roi의 중심 x좌표
		center_y = Mat_img.cols / 2; // 1/2 roi의 중심 y좌표

									 // 기존 data2 출력
		printf("data2\n");
		for (int i = 0; i < Mat_img.rows; i++) {
			for (int j = 0; j < Mat_img.cols; j++) {
				if (i == center_y && j == center_x) {
					printf("7");
				}

				else if (Mat_img.at<uchar>(i, j) == 255) {
					printf("*");
				}

				else if (Mat_img.at<uchar>(i, j) == 0) {
					printf("@");
				}
			}
			printf("\n");
		}

		flag = 0; // roi 좌우 중심 맞추기
		while (!flag) {
			int right_count = 0;
			int right_count2 = 0;
			int noise = 0;
			for (int i = center_x + 1; i < Mat_img.rows; i++) {
				if (Mat_img.at<uchar>(center_y, i) == 0) {
					if (noise == 0) {
						right_count++;
					}
					right_count2++;
				}
				else {
					if (Mat_img.at<uchar>(center_y, i - 1) == 0) { // @ -> *
						noise++;
					}

					if (noise > 1) {
						break;
					}

					right_count2++;
				}
			}

			right_count = (noise == 1) ? right_count : right_count2;

			int left_count = 0;
			int left_count2 = 0;
			noise = 0;
			for (int i = center_x - 1; i > 0; i--) {
				if (Mat_img.at<uchar>(center_y, i) == 0) {
					if (noise == 0) {
						left_count++;
					}
					left_count2++;
				}
				else {
					if (Mat_img.at<uchar>(center_y, i + 1) == 0) { // @ -> *
						noise++;
					}

					if (noise > 1) {
						break;
					}

					left_count2++;
				}
			}

			left_count = (noise == 1) ? left_count : left_count2;

			if (abs(right_count - left_count) <= 1) {
				flag = 1;
				eye_radios[0] = left_count > right_count ? left_count : right_count;
				printf("<break poin3> left_count: %d, right_count %d\n", left_count, right_count);
			}

			else if (right_count > left_count) {
				center_x++;
			}

			else if (right_count < left_count) {
				center_x--;
			}
		}

		flag = 0; // 눈 동공의 center_y값 찾기
		while (!flag) {
			int up_count = 0;
			int up_count2 = 0;
			int noise = 0;
			for (int i = center_y - 1; i > 0; i--) {
				if (Mat_img.at<uchar>(i, center_x) == 0) {
					if (noise == 0) {
						up_count++;
					}
					up_count2++;
				}
				else {
					if (Mat_img.at<uchar>(i + 1, center_x) == 0) {
						noise++;
					}

					if (noise > 1) {
						break;
					}

					up_count2++;
				}
			}

			up_count = noise == 1 ? up_count : up_count2;

			int down_count = 0;
			int down_count2 = 0;
			noise = 0;
			for (int i = center_y + 1; i < Mat_img.rows; i++) {
				if (Mat_img.at<uchar>(i, center_x) == 0) {
					if (noise == 0) {
						down_count++;
					}
					down_count2++;
				}
				else {
					if (Mat_img.at<uchar>(i - 1, center_x) == 0) {
						noise++;
					}

					if (noise > 1) {
						break;
					}

					down_count2++;
				}
			}

			down_count = noise == 1 ? down_count : down_count2;

			if (abs(down_count - up_count) <= 1) {
				flag = 1;
				eye_radios[1] = up_count > down_count ? up_count : down_count;
				printf("<break poin4> up_count: %d, down_count %d\n", up_count, down_count);
			}

			else if (up_count > down_count) {
				center_y--;
			}

			else if (up_count < down_count) {
				center_y++;
			}
		}


		// test
		for (int i = 0; i < Mat_img.rows; i++) {
			for (int j = 0; j < Mat_img.cols; j++) {
				if (i == center_y && j == center_x) {
					printf("7");
				}

				else if (Mat_img.at<uchar>(i, j) == 255) {
					printf("*");
				}

				else {
					printf("@");
				}
			}
			printf("\n");
		}

		/* 이미지 출력
		for (int i = 0; i < Mat_img.rows; i++) {
		for (int j = 0; j < Mat_img.cols; j++) {
		printf("%d ", Mat_img.at<uchar>(i, j));
		}
		printf("\n");
		}
		*/

		printf("타원의 짧은 반지름 및 긴 반지름 %d %d\n", eye_radios[1], eye_radios[0]); // 타원의 짧은 반지름과 긴 반지름
		radios = eye_radios[0] > eye_radios[1] ? eye_radios[0] : eye_radios[1];
		//cvCircle(img2, cvPoint(center_x, center_y), radios, CV_RGB(255, 0, 0), 1, 8, 0); // 눈동자 영역 detect



		// 데이터 변형 처리
		cvResetImageROI(img2);
		//cvSetImageROI(img2, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios, info[1][1] - (info[1][2] / 2) + center_y - radios, radios * 2, radios * 2));
		//cvAddS(img2, cvScalar(200), img2);

		IplImage* mask_img = cvCreateImage(cvSize(radios * 2, radios * 2), IPL_DEPTH_8U, 1); // 3
		IplImage* test = cvCreateImage(cvSize(100, 100), IPL_DEPTH_8U, 3);
		cvSet(mask_img, cvScalarAll(255));
		cvSet(test, cvScalarAll(255));

		cvCircle(mask_img, cvPoint(radios, radios), radios*0.85, CV_RGB(0, 0, 0), -1, 8, 0);
		cvCircle(mask_img, cvPoint(radios, radios), radios*0.45, CV_RGB(255, 255, 255), -1, 8, 0);

		cout << "radios: " << radios << endl;

		cvCircle(test, cvPoint(50, 50), 50, CV_RGB(255, 0, 0), 1, 8, 0);
		cvCircle(test, cvPoint(50, 50), 40, CV_RGB(0, 0, 0), -1, 8, 0);
		cvCircle(test, cvPoint(50, 50), 20, CV_RGB(255, 255, 255), -1, 8, 0);

		//cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
		//cvShowImage("mask", mask_img);

		//cvNamedWindow("testa", CV_WINDOW_AUTOSIZE);
		//cvShowImage("testa", test);
		//cvSet(mask_img, CV_RGB(0, 0, 0));
		//cvCircle(mask_img, cvPoint(radios, radios), radios, CV_RGB(0, 0, 0), 1, 8, -1);

		//double value = cvGetReal2D(mask_img, 0, 0);

		int target_position[10000][2] = { 0 };
		int t_index = 0;
		for (int i = 0; i < radios * 2; i++) {
			for (int j = 0; j < radios * 2; j++) {
				double value = cvGetReal2D(mask_img, i, j);
				//int a = value > 100 ? 2 : 0;
				//cout << a << " ";

				if (value < 100) {
					target_position[t_index][0] = i;
					target_position[t_index][1] = j;
					t_index++;
				}
			}
		}

		//cvResetImageROI(img2);

		for (int i = 0; i < t_index; i++) { // donut모양으로 
			cvSetImageROI(m_pImage, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios + target_position[i][0], info[1][1] - (info[1][2] / 2) + center_y - radios + target_position[i][1], 1, 1));
			cvSetImageROI(background, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios + target_position[i][0], info[1][1] - (info[1][2] / 2) + center_y - radios + target_position[i][1], 1, 1));
			cvAddS(m_pImage, cvScalar(150), m_pImage);
			//cvAddWeighted(img2, 0.6, background, 0.4, 0.0, img2);
			cvResetImageROI(m_pImage);
			cvResetImageROI(background);
		}
		Invalidate(FALSE);
	}
	else 
		MessageBox(_T("이미지를 먼저 로드해주세요."));
}

vector<vector<int> > CMFCApplication1Dlg::detectFace(Mat frame) {
	vector<Rect> faces;
	vector<Rect> eyes;
	vector<vector<int> > info(2, vector<int>(3, 0));
	int info_index = 0;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	if (!face_cascade.load(face_cascade_name)) {
		printf("error face cascade loading\n");
	}
	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("error eyes cascade loading\n");
	}

	//Detect Face
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(255, 0, 0), 2, 8, 0);

		Mat faceR01 = frame_gray(faces[i]);

		//Detect eyes
		eyes_cascade.detectMultiScale(faceR01, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++) {
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			//circle(frame, center, radius, Scalar(255, 0, 0), 2, 8, 0);

			//printf("center: %d %d %d", center.x, center.y, radius);
			info[info_index][0] = center.x;
			info[info_index][1] = center.y;
			info[info_index][2] = radius;
			info_index++;
		}
	}

	//imshow("Face detection", frame);
	return info;
}

/* 
입력 : 이미지 data frame,
출력 : 눈 영역 roi의 center.x, center.y, r vecor info
*/
vector<vector<int> > detectFace(Mat frame) {
	vector<Rect> faces;
	vector<Rect> eyes;
	vector<vector<int> > info(2, vector<int>(3, 0));
	int info_index = 0;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//Detect Face
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(255, 0, 0), 2, 8, 0);

		Mat faceR01 = frame_gray(faces[i]);

		//Detect eyes
		eyes_cascade.detectMultiScale(faceR01, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	
		for (size_t j = 0; j < eyes.size(); j++) {
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			//circle(frame, center, radius, Scalar(255, 0, 0), 2, 8, 0);

			//printf("center: %d %d %d", center.x, center.y, radius);
			info[info_index][0] = center.x;
			info[info_index][1] = center.y;
			info[info_index][2] = radius;
			info_index++;
		}
	}

	//imshow("Face detection", frame);
	return info;
}

void swap_pix(int x, int y, int width, int height, IplImage* img1) {
	unsigned char arr[100000][3] = { 0 };
	int arr_index = 0;

	cout << width << " " << height << endl;

	for (int i = x; i < x + height; i++) {
		for (int j = y; j < y + width; j++) {
			arr[arr_index][0] = img1->imageData[(i*img1->widthStep) + j * 3 + 0];
			arr[arr_index][1] = img1->imageData[(i*img1->widthStep) + j * 3 + 1];
			arr[arr_index][2] = img1->imageData[(i*img1->widthStep) + j * 3 + 2];

			arr_index++;
		}
	}

	for (int i = 0; i < arr_index; i++) // 픽셀 값 mix
	{
		int j = rand() % arr_index;
		int k = rand() % arr_index;
		if (j != k)
		{
			unsigned char temp_1 = arr[j][0];
			unsigned char temp_2 = arr[j][1];
			unsigned char temp_3 = arr[j][2];

			arr[j][0] = arr[k][0];
			arr[j][1] = arr[k][1];
			arr[j][2] = arr[k][2];

			arr[k][0] = temp_1;
			arr[k][1] = temp_2;
			arr[k][2] = temp_3;
		}
	}

	arr_index = 0;

	for (int i = x; i < x + height; i++) {
		for (int j = y; j < y + width; j++) {
			img1->imageData[(i*img1->widthStep) + j * 3 + 0] = arr[arr_index][0];
			img1->imageData[(i*img1->widthStep) + j * 3 + 1] = arr[arr_index][1];
			img1->imageData[(i*img1->widthStep) + j * 3 + 2] = arr[arr_index][2];

			arr_index++;
		}
	}

}
