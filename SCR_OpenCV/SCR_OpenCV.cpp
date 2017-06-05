// SCR_OpenCV.cpp : Defines the exported functions for the DLL application.
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <cv.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <cxcore.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <highgui.h>
#define _CRT_SECURE_NO_DEPRECATE
#include "iostream"
//#include "mat.hpp"

using namespace std;
using namespace cv;

extern "C"
{
    __declspec(dllexport) int Demo(LPCWSTR);
    __declspec(dllexport) int Process(LPCWSTR);
    __declspec(dllexport) vector<uchar> ThresholdConversion(LPCWSTR);
}


extern "C" int __cdecl Demo(LPCWSTR filename)
{
    IplImage* frame = cvLoadImage((char*)filename, 1);

    cvSmooth(frame, frame, CV_BLUR, 3, 0, 2.0, 2.0);

    if (frame == NULL){ 
        printf("File does not exist!");
        exit(0);
    }

    cvNamedWindow("Hi", CV_WINDOW_AUTOSIZE);
    cvShowImage("My Window", frame);
    cvSaveImage("image.jpg", frame);

    cvWaitKey();
    cvDestroyWindow("Hi");

    return 0;
}

extern "C" int __cdecl Process(LPCWSTR filename)
{
    //cvThreshold
    IplImage *src = 0, *dst = 0, *dst2 = 0;
    src = cvLoadImage((char*)filename, 0);
    assert(src != 0);

    dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
    cvThreshold(src, dst, 125, 250, CV_THRESH_BINARY);
    //cvReleaseImage(&src);

    //cvFindContours
    CvSeq* contours = 0;
    CvMemStorage* storage = cvCreateMemStorage(0);

    cvFindContours(dst, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    cvDrawContours(src, contours, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), 2, 1, CV_AA, cvPoint(0, 0));

    return 0;
}

//extern "C" vector<uchar> __cdecl ThresholdConversion(LPCWSTR filename)
//{
//    //cvThreshold
//    IplImage *src = 0, *dst = 0, *dst2 = 0;
//
//    src = cvLoadImage((char*)filename, 0);
//
//    assert(src != 0);
//
//    dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
//
//    cvThreshold(src, dst, 125, 250, CV_THRESH_BINARY);
//
//    //cvNamedWindow("cvThreshold", 1);
//    //cvShowImage("cvThreshold", dst);
//
//    //cvWaitKey(0);
//
//    cvReleaseImage(&src);
//
//    //cvDestroyAllWindows();
//
//    //.//return *dst;
//    //return 0;
//
//    //byte resultImage;
//    
//    vector<uchar> buf;
//    vector<uchar> byteArray;
//    Mat matImage = dst;
//    imencode(".bmp", matImage, buf);
//    //int size = buf.size();
//    copy(buf.begin(), buf.end(), byteArray);
//    //matImage.ToBitmap();
//
//    return byteArray;
//}

