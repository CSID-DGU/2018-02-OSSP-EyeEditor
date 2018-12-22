


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
#include <time.h>

using namespace cv;
using namespace std;

vector<vector<int> > detectFace(Mat frame); // 얼굴 및 눈 영역을 잡아주는 detectFace 함수
void swap_pix(int x, int y, int width, int height, IplImage* img1); // back_ground 값을 동적으로 바꾸어주는 함수

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String img_name = "iu.jpg";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char** argv) {
   Mat img = imread(img_name); // Mat구조체로 이미지 load하기
   IplImage* img2 = new IplImage(img); // iplimage구조체로 이미지 load하기
   IplImage* background = cvLoadImage("background.jpg"); // 미리 준비한 background 
   int eye_radios[2]; // 타원의 긴 반지름, 작은 반지름
   srand((unsigned)time(NULL));

   vector<vector<int> > info(2, vector<int>(3, 0));

   IplImage* gray; // gray 이미지 처리를 위한 iplimage
   IplImage* gray2; // gray 이미지 처리를 위한 iplimag

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

   // 두 눈 영역 roi의 center값을 이은 직선
   //cvLine(img2, cvPoint(info[1][0], info[1][1]), cvPoint(info[0][0], info[0][1]), CV_RGB(255, 0, 0), 1, 8);

   // roi의 범위 1/2로 줄이기
   cvSetImageROI(img2, cvRect(info[0][0]-(info[0][2]/2), info[0][1]-(info[0][2]/2), info[0][2], info[0][2]));
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
      for (int i = center_x + 1 ; i < Mat_img.rows; i++) {
         if (Mat_img.at<uchar>(center_y, i) == 0 ) {
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

      if (abs(right_count - left_count) <= 1 ) {
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
            if (Mat_img.at<uchar>(i+1, center_x) == 0) {
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
            if (Mat_img.at<uchar>(i-1, center_x) == 0) {
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
   //cvSetImageROI(img2, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios, info[0][1] - (info[0][2] / 2) + center_y - radios, radios * 2, radios * 2));
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
      cvSetImageROI(img2, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios + target_position2[i][0], info[0][1] - (info[0][2] / 2) + center_y - radios + target_position2[i][1], 1, 1));
      cvSetImageROI(background, cvRect(info[0][0] - (info[0][2] / 2) + center_x - radios + target_position2[i][0], info[0][1] - (info[0][2] / 2) + center_y - radios + target_position2[i][1], 1, 1));
      cvAddS(img2, cvScalar(150), img2);
      //cvAddWeighted(img2, 0.6, background, 0.4, 0.0, img2);
      cvResetImageROI(img2);
      cvResetImageROI(background);
   }
   

   cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
   cvShowImage("Example2", gray);

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
   cvSetImageROI(img2, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios, info[1][1] - (info[1][2] / 2) + center_y - radios, radios*2, radios*2));
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

   cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
   cvShowImage("mask", mask_img);

   cvNamedWindow("testa", CV_WINDOW_AUTOSIZE);
   cvShowImage("testa", test);
   //cvSet(mask_img, CV_RGB(0, 0, 0));
   //cvCircle(mask_img, cvPoint(radios, radios), radios, CV_RGB(0, 0, 0), 1, 8, -1);
   
   //double value = cvGetReal2D(mask_img, 0, 0);
   
   int target_position[10000][2] = { 0 };
   int t_index = 0;
   for (int i = 0; i < radios*2; i++) {
      for (int j = 0; j < radios*2; j++) {
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

   cvResetImageROI(img2); 

   for (int i = 0; i < t_index; i++) { // donut모양으로 
      cvSetImageROI(img2, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios + target_position[i][0], info[1][1] - (info[1][2] / 2) + center_y - radios + target_position[i][1], 1, 1));
      cvSetImageROI(background, cvRect(info[1][0] - (info[1][2] / 2) + center_x - radios + target_position[i][0], info[1][1] - (info[1][2] / 2) + center_y - radios + target_position[i][1], 1, 1));
      //cvAddS(img2, cvScalar(150), img2);
      cvAddWeighted(img2, 0.6, background, 0.4, 0.0, img2);
      cvResetImageROI(img2);
      cvResetImageROI(background);
   }

   cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
   cvShowImage("Example1", img2);

   cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
   cvShowImage("Example3", gray2);

   cvWaitKey(0);

   cvReleaseImage(&img2);
   cvDestroyWindow("Example1");

   cvReleaseImage(&gray);
   cvDestroyWindow("Example2");

   cvReleaseImage(&gray2);
   cvDestroyWindow("Example3");

   cvReleaseImage(&mask_img);
   cvDestroyWindow("mask");

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

