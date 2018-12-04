/*
#include <opencv\cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void swap_pix(int x, int y, int width, int height, IplImage* img1) {
	unsigned char arr[10000][3] = { 0 };
	int arr_index = 0;

	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
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

	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			img1->imageData[(i*img1->widthStep) + j * 3 + 0] = arr[arr_index][0];
			img1->imageData[(i*img1->widthStep) + j * 3 + 1] = arr[arr_index][1];
			img1->imageData[(i*img1->widthStep) + j * 3 + 2] = arr[arr_index][2];

			arr_index++;
		}
	}

}


int main() {
	IplImage* img1 = cvLoadImage("iu.jpg");
	srand((unsigned)time(NULL));

	//CvPoint mypoint(q + 12, p + 12);

	//cvCircle(img1, mypoint, 20, CV_RGB(255, 0, 0), 1, 8, 0);

   // cvSetImageROI(img1, cvRect(360, 240, 25, 25));
	//cvSetImageROI(img2, cvRect(360, 240, 25, 25));
	//cvAddWeighted(img1, 0.5, img2, 0.5, 0.0, img1);
	//cvResetImageROI(img1);

	swap_pix(240, 360, 25, 25, img1);

	cvNamedWindow("Example3-14");
	cvShowImage("Example3-14", img1);
	cvWaitKey(0);

	cvDestroyWindow("Example3-14");
	cvReleaseImage(&img1);

	return 0;
}

*/


/* 단순 이미지 출력
#include <opencv2/highgui.hpp>

int main(int args, char** argv) {
	IplImage* img = cvLoadImage("eye.jpg");
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", img);
	cvWaitKey(0);

	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
	
	return 0;
}
*/

 //얼굴 및 눈 영역 박스
#include <iostream>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <tchar.h>
#include <math.h>

using namespace cv;
using namespace std;

vector<vector<int> > detectFace(Mat frame); // 얼굴 및 눈 영역을 잡아주는 detectFace 함수

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String img_name = "sample6.jpg";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char** argv) {
	Mat img = imread(img_name); // Mat구조체로 이미지 load하기
	IplImage* img2 = new IplImage(img); // iplimage구조체로 이미지 load하기
	IplImage* background = cvLoadImage("background.jpg"); // 미리 준비한 background 
	int eye_radios[2]; 

	vector<vector<int> > info(2, vector<int>(3, 0));

	IplImage* gray; // gray 이미지 처리를 위한 iplimage

	if (img.data == NULL) {
		printf("이미지 열기 실패");
		return -1;
	}
	
	if (!face_cascade.load(face_cascade_name)) {
		printf("error face cascade loading\n");
	}
	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("error eyes cascade loading\n");
	}

	info = detectFace(img);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			cout << info[i][j] << " ";
		}
		cout << endl;
	}

	// 두 눈 영역 roi의 center값을 이은 직선
	//cvLine(img2, cvPoint(info[1][0], info[1][1]), cvPoint(info[0][0], info[0][1]), CV_RGB(255, 0, 0), 1, 8);

	// roi의 범위 1/2로 줄이기
	cvSetImageROI(img2, cvRect(info[0][0]-(info[0][2]/2), info[0][1]-(info[0][2]/2), info[0][2], info[0][2]));
    
	gray = cvCreateImage(cvGetSize(img2), IPL_DEPTH_8U, 1); // gray instance 생성
	cvCvtColor(img2, gray, CV_RGB2GRAY); // img2를 gray 이미지로 변환
	cvSmooth(gray, gray, CV_MEDIAN, 7, 7, 0, 0); // gray 이미지 노이즈 데이터 처리
	cvThreshold(gray, gray, 70, 255, CV_THRESH_BINARY); // gray 이미지를 이진화 처리
	
	Mat Mat_img = cvarrToMat(gray); // iplimage 구조체 gray 이미지를 mat 구조체로 변환

	int center_x = Mat_img.rows / 2; // 1/2 roi의 중심 x좌표
	int center_y = Mat_img.cols / 2; // 1/2 roi의 중심 y좌표

	printf("center_x %d center_y %d\n", center_x, center_y); // roi's center

	// 기존 data 출력
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

	int flag = 0; // 눈 동공의 center_x값 찾기
	while (!flag) {
		int right_count = 0;
		for (int i = center_x + 1 ; i <= Mat_img.rows; i++) {
			if (Mat_img.at<uchar>(center_y, i) == 0 ) {
				right_count++;
			}
			else {
				break;
			}
		}
		printf("###right_count: %d\n", right_count);

		int left_count = 0;
		for (int i = center_x - 1; i >= 0; i--) {
			if (Mat_img.at<uchar>(center_y, i) == 0) {
				left_count++;
			}
			else {
				break;
			}
		}

		printf("#left_count: %d\n", left_count);

		if (abs(right_count - left_count) <= 1 ) {
			flag = 1;
			eye_radios[0] = right_count;
			printf("break right_count: %d, left_count %d\n", right_count, left_count);
		}

		else if (right_count > left_count) {
			center_x++;
		}

		else if (right_count < left_count) {
			center_x--;
		}
	}
	printf("beter coding center_y: %d\n", center_y);

	
	flag = 0; // 눈 동공의 center_y값 찾기
	while (!flag) {
		int up_count = 0;
		for (int i = center_y - 1; i >= 0; i--) {
			if (Mat_img.at<uchar>(i, center_x) == 0) {
				up_count++;
			}
			else {
				break;
			}
		}
		printf("###up_count: %d\n", up_count);

		int down_count = 0;
		for (int i = center_y + 1; i <= Mat_img.rows; i++) {
			if (Mat_img.at<uchar>(i, center_x) == 0) {
				down_count++;
			}
			else {
				break;
			}
		}

		printf("#down_count: %d\n", down_count);

		if (abs(down_count - up_count) <= 1) {
			flag = 1;
			eye_radios[1] = down_count;
			printf("break up_count: %d, down_count %d\n", up_count, down_count);
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

	printf("%d %d", eye_radios[0], eye_radios[1]);
	int radios = eye_radios[0] > eye_radios[1] ? eye_radios[0] : eye_radios[1];
	cvCircle(img2, cvPoint(center_x, center_y), radios, CV_RGB(255, 0, 0), 1, 8, 0);

	cvResetImageROI(img2);
	
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example1", img2);

	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example2", gray);
	cvWaitKey(0);

	cvReleaseImage(&img2);
	cvDestroyWindow("Example1");

	cvReleaseImage(&gray);
	cvDestroyWindow("Example2");

	//cvSetImageROI(img2, detectFace(img));
	//info = detectFace(img);
	
	//cvAddS(img2, cvScalar(200), img2);
	//cvLine(img2, cvPoint(124, 154), cvPoint(188, 152), CV_RGB(255, 0, 0), 1, 8);

	

	//cvSetImageROI(img2, cvRect(info[0][0], info[0][1], info[0][2] / 2 , info[0][2] / 2));
	//cvSetImageROI(img2, cvRect(0, 0, 50, 50));
	//cvAddWeighted(img2, 0.5, img3, 0.5, 0.0, img2);
	//cvResetImageROI(img2);

	

	return 0;
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


/*
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[]) {
	IplImage * image = cvLoadImage("selfie.jpg");

	cvShowImage("test image", image);
	cvWaitKey(0);

	cvReleaseImage(&image);
}
*/

/*웹캠 열기

#include <opencv\cv.h> 
#include "opencv2\opencv.hpp"
#include <opencv\highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**) {
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cout << "웹캠을 열 수 없습니다." << endl;
		return 1;
	}

	//캡처 영상을 320x240으로 지정  
	capture.set(CAP_PROP_FRAME_WIDTH, 320);
	capture.set(CAP_PROP_FRAME_HEIGHT, 240);


	Mat frame;
	namedWindow("input", 1);


	while (1)
	{
		//웹캡으로부터 한 프레임을 읽어옴  
		capture >> frame;

		//화면에 영상을 보여줌
		imshow("input", frame);

		//ESC키 누르면 종료
		if (waitKey(1) == 27) break;
	}

	return 0;
}
*/